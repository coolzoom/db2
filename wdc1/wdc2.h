#pragma once

#include<string>
#include<vector>
#include<cstdint>
#include<array>
#include<stdexcept>

namespace db2
{
namespace wdc2
{
using namespace std::string_literals;
struct copy_table_entry
{
	uint32_t id_of_new_row;
	uint32_t id_of_copied_row;
};

enum class field_compression
{
	none,bitpacked,common_data,indexed,indexed_array
};

struct field_storage_info
{
	std::uint16_t offset_bits;
	std::uint16_t field_size;
	std::uint32_t additional_data_size;
	field_compression type;
	std::array<std::uint32_t,3> values;
};
struct relationship_entry
{
	uint32_t foreign_id;
	uint32_t record_index;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& o,const relationship_entry& r)
{
	return o<<r.foreign_id<<'\t'<<r.record_index;
}
struct relationship_mapping
{
	uint32_t            min_id;
	uint32_t            max_id;
	std::vector<relationship_entry>  entries;
};
struct field_structure
{
	std::uint16_t size,position;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& o,const field_structure& fs)
{
	return o<<fs.position<<"\tsize:"<<((32-fs.size)>>3);
}

struct offset_map_entry
{
	uint32_t offset;                                          // this offset is absolute, not relative to another structure; this can (and often will) be zero, in which case you should ignore that entry and move on
	uint16_t length;                                          // this is the length of the record located at the specified offset
};

struct dheader
{
	std::uint32_t record_count;
	std::uint32_t field_count;
	std::uint32_t record_size;
	std::uint32_t string_table_size;
	std::uint32_t table_hash;             // hash of the table name
	std::uint32_t layout_hash;            // this is a hash field that changes only when the structure of the data changes
	std::uint32_t min_id;
	std::uint32_t max_id;
	std::uint32_t locale;                 // as seen in TextWowEnum
	std::uint32_t copy_table_size;
	std::uint16_t flags;                  // possible values are listed in Known Flag Meanings
	std::uint16_t id_index;               // this is the index of the field containing ID values; this is ignored if flags & 0x04 != 0
	std::uint32_t total_field_count;      // in WDC1 this value seems to always be the same as the 'field_count' value
	std::uint32_t bitpacked_data_offset;  // relative position in record where bitpacked data begins; not important for parsing the file
	std::uint32_t lookup_column_count;
	std::uint32_t offset_map_offset;      // Offset to array of struct {std::uint32_t offset; uint16_t size;}[max_id - min_id + 1];
	std::uint32_t id_list_size;           // List of ids present in the DB file
	std::uint32_t field_storage_info_size;
	std::uint32_t common_data_size;
	std::uint32_t pallet_data_size;
	std::uint32_t section_count;
};

struct relationship_map_header
{
	uint32_t num_entries;
	uint32_t min_id;
	uint32_t max_id;
};


struct section_header
{
	uint32_t wdc2_unk_header1;       // always 0 in Battle For Azeroth (8.0.1.26231) and unnamed in client binary
	uint32_t wdc2_unk_header2;       // always 0 in Battle For Azeroth (8.0.1.26231) and unnamed in client binary
	uint32_t file_offset;            // absolute position to the beginning of the section
	uint32_t record_count;           // 'record_count' for the section
	uint32_t string_table_size;      // 'string_table_size' for the section
	uint32_t copy_table_size;
	uint32_t offset_map_offset;      // Offset to array of struct {uint32_t offset; uint16_t size;}[max_id - min_id + 1];
	uint32_t id_list_size;           // List of ids present in the DB file
	uint32_t relationship_data_size;
};

template<typename T>
struct section
{
	section_header header;
	std::vector<T> records;
	std::string name;
	std::vector<copy_table_entry> copy_table;
	relationship_mapping relationship_map;
	section(const section_header& h,const std::byte* s):header(h)
	{
		
	}
};


/*template<typename T>
std::pair<section_header,std::string> serialize(const section<T> &d)
{
	
}*/

template<typename T>
struct wdc2
{
	std::vector<field_structure> fields;
	std::uint32_t locale;
	std::uint32_t table_hash,layout_hash;
	std::uint32_t min_id,max_id;
	std::vector<copy_table_entry> copy_table;
	std::uint16_t flags;
	std::uint16_t id_index;
	std::uint32_t total_field_count;
	std::uint32_t bitpacked_data_offset,lookup_column_count,offset_map_offset;
	std::vector<std::uint32_t> ids;
	std::vector<field_storage_info> field_storages;
	std::vector<offset_map_entry> offsets;
	std::string pallet,common;
//	std::size_t section_count;
	std::vector<secton<T>> sections;
	wdc2(const std::string &s)
	{
		if(s.front()!='W'||s[1]!='D'||s[2]!='C'||s[3]!='2')
			throw std::runtime_error("not WDC2 which is ("s+s.substr(0,4)+")"s);
		auto p(s.data()+4);
		decltype(auto) header(*reinterpret_cast<const dheader*>(p));
		if(header.record_size!=sizeof(T))
			throw std::runtime_error("record_type incorrect size : ("s+std::to_string(sizeof(T))+") which should be("s+std::to_string(header.record_size)+')');
		locale = header.locale;
		table_hash = header.table_hash;
		layout_hash = header.layout_hash;
		min_id = header.min_id;
		max_id = header.max_id;
		flags = header.flags;
		id_index = header.id_index;
		total_field_count = header.total_field_count;
		bitpacked_data_offset = header.bitpacked_data_offset;
		lookup_column_count = header.lookup_column_count;
		offset_map_offset = header.offset_map_offset;
		section_count = header.section_count;
		auto lmd([&p](auto &vec,std::size_t size)
		{
			auto b(reinterpret_cast<typename std::remove_reference_t<decltype(vec)>::const_pointer>(p));
			auto e(b+size);
			vec.assign(b,e);
			p=reinterpret_cast<const char*>(e);
		});
		p+=sizeof(dheader);
		lmd(fields,header.field_count);
		if(!(flags&1))
		{
			lmd(records,header.record_count);
			lmd(name,header.string_table_size);
		}
		else
		{
			throw std::logic_error("currently unsupported flag :"s+std::to_string(flags));
		}
		lmd(ids,header.id_list_size/4);
		lmd(copy_table,header.copy_table_size/sizeof(copy_table_entry));
		lmd(field_storages,header.field_storage_info_size/sizeof(field_storage_info));
		lmd(pallet,header.pallet_data_size);
		lmd(common,header.common_data_size);
		if(header.section_count)
		{
			decltype(auto) rhd(*reinterpret_cast<const relationship_map_header*>(p));
			relationship_map.min_id=rhd.min_id;
			relationship_map.max_id=rhd.max_id;
			p+=sizeof(relationship_map_header);
			lmd(relationship_map.entries,rhd.num_entries);
		}
	}
};

template<typename T>
std::string serialize(const wdc2<T> &d)
{
	auto r("WDC2"s);
	dheader h;
	h.record_count=d.records.size();
	h.field_count=d.fields.size();
	h.record_size=sizeof(T);
	h.string_table_size=d.name.size();
	h.table_hash=d.table_hash;             // hash of the table name
	h.layout_hash=d.layout_hash;            // this is a hash field that changes only when the structure of the data changes
	h.min_id=d.min_id;
	h.max_id=d.max_id;
	h.locale=d.locale;                 // as seen in TextWowEnum
	h.copy_table_size=d.copy_table.size()*sizeof(d.copy_table_entry);
	h.flags=d.flags;                  // possible values are listed in Known Flag Meanings
	h.id_index=d.id_index;               // this is the index of the field containing ID values; this is ignored if flags & 0x04 != 0
	h.total_field_count=d.total_field_count;      // in WDC1 this value seems to always be the same as the 'field_count' value
	h.bitpacked_data_offset=d.bitpacked_data_offset;  // relative position in record where bitpacked data begins; not important for parsing the file
	h.lookup_column_count=d.lookup_column_count;
	h.offset_map_offset=d.offset_map_offset;      // Offset to array of struct {	h.offset; uint16_t size;}[max_id - min_id + 1];
	h.id_list_size=d.ids.size()*4;           // List of ids present in the DB file
	h.field_storage_info_size=d.field_storages.size()*sizeof(d.field_storage_info);
	h.common_data_size=d.common.size();
	h.pallet_data_size=d.pallet.size();
	h.section_count=d.section_count;
	r.append(reinterpret_cast<const char*>(&d.h),reinterpret_cast<const char*>(&d.h+1));
	auto lmd([&r](auto &vec)
	{
		r.append(reinterpret_cast<const char*>(vec.data()),reinterpret_cast<const char*>(vec.data()+vec.size()));
	});
	lmd(d.fields);
	lmd(d.records);
	lmd(d.name);
	lmd(d.ids);
	lmd(d.copy_table);
	lmd(d.field_storages);
	lmd(d.pallet);
	lmd(d.common);
	if(d.section_count)
	{
		std::array<relationship_map_header,1> rhd;
		rhd.front().num_entries=d.relationship_map.entries.size();
		rhd.front().min_id=d.relationship_map.min_id;
		rhd.front().max_id=d.relationship_map.max_id;
		lmd(d.rhd);
		lmd(d.relationship_map.entries);
	}
	return r;
}


}
}