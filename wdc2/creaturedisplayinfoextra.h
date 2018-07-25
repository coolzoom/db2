#pragma once
#include"racesex.h"
#include<array>
#include<bitset>
#include"unknown.h"
#include"wclass.h"

namespace wdc2
{

struct creaturedisplayinfoextra
{
	std::array<std::uint8_t,12> a;
};


template<typename T>
auto get(const creaturedisplayinfoextra& unk,std::size_t B,std::size_t size)
{
	union
	{
		creaturedisplayinfoextra un;
		std::bitset<8*sizeof(creaturedisplayinfoextra)> b;
	}u{};
	u.un=unk;
	T v(0);
	for(std::size_t i(0);i!=size;++i)
		v|=(u.b.test(i+B)<<i);
	return v;
}

template<typename T>
void set(creaturedisplayinfoextra& unk,std::size_t B,std::size_t size,T t)
{
	union
	{
		creaturedisplayinfoextra un;
		std::bitset<8*sizeof(creaturedisplayinfoextra)> b;
	}u{unk};
	for(std::size_t i(B),e(B+size);i!=e;++i,(t>>=1))
		u.b.set(i,t&1);
	unk=u.un;
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfoextra& e)
{
	out<<rcsx(get<std::uint8_t>(e,0,8))<<'\t'<<cls(get<std::uint8_t>(e,8,4))<<"\tsd:"<<get<std::uint32_t>(e,41,20)<<"\thd:"<<get<std::uint32_t>(e,61,20);
	return (out);
}

}