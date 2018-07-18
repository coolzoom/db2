#pragma once
#include<cstdint>
#include<array>

namespace db2
{
struct animationdata
{
//	std::array<std::uint16_t,6> a;
	std::uint32_t id;
	std::uint16_t a,b;
	std::uint8_t c;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const animationdata& e)
{
	out<<e.id<<' '<<e.a<<' '<<e.b<<' '<<static_cast<std::uint32_t>(e.c);
	return (out);
//	return out<<e.a<<' '<<e.e<<' '<<e.b<<' '<<e.c<<' '<<static_cast<std::uint32_t>(e.d);
}

}