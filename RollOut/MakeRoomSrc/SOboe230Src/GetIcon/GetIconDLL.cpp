// GetIcon.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"

HINSTANCE g_hInstance;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch( ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hInstance = ( HINSTANCE)hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

