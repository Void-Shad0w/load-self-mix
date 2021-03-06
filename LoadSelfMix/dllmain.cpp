#include <windows.h>
#include <memory>
#include <cassert>
typedef BOOL(WINAPI *PFN_SFILEOPENARCHIVE)(char *archivename, DWORD dwPriority, DWORD dwFlags, HANDLE *handle);
typedef BOOL(WINAPI *PFNSFILECLOSEARCHIVE)(HANDLE handle);

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID /*lpReserved*/
)
{
	static HMODULE hStormDll = GetModuleHandleW(L"storm.dll");
	std::unique_ptr<HANDLE> hFile{std::make_unique<HANDLE>()};

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		static PFN_SFILEOPENARCHIVE  pfnStorm_266 = reinterpret_cast<PFN_SFILEOPENARCHIVE>(GetProcAddress(hStormDll, reinterpret_cast<const char*>(266)));
		char self_path[MAX_PATH] = { '\0' };
		GetModuleFileNameA(hModule, self_path, MAX_PATH);
		pfnStorm_266(self_path, 0x10, 0, hFile.get());
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		static PFNSFILECLOSEARCHIVE  pfnStorm_252 = reinterpret_cast<PFNSFILECLOSEARCHIVE>(GetProcAddress(hStormDll, reinterpret_cast<const char*>(252)));
		pfnStorm_252(hFile.get());
		hFile.reset();
	}

	return TRUE;
}