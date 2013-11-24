#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"

#include <vector>

namespace hSDK
{
	struct Properties
	{
		virtual ~Properties() = 0;

		using string = hSDK::string;

		struct Property
		{
			string name;
			string desc;

		private:
			Property(std::uint32_t t, string const &n, string const &d, bool c = false, bool b = false, bool s = false, bool p = false)
			: name(n)
			, desc(d)
			, type(t)
			, check(c)
			, bold(b)
			, ssel(s)
			, preq(p)
			{
			}
			Property(Property const &) = delete;
			Property(Property &&) = delete;
			Property &operator=(Property const &) = delete;
			Property &operator=(Property &&) = delete;
			virtual ~Property() = 0;

			std::uint32_t type;
			bool check;
			bool bold;
			bool ssel;
			bool preq;
			struct Param;
			virtual std::unique_ptr<Param> param();

			struct Value;
			virtual std::unique_ptr<Value> value();
			virtual void value(std::unique_ptr<Value>);

			friend struct ::hSDK::Properties;
		};
		struct ButtonProp final : Property
		{
			ButtonProp(string const &name, string const &description, string const &text = T_"Edit", bool checkbox = false, bool bold = false, bool singlesel = false);

			string text;

		private:
			virtual std::unique_ptr<Value> value() override;
		};
		struct CheckboxProp final : Property
		{
			CheckboxProp(string const &name, string const &description, bool checked, bool bold = false, bool singlesel = false);

			bool checked;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct ColorProp final : Property
		{
			ColorProp(string const &name, string const &description, std::uint32_t color, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::uint32_t color;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct ListProp final : Property
		{
			using Items_t = std::vector<string>;

			ListProp(string const &name, string const &description, Items_t const &items, bool button = false, bool checkbox = false, bool bold = false, bool singlesel = false);

			Items_t items;
			Items_t::const_iterator selected;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct DirectionProp final : Property
		{
			enum struct Style
			{
				Flat,
				ThreeD
			};

			DirectionProp(string const &name, string const &description, std::int32_t num_dirs = 32, Style style = Style::Flat, bool slider = false, bool empty = false, bool buttons = true, bool checkbox = false, bool bold = false, bool singlesel = false);

			Style style;
			std::int32_t num_dirs;
			bool slider;
			bool empty;
			bool buttons;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct FloatProp final : Property
		{
			FloatProp(string const &name, string const &description, float v = 0.0f, bool checkbox = false, bool bold = false, bool singlesel = false);

			float v;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct StringProp final : Property
		{
			enum struct Type
			{
				Normal,
				Multiline,
				Static
			};

			StringProp(string const &name, string const &description, string s = T_"", Type t = Type::Multiline, bool checkbox = false, bool bold = false, bool singlesel = false);

			string s;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct IntProp final : Property
		{
			IntProp(string const &name, string const &description, std::uint32_t v = 0, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::uint32_t v;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct FilenameProp final : Property
		{
			FilenameProp(string const &name, string const &description, string fname, bool file_must_exist = true, bool path_must_exist = true, bool hide_read_only = false, bool checkbox = false, bool bold = false, bool singlesel = false);

			string fname;
			string filter;
			bool file_must_exist;
			bool path_must_exist;
			bool hide_read_only;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct FolderProp final : Property
		{
			FolderProp(string const &name, string const &description, std::vector<std::unique_ptr<Property>> props);

			std::vector<std::unique_ptr<Property>> props;
		};/*
		struct FontProp final : Property
		{
			FontProp(string const &name, string const &description, LOGFONT font, bool checkbox = false, bool bold = false, bool singlesel = false);

			LOGFONT font;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};*/
		struct HeaderProp final : Property
		{
			HeaderProp(string const &name, string const &description);
		};/*
		struct IconListProp final : Property
		{
			using Items_t = std::vector<std::pair<?Icon?, string>>;

			IconListProp(string const &name, string const &description, Items_t const &items, bool checkbox = false, bool bold = false, bool singlesel = false);

			Items_t items;
			Items_t::const_iterator selected;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};*/
		struct ImageListProp final : Property
		{
			using Images_t = std::vector<std::uint16_t>;

			ImageListProp(string const &name, string const &description, Images_t const &images, bool checkbox = false, bool bold = false, bool singlesel = false);

			Images_t images;

		private:
			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct ImageFilenameProp final : Property
		{
			ImageFilenameProp(string const &name, string const &description, string fname, bool allow_anims = false, bool checkbox = false, bool bold = false, bool singlesel = false);

			string fname;
			bool allow_anims;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct SizeProp final : Property
		{
			using Size_t = std::pair<std::int32_t, std::int32_t>
			using Sizes_t = std::vector<Size_t>;

			SizeProp(string const &name, string const &description, Size_t const &size = {0, 0}, Sizes_t const &predefined = {}, bool checkbox = false, bool bold = false, bool singlesel = false);

			Size_t size;
			Sizes_t predefined;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct SpinProp final : Property
		{
			SpinProp(string const &name, string const &description, std::int16_t min, std::int16_t max, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::int16_t min, max;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct SliderProp final : Property
		{
			SliderProp(string const &name, string const &description, std::int32_t min, std::int32_t max, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::int32_t min, max;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct CustomProp : Property
		{
			//...

		private:
			virtual std::unique_ptr<Param> param() override final;

			virtual std::unique_ptr<Value> value() override final;
			virtual void value(std::unique_ptr<Value>) override final;
		};

	protected:
		Properties()
		{
		}

	private:
		Properties(Properties const &) = delete;
		Properties(Properties &&) = delete;
		Properties &operator=(Properties const &) = delete;
		Properties &operator=(Properties &&) = delete;
	};
	inline Properties::~Properties() = default;
	inline Properties::Property::~Property() = default;
}

#endif
