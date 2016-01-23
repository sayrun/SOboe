#include <windows.h>
#include "NetEx_IF.h"
#include "Mltcstsrch.h"

HANDLE				g_hModule = 0;
CRITICAL_SECTION	g_csFileWrite;
CRITICAL_SECTION	g_csDInput;

MCSTNETORGDATA	g_stOrgData = 
{
	FALSE,	// BOOL blInitialized;
	NULL,	// HANDLE hStopEvent;
	0,		//ULONG ulRcvAddress;
	0,		//ULONG ulMyAddress;
	0,		//UINT unMultiPort;
	0,		//int nTTL;
	0,		//BOOL blAbsence;
	NULL,	//RECEIVEDATA lpfnRcvData;
	NULL,	//CEntryList* pcEntryList;
	NULL,	//CGroupList* pcGroupList;
	NULL,	//char* pszEntryName;
	NULL,	//char* pszNickname;
	NULL	//char* pszGroupName;
};

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch( ul_reason_for_call )
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
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
