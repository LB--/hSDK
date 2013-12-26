#ifndef hSDK_Translation_HeaderPlusPlus
#define hSDK_Translation_HeaderPlusPlus

#include "hSDK.hpp"

#include <vector>

#include "json.h"

namespace hSDK
{
	struct Translation
	{
		virtual ~Translation() = default;

		virtual string Name() const = 0;
		virtual string Author() const = 0;
		virtual string Copyright() const = 0;
		virtual string Description() const = 0;
		virtual string Website() const = 0;
		virtual string HelpFile() const = 0;

		struct MenuBase
		{
			virtual ~MenuBase() = 0;

		private:
			MenuBase() = default;
			friend struct ::hSDK::Translation;
		};
		using Menu_t = std::vector<std::unique_ptr<MenuBase>>;
		struct MenuSeparator final : MenuBase
		{
		};
		struct MenuItem : MenuBase
		{
			virtual ~MenuItem() = 0;

			string text;
			bool enabled;

		private:
			MenuItem(string const &t, bool e)
			: text(t)
			, enabled(e)
			{
			}
			friend struct ::hSDK::Translation;
		};
		struct ACEMenuItem final : MenuItem
		{
			ACE_ID_t id;

			ACEMenuItem(ACE_ID_t Id, string const &t, bool e = true)
			: MenuItem(t, e)
			, id(Id)
			{
			}
		};
		struct SubMenu final : MenuItem
		{
			Menu_t items;

			SubMenu(Menu_t const &i, string const &t, bool e = true)
			: MenuItem(t, e)
			, items(i)
			{
			}
		};

		virtual Menu_t Menu(string const &subtype, ACE ace) const = 0;

		using ParamID_t = std::uint16_t;
		virtual string ParamName(string const &subtype, ACE ace, ACE_ID_t ace_id, ParamID_t param_id) const = 0;

		virtual string DisplayText(string const &subtype, ACE ace, ACE_ID_t ace_id) const = 0;
	};
	Translation::MenuBase::~MenuBase() = default;
	Translation::MenuItem::~MenuItem() = default;
	struct JsonTranslation : Translation
	{
		JsonTranslation(std::shared_ptr<json_value> json);

		virtual string Name() const override;
		virtual string Author() const override;
		virtual string Copyright() const override;
		virtual string Description() const override;
		virtual string Website() const override;
		virtual string HelpFile() const override;

		virtual Menu_t Menu(string const &subtype, ACE ace) const override;

		virtual string ParamName(string const &subtype, ACE ace, ACE_ID_t ace_id, ParamID_t param_id) const override;

		virtual string DisplayText(string const &subtype, ACE ace, ACE_ID_t ace_id) const override;

	private:
		std::shared_ptr<json_value> json;

		static Menu_t MenuGen(std::vector<json_value *> const &items);
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
