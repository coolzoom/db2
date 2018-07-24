#pragma once

#include<cstdint>
#include"race.h"

namespace wdc2
{
struct racesex
{
	std::uint8_t race:6;
	std::uint8_t sex:2;
};

inline auto rc(const racesex& r)
{
	return static_cast<race>(r.race);
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const racesex& e)
{
	return out<<race(e.race)<<' '<<sex(e.sex);
}

}