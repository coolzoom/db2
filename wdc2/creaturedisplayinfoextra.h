#pragma once
#include"race.h"
#include<array>
#include<bitset>

namespace wdc2
{
struct creaturedisplayinfoextra
{
	std::uint8_t idk;
	std::uint8_t flags;
	std::array<std::uint8_t,2> c;
	struct
	{
		race r : 6;
		std::byte:3;
	//	std::byte r;
		sex s:1;
	}cn;
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
	return out<<"idk:"<<e.idk<<"\tflags:"<<static_cast<std::uint16_t>(e.flags)<<"\trace:"<<e.cn.r<<"\tsex:"<<e.cn.s<<"\tsd:"<<e.sd<<"\thd:"<<e.hd<<"\tunk:"<<e.unk;
}
}