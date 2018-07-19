#include<exception>
#include<iostream>
#include<fstream>
#include<iterator>
#include"db2.h"
#include"chrraces.h"
#include"creaturedisplayinfoextra.h"
#include<random>

int main()
try
{
{
	std::ifstream fin("creaturedisplayinfoextra.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::creaturedisplayinfoextra> db2(raw);
//	for(std::size_t i(0);i!=creaturedisplayinfoextra.field_storages.size();++i)
//		std::cout<<i<<'\n'<<creaturedisplayinfoextra.field_storages[i]<<"\n\n\n";
	std::size_t pos(0),counter(0);
	for(auto &ele : db2.sections)
		for(auto &ele1 : ele.records)
		{
			if(ele1.sd==0&&ele1.hd!=0)
			{
				std::cout<<pos<<' '<<ele1<<'\n';
				++counter;
			}
			++pos;
		}
	std::cout<<"total: "<<pos<<" broken old model references:"<<counter<<'\n';
	auto ser(serialize(db2));
}
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> db2(raw);
//	for(std::size_t i(0);i!=db2.field_storages.size();++i)
//		std::cout<<i<<'\n'<<db2.field_storages[i]<<"\n\n\n";
/*	for(const auto &ele : chrraces.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}
	auto ser(serialize(db2));*/
}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}