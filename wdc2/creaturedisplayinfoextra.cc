#include"creaturedisplayinfoextra.h"
#include<array>
#include<exception>
#include<iostream>
#include<fstream>
#include"db2.h"
#include<string>
inline auto read_db2(std::string const& str)
{
	std::ifstream fin(str,std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	return std::string(std::istreambuf_iterator<char>(fin),std::istreambuf_iterator<char>());
}

template<typename T>
inline void write_db2(std::string const& str,wdc2::db2<T> const &w)
{
	auto sr(serialize(w));
	std::ofstream fout(str,std::ofstream::binary);
	fout.rdbuf()->sputn(sr.data(),sr.size());
}


int main()
try
{
	wdc2::db2<wdc2::creaturedisplayinfoextra> extra(read_db2("creaturedisplayinfoextra.db2"));
	std::cout<<extra<<'\n';
	decltype(auto) sections(extra.sections.at(0));
	decltype(auto) records(sections.records);
	decltype(auto) ids(sections.ids);
		std::unordered_map<std::size_t,std::size_t> umap;
	for(std::size_t i(0);i!=extra_ids.size();++i)
		umap[extra_ids[i]]=i;
	
//	std::cout<<records.size()<<' '<<ids.size()<<'\n';

}
catch(const std::exception &e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}