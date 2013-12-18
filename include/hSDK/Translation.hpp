#ifndef hSDK_Translation_HeaderPlusPlus
#define hSDK_Translation_HeaderPlusPlus

#include "hSDK.hpp"

#include <vector>

namespace hSDK
{
	struct Translation
	{
		virtual ~Translation() = default;

		struct MenuItem
		{
			virtual ~MenuItem() = default;

			string text;
			bool enabled;
			ACE_ID_t id;
		};
		struct SubMenu : MenuItem
		{
			std::vector<std::unique_ptr<MenuItem>> items;
		};

		virtual Submenu ActionMenu() const = 0;
		virtual Submenu ConditionMenu() const = 0;
		virtual Submenu ExpressionMenu() const = 0;

		using ParamID_t = std::uint16_t;
		virtual string ParamName(ACE ace, ACE_ID_t ace_id, ParamID_t param_id) const = 0;

		virtual string DisplayText(ACE ace, ACE_ID_t ace_id) const = 0;
	};
	struct JsonTranslation : Translation
	{
		JsonTranslation(/**/);
	};
	struct ResourceJsonTranslation : JsonTranslation
	{
		ResourceJsonTranslation(string const &lang);
	};
	struct ExternalJsonTranslation : JsonTranslation
	{
		ExternalJsonTranslation(string const &file);
	};
}

#endif
