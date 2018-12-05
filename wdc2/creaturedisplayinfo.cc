#include"creaturedisplayinfo.h"
#include"creaturedisplayinfoextra.h"
#include<array>
#include<exception>
#include<iostream>
#include<fstream>
#include"db2.h"
#include<string>
#include<unordered_map>
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
	wdc2::db2<wdc2::creaturedisplayinfo> db2(read_db2("creaturedisplayinfo.db2"));
	wdc2::db2<wdc2::creaturedisplayinfoextra> extra(read_db2("creaturedisplayinfoextra.db2.fixed"));

	auto &records(db2.sections.at(0).records);
	auto &extra_section(extra.sections.at(0));
	decltype(auto) extra_ids(extra_section.ids);

	auto &extra_records(extra_section.records);
	std::unordered_map<std::size_t,std::size_t> umap;
	for(std::size_t i(0);i!=extra_ids.size();++i)
		umap[extra_ids[i]]=i;
	for(std::size_t i(0);i!=records.size();++i)
	{
		auto &ele(records[i]);
		if(wdc2::extra(ele)!=0)
		{
			switch(model(ele))
			{
			case 7599:	//female
				model(ele,50);
			break;
			case 7661:	//male
				model(ele,49);
			break;
			}
			std::cout<<ele<<'\n';
		}
	}
//	write_db2("creaturedisplayinfo.db2.fixed",db2);
//	std::cout<<i<<' '<<records[i]<<'\n';

//	records.at(49)
//		std::cout<<model_id(e1.records.at(56658))<<'\n';
//	std::cout<<db2.sections.size()<<'\n';

}
catch(const std::exception &e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}