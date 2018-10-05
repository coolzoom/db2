#include"modelfiledata.h"
#include<array>
#include<exception>
#include<iostream>
#include<fstream>
#include"db2.h"
#include<string>

int main()
try
{
	std::ifstream fin("modelfiledata.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::modelfiledata> db2(raw);
/*	for(const auto &ele : db2.field_storages)
			std::cout<<ele<<'\n';
	std::cout<<db2.sections.size()<<'\n';*/
	for(const auto &e1 : db2.sections)
		for(std::size_t i(0);i!=e1.records.size();++i)
		{
			if(file_id(e1.records[i])==119563)
				std::cout<<"sd:"<<i<<' '<<e1.records[i]<<'\n';
			else if(file_id(e1.records[i])==1000764)
				std::cout<<"hd:"<<i<<' '<<e1.records[i]<<'\n';
		}
}
catch(const std::exception &e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}