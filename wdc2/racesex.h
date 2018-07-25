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

inline decltype(auto) rcsx(std::uint8_t& r)
{
	return reinterpret_cast<racesex&>(r);
}

inline decltype(auto) rcsx(const std::uint8_t& r)
{
	return reinterpret_cast<const racesex&>(r);
}

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