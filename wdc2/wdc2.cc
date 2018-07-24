#include<exception>
#include<iostream>
#include<fstream>
#include<iterator>
#include"db2.h"
#include"chrraces.h"
#include"creaturedisplayinfoextra.h"
#include<random>
#include<unordered_map>

int main()
try
{
{
	std::ifstream fin("creaturedisplayinfoextra.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::creaturedisplayinfoextra> db2(raw);
//	for(std::size_t i(0);i!=creaturedisplayinfoextra.field_storages.size();++i)
//		std::cout<<i<<'\n'<<creaturedisplayinfoextra.field_storages[i]<<"\n\n\n";
	std::unordered_map<std::size_t,std::array<std::vector<std::uint16_t>,2>> m;
	for(auto &chr : db2.sections)
	for(const auto &ele : chr.records)
	{
		if(ele.sd)
			m[ele.a.r].at(static_cast<std::size_t>(ele.a.s)).emplace_back(ele.sd);
	}
	std::random_device rdv;
	std::vector<decltype(rdv)::result_type> seeds;
	for(std::size_t i(0);i!=1000;++i)
		seeds.emplace_back(rdv());
	std::seed_seq sq(seeds.cbegin(),seeds.cend());
	std::mt19937_64 eng(sq);
	std::size_t count(0);
	for(auto &chr : db2.sections)
		for(std::size_t i(0);i!=chr.records.size();++i)
		{
			decltype(auto) ele(chr.records[i]);
			if(!ele.sd&&ele.hd)
			{
				++count;
				const auto &f(m.at(ele.a.r).at(static_cast<std::size_t>(ele.a.s)));
				std::cout<<ele<<'\n';
				if(f.empty())
					throw std::runtime_error("empty");
				std::uniform_int_distribution<std::size_t> b(0,f.size()-1);
				ele.sd=f[b(eng)];
			}
		}
	std::cout<<"fixed "<<count<<" broken SD textures\n";
	std::ofstream fout("creaturedisplayinfoextra.db2.fixed",std::ofstream::binary);
	auto serializer(serialize(db2));
	fout.rdbuf()->sputn(serializer.data(),serializer.size());

}
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> db2(raw);
/*	std::cout<<db2.sections.front().string_table<<'\n';
	for(const auto &ele : db2.sections.front().records)
	{
		std::cout<<ele<<'\n';
	}
	for(std::size_t i(0);i!=db2.field_storages.size();++i)
		std::cout<<i<<'\n'<<db2.field_storages[i]<<"\n\n\n";
	for(const auto &ele : chrraces.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}
	auto ser(serialize(db2));*/
	decltype(auto) records(db2.sections.at(0).records);
	for(std::size_t i(0);i!=records.size();++i)
		std::cout<<i<<' '<<records[i]<<'\n';
	auto &ele = db2.sections.at(0).records.at(0);
	enable_old_model(ele);
	std::cout<<ele<<'\n';
	std::ofstream fout("chrraces.db2.fixed",std::ofstream::binary);
	auto serializer(serialize(db2));
	fout.rdbuf()->sputn(serializer.data(),serializer.size());
}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}