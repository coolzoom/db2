#include<fstream>
#include<string>
#include<exception>
#include<vector>
#include<algorithm>
#include<string_view>
#include<iostream>

int main()
{
	try
	{
		std::ifstream fin("listfile.txt");
		if(!fin)
			fin.exceptions(std::ifstream::failbit);
		std::vector<std::string> vec;
		using namespace std::string_view_literals;
		constexpr auto strv("textures/bakednpctextures/creaturedisplayextra-"sv);
		for(std::string s;std::getline(fin,s);)
			if(s.find(strv)!=std::string::npos)
				vec.emplace_back(std::move(s));
		sort(vec.begin(),vec.end());
		std::ofstream fout("out.txt");
		for(const auto &ele : vec)
			if(ele.find("_hd.blp"sv)!=std::string::npos)
			{
				std::string d(ele.cbegin(),ele.cend()-7);
				d.append(".blp"sv);
				if(!std::binary_search(vec.cbegin(),vec.cend(),d))
					fout<<ele<<" lack of "<<d<<'\n';
			}
	}
	catch(const std::exception& e)
	{
		std::cerr<<e.what()<<'\n';
		return 1;
	}
}