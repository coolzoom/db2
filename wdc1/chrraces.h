#pragma once
#include<array>

namespace db2
{
struct chrraces
{
	std::array<uint8_t,24> bf;
	uint32_t m_flags;
	std::array<uint8_t,76> com;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const chrraces& e)
{
	out<<e.m_flags;
	if(e.m_flags&0x80)
		out<<" (Old models are disabled)";
	return out;
}
}