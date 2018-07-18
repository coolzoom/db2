#include"db2.h"
#include"chrraces.h"
#include<exception>
#include<iostream>
#include<fstream>
#include<iterator>

int main()
try
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	wdc2::db2<wdc2::chrraces> chrraces(std::string((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>()));
	for(const auto &ele : chrraces.fields)
		std::cout<<ele<<'\n';
	for(const auto &ele : chrraces.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}