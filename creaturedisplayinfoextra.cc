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
		std::ofstream fout("creaturedisplayinfoextra_out.db2",std::ofstream::binary);
		auto str(chr.serialize());
		fout.rdbuf()->sputn(str.data(),str.size());
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}