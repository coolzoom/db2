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
	// None -- the field is a 8-, 16-, 32-, or 64-bit integer in the record data
	none,
	// Bitpacked -- the field is a bitpacked integer in the record data.  It
	// is field_size_bits long and starts at field_offset_bits.
	// A bitpacked value occupies
	//   (field_size_bits + (field_offset_bits & 7) + 7) / 8
	// bytes starting at byte
	//   field_offset_bits / 8
	// in the record data.  These bytes should be read as a little-endian value,
	// then the value is shifted to the right by (field_offset_bits & 7) and
	// masked with ((1ull << field_size_bits) - 1).
	bitpacked,
	// Common data -- the field is assumed to be a default value, and exceptions
	// from that default value are stored in the corresponding section in
	// common_data as pairs of { std::uint32_t record_id; std::uint32_t value; }.
	common_data,
	// Bitpacked indexed -- the field has a bitpacked index in the record data.
	// This index is used as an index into the corresponding section in
	// pallet_data.  The pallet_data section is an array of std::uint32_t, so the index
	// should be multiplied by 4 to obtain a byte offset.
	bitpacked_indexed,
	// Bitpacked indexed array -- the field has a bitpacked index in the record
	// data.  This index is used as an index into the corresponding section in
	// pallet_data.  The pallet_data section is an array of std::uint32_t[array_count],
	//
	bitpacked_indexed_array,
	// Same as field_compression_bitpacked
	bitpacked_signed
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& o,const field_compression& fs)
{
	using namespace std::string_literals;
	switch(fs)
	{
		case field_compression::none: return o<<"none"s;
		case field_compression::bitpacked: return o<<"bitpacked"s;
		case field_compression::common_data: return o<<"common data"s;
		case field_compression::bitpacked_indexed: return o<<"bitpacked_indexed"s;
		case field_compression::bitpacked_indexed_array: return o<<"bitpacked_indexed_array"s;
		case field_compression::bitpacked_signed: return o<<"bitpacked_signed"s;
		default: return o<<"unknown("s<<static_cast<unsigned>(fs)<<')';
	}
}

struct field_storage_info
{
	std::uint16_t offset_bits;
	std::uint16_t field_size;
	std::uint32_t additional_data_size;
	field_compression type;
	std::array<std::uint32_t,3> values;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& o,const field_storage_info& fs)
{
	using namespace std::string_literals;
	o<<"offset_bits\t"s<<fs.offset_bits<<
		"\nfield_size\t"s<<fs.field_size<<
		"\nadditional_data_size\t"s<<fs.additional_data_size<<
		"\ntype\t"s<<fs.type;
	switch(fs.type)
	{
	case field_compression::none:break;
	case field_compression::bitpacked:
		o<<"\n\nbitpacking_offset_bits\t"s<<fs.values.front();
		o<<"\nbitpacking_size_bits\t"s<<fs.values[1];
		o<<"\nflags\t"s<<fs.values[2];
	break;
	case field_compression::common_data:
		o<<"\n\ndefault_value\t"s<<fs.values.front();
	break;
	case field_compression::bitpacked_indexed:
		o<<"\n\nbitpacking_offset_bits\t"s<<fs.values.front();
		o<<"\nbitpacking_size_bits\t"s<<fs.values[1];
	break;
	case field_compression::bitpacked_indexed_array:
		o<<"\n\nbitpacking_offset_bits\t"s<<fs.values.front();
		o<<"\nbitpacking_size_bits\t"s<<fs.values[1];
		o<<"\narray_count\t"s<<fs.values[2];
	break;
	default:
		o<<"\n\n"s;
		for(std::size_t i(0);i!=fs.values.size();++i)
			o<<fs.values[i]<<'\t';
	}
	return (o);
}

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
	std::uint32_t relationship_data_size;
};

struct relationship_map_header
{
	uint32_t num_entries;
	uint32_t min_id;
	uint32_t max_id;
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
	std::size_t relationship_data_size;
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
		relationship_data_size = header.relationship_data_size;
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
			decltype(auto) rhd(*reinterpret_cast<const relationship_map_header*>(p));
			relationship_map.min_id=rhd.min_id;
			relationship_map.max_id=rhd.max_id;
			p+=sizeof(relationship_map_header);
			lmd(relationship_map.entries,rhd.num_entries);
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
		h.relationship_data_size=relationship_data_size;
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
		if(relationship_data_size)
		{
			std::array<relationship_map_header,1> rhd;
			rhd.front().num_entries=relationship_map.entries.size();
			rhd.front().min_id=relationship_map.min_id;
			rhd.front().max_id=relationship_map.max_id;
			lmd(rhd);
			lmd(relationship_map.entries);
		}
		return r;
	}
};

}
}