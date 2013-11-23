#include "impl.hpp"

namespace hSDK
{
	ED *&impl::ed = EdittimeInfo::Current().ed;
	RD *&impl::rd = RuntimeInfo::Current().rd;
}
