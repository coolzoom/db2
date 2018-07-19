#pragma once
#include"race.h"

namespace wdc2
{
struct creaturedisplayinfoextra
{
	std::uint8_t idk;
	std::uint8_t flags;
	std::uint16_t c;
	race r;
	sex s;
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
	return out<<"idk:"<<static_cast<std::uint16_t>(e.idk)<<"\tflags:"<<static_cast<std::uint16_t>(e.flags)<<"\trace:"<<e.r<<"\tsex:"<<e.s<<"\tsd:"<<e.sd<<"\thd:"<<e.hd<<"\tunk:"<<e.unk;
}
}