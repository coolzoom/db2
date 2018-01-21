#pragma once
#include<array>
#include"race.h"

namespace dbc
{
struct creaturedisplayinfo
{
std::uint32_t id;
std::uint32_t m_modelID;
std::uint32_t m_soundID;
std::uint32_t m_extendedDisplayInfoID;
float m_creatureModelScale;
std::uint32_t m_creatureModelAlpha;
std::array<std::uint32_t,3> m_textureVariation;
std::uint32_t m_portraitTextureName;
std::uint32_t m_portraitCreatureDisplayInfoID;
std::uint32_t m_sizeClass;
std::uint32_t m_bloodID;
std::uint32_t m_NPCSoundID;
std::uint32_t m_particleColorID;
std::uint32_t m_creatureGeosetData;
std::uint32_t m_objectEffectPackageID;
std::uint32_t m_animReplacementSetID;
std::uint32_t m_flags;
std::uint32_t m_gender;
std::uint32_t m_stateSpellVisualKitID;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfo& e)
{
	return out<<e.id;
}
}

namespace db2
{
struct creaturedisplayinfo
{
std::uint32_t id;
std::uint32_t extraid;
std::array<std::uint8_t,16> a;
};

template<typename ostrm>
decltype(auto) operator<<(ostrm& out,const creaturedisplayinfo& e)
{
	return out<<e.id<<'\t'<<e.extraid;
}
}