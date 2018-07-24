#pragma once
#include"race.h"
#include<array>
#include<bitset>

namespace wdc2
{
struct creaturedisplayinfoextra
{
//	std::uint8_t idk;
//	std::uint8_t flags;
	struct
	{
		uint8_t r:6;
		uint8_t s:2;		
	}a;
	std::array<std::uint8_t,5> c;

	std::uint16_t sd;
/*	race r;
	sex s;
	std::array<std::uint8_t,2> a;*/
	std::uint16_t hd;
	std::uint16_t unk;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	out<<rc(e.a.r)<<'\t'<<sx(e.a.s)<<"\tsd:"<<e.sd<<"\thd:"<<e.hd;
//	for(const auto &ele : e.c)
//		out<<static_cast<std::uint16_t>(ele)<<' ';
	return (out);
//	return out<<"idk:"<<e.idk<<"\tflags:"<<static_cast<std::uint16_t>(e.flags)<<"\trace:"<<e.cn.r<<"\tsex:"<<e.cn.s<<"\t"<<<<"\tunk:"<<e.unk;
}
}