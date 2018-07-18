#pragma once

#include<string>
#include<vector>
#include<cstdint>
#include<array>
#include<stdexcept>

namespace dbc
{
namespace wdbc
{
using namespace std::string_literals;
struct dheader
{
	std::uint32_t record_count;
	std::uint32_t field_count;                                         // array fields count as the size of array for WDB2
	std::uint32_t record_size;
	std::uint32_t string_table_size;                                   // string block almost always contains at least one zero-byte
};

template<typename T>
struct wdbc
{
	std::vector<T> records;
	std::string name;
	wdbc(const std::string &s)
	{
		if(s.front()!='W'||s[1]!='D'||s[2]!='B'||s[3]!='C')
			throw std::runtime_error("not WDBC which is ("s+s.substr(0,4)+")"s);
		auto p(s.data()+4);
		decltype(auto) header(*reinterpret_cast<const dheader*>(p));
		if(header.record_size!=sizeof(T))
			throw std::runtime_error("record_type incorrect size : ("s+std::to_string(sizeof(T))+") which should be("s+std::to_string(header.record_size)+')');
		auto lmd([&p](auto &vec,std::size_t size)
		{
			auto b(reinterpret_cast<typename std::remove_reference_t<decltype(vec)>::const_pointer>(p));
			auto e(b+size);
			vec.assign(b,e);
			p=reinterpret_cast<const char*>(e);
		});
		p+=sizeof(dheader);
		lmd(records,header.record_count);
		lmd(name,header.string_table_size);
	}
};

}
}