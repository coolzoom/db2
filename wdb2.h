#pragma once

#include<string>
#include<vector>
#include<cstdint>
#include<array>
#include<stdexcept>

namespace db2
{
namespace wdb2
{
using namespace std::string_literals;
struct dheader
{
	std::uint32_t record_count;
	std::uint32_t field_count;                                         // array fields count as the size of array for WDB2
	std::uint32_t record_size;
	std::uint32_t string_table_size;                                   // string block almost always contains at least one zero-byte
	std::uint32_t table_hash;
	std::uint32_t build;
	std::uint32_t timestamp_last_written;                              // set to time(0); when writing in WowClientDB2_Base::Save()
	std::uint32_t min_id;
	std::uint32_t max_id;
	std::uint32_t locale;                                              // as seen in TextWowEnum
	std::uint32_t copy_table_size;                                     // always zero in WDB2 (?) - see WDB3 for information on how to parse this
};

template<typename T>
struct wdb2
{
	std::vector<T> records;
	std::string name;
	std::uint32_t locale;
	std::uint32_t table_hash;
	std::uint32_t min_id,max_id;
	std::uint32_t timestamp_last_written;
	std::uint32_t build;
	std::vector<std::uint32_t> indices;
	std::vector<std::uint16_t> string_lengths;
	wdb2(const std::string &s)
	{
		if(s.front()!='W'||s[1]!='D'||s[2]!='B'||s[3]!='2')
			throw std::runtime_error("not WDB2 which is ("s+s.substr(0,4)+")"s);
		auto p(s.data()+4);
		decltype(auto) header(*reinterpret_cast<const dheader*>(p));
		if(header.record_size!=sizeof(T))
			throw std::runtime_error("record_type incorrect size : ("s+std::to_string(sizeof(T))+") which should be("s+std::to_string(header.record_size)+')');
		locale = header.locale;
		table_hash = header.table_hash;
		min_id = header.min_id;
		max_id = header.max_id;
		build = header.build;
		timestamp_last_written = header.timestamp_last_written;
		auto lmd([&p](auto &vec,std::size_t size)
		{
			auto b(reinterpret_cast<typename std::remove_reference_t<decltype(vec)>::const_pointer>(p));
			auto e(b+size);
			vec.assign(b,e);
			p=reinterpret_cast<const char*>(e);
		});
		p+=sizeof(dheader);
		if(max_id)
		{
			lmd(indices,max_id-min_id+1);
			lmd(string_lengths,max_id-min_id+1);
		}
		lmd(records,header.record_count);
		lmd(name,header.string_table_size);
	}
};

}
}