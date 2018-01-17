#include<iostream>
#include<exception>
#include<fstream>
#include"chrraces.h"
#include"wdc1.h"

int main()
{
	try
	{
		std::ifstream fin("chrraces.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		decltype(auto) rdbuf(*fin.rdbuf());
		std::string s;
		for(int ch;(ch=rdbuf.sbumpc())!=EOF;s.push_back(ch));
		db2::wdc1::wdc1<db2::chrraces> chr(s);
		auto b(chr.serialize());
		db2::wdc1::wdc1<db2::chrraces> chr2(b);
		auto c(chr2.serialize());
		std::cout<<s.size()<<' '<<b.size()<<' '<<c.size()<<'\n';
		std::cout<<(b==c)<<'\n';
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}