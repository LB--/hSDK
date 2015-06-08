#include "hSDK/Translation.hpp"

#include "hSDK/Resources.hpp"

#include <sstream>
#include <fstream>
#include <streambuf>
#include <map>

namespace hSDK
{
	JsonTranslation::JsonTranslation(std::shared_ptr<json_value const> json_)
	: json(json_)
	{
	}
	string JsonTranslation::Name() const
	{
		auto v = (*json)["About"]["Name"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	string JsonTranslation::Author() const
	{
		auto v = (*json)["About"]["Author"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	string JsonTranslation::Copyright() const
	{
		auto v = (*json)["About"]["Copyright"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	string JsonTranslation::Description() const
	{
		auto v = (*json)["About"]["Description"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	string JsonTranslation::Website() const
	{
		auto v = (*json)["About"]["Website"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	string JsonTranslation::HelpFile() const
	{
		auto v = (*json)["About"]["Help file"];
		return str_fr(string8{ v.u.string.ptr, v.u.string.length });
	}
	auto JsonTranslation::MenuGen(std::vector<json_value *> const &items)
	-> Menu_t
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
					static_cast<ACE_ID_t>(static_cast<json_int_t>(*id)),
					str_fr(string8{tx->u.string.ptr, tx->u.string.length}),
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
				json_value *sn = arr[name_off];
				m.push_back(Menu_t::value_type{new SubMenu
				(
					sub,
					str_fr(string8{sn->u.string.ptr, sn->u.string.length}),
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
			{ACE::Condition, "Condition menu"},
			{ACE::Expression, "Expression menu"}
		};
		json_value const &menu = (*json)["Sub Types"][str_to8(subtype).c_str()]["Menus"][mm.at(ace)];
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
		{ACE::Condition, "Conditions"},
		{ACE::Expression, "Expressions"}
	};
	string JsonTranslation::ParamName(string const &subtype, ACE ace, ACE_ID_t ace_id, ParamID_t param_id) const
	{
		for(json_value const *jace : (*json)["Sub Types"][str_to8(subtype).c_str()][nm.at(ace)].u.array)
		{
			json_value const &id = (*jace)[0];
			if(id.type == json_integer && static_cast<ACE_ID_t>(static_cast<json_int_t>(id)) == ace_id)
			{
				json_value const &text = (*jace)[2][param_id];
				if(text.type == json_string)
				{
					return str_fr(string8{text.u.string.ptr, text.u.string.length});
				}
			}
		}
		std::ostringstream oss;
		oss << "\"" << str_to8(subtype) << "\"->" << nm.at(ace) << "->#" << ace_id << "->P" << param_id;
		return str_fr(oss.str());
	}
	string JsonTranslation::DisplayText(string const &subtype, ACE ace, ACE_ID_t ace_id) const
	{
		for(json_value const *jace : (*json)["Sub Types"][str_to8(subtype).c_str()][nm.at(ace)].u.array)
		{
			json_value const &id = (*jace)[0];
			if(id.type == json_integer && static_cast<ACE_ID_t>(static_cast<json_int_t>(id)) == ace_id)
			{
				json_value const &text = (*jace)[1];
				if(text.type == json_string)
				{
					return str_fr(string8{text.u.string.ptr, text.u.string.length});
				}
			}
		}
		std::ostringstream oss;
		oss << "\"" << str_to8(subtype) << "\"->" << nm.at(ace) << "->#" << ace_id;
		return str_fr(oss.str());
	}

	static json_value &RcJson()
	{
		static Resource jsonrc = Resource::Get(T_("ExtensionTranslations"));
		struct json_deleter
		{
			void operator()(json_value *jv) const
			{
				return json_value_free(jv);
			}
		};
		static std::unique_ptr<json_value, json_deleter> json {json_parse(jsonrc.data, jsonrc.size)};
		return *json.get();
	}
	ResourceJsonTranslation::ResourceJsonTranslation(string const &lang)
	: JsonTranslation(std::shared_ptr<json_value const>{&(RcJson()[str_to8(lang).c_str()]), [](json_value const *){}})
	{
	}

	static std::shared_ptr<json_value> FileJson(string const &file)
	{
		auto json = EntireFile(file);
		return std::shared_ptr<json_value>{json_parse(json.c_str(), json.size()), json_value_free};
	}
	ExternalJsonTranslation::ExternalJsonTranslation(string const &file)
	: JsonTranslation(FileJson(file))
	{
	}
}
