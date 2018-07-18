#include"spellvisualkit.h"
#include<exception>
#include<iostream>
#include<fstream>

int main()
{
try
{
	std::ifstream fin("spellvisualkit.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	const std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	db2::wdc1::wdc1<db2::spellvisualkit> chr(s);
	for(std::size_t i(0);i!=chr.records.size();++i)
		std::cout<<i<<' '<<chr.records[i]<<'\n';
}
catch(const std::exception &ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}
}