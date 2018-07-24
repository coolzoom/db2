#include<iostream>
#include<exception>
#include<fstream>
#include"charsections.h"
#include"wdc1.h"
#include<iterator>

int main()
{
	try
	{
		std::ifstream fin("charsections.db2",std::ifstream::binary);
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::string s((std::istreambuf_iterator<char>(fin)),std::istreambuf_iterator<char>());
		db2::wdc1::wdc1<db2::charsections> chr(s);
		
		auto lmd([&chr](const db2::charsections::field_type f)
		{
			for(std::size_t i(0);i!=chr.records.size();++i)
			{
				const auto &ele(chr.records[i]);
				if(ele.race==db2::race::bloodelf&&
					(f==ele.field||ele.field==static_cast<db2::charsections::field_type>(static_cast<std::uint8_t>(f)+5)))
				{
					std::cout<<i<<' '<<ele<<'\n';
				}
			}
			std::cout.put('\n');
		});
		std::vector<std::size_t> places;
		for(std::size_t i(0);i!=chr.records.size();++i)
		{
			const auto &ele(chr.records[i]);
			if(ele.race==db2::race::tauren)
			{
				places.emplace_back(i);
			}
		}
		std::vector<db2::charsections> bloodelves;
		for(std::size_t i(0);i!=chr.records.size();++i)
		{
			const auto &ele(chr.records[i]);
			if(ele.race==db2::race::bloodelf)
			{
				bloodelves.emplace_back(ele);
			}
		}

		std::vector<db2::charsections> vec;
		for(const auto &ele : bloodelves)
		{	
			if(ele.c[1]<6&&ele.field==db2::charsections::field_type::skin)
			{
				vec.emplace_back(ele);
				vec.back().flag.value =33;
				vec.back().c[1]+=19;
			}
		}
		
		for(const auto &ele : bloodelves)
		{	
			if(ele.c.front()<6&ele.c[1]<6&&ele.field==db2::charsections::field_type::face)
			{
				vec.emplace_back(ele);
				vec.back().c[1]+=19;
				vec.back().flag.value = 33;
			}
		}
		std::vector<db2::charsections> cvec;
		std::size_t j(0);
		for(std::size_t i(0);i!=chr.records.size();++i)
		{
			if(i==places.at(j))
			{
				if(++j==vec.size())
				{
					for(++i;i!=chr.records.size();++i)
						cvec.emplace_back(chr.records[i]);
					break;
				}
			}
			else
			{
				cvec.emplace_back(chr.records[i]);
			}
		}
		cvec.insert(cvec.end(),vec.cbegin(),vec.cend());
		chr.records=std::move(cvec);
		for(std::uint8_t i(0);i!=5;++i)
			lmd(static_cast<db2::charsections::field_type>(i));
		std::ofstream fout("D:\\blizzard\\World of Warcraft\\dbfilesclient\\charsections.db2",std::ofstream::binary);
		auto ser(chr.serialize());
		fout.rdbuf()->sputn(ser.data(),ser.size());
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}	
}