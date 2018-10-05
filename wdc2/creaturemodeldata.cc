#include"creaturemodeldata.h"
#include<array>
#include<exception>
#include<iostream>
#include<fstream>
#include"db2.h"
#include<string>

int main()
try
{
	std::ifstream fin("creaturemodeldata.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::creaturemodeldata> db2(raw);
	for(const auto &ele : db2.field_storages)
			std::cout<<ele<<'\n';
	std::cout<<db2.sections.size()<<'\n';

	decltype(auto) records(db2.sections.at(0).records);
	for(std::size_t i(0);i!=records.size();++i)
		std::cout<<i<<' '<<records[i]<<'\n';
//	records.at(49)
//		std::cout<<model_id(e1.records.at(56658))<<'\n';
//	std::cout<<db2.sections.size()<<'\n';

}
catch(const std::exception &e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}