#pragma once
#include<array>
#include"race.h"

namespace db2
{
struct creaturedisplayinfoextra
{
	std::uint32_t sd;
	std::uint32_t hd;
	race r;
	sex s;
	std::array<std::uint8_t,10> a;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	return out<<"race:"<<e.r<<"\tsex:"<<e.s<<"\tsd:"<<e.sd<<"\thd:"<<e.hd;
}
}

namespace dbc
{
struct creaturedisplayinfoextra
{
	std::uint32_t id;
	race r;
	std::uint32_t sex;
	std::uint32_t skin;
	std::uint32_t face;
	std::uint32_t hairstyle;
	std::uint32_t haircolor;
	std::uint32_t facialhair;
	std::array<uint32_t,11> npcitemdisplay;
	std::uint32_t flags;
	std::uint32_t sd;
	std::uint32_t hd;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	return out<<"race:"<<e.r<<"\tsd:"<<e.sd<<"\thd:"<<e.hd;
}
}