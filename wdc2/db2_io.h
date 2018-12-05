#pragma once
#include<fstream>
#include"db2.h"

namespace wdc2
{
inline auto read_db2(std::string const& str)
{
	std::ifstream fin(str,std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	return std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}

inline auto write_db2(std::string const& str)
{
	std::ofstream fout(str,std::ifstream::binary);
	if(!fout)
		fout.exceptions(std::ifstream::failbit);
	return std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}
}