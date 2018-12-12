#pragma once
#include"db2.h"
#include<fstream>

namespace wdc3
{
inline auto read_db2(std::string const& str)
{
	std::ifstream fin(str,std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	return std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}

template<typename T>
inline void write_db2(std::string const& str,db2<T> const &w)
{
	auto sr(serialize(w));
	std::ofstream fout(str,std::ofstream::binary);
	fout.rdbuf()->sputn(sr.data(),sr.size());
}

}