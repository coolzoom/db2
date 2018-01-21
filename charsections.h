#ifndef CHARSECTIONS_H
#define CHARSECTIONS_H
#include<array>
#include"race.h"

namespace db2
{
namespace charsections_details
{
struct flag
{
	std::uint16_t value;
	bool npc() const
	{
		return value&8;
	}
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const flag& e)
{
	out<<static_cast<std::uint32_t>(e.value);
	if(e.value&1)
		out<<"[P]";
	if(e.value&2)
		out<<"[B]";
	if(e.value&4)
		out<<"[DK]";
	if(e.value&8)
		out<<"[NPC]";
	if(e.value&64)
		out<<"[DH]";
	return (out);
}

enum class field : std::uint8_t
{skin,face,facial,hair,underwear,hd_skin,hd_face,hd_facial,hd_hair,hd_underwear,demon_hunter_tatto=11};
template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const field& e)
{
	switch(e)
	{
	case field::skin:
		out<<"Skin";
	break;
	case field::face:
		out<<"Face";
	break;
	case field::facial:
		out<<"Facial";
	break;
	case field::hair:
		out<<"Hair";
	break;
	case field::underwear:
		out<<"Underwear";
	break;
	case field::hd_skin:
		out<<"HD Skin";
	break;
	case field::hd_face:
		out<<"HD Face";
	break;
	case field::hd_facial:
		out<<"HD Facial";
	break;
	case field::hd_hair:
		out<<"HD Hair";
	break;
	case field::hd_underwear:
		out<<"HD Underwear";
	break;
	case field::demon_hunter_tatto:
		out<<"DH Tatto";
	break;
	default:
		out<<"Unknown("<<static_cast<std::uint32_t>(e)<<')';
	}
	return (out);
}
}

struct charsections
{
	std::array<std::uint32_t,3> textures;
	charsections_details::flag flag;
	race race;
	sex sex;
	using field_type = charsections_details::field;
	field_type field;
	std::array<std::uint8_t,3> c;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const charsections& e)
{
	for(const auto &ele : e.textures)
		out<<ele<<'\t';
	out<<e.flag<<'\t'<<e.race<<'\t'<<e.sex<<'\t'<<e.field;
	for(const auto &ele : e.c)
		out<<'\t'<<(unsigned)ele;
	return (out);
}
}
#endif