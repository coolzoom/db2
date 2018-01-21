#pragma once
#include<array>

namespace db2
{
struct modelfiledata
{
	std::array<std::uint8_t,6> a;
/*	std::uint32_t id;
	std::uint8_t unk,file_completeid;*/
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const modelfiledata& e)
{
//	for(const auto &ele : e.a)
//		out<<std::uint32_t(ele)<<'\t';
	return (out<<*reinterpret_cast<const std::uint32_t*>(e.a.data()));
//	return (out<<e.q);
//	return out<<"id:"<<e.id<<"\tunk:"<<e.unk<<"\tcomplete:"<<e.file_completeid;
}
}