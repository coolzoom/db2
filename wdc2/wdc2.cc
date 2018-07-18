#include<exception>
#include<iostream>
#include<fstream>
#include<iterator>
#include"db2.h"
#include"chrraces.h"
#include"creaturedisplayinfoextra.h"


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
/*	for(const auto &ele : db2.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}*/
	std::cout<<db2.sections.front().records.size()<<'\n';
	auto ser(serialize(db2));
	if(raw==ser)
		std::cout<<"OK\n";
	else
	{
		std::cout<<"NO\nRaw:"<<raw.size()<<" Ser:"<<ser.size()<<'\n';
/*		if(raw.size()==ser.size())
		{
			for(std::size_t i(0);i!=raw.size();++i)
				if(raw[i]!=ser[i])
				{
					std::cout<<i<<' '<<static_cast<int>(raw[i])<<' '<<static_cast<int>(ser[i])<<'\n';
				}
		}*/
	}
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
	
/*	if(raw==ser)
		std::cout<<"OK\n";
	else
	{
		std::cout<<"NO\nRaw:"<<raw.size()<<" Ser:"<<ser.size()<<'\n';
		if(raw.size()==ser.size())
		{
			for(std::size_t i(0);i!=raw.size();++i)
				if(raw[i]!=ser[i])
				{
					std::cout<<i<<' '<<static_cast<std::size_t>(raw[i])<<' '<<static_cast<std::size_t>(ser[i])<<'\n';
				}
		}
	}*/
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}