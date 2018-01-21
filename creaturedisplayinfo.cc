#include<iostream>
#include<exception>
#include<fstream>
#include"creaturedisplayinfo.h"
#include"wdc1.h"
#include"wdbc.h"
#include<iterator>

int main()
{
	try
	{
		{
		std::ifstream fin("creaturedisplayinfo.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		db2::wdc1::wdc1<db2::creaturedisplayinfo> chr(s);
		for(const auto &ele : chr.records)
		{
			std::cout<<ele<<'\n';
		}
		std::cout<<chr.records.size()<<'\n';
		}
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}