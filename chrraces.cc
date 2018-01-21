#include<iostream>
#include<exception>
#include<fstream>
#include"chrraces.h"
#include"creaturedisplayinfoextra.h"
#include"wdc1.h"
#include<iterator>
#include<random>
#include<unordered_map>

auto creaturedisplayinfoextra()
{
	std::ifstream fin("creaturedisplayinfoextra.db2",std::ifstream::binary);
	if(!fin)
		fin.exceptions(std::ifstream::failbit);
	std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
	db2::wdc1::wdc1<db2::creaturedisplayinfoextra> chr(s);
	std::unordered_map<db2::race,std::array<std::vector<std::uint32_t>,2>> m;
	for(const auto &ele : chr.records)
	{
		if(ele.sd)
			m[ele.r][static_cast<std::size_t>(ele.s)].emplace_back(ele.sd);
	}
	std::random_device rdv;
	std::vector<decltype(rdv)::result_type> seeds;
	for(std::size_t i(0);i!=1000;++i)
		seeds.emplace_back(rdv());
	std::seed_seq sq(seeds.cbegin(),seeds.cend());
	std::mt19937_64 eng(sq);
	const auto &records(chr.records);
	std::size_t count(0);
	for(std::size_t i(0);i!=chr.records.size();++i)
	{
		decltype(auto) ele(chr.records[i]);
		if(!ele.sd)
		{
			++count;
			const auto &f(m.at(ele.r).at(static_cast<std::size_t>(ele.s)));
			if(f.empty())
				throw std::runtime_error("empty");
			std::uniform_int_distribution<std::size_t> b(0,f.size()-1);
			ele.sd=f[b(eng)];
		}
	}
	std::cout<<"fixed "<<count<<" broken SD textures\n";
	return chr.serialize();
}

int main(int argc,char **argv)
{
	try
	{
		if(argc<2)
		{
			std::cerr<<"Usage: "<<*argv<<" <race name:ne|tauren|draenei|be> <race name> ....\n";
			return 1;
		}
		auto cr_str(creaturedisplayinfoextra());
		std::ifstream fin("chrraces.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		db2::wdc1::wdc1<db2::chrraces> chr(s);
		auto &records(chr.records);
		std::vector<std::string> args(argv+1,argv+argc);
		for(auto &ele : args)
		{
			for(auto &e : ele)
			{
				if('A'<=e&&e<='Z')
					e+='a'-'A';
			}
		}
		auto set_flag([&records](std::size_t pos,const std::string& str)
		{
			decltype(auto) flag((records.at(pos).m_flags));
			flag^=0x80;
			decltype(auto) cout_rdbuf(*std::cout.rdbuf());
			cout_rdbuf.sputn(str.data(),str.size());
			if(flag&0x80)
				std::cout<<" old models is now disabled\n";
			else
				std::cout<<" old models is now enabled\n";
		});
		using namespace std::string_literals;
		for(const auto &ele : args)
		{
			if(ele=="ne"s||ele=="nightelf"s)
				set_flag(3,"Night elf"s);
			else if(ele=="tauren"s)
				set_flag(6,"Tauren"s);
			else if(ele=="draenei"s)
				set_flag(10,"Draenei"s);
			else if(ele=="be"s||ele=="bloodelf"s)
				set_flag(9,"Blood elf"s);
		}
		std::ofstream fout("output.txt");
		for(std::size_t i(0);i!=records.size();++i)
		{
			fout<<i;
			switch(i)
			{
				case 3:
					fout<<"(Night elf)";
				break;
				case 6:
					fout<<"(Tauren)";
				break;
				case 9:
					fout<<"(Blood elf)";
				break;
				case 10:
					fout<<"(Draenei)";
				break;
			}
			fout<<":\t";
			fout<<records[i]<<'\n';
		}
		std::cout<<"Log has been saved to output.txt\n";
		{
		std::ofstream fout_cr("creaturedisplayinfoextra_out.db2",std::ofstream::binary);
		fout_cr.rdbuf()->sputn(cr_str.data(),cr_str.size());
		}
		{
		std::ofstream fout_chr("chrraces_out.db2",std::ofstream::binary);
		auto str(chr.serialize());
		fout_chr.rdbuf()->sputn(str.data(),str.size());
		}
		std::cout<<"Done\n";
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}