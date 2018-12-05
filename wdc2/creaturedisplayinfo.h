#pragma once
#include<cstdint>
#include<array>
#include"raw.h"
#include"unknown.h"

namespace wdc2
{

struct creaturedisplayinfo:unknown<13>{};

inline auto model(creaturedisplayinfo const& e)
{
	return get<std::uint32_t>(e,18,14);
}

inline void model(creaturedisplayinfo& e,std::uint32_t u)
{
	 set(e,18,14,u);
}

inline auto id(creaturedisplayinfo const& e)
{
	return get<std::uint32_t>(e,0,18);
}

inline void id(creaturedisplayinfo& e,std::uint32_t u)
{
	 set(e,0,18,u);
}

inline auto extra(creaturedisplayinfo const& e)
{
	return get<std::uint32_t>(e,43,19);
}

inline void extra(creaturedisplayinfo& e,std::uint32_t u)
{
	 set(e,43,19,u);
}


template<typename ostrm>
inline decltype(auto) operator<<(ostrm& out,creaturedisplayinfo const& e)
{
	out<<"id:"<<id(e)<<"\tmodel:"<<model(e)<<"\textra:"<<extra(e)<<'\n';
	return (out);
}

}