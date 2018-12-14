#pragma once
#include<array>
#include"unknown.h"

namespace wdc3
{

struct creaturedisplayinfoextra:unknown<12>{};

inline auto raceid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,0,6);
}

inline void raceid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,0,6,u);
}
inline auto sexid(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,6,2);
}

inline void sexid(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,6,2,u);
}
inline auto sd(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,42,21);
}

inline void sd(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,42,21,u);
}


inline auto hd(creaturedisplayinfoextra const& e)
{
	return get<std::uint32_t>(e,63,21);
}

inline void hd(creaturedisplayinfoextra& e,std::uint32_t u)
{
	set(e,63,21,u);
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	return out<<raceid(e)<<'\t'<<sexid(e)<<"\tsd:"<<sd(e)<<"\thd:"<<hd(e);
}

}