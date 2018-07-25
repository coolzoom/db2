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
#include<cctype>

int main(int argc,char **argv)
try
{
	if(argc==1)
	{
		std::cerr<<"Usage: "<<*argv<<" <race name1> <race name2>...\n";
		return 1;
	}
	
	std::size_t count(0);
	std::string cre;
{
	std::ifstream fin("creaturedisplayinfoextra.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::creaturedisplayinfoextra> db2(raw);
	std::unordered_map<std::uint8_t,std::vector<std::uint32_t>> m;
	std::unordered_map<std::uint16_t,std::vector<std::uint32_t>> m_class;
	for(auto &chr : db2.sections)
	for(const auto &ele : chr.records)
	{
		auto sd(wdc2::get<std::uint32_t>(ele,41,20));
		if(sd)
		{
			m[wdc2::get<std::uint8_t>(ele,0,8)].emplace_back(sd);
			m_class[wdc2::get<std::uint16_t>(ele,0,12)].emplace_back(sd);
		}
	}
	std::random_device rdv;
	std::vector<decltype(rdv)::result_type> seeds;
	for(std::size_t i(0);i!=1000;++i)
		seeds.emplace_back(rdv());
	std::seed_seq sq(seeds.cbegin(),seeds.cend());
	std::mt19937_64 eng(sq);
	for(auto &chr : db2.sections)
		for(std::size_t i(0);i!=chr.records.size();++i)
		{
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
				}
				else
				{
					std::uniform_int_distribution<std::size_t> b(0,cit->second.size()-1);
					wdc2::set(ele,41,20,cit->second[b(eng)]);
				}
			}
		}
	cre.assign(serialize(db2));
}
{
	std::ifstream fin("chrraces.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string raw((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	wdc2::db2<wdc2::chrraces> db2(raw);
	decltype(auto) records(db2.sections.at(0).records);
	for(std::size_t i(1);i<argc;++i)
	{
		std::string s(argv[i]);
		for(auto &ele : s)
			ele=std::tolower(ele);
		if(s=="human"||s=="worgen")
		{
			enable_old_model(records.at(0));	//enable human
			enable_old_model(records.at(22));	//enable worgen since they share same files		
		}
		else if(s=="dwarf")
			enable_old_model(records.at(2));	//enable ne
		else if(s=="ne")
			enable_old_model(records.at(3));	//enable ne
		else if(s=="gnome")
			enable_old_model(records.at(6));	//enable ne
		else if(s=="bloodelf")
			enable_old_model(records.at(9));	//enable be
		else if(s=="draenei")
			enable_old_model(records.at(10));	//enable draenei
		else
		{
			std::cerr<<"race: "<<s<<" does not exist\n";
			return 1;
		}
		std::cout<<s<<'\n';
		
	}
	std::cout<<"fixed "<<count<<" broken SD textures\n";
	auto serializer(serialize(db2));
	{
	std::ofstream fout("creaturedisplayinfoextra.db2.fixed",std::ofstream::binary);
	fout.rdbuf()->sputn(cre.data(),cre.size());
	}
	{
	std::ofstream fout("chrraces.db2.fixed",std::ofstream::binary);
	fout.rdbuf()->sputn(serializer.data(),serializer.size());
	}
}
}
catch(const std::exception& e)
{
	std::cerr<<e.what()<<'\n';
	return 1;
}