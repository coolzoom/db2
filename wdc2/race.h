#pragma once

namespace wdc2
{
enum class race : std::uint8_t
{
	human,orc,dwarf,nightelf,undead,tauren,gnome,troll,goblin,bloodelf,draenei,felorc,naga,broken,worgen=22,pandaren=24,alliance_traitor,horde_traitor
};

inline auto rc(std::uint8_t r)
{
	return static_cast<race>(r);
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const race& e)
{
	switch(e)
	{
	case race::human:
		out<<"Human";
	break;
	case race::orc:
		out<<"Orc";
	break;
	case race::dwarf:
		out<<"Dwarf";
	break;
	case race::nightelf:
		out<<"Night Elf";
	break;
	case race::undead:
		out<<"Undead";
	break;
	case race::tauren:
		out<<"Tauren";
	break;
	case race::gnome:
		out<<"Gnome";
	break;
	case race::troll:
		out<<"Troll";
	break;
	case race::goblin:
		out<<"Goblin";
	break;
	case race::bloodelf:
		out<<"Blood Elf";
	break;
	case race::draenei:
		out<<"Draenei";
	break;
	case race::felorc:
		out<<"Fel Orc";
	break;
	case race::naga:
		out<<"Naga";
	break;
	case race::broken:
		out<<"Broken";
	break;
	case race::worgen:
		out<<"Worgen";
	break;
	case race::pandaren:
		out<<"Pandaren";
	break;
	case race::alliance_traitor:
		out<<"Alliance Traitor";
	break;
	case race::horde_traitor:
		out<<"Horde Traitor";
	break;
	default:
		out<<"Unknown("<<static_cast<std::uint32_t>(e)<<')';
	}
	return (out);
}


enum class sex : std::uint8_t
{male,female};

inline auto sx(std::uint8_t r)
{
	return static_cast<sex>(r);
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const sex& e)
{
	switch(e)
	{
	case sex::male:
		out.put('M');
	break;
	case sex::female:
		out.put('F');
	break;
	default:
		out<<"Unknown("<<static_cast<std::uint32_t>(e)<<") ";
	}
	return (out);
}

}