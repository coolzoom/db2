#pragma once
#include<array>
#include"wdc1.h"
namespace db2
{
struct spellvisualkit
{
	std::uint32_t t;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const spellvisualkit& e)
{
	return out<<e.t;
}
}