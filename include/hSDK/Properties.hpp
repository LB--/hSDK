#ifndef hSDK_Properties_HeaderPlusPlus
#define hSDK_Properties_HeaderPlusPlus

#include "hSDK.hpp"
#include "hSDK/BinaryStream.hpp"

#include <vector>
#include <set>
#include <map>

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
			bool checked;

			virtual ~Property() = 0;
		private:
			Property(std::int32_t t, string const &n, string const &d, std::int32_t opt, bool c = false)
			: name(n)
			, desc(d)
			, checked(c)
			, type(t)
			, options(opt)
			{
			}
			Property(Property const &) = delete;
			Property(Property &&) = delete;
			Property &operator=(Property const &) = delete;
			Property &operator=(Property &&) = delete;

			std::int32_t type;
			std::int32_t options;
			struct Param;
			virtual std::unique_ptr<Param> param();

			struct Value;
			virtual std::unique_ptr<Value> value();
			virtual void value(std::unique_ptr<Value>);

			struct impl;
			virtual std::unique_ptr<impl> prop();

			friend struct ::hSDK::Properties;
		};
		using Properties_t = std::vector<std::unique_ptr<Property>>;
		struct ButtonProp final : Property
		{
			ButtonProp(string const &name, string const &description, string const &text = T_"Edit", bool checkbox = false, bool bold = false, bool singlesel = false);

			string text;

		private:
			virtual std::unique_ptr<Param> param() override;
		};
		struct CheckboxProp final : Property
		{
			CheckboxProp(string const &name, string const &description, bool checked, bool bold = false, bool singlesel = false);
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

			using Directions_t = std::set<std::uint8_t>;

			DirectionProp(string const &name, string const &description, std::int32_t num_dirs = 32, Directions_t const &dirs = {}, Style style = Style::Flat, bool multi = true, bool slider = false, bool empty = false, bool buttons = true, bool checkbox = false, bool bold = false, bool singlesel = false);

			Style style;
			std::int32_t num_dirs;
			bool multi;
			bool slider;
			bool empty;
			bool buttons;
			Directions_t dirs;

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
			enum struct CaseChange
			{
				None,
				Lowercase,
				Uppercase
			};

			StringProp(string const &name, string const &description, string const &s = T_"", Type t = Type::Multiline, CaseChange cc = CaseChange::None, bool password = false, bool checkbox = false, bool bold = false, bool singlesel = false);

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
			FilenameProp(string const &name, string const &description, string const &fname, string const &filter, bool file_must_exist = true, bool path_must_exist = true, bool hide_read_only = false, bool checkbox = false, bool bold = false, bool singlesel = false);

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
			FolderProp(string const &name, string const &description, Properties_t const &props);

			Properties_t props;

		private:
			virtual std::unique_ptr<impl> prop() override;
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
			ImageFilenameProp(string const &name, string const &description, string const &fname, bool allow_anims = false, bool checkbox = false, bool bold = false, bool singlesel = false);

			string fname;
			bool allow_anims;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct SizeProp final : Property
		{
			using Size_t = std::pair<std::int32_t, std::int32_t>;
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
			SpinProp(string const &name, string const &description, std::int16_t min, std::int16_t max, std::int16_t v, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::int16_t min, max;
			std::int16_t v;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct SliderProp final : Property
		{
			SliderProp(string const &name, string const &description, std::int32_t min, std::int32_t max, std::int32_t v, bool checkbox = false, bool bold = false, bool singlesel = false);

			std::int32_t min, max;
			std::int32_t v;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};/*
		struct CustomProp : Property
		{
			//

		protected:
			CustomProp(string const &name, string const &description, bool checkbox = false, bool bold = false, bool singlesel = false);

			bool isActive();

			virtual void draw() = 0;
			virtual void serialize(BinaryStream::Out os) = 0;
			virtual void deserialize(BinaryStream::In is) = 0;

		private:
			struct Impl;

			virtual std::unique_ptr<Param> param() override final;

			virtual std::unique_ptr<Value> value() override final;
			virtual void value(std::unique_ptr<Value>) override final;
		};*/
		struct ReferenceToProp final : Property
		{
			ReferenceToProp(Property &prop);

			Property &property;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;

			virtual std::unique_ptr<impl> prop() override;
		};
		struct AdvancedProp final : Property
		{
			AdvancedProp(Property &prop, bool list = true, bool removable = true, bool renamable = true, bool movable = true);

			Property &property;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;

			virtual std::unique_ptr<impl> prop() override;
		};

		//Not documented in MMF2 SDK Help:
		struct UrlProp final : Property
		{
			UrlProp(string const &name, string const &description, string const &url, bool checkbox = false, bool bold = false, bool singlesel = false);

			string url;

		private:
			virtual std::unique_ptr<Param> param() override;
		};
		struct DirectoryProp final : Property
		{
			DirectoryProp(string const &name, string const &description, string const &dir, bool checkbox = false, bool bold = false, bool singlesel = false);

			string dir;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};
		struct FloatSpinProp final : Property
		{
			FloatSpinProp(string const &name, string const &description, float min, float max, float v, float delta = 0.01f, bool checkbox = false, bool bold = false, bool singlesel = false);

			float min, max, delta;
			float v;

		private:
			virtual std::unique_ptr<Param> param() override;

			virtual std::unique_ptr<Value> value() override;
			virtual void value(std::unique_ptr<Value>) override;
		};

		enum struct Tab
		{
			General,
			Specific,
			Display,
			Window,
			SizeAndPosition,
			TextOptions,
			Movements,
			RuntimeOptions,
			AlterableValues,
			Behaviors,
			About,
			BluRay,
			iPhone,
			Android,
			XNA,
			Mac,
			SmileyFace
		};
		using Tabs_t = std::map<Tab, Properties_t>;

	protected:
		Properties()
		{
		}

		void addProps(Tabs_t &&tab)
		{
			for(auto &t : tab)
			{
				if(props.find(t.first) == props.end())
				{
					props.emplace(t.first, std::move(t.second));
				}
				else for(auto &&p : t.second)
				{
					props.at(t.first).emplace_back(p);
				}
			}
		}

	private:
		Tabs_t props;

		Properties(Properties const &) = delete;
		Properties(Properties &&) = delete;
		Properties &operator=(Properties const &) = delete;
		Properties &operator=(Properties &&) = delete;
	};
	inline Properties::~Properties() = default;
	inline Properties::Property::~Property() = default;
}

#endif
