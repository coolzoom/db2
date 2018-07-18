#pragma once
#include<array>

namespace db2
{
struct texturefiledata
{
//	std::uint16_t id;
//	std::uint16_t name_pos,path_pos;
//	std::array<std::uint8_t,6> a;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const texturefiledata& e)
{
	return out<<static_cast<std::uint32_t>(e.a.front());
//	return out<<(*reinterpret_cast<const uint32_t*>(e.a.data())&((1<<24)-1))<<'\t'<<*reinterpret_cast<const uint16_t*>(e.a.data()+3);
}
}