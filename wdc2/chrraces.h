#pragma once
#include<array>

namespace wdc2
{

struct chrraces
{
	std::array<uint32_t,7> bf;
	std::int32_t flags;
	std::array<uint32_t,25> after;
	std::array<uint8_t,26> com;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const chrraces& e)
{
	
/*	out<<' '<<ele;
	for(const auto &ele:e.bf)
	out<<e.flags;
	if(e.m_flags&0x80)
		out<<" (Old models are disabled)";*/
	return out;
}
}