#pragma once

namespace wdc2
{
enum class wclass : std::uint8_t
{npc,warrior,paladin,hunter,rogue,priest,deathknight,shaman,mage,warlock,monk,druid,demonhunter};

inline auto cls(std::uint8_t r)
{
	return static_cast<wclass>(r);
}

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const wclass& e)
{
	switch(e)
	{
	case wclass::npc:
		return out<<"npc";
	case wclass::warrior:
		return out<<"warrior";
	case wclass::paladin:
		return out<<"paladin";
	case wclass::hunter:
		return out<<"hunter";
	case wclass::rogue:
		return out<<"rogue";
	case wclass::priest:
		return out<<"priest";
	case wclass::deathknight:
		return out<<"deathknight";
	case wclass::shaman:
		return out<<"shaman";
	case wclass::mage:
		return out<<"mage";
	case wclass::warlock:
		return out<<"warlock";
	case wclass::monk:
		return out<<"monk";
	case wclass::druid:
		return out<<"druid";
	case wclass::demonhunter:
		return out<<"demonhunter";
	default:
		return out<<"Unknown("<<static_cast<std::uint16_t>(e)<<") ";
	}
}
}