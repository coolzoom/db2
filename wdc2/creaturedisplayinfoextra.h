#pragma once
#include"race.h"

namespace wdc2
{
struct creaturedisplayinfoextra
{
	std::uint32_t sd;
	std::uint32_t hd;
/*	race r;
	sex s;
	std::array<std::uint8_t,2> a;*/
	std::uint32_t uk;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	return out<<"sd:"<<e.sd<<"\thd:"<<e.hd<<"\t"<<e.uk;
}
}