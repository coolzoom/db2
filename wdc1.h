#pragma once

#include<string>
#include<vector>
#include<cstdint>
#include<array>
#include<stdexcept>

namespace db2
{
namespace wdc1
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
struct relationship_mapping
{
//	uint32_t            num_entries;
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
	std::uint32_t relationship_data_size;
};
template<typename T>
struct wdc1
{
	std::vector<T> records;
	std::vector<field_structure> fields;
	std::string name;
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
	relationship_mapping relationship_map;
	wdc1(const std::string &s)
	{
		if(s.front()!='W'||s[1]!='D'||s[2]!='C'||s[3]!='1')
			throw std::runtime_error("not WDC1 which is ("s+s.substr(0,4)+")"s);
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
		if(header.relationship_data_size)
		{
			throw std::logic_error("currently unsupported relationship_data_size :"s+std::to_string(header.relationship_data_size));
		}
	}
	std::string serialize() const
	{
		auto r("WDC1"s);
		dheader h;
		h.record_count=records.size();
		h.field_count=fields.size();
		h.record_size=sizeof(T);
		h.string_table_size=name.size();
		h.table_hash=table_hash;             // hash of the table name
		h.layout_hash=layout_hash;            // this is a hash field that changes only when the structure of the data changes
		h.min_id=min_id;
		h.max_id=max_id;
		h.locale=locale;                 // as seen in TextWowEnum
		h.copy_table_size=copy_table.size()*sizeof(copy_table_entry);
		h.flags=flags;                  // possible values are listed in Known Flag Meanings
		h.id_index=id_index;               // this is the index of the field containing ID values; this is ignored if flags & 0x04 != 0
		h.total_field_count=total_field_count;      // in WDC1 this value seems to always be the same as the 'field_count' value
		h.bitpacked_data_offset=bitpacked_data_offset;  // relative position in record where bitpacked data begins; not important for parsing the file
		h.lookup_column_count=lookup_column_count;
		h.offset_map_offset=offset_map_offset;      // Offset to array of struct {	h.offset; uint16_t size;}[max_id - min_id + 1];
		h.id_list_size=ids.size()*4;           // List of ids present in the DB file
		h.field_storage_info_size=field_storages.size()*sizeof(field_storage_info);
		h.common_data_size=common.size();
		h.pallet_data_size=pallet.size();
		h.relationship_data_size=0;
		r.append(reinterpret_cast<const char*>(&h),reinterpret_cast<const char*>(&h+1));
		auto lmd([&r](auto &vec)
		{
			r.append(reinterpret_cast<const char*>(vec.data()),reinterpret_cast<const char*>(vec.data()+vec.size()));
		});
		lmd(fields);
		lmd(records);
		lmd(name);
		lmd(ids);
		lmd(copy_table);
		lmd(field_storages);
		lmd(pallet);
		lmd(common);
		return r;
	}
};

}
}