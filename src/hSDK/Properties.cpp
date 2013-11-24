#include "hSDK/Properties.hpp"

#include "MMF2API.hpp"

namespace hSDK
{
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
