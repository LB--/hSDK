#include "Translation.hpp"

#include <sstream>

namespace hSDK
{
	JsonTranslation::JsonTranslation(std::shared_ptr<json_value> json_)
	: json(json_)
	{
	}
	static Menu_t JsonTranslation::MenuGen(std::vector<json_value *> const &items)
	{
		Menu_t m;
		for(json_value *i : items)
		{
			if(i->type != json_array || i->u.array.length < 2)
			{
				m.push_back(Menu_t::value_type{new MenuSeparator});
			}
			else if(i->u.array.length == 2 && i->u.array.values[0]->type == json_integer)
			{
				json_value *id = i->u.array.values[0];
				json_value *tx = i->u.array.values[1];
				m.push_back(Menu_t::value_type{new ACEMenuItem
				(
					*id,
					static_cast<char const *>(*tx),
					id->type == json_integer
				)});
			}
			else
			{
				auto &arr = i->u.array.values;
				std::ptrdiff_t name_off = (arr[0]->type == json_string? 0 : 1);
				Menu_t sub = MenuGen(std::vector<json_value *>
				(
					arr + 1 + name_off,
					arr + i->u.array.length
				));
				m.push_back(Menu_t::value_type{new SubMenu
				(
					sub,
					*arr[name_off],
					name_off == 0
				)});
			}
		}
		return m;
	}
	auto JsonTranslation::Menu(string const &subtype, ACE ace) const
	-> Menu_t
	{
		static std::map<ACE, char const *> const mm
		{
			{ACE::Action, "Action menu"},
			{ACE::Conditon, "Condition menu"},
			{ACE::Expression, "Expression menu"}
		};
		json_value &menu = (*json)["Sub Types"][subtype.c_str()]["Menus"][mm[ace]];
		if(menu.type == json_array)
		{
			return MenuGen(std::vector<json_value *>
			(
				menu.u.array.values,
				menu.u.array.values + menu.u.array.length
			));
		}
		return Menu_t{};
	}
	static std::map<ACE, char const *> const nm
	{
		{ACE::Action, "Actions"},
		{ACE::Conditon, "Conditions"},
		{ACE::Expression, "Expressions"}
	};
	string JsonTranslation::ParamName(string const &subtype, ACE ace, ACE_ID_t ace_id, ParamID_t param_id) const
	{
		for(json_value *jace : (*json)["Sub Types"][subtype.c_str()][nm[ace]])
		{
			json_value &id = (*jace)[0];
			if(id.type == json_integer && id == ace_id)
			{
				json_value &text = (*jace)[2][param_id];
				if(text.type == json_string)
				{
					return static_cast<char const *>(text);
				}
			}
		}
		std::ostringstream oss;
		oss << "\"" << subtype << "\"->" << nm[ace] << "->#" << ace_id << "->P" << param_id;
		return oss.str();
	}
	string JsonTranslation::DisplayText(string const &subtype, ACE ace, ACE_ID_t ace_id) const
	{
		for(json_value *jace : (*json)["Sub Types"][subtype.c_str()][nm[ace]])
		{
			json_value &id = (*jace)[0];
			if(id.type == json_integer && id == ace_id)
			{
				json_value &text = (*jace)[1];
				if(text.type == json_string)
				{
					return static_cast<char const *>(text);
				}
			}
		}
		std::ostringstream oss;
		oss << "\"" << subtype << "\"->" << nm[ace] << "->#" << ace_id;
		return oss.str();
	}
}
