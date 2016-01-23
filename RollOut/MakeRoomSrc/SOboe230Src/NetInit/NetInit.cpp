// NetInit.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <winsock.h>
#include "NetInit.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

int InitializeWinsock( WORD* pwVersion)
{
	WSADATA	stWSAData;
	WORD wVerReq = MAKEWORD( 2, 2);
	int nResult = WSAStartup( wVerReq, &stWSAData);
	*pwVersion = stWSAData.wVersion;
	return nResult;
}

int TerminateWinsock( void)
{
	return WSACleanup();
}
