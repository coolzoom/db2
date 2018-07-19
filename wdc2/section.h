#pragma once
#include"raw.h"
#include"cvs.h"
#include<vector>
#include<string>

namespace wdc2
{

template<typename T>
struct section
{
	std::vector<T> records;
	std::string string_table;
	std::vector<std::uint32_t> ids;
	std::vector<copy_table_entry> copy_table;
//	std::vector<offset_map_entry> offsets;
//	relationship_mapping relationship_map;
	template<typename Q>
	section(const std::string &str,Q& p,const section_header& header):records(check_section_validity<T>(str,p,header)),
		string_table(cvs<char>(str,p,header.string_table_size)),
		ids(cvs<std::uint32_t>(str,p,header.id_list_size/4)),
		copy_table(cvs<copy_table_entry>(str,p,header.copy_table_size/sizeof(copy_table_entry)))
	{
		using namespace std::string_literals;
		if(header.relationship_data_size)
			throw std::runtime_error("not support relationship_map"s);
	}
};

template<typename T>
auto svc(std::string& str,const section<T>& sct)
{
	section_header h{};
	h.file_offset=str.size();
	svc(str,sct.records,h.record_count);
	svc(str,sct.string_table,h.string_table_size);
	svc(str,sct.ids);
	h.id_list_size=sct.ids.size()*4;
	svc(str,sct.copy_table);
	h.copy_table_size=sct.copy_table.size()*sizeof(copy_table_entry);
	return h;
}

}