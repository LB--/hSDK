#include "Impl.hpp"

#include "FusionAPI.hpp"

namespace hSDK
{
	ED *&Impl::ed = EdittimeInfo::Current().ed;
	RD *&Impl::rd = RuntimeInfo::Current().rd;
	mv *Impl::mV = nullptr;

	bool isUnicode()
	{
		return Impl::mV->mvCallFunction(nullptr, EF_ISUNICODE, 0, 0, 0) != 0;
	}
	bool isHWA()
	{
		return Impl::mV->mvCallFunction(nullptr, EF_ISHWA, 0, 0, 0) != 0;
	}
}
