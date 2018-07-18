#pragma once
#include<array>
#include<bitset>

namespace wdc2
{

struct chrraces
{
	std::array<std::uint32_t,40> a;
};

inline constexpr decltype(auto) flags(chrraces& c)
{
	return c.a[7];
}

inline constexpr void enable_old_model(chrraces& c)
{
	flags(c)&=~0x80u;
}

inline constexpr decltype(auto) flags(const chrraces& c)
{
	return c.a[7];
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const chrraces& e)
{

	out<<flags(e);
//	for(const auto &ele:e.a)
//		out<<ele<<' ';
	if(flags(e)&0x8)
		out<<" (Playable)";
	if(flags(e)&0x80)
		out<<" (Old models are disabled)";
	return out;
}
}