#include "StdeMail.h"
#include "NetEx_IF.h"
#include "email.h"

HANDLE			g_hModule;
EMAILORGDATA	g_stOrgData;

extern CRITICAL_SECTION g_csFileWrite;
extern CRITICAL_SECTION	g_csDInput;

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
		ZeroMemory( &g_stOrgData, sizeof( EMAILORGDATA));
		InitializeCriticalSection( &g_csFileWrite);
		InitializeCriticalSection( &g_csDInput);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DeleteCriticalSection( &g_csFileWrite);
		DeleteCriticalSection( &g_csDInput);
		break;
    }
    return TRUE;
}
