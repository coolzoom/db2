#include<iostream>
#include<exception>
#include<fstream>
#include"texturefiledata.h"
#include"wdc1.h"
#include<iterator>

int main()
{
	try
	{
		std::ifstream fin("texturefiledata.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		static_assert(sizeof(db2::texturefiledata)==6,"not ok");
		db2::wdc1::wdc1<db2::texturefiledata> chr(s);
		std::cout<<chr.records.size()<<'\n';
		for(const auto &ele : chr.records)
		{
			std::cout<<ele<<'\n';
//			std::cout<<ele<<'\n';
//			std::cout<<chr.name.at(ele.name_pos)<<'\n';
		}
//		std::cout<<(chr.serialize()==s)<<'\n';
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}