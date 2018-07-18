#include<iostream>
#include<exception>
#include<fstream>
#include"creaturedisplayinfoextra.h"
#include"wdc1.h"
#include<iterator>
#include<unordered_map>
#include<vector>
#include<map>
#include<random>

int main()
{
try
{
	std::ifstream fin("creaturedisplayinfoextra.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	db2::wdc1::wdc1<db2::creaturedisplayinfoextra> chr(s);
	std::size_t count(0);
	for(const auto &ele : chr.records)
	{
		if(!ele.sd)
			std::cout<<count++<<' '<<ele<<'\n';
	}
	std::cout<<count<<' '<<chr.records.size()<<' '<<100*static_cast<double>(count)/chr.records.size()<<"%\n";
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}
}