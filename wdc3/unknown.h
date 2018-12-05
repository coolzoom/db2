#pragma once
#include<array>
#include<cstdint>
#include<bitset>

namespace wdc2
{
template<std::size_t size>
struct unknown
{
	std::array<std::uint8_t,size> a;
};

template<typename ostrm,std::size_t T>
inline decltype(auto) operator<<(ostrm& out,const unknown<T>& c)
{
	for(std::size_t i(0);i!=c.a.size();++i)
	{
		if(i)
			out.put(' ');
		out<<static_cast<std::uint16_t>(c.a[i]);
	}
	return (out);
}

template<typename T,std::size_t N>
auto get(const unknown<N>& unk,std::size_t B,std::size_t size)
{
	union
	{
		unknown<N> un;
		std::bitset<8*N> b;
	}u{unk};
	T v(0);
	for(std::size_t i(0);i!=size;++i)
		v|=(u.b.test(i+B)<<i);
	return v;
}

template<std::size_t N,typename T>
void set(unknown<N>& unk,std::size_t B,std::size_t size,T t)
{
	union
	{
		unknown<N> un;
		std::bitset<8*N> b;
	}u{unk};
	for(std::size_t i(B),e(B+size);i!=e;++i,(t>>=1))
		u.b.set(i,t&1);
	unk=u.un;
}


}