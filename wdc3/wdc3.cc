#include"db2_io.h"
#include<exception>
#include<iostream>
#include"creaturedisplayinfoextra.h"
#include"creaturedisplayinfo.h"
#include"chrraces.h"
#include<unordered_map>
#include<unordered_set>
#include<vector>

inline auto convert(std::vector<wdc3::creaturedisplayinfo> &records)
{
	std::unordered_map<std::uint32_t,std::size_t> um;
	for(std::size_t i(0);i!=records.size();++i)
	{
		auto &ele(records[i]);
		um[id(ele)]=i;
	}
	return um;
}
int main(int argc,char** argv)
try
{
	if(argc==1)
	{
		std::cerr<<"Usage: "<<*argv<<" <race name1> <race name2>...\n";
		return 1;
	}
	wdc3::db2<wdc3::creaturedisplayinfoextra> creaturedisplayinfoextra(wdc3::read_db2("creaturedisplayinfoextra.db2"));
	std::size_t gg(0);
	wdc3::db2<wdc3::creaturedisplayinfo> creaturedisplayinfo(wdc3::read_db2("creaturedisplayinfo.db2"));
	auto creature_id_to_index(convert(creaturedisplayinfo.sections.at(0).records));
	std::unordered_set<std::uint32_t> uset;
	std::unordered_set<std::uint32_t> both;
	{
		decltype(auto) sec(creaturedisplayinfoextra.sections.at(0));
		for(std::size_t i(0);i!=sec.records.size();++i)
		{
			auto &ele(sec.records[i]);
			if(!sd(ele)&&hd(ele))
			{
				sd(ele,hd(ele));
				uset.emplace(sec.ids.at(i));
			}
			else if(sd(ele)&&hd(ele))
			{
				both.emplace(sec.ids.at(i));
			}
		}
	}
	std::unordered_map<std::uint32_t,std::uint32_t> new_to_old_map;
	std::unordered_map<std::uint32_t,std::uint32_t> old_to_new_map;
	wdc3::db2<wdc3::chrraces> chrraces(wdc3::read_db2("chrraces.db2"));
	{
	auto &records(chrraces.sections.at(0).records);
	auto &cr(creaturedisplayinfo.sections.at(0).records);
	for(auto const &e : records)
	{
		auto hdm(hd_male(e)),hdfm(hd_female(e));
		if(hdm)
		{
			auto sd(model(cr.at(creature_id_to_index.at(sd_male(e)))));
			auto hd(model(cr.at(creature_id_to_index.at(hdm))));
			old_to_new_map[sd]=hd;
			new_to_old_map[hd]=sd;
		}
		if(hdfm)
		{
			auto sd(model(cr.at(creature_id_to_index.at(sd_female(e)))));
			auto hd(model(cr.at(creature_id_to_index.at(hdfm))));
			old_to_new_map[sd]=hd;
			new_to_old_map[hd]=sd;
		}
	}
	std::size_t incorrect_hd_sd_textures(0);
	for(std::size_t i(0);i!=cr.size();++i)
	{
		auto &e(cr.at(i));
		auto ex(extra(e));
		if(ex&&both.count(ex))
		{
			auto it(new_to_old_map.find(model(e)));
			if(it!=new_to_old_map.cend())
			{
				model(e,it->second);
				++incorrect_hd_sd_textures;
			}
		}
	}
	std::cout<<"fixed "<<incorrect_hd_sd_textures<<" incorrect hd sd textures \n";
	for(auto &e : cr)
	{
		auto ex(extra(e));
		if(ex&&uset.count(ex))
		{
			auto it(old_to_new_map.find(model(e)));
			if(it!=old_to_new_map.cend())
			{
				model(e,it->second);
			}
		}
	}

	for(std::size_t i(1),argc_s(argc);i<argc_s;++i)
	{
		std::string s(argv[i]);
		for(auto &ele : s)
			ele=std::tolower(ele);
		if(s=="hum"||s=="human"||s=="worgen")
		{
			enable_old_model(records.at(0));	//enable human
			enable_old_model(records.at(22));	//enable worgen since they share same files		
		}
//		else if(s=="orc")
//		{
//			enable_old_model(records.at(1));	//enable orc not usable
//			upright_orc = true;				//disable upright orc
//		}
		else if(s=="dwarf")
			enable_old_model(records.at(2));	//enable dwarf
		else if(s=="ne"||s=="nightelf")
			enable_old_model(records.at(3));	//enable ne
//		else if(s=="tauren")
//			enable_old_model(records.at(5));	//enable tauren NOT USABLE
		else if(s=="gnome")
			enable_old_model(records.at(6));	//enable gnome
		else if(s=="troll")
			enable_old_model(records.at(7));	//enable troll
		else if(s=="bloodelf"||s=="be")
			enable_old_model(records.at(9));	//enable be
		else if(s=="draenei")
			enable_old_model(records.at(10));	//enable draenei
//		else if(s=="undead")
//			enable_old_model(records.at(4));	//enable undead
		else
		{
			std::cerr<<"race: "<<s<<" does not exist\n";
			return 1;
		}
		std::cout<<s<<'\n';
	}
	}
	std::cout<<"fixed "<<uset.size()<<" broken SD textures\n";
	wdc3::write_db2("chrraces.db2.fixed",chrraces);
	wdc3::write_db2("creaturedisplayinfo.db2.fixed",creaturedisplayinfo);
	wdc3::write_db2("creaturedisplayinfoextra.db2.fixed",creaturedisplayinfoextra);
}
catch(std::exception const&ex)
{
	std::cerr<<ex.what()<<'\n';
	return 1;
}