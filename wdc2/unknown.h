#pragma once
#include<array>
#include<cstdint>

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

}