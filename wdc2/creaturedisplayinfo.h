#pragma once
#include<cstdint>
#include<array>
#include"raw.h"
#include"unknown.h"

namespace wdc2
{

struct creaturedisplayinfo:unknown<13>{};

inline auto model_id(const creaturedisplayinfo& e)
{
	return get<std::uint32_t>(e,18,14);
}

inline void model_id(creaturedisplayinfo& e,std::uint32_t u)
{
	 set(e,18,14,u);
}

inline auto id(const creaturedisplayinfo& e)
{
	return get<std::uint32_t>(e,0,18);
}

inline void id(creaturedisplayinfo& e,std::uint32_t u)
{
	 set(e,0,18,u);
}

template<typename ostrm>
inline decltype(auto) operator<<(ostrm& out,const creaturedisplayinfo& e)
{
	out<<"ID:"<<id(e)<<"\tMODEL ID:"<<model_id(e);
	return (out);
}

}