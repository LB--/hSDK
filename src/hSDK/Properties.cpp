#include "hSDK/Properties.hpp"

#include <limits>

#include "MMF2API.hpp"

namespace hSDK
{

	struct Properties::Property::Param
	{
		virtual ~Param() = default;

		virtual LPARAM lParam() const = 0;

		operator LPARAM() const
		{
			return lParam();
		}
	};
	auto Properties::Property::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			virtual LPARAM lParam() const override
			{
				return 0;
			}
		};
		return std::unique_ptr<Param>(new Derived);
	}

	struct Properties::Property::Value
	{
		CPropValue *v;
		Value(CPropValue *val)
		: v(val)
		{
		}
		static auto New(CPropValue *val)
		-> std::unique_ptr<Value>
		{
			return std::unique_ptr<Value>(new Value(val));
		}

		template<typename T>
		auto as()
		-> typename std::enable_if
		<
			std::is_base_of<CPropValue, T>::value,
			T *
		>::type
		{
			return static_cast<T *>(v);
		}
		operator CPropValue *()
		{
			return v;
		}
	};
	auto Properties::Property::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(nullptr);
	}
	void Properties::Property::value(std::unique_ptr<Value>)
	{
	}

	struct Properties::Property::impl
	{
		std::vector<std::unique_ptr<PropData>> props;
		virtual ~impl() = 0;
	};
	inline Properties::Property::impl::~impl() = default;
	auto Properties::Property::prop()
	-> std::unique_ptr<impl>
	{
		struct derived_impl : impl
		{
			string const name;
			string const desc;
			derived_impl(Property const &prop)
			: name(prop.name)
			, desc(prop.desc)
			{
				std::unique_ptr<PropData> p {new PropData};
				p->dwID = 0;
				p->sName = reinterpret_cast<UINT_PTR>(name.c_str());
				p->sInfo = reinterpret_cast<UINT_PTR>(desc.c_str());
				p->lType = prop.type;
				p->dwOptions = prop.options;
				p->lCreateParam = reinterpret_cast<LPARAM>(nullptr);
				props.emplace_back(std::move(p));
			}
		};
		return std::unique_ptr<impl>(new derived_impl(*this));
	}

	namespace
	{
		static std::int32_t popt(bool checkbox, bool bold, bool ssel, bool pr = false, bool password = false, bool lowercase = false, bool uppercase = false, bool remove = false, bool rename = false, bool move = false, bool list = false)
		{
			return        PROPOPT_NIL
			| (checkbox?  PROPOPT_CHECKBOX : 0)
			| (bold?      PROPOPT_BOLD : 0)
			| (ssel?      PROPOPT_SINGLESEL : 0)
			| (pr?        PROPOPT_PARAMREQUIRED : 0)
			| (remove?    PROPOPT_REMOVABLE : 0)
			| (rename?    PROPOPT_RENAMABLE : 0)
			| (move?      PROPOPT_MOVABLE : 0)
			| (list?      PROPOPT_LIST : 0)
			| (password?  PROPOPT_EDIT_PASSWORD : 0)
			| (lowercase? PROPOPT_EDIT_LOWERCASE : 0)
			| (uppercase? PROPOPT_EDIT_UPPERCASE : 0);
		}
	}

	Properties::ButtonProp::ButtonProp(string const &n, string const &d, string const &t, bool c, bool b, bool s)
	: Property(PROPTYPE_EDITBUTTON, n, d, popt(c, b, s))
	, text(t)
	{
	}
	auto Properties::ButtonProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			string const text;
			Derived(string const &txt)
			: text(txt)
			{
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(text.c_str());
			}
		};
		return std::unique_ptr<Param>(new Derived(text));
	}

	Properties::CheckboxProp::CheckboxProp(string const &n, string const &d, bool c, bool b, bool s)
	: Property(PROPTYPE_LEFTCHECKBOX, n, d, popt(false, b, s), c)
	{
	}

	Properties::ColorProp::ColorProp(string const &n, string const &d, std::uint32_t col, bool c, bool b, bool s)
	: Property(PROPTYPE_COLOR, n, d, popt(c, b, s))
	, color(col)
	{
	}
	auto Properties::ColorProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropDWordValue(color));
	}
	void Properties::ColorProp::value(std::unique_ptr<Value> v)
	{
		color = v->as<CPropDWordValue>()->m_dwValue;
	}

	Properties::ListProp::ListProp(string const &n, string const &d, Items_t const &i, bool bu, bool c, bool b, bool s)
	: Property((bu? PROPTYPE_COMBOBOXBTN : PROPTYPE_COMBOBOX), n, d, popt(c, b, s, true))
	, items(i)
	{
		selected = items.end();
	}
	auto Properties::ListProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			Items_t const items;
			std::unique_ptr<char_t const *[]> list;
			Derived(Items_t const &i)
			: items(i)
			, list(new char_t const *[i.size()+2])
			{
				list[0] = list[items.size()+1] = nullptr;
				for(Items_t::size_type i = 0; i < items.size(); ++i)
				{
					list[i+1] = items[i].c_str();
				}
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(list.get());
			}
		};
		return std::unique_ptr<Param>(new Derived(items));
	}
	auto Properties::ListProp::value()
	-> std::unique_ptr<Value>
	{
		if(selected == items.end())
		{
			return Value::New(new CPropDWordValue(-1ul));
		}
		else
		{
			return Value::New(new CPropDWordValue(selected-items.begin()));
		}
	}
	void Properties::ListProp::value(std::unique_ptr<Value> v)
	{
		std::uint32_t sel = v->as<CPropDWordValue>()->m_dwValue;
		if(sel == -1ul)
		{
			selected = items.end();
		}
		else
		{
			selected = items.begin()+sel;
		}
	}

	Properties::DirectionProp::DirectionProp(string const &n, string const &d, std::int32_t nd, Directions_t const &ds, Style st, bool m, bool sl, bool e, bool bu, bool c, bool b, bool s)
	: Property(PROPTYPE_DIRCTRL, n, d, popt(c, b, s, true))
	, style(st)
	, num_dirs(nd)
	, multi(m)
	, slider(sl)
	, empty(e)
	, buttons(bu)
	, dirs(ds)
	{
	}
	auto Properties::DirectionProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			DirCtrlCreateParam p;
			Derived(DirectionProp const &dp)
			{
				p.bMultiSel = dp.multi ? 1 : 0;
				p.numDirs = dp.num_dirs;
				p.style = DCS_NOBORDER;
				switch(dp.style)
				{
				case Style::Flat:   p.style |= DCS_FLAT; break;
				case Style::ThreeD: p.style |= DCS_3D;   break;
				default:                                 break;
				}
				if(dp.slider)  p.style |= DCS_SLIDER;
				if(dp.empty)   p.style |= DCS_EMPTY;
				if(dp.buttons) p.style |= DCS_SETALL_BTNS;
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(&p);
			}
		};
		return std::unique_ptr<Param>(new Derived(*this));
	}
	auto Properties::DirectionProp::value()
	-> std::unique_ptr<Value>
	{
		PropDirValue dv = {0, 0, num_dirs, 0};
		if(multi)
		{
			for(std::uint8_t i = 0; i < 32; ++i)
			{
				if(dirs.find(i) != dirs.end())
				{
					dv.selDir32 |= (1 << i);
				}
			}
		}
		else
		{
			dv.selDir = *dirs.begin();
		}
		return Value::New(new CPropDataValue(sizeof(PropDirValue), reinterpret_cast<LPBYTE>(&dv)));
	}
	void Properties::DirectionProp::value(std::unique_ptr<Value> v)
	{
		PropDirValue &dv = *reinterpret_cast<PropDirValue *>(&(v->as<CPropDataValue>()->m_pData));
		num_dirs = dv.numDirs;
		dirs.clear();
		if(multi)
		{
			for(std::uint8_t i = 0; i < 32; ++i)
			{
				if(dv.selDir32 & (1 << i))
				{
					dirs.insert(i);
				}
			}
		}
		else
		{
			dirs.insert(dv.selDir);
		}
	}

	Properties::FloatProp::FloatProp(string const &n, string const &d, float val, bool c, bool b, bool s)
	: Property(PROPTYPE_EDIT_FLOAT, n, d, popt(c, b, s))
	, v(val)
	{
	}
	auto Properties::FloatProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropFloatValue(v));
	}
	void Properties::FloatProp::value(std::unique_ptr<Value> val)
	{
		v = val->as<CPropFloatValue>()->m_fValue;
	}

	Properties::StringProp::StringProp(string const &n, string const &d, string const &str, Type t, CaseChange cc, bool pw, bool c, bool b, bool s)
	: Property((t == Type::Normal? PROPTYPE_EDIT_STRING : (t == Type::Multiline? PROPTYPE_EDIT_MULTILINE : PROPTYPE_STATIC)), n, d, popt(c, b, s, false, pw, cc == CaseChange::Lowercase, cc == CaseChange::Uppercase))
	{
	}
	auto Properties::StringProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropStringValue(s.c_str()));
	}
	void Properties::StringProp::value(std::unique_ptr<Value> v)
	{
		s = v->as<CPropStringValue>()->
	#ifndef UNICODE
		m_pStr
	#else
		m_pWStr
	#endif
		;
	}

	Properties::IntProp::IntProp(string const &n, string const &d, std::uint32_t val, bool c, bool b, bool s)
	: Property(PROPTYPE_EDIT_NUMBER, n, d, popt(c, b, s))
	, v(val)
	{
	}
	auto Properties::IntProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropDWordValue(v));
	}
	void Properties::IntProp::value(std::unique_ptr<Value> val)
	{
		v = val->as<CPropDWordValue>()->m_dwValue;
	}

	Properties::FilenameProp::FilenameProp(string const &n, string const &d, string const &fn, string const &filt, bool fme, bool pme, bool hro, bool c, bool b, bool s)
	: Property(PROPTYPE_FILENAME, n, d, popt(c, b, s, true))
	, fname(fn)
	, filter(filt)
	, file_must_exist(fme)
	, path_must_exist(pme)
	, hide_read_only(hro)
	{
	}
	auto Properties::FilenameProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			FilenameCreateParam p;
			string const filt;
			Derived(FilenameProp const &fp)
			: filt(fp.filter)
			{
				p.extFilter = filt.c_str();
				p.options = 0;
				if(fp.file_must_exist) p.options |= OFN_FILEMUSTEXIST;
				if(fp.path_must_exist) p.options |= OFN_PATHMUSTEXIST;
				if(fp.hide_read_only)  p.options |= OFN_HIDEREADONLY;
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(&p);
			}
		};
		return std::unique_ptr<Param>(new Derived(*this));
	}
	auto Properties::FilenameProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropStringValue(fname.c_str()));
	}
	void Properties::FilenameProp::value(std::unique_ptr<Value> v)
	{
		fname = v->as<CPropStringValue>()->
	#ifndef UNICODE
		m_pStr
	#else
		m_pWStr
	#endif
		;
	}

	Properties::FolderProp::FolderProp(string const &n, string const &d, Properties_t const &p)
	: Property(PROPTYPE_FOLDER, n, d, PROPOPT_NIL)
	, props(p)
	{
	}
	auto Properties::FolderProp::prop()
	-> std::unique_ptr<impl>
	{
		struct derived_impl : impl
		{
			std::vector<std::pair<string, string> const> nds;
			derived_impl(FolderProp const &fp)
			{
				{
					nds.emplace_back(fp.name, fp.desc);
					std::unique_ptr<PropData> p {new PropData};
					p->dwID = 0;
					p->sName = reinterpret_cast<UINT_PTR>(nds.back().first.c_str());
					p->sInfo = reinterpret_cast<UINT_PTR>(nds.back().second.c_str());
					p->lType = fp.type;
					p->dwOptions = fp.options;
					p->lCreateParam = reinterpret_cast<LPARAM>(nullptr);
					props.emplace_back(std::move(p));
				}
				for(auto const &prop : fp.props)
				{
					nds.emplace_back(prop->name, prop->desc);
					std::unique_ptr<PropData> p {new PropData};
					p->dwID = 0;
					p->sName = reinterpret_cast<UINT_PTR>(nds.back().first.c_str());
					p->sInfo = reinterpret_cast<UINT_PTR>(nds.back().second.c_str());
					p->lType = prop->type;
					p->dwOptions = prop->options;
					p->lCreateParam = reinterpret_cast<LPARAM>(nullptr);
					props.emplace_back(std::move(p));
				}
				{
					std::unique_ptr<PropData> p {new PropData};
					p->dwID = -1;
					p->sName = reinterpret_cast<UINT_PTR>(nullptr);
					p->sInfo = reinterpret_cast<UINT_PTR>(nullptr);
					p->lType = PROPTYPE_FOLDER_END;
					p->dwOptions = PROPOPT_NIL;
					p->lCreateParam = reinterpret_cast<LPARAM>(nullptr);
					props.emplace_back(std::move(p));
				}
			}
		};
		return std::unique_ptr<impl>(new derived_impl(*this));
	}

	Properties::HeaderProp::HeaderProp(string const &n, string const &d)
	: Property(PROPTYPE_GROUP, n, d, PROPOPT_NIL)
	{
	}

	Properties::ImageListProp::ImageListProp(string const &n, string const &d, Images_t const &i, bool c, bool b, bool s)
	: Property(PROPTYPE_IMAGELIST, n, d, popt(c, b, s))
	, images(i)
	{
	}
	auto Properties::ImageListProp::value()
	-> std::unique_ptr<Value>
	{
		while(images.size() > std::numeric_limits<std::uint16_t>::max())
		{
			images.pop_back();
		}
		std::uint16_t size = static_cast<std::uint16_t>(images.size());
		std::unique_ptr<std::uint16_t []> imgs {new std::uint16_t[size+1]};
		for(std::uint16_t i = 0; i < size; ++i)
		{
			imgs[i+1] = images[i];
		}
		return Value::New(new CPropDataValue((size+1)*sizeof(std::uint16_t), reinterpret_cast<LPBYTE>(imgs.get())));
	}
	void Properties::ImageListProp::value(std::unique_ptr<Value> v)
	{
		std::uint16_t *imgs = reinterpret_cast<std::uint16_t *>(v->as<CPropDataValue>()->m_pData);
		std::uint16_t size = *imgs;
		images.clear();
		for(std::uint16_t i = 0; i < size; ++i)
		{
			images.push_back(imgs[i+1]);
		}
	}

	Properties::ImageFilenameProp::ImageFilenameProp(string const &n, string const &d, string const &fn, bool aa, bool c, bool b, bool s)
	: Property(PROPTYPE_PICTUREFILENAME, n, d, popt(c, b, s, true))
	, fname(fn)
	, allow_anims(aa)
	{
	}
	auto Properties::ImageFilenameProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			BOOL allow_anims;
			Derived(bool aa)
			: allow_anims(aa ? TRUE : FALSE)
			{
			}
			virtual LPARAM lParam() const override
			{
				return static_cast<LPARAM>(allow_anims);
			}
		};
		return std::unique_ptr<Param>(new Derived(allow_anims));
	}
	auto Properties::ImageFilenameProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropStringValue(fname.c_str()));
	}
	void Properties::ImageFilenameProp::value(std::unique_ptr<Value> v)
	{
		fname = v->as<CPropStringValue>()->
	#ifndef UNICODE
		m_pStr
	#else
		m_pWStr
	#endif
		;
	}

	Properties::SizeProp::SizeProp(string const &n, string const &d, Size_t const &sz, Sizes_t const &p, bool c, bool b, bool s)
	: Property(PROPTYPE_SIZE, n, d, popt(c, b, s, true))
	, size(sz)
	, predefined(p)
	{
	}
	auto Properties::SizeProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			std::unique_ptr<std::int32_t []> sizes;
			Derived(Sizes_t const &pr)
			: sizes(new std::int32_t[(pr.size()+1)*2])
			{
				sizes[pr.size()*2 +0] = 0;
				sizes[pr.size()*2 +1] = 0;
				for(Sizes_t::size_type i = 0; i < pr.size(); ++i)
				{
					sizes[i*2 +0] = pr[i].first;
					sizes[i*2 +1] = pr[i].second;
				}
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(sizes.get());
			}
		};
		return std::unique_ptr<Param>(new Derived(predefined));
	}
	auto Properties::SizeProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropSizeValue(size.first, size.second));
	}
	void Properties::SizeProp::value(std::unique_ptr<Value> v)
	{
		auto const &sz = *(v->as<CPropSizeValue>());
		size.first = sz.m_cx;
		size.second = sz.m_cy;
	}

	Properties::SpinProp::SpinProp(string const &n, string const &d, std::int16_t mn, std::int16_t mx, std::int16_t val, bool c, bool b, bool s)
	: Property(PROPTYPE_SPINEDIT, n, d, popt(c, b, s, true))
	, min(mn)
	, max(mx)
	, v(val)
	{
	}
	auto Properties::SpinProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			MinMaxParam mmp;
			Derived(std::int16_t min, std::int16_t max)
			: mmp{min, max}
			{
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(&mmp);
			}
		};
		return std::unique_ptr<Param>(new Derived(min, max));
	}
	auto Properties::SpinProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropDWordValue(v));
	}
	void Properties::SpinProp::value(std::unique_ptr<Value> val)
	{
		v = static_cast<std::int16_t>(val->as<CPropDWordValue>()->m_dwValue);
	}

	Properties::SliderProp::SliderProp(string const &n, string const &d, std::int32_t mn, std::int32_t mx, std::int32_t val, bool c, bool b, bool s)
	: Property(PROPTYPE_SLIDEREDIT, n, d, popt(c, b, s, true))
	, min(mn)
	, max(mx)
	, v(val)
	{
	}
	auto Properties::SliderProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			MinMaxParam mmp;
			Derived(std::int32_t min, std::int32_t max)
			: mmp{min, max}
			{
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(&mmp);
			}
		};
		return std::unique_ptr<Param>(new Derived(min, max));
	}
	auto Properties::SliderProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropDWordValue(v));
	}
	void Properties::SliderProp::value(std::unique_ptr<Value> val)
	{
		v = static_cast<std::int32_t>(val->as<CPropDWordValue>()->m_dwValue);
	}

	Properties::ReferenceToProp::ReferenceToProp(Property &p)
	: Property(p.type, p.name, p.desc, p.options)
	, property(p)
	{
	}
	auto Properties::ReferenceToProp::param()
	-> std::unique_ptr<Param>
	{
		return property.param();
	}
	auto Properties::ReferenceToProp::value()
	-> std::unique_ptr<Value>
	{
		return property.value();
	}
	void Properties::ReferenceToProp::value(std::unique_ptr<Value> v)
	{
		return property.value(std::move(v));
	}
	auto Properties::ReferenceToProp::prop()
	-> std::unique_ptr<impl>
	{
		return property.prop();
	}

	Properties::AdvancedProp::AdvancedProp(Property &p, bool l, bool rm, bool rn, bool m)
	: Property(p.type, p.name, p.desc, p.options|popt(false, false, false, false, false, false, false, rm, rn, m, l))
	, property(p)
	{
	}
	auto Properties::AdvancedProp::param()
	-> std::unique_ptr<Param>
	{
		return property.param();
	}
	auto Properties::AdvancedProp::value()
	-> std::unique_ptr<Value>
	{
		return property.value();
	}
	void Properties::AdvancedProp::value(std::unique_ptr<Value> v)
	{
		return property.value(std::move(v));
	}
	auto Properties::AdvancedProp::prop()
	-> std::unique_ptr<impl>
	{
		return property.prop();
	}

	Properties::UrlProp::UrlProp(string const &n, string const &d, string const &URL, bool c, bool b, bool s)
	: Property(PROPTYPE_URLBUTTON, n, d, popt(c, b, s, true))
	, url(URL)
	{
	}
	auto Properties::UrlProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			string const url;
			Derived(string const &URL)
			: url(URL)
			{
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(url.c_str());
			}
		};
		return std::unique_ptr<Param>(new Derived(url));
	}

	Properties::FloatSpinProp::FloatSpinProp(string const &n, string const &d, float mn, float mx, float val, float dt, bool c, bool b, bool s)
	: Property(PROPTYPE_SPINEDITFLOAT, n, d, popt(c, b, s, true))
	, min(mn)
	, max(mx)
	, delta(dt)
	, v(val)
	{
	}
	auto Properties::FloatSpinProp::param()
	-> std::unique_ptr<Param>
	{
		struct Derived : Param
		{
			MinMaxFloatParam mmfp;
			Derived(float min, float max, float delta)
			: mmfp{min, max, delta}
			{
			}
			virtual LPARAM lParam() const override
			{
				return reinterpret_cast<LPARAM>(&mmfp);
			}
		};
		return std::unique_ptr<Param>(new Derived(min, max, delta));
	}
	auto Properties::FloatSpinProp::value()
	-> std::unique_ptr<Value>
	{
		return Value::New(new CPropFloatValue(v));
	}
	void Properties::FloatSpinProp::value(std::unique_ptr<Value> val)
	{
		v = val->as<CPropFloatValue>()->m_fValue;
	}

	std::int32_t PropTab(Properties::Tab t)
	{
		switch(t)
		{
		case Properties::Tab::General:         return PROPID_TAB_GENERAL;
		case Properties::Tab::Specific:        return PROPID_TAB_CUSTOM1;
		case Properties::Tab::Display:         return PROPID_TAB_DISPLAY;
		case Properties::Tab::Window:          return PROPID_TAB_WINDOW;
		case Properties::Tab::SizeAndPosition: return PROPID_TAB_SIZEPOS;
		case Properties::Tab::TextOptions:     return PROPID_TAB_TEXTOPT;
		case Properties::Tab::Movements:       return PROPID_TAB_MOVEMENT;
		case Properties::Tab::RuntimeOptions:  return PROPID_TAB_RUNOPT;
		case Properties::Tab::AlterableValues: return PROPID_TAB_VALUES;
		case Properties::Tab::Behaviors:       return PROPID_TAB_EVENTS;
		case Properties::Tab::About:           return PROPID_TAB_ABOUT;
		case Properties::Tab::BluRay:          return PROPID_TAB_BLURAY;
		case Properties::Tab::iPhone:          return PROPID_TAB_IPHONE;
		case Properties::Tab::Android:         return PROPID_TAB_ANDROID;
		case Properties::Tab::XNA:             return PROPID_TAB_XNA;
		case Properties::Tab::Mac:             return PROPID_TAB_MAC;
		default:                               return PROPID_TAB_CUSTOM2;
		}
	}
}
