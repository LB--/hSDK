#include "hSDK/Resources.hpp"
#include "DLL.hpp"

namespace hSDK
{
	auto Resource::Get(string const &name)
	-> Resource
	{
		auto fr = FindResource(DLL, name.c_str(), T_("hSDK"));
		if(fr == NULL) return {"", 0u};

		auto lr = LoadResource(DLL, fr);
		if(lr == NULL) return {"", 0u};

		auto res = LockResource(lr);
		if(res == NULL) return {"", 0u};
		auto size = SizeofResource(DLL, fr);
		return {reinterpret_cast<data_t>(res), static_cast<std::size_t>(size)};
	}
}
