#pragma once

namespace wdc2
{

struct field_structure
{
	std::uint16_t size,position;
};
template<typename ostrm>
decltype(auto) operator<<(ostrm& o,const field_structure& fs)
{
	return o<<fs.position<<"\tsize:"<<((32-fs.size)>>3);
}

}