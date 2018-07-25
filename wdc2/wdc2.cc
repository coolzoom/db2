#include<exception>
#include<iostream>
#include<fstream>
#include<iterator>
#include"db2.h"
#include"chrraces.h"
#include"creaturedisplayinfoextra.h"
#include"unknown.h"
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
	std::unordered_map<std::uint8_t,std::vector<std::uint32_t>> m;
	std::unordered_map<std::uint16_t,std::vector<std::uint32_t>> m_class;
	for(auto &chr : db2.sections)
	for(const auto &ele : chr.records)
	{
//		decltype(auto) e(ele);//wdc2::to<wdc2::creaturedisplayinfoextra>(ele));
		auto sd(wdc2::get<std::uint32_t>(ele,41,20));
		if(sd)
		{
//			std::cout<<sd<<'\n';
			m[wdc2::get<std::uint8_t>(ele,0,8)].emplace_back(sd);
			m_class[wdc2::get<std::uint16_t>(ele,0,12)].emplace_back(sd);
		}
//		else
//			std::cout<<ele<<'\n';
	}
/*	for(const auto &ele : m)
		std::cout<<ele.first<<' '<<ele.second<<'\n';*/
	
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
//			decltype(auto) ele(wdc2::to<wdc2::creaturedisplayinfoextra>(chr.records[i]));
			decltype(auto) ele(chr.records[i]);
			auto sd(wdc2::get<std::uint32_t>(ele,41,20)),
				hd(wdc2::get<std::uint32_t>(ele,61,20));

			if(!sd&&hd)
			{
				++count;
				auto cit (m_class.find(wdc2::get<std::uint8_t>(ele,0,12)));
				if(cit==m_class.cend())
				{
					const auto &f(m.at(wdc2::get<std::uint8_t>(ele,0,8)));
					std::uniform_int_distribution<std::size_t> b(0,f.size()-1);
					wdc2::set(ele,41,20,f[b(eng)]);
					std::cout<<i<<ele<<'\n';
				}
				else
				{
					std::uniform_int_distribution<std::size_t> b(0,cit->second.size()-1);
					wdc2::set(ele,41,20,cit->second[b(eng)]);
					std::cout<<i<<" class "<<ele<<'\n';
				}
			}
		}
	std::cout<<"fixed "<<count<<" broken SD textures\n";
	auto serializer(serialize(db2));
	std::ofstream fout("creaturedisplayinfoextra.db2.fixed",std::ofstream::binary);
	fout.rdbuf()->sputn(serializer.data(),serializer.size());

}
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> db2(raw);
	std::cout<<db2.sections.front().string_table<<'\n';
	for(const auto &ele : db2.sections.front().records)
	{
		std::cout<<ele<<'\n';
	}
	for(std::size_t i(0);i!=db2.field_storages.size();++i)
		std::cout<<i<<'\n'<<db2.field_storages[i]<<"\n\n\n";
	for(const auto &ele : db2.sections)
		for(std::size_t i(0);i!=ele.records.size();++i)
		{
			std::cout<<i<<' '<<ele.records[i]<<'\n';
		}
	auto ser(serialize(db2));
	decltype(auto) records(db2.sections.at(0).records);
	for(std::size_t i(0);i!=records.size();++i)
		std::cout<<i<<' '<<records[i]<<'\n';
	auto &ele = db2.sections.at(0).records.at(0);
	enable_old_model(ele);
	std::cout<<ele<<'\n';
	auto serializer(serialize(db2));
	std::ofstream fout("chrraces.db2.fixed",std::ofstream::binary);
	fout.rdbuf()->sputn(serializer.data(),serializer.size());
}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}