#include "Impl.hpp"

namespace hSDK
{
	ED *&Impl::ed = EdittimeInfo::Current().ed;
	RD *&Impl::rd = RuntimeInfo::Current().rd;
}
