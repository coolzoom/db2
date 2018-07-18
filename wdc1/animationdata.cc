#include"animationdata.h"
#include<iostream>
#include<exception>
#include<fstream>
#include"wdc1.h"

int main()
{
try
{
	std::ifstream fin("animationdata.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	db2::wdc1::wdc1<db2::animationdata> chr(s);
	for(std::size_t i(0);i!=chr.records.size();++i)
		std::cout<<i<<' '<<chr.records[i]<<'\n';
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}
}