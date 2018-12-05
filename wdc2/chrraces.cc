#include"chrraces.h"
#include<array>
#include<exception>
#include<iostream>
#include<fstream>
#include"db2.h"
#include<string>

int main()
try
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> db2(raw);
//	for(const auto &ele : db2.sections.at(0).records)
//			std::cout<<ele<<'\n';
	std::cout<<db2.sections.size()<<'\n';
	std::size_t pos(0);
	for(const auto &ele : db2.sections.front().records)
	{
//		if(file_id(ele)==119563)
			std::cout<<pos<<' '<<ele<<'\n';
		++pos;
	}
}
catch(const std::exception &e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}