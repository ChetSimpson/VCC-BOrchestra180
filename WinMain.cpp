#include <windows.h>
#include "resource.h"
#include "BOrchestra180.h"


HINSTANCE HtxModuleInstance = nullptr;


BOOL WINAPI DllMain(HINSTANCE instance, DWORD /*readon*/, LPVOID /*reserved*/)
{
	HtxModuleInstance = instance;
	return TRUE;
}


extern "C" void ProgramPak_GetName(char* ModName, char* CatNumber, DYNAMICMENUCALLBACK /*dynamicMenuCallback*/)
{
	LoadString(HtxModuleInstance, IDS_MODULE_NAME, ModName, MAX_LOADSTRING);
	LoadString(HtxModuleInstance, IDS_CATNUMBER, CatNumber, MAX_LOADSTRING);
}


