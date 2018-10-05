#pragma once
#include<cstdint>
#include<array>
#include"raw.h"
#include"unknown.h"

namespace wdc2
{

struct creaturemodeldata:unknown<52>{};

inline auto file_id(const creaturemodeldata& e)
{
	return get<std::uint32_t>(e,199,22);
}

inline void file_id(creaturemodeldata& e,std::uint32_t u)
{
	 set(e,199,22,u);
}

inline auto id(const creaturemodeldata& e)
{
	return get<std::uint32_t>(e,0,32);
}

inline void id(creaturemodeldata& e,std::uint32_t u)
{
	 set(e,0,32,u);
}

template<typename ostrm>
inline decltype(auto) operator<<(ostrm& out,const creaturemodeldata& e)
{
	out<<"FILE ID:"<<file_id(e);
	return (out);
}

}