#include<iostream>
#include<exception>
#include<fstream>
#include"modelfiledata.h"
#include"wdc1.h"
#include<iterator>

int main()
{
	try
	{
		std::ifstream fin("modelfiledata.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		db2::wdc1::wdc1<db2::modelfiledata> chr(s);
		std::cout<<chr.records.size()<<'\n';
		for(const auto &ele : chr.fields)
		{
			std::cout<<ele<<'\n';
//			std::cout<<std::uint32_t(ele.q)<<'\n';
		}
//		std::cout<<(chr.serialize()==s)<<'\n';
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}