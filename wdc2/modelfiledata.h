#pragma once
#include<cstdint>
#include<array>
#include"raw.h"
#include"unknown.h"

namespace wdc2
{

struct modelfiledata:unknown<5>{};

inline auto file_id(const modelfiledata& e)
{
	return get<std::uint32_t>(e,0,23);
}

inline auto model_id(const modelfiledata& e)
{
	return get<std::uint16_t>(e,23,16);
}

inline void file_id(modelfiledata& e,std::uint32_t u)
{
	set(e,0,23,u);
}

inline void model_id(modelfiledata& e,std::uint16_t u)
{
	 set(e,23,16,u);
}


template<typename ostrm>
inline decltype(auto) operator<<(ostrm& out,const modelfiledata& e)
{
	out<<"FILE ID:"<<file_id(e)<<"\tMODEL ID:"<<model_id(e);
	return (out);
}

}