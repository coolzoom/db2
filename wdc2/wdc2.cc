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
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> chrraces(raw);
/*	for(const auto &ele : chrraces.fields)
		std::cout<<ele<<'\n';
	for(const auto &ele : chrraces.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}*/
	auto ser(serialize(chrraces));
	if(raw==ser)
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
	}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}