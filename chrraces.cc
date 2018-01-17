#include<iostream>
#include<exception>
#include<fstream>
#include"chrraces.h"
#include"wdc1.h"
#include<iterator>

int main()
{
	try
	{
		std::ifstream fin("chrraces.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		db2::wdc1::wdc1<db2::chrraces> chr(s);
/*		for(const auto &ele : chr.records)
		{
			std::cout<<ele<<'\n';
		}*/
//		std::cout<<chr.name<<'\n';
//		std::cout<<chr.ids.size()<<'\n';
		auto b(chr.serialize());
		db2::wdc1::wdc1<db2::chrraces> chr2(b);
//		std::cout<<chr2.name<<'\n';
		auto c(chr2.serialize());
		std::cout<<s.size()<<' '<<b.size()<<' '<<c.size()<<'\n';
		std::cout<<(s==b)<<' '<<(b==c)<<'\n';
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}