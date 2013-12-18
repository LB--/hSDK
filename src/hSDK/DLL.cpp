#include "DLL.hpp"

static HINSTANCE Dll = 0;
extern "C" BOOL WINAPI DllMain(HINSTANCE dll, DWORD reason, LPVOID)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		Dll = dll;
	}
	return TRUE;
}

namespace hSDK
{
	HMODULE const &DLL = Dll;
}
