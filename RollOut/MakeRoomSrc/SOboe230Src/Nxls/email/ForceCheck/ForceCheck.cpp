// ForceCheck.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: この位置にコードを記述してください。
	HANDLE	hForceRead = OpenEvent( EVENT_MODIFY_STATE, FALSE, "email|nxl#ForceRead");

	if( NULL != hForceRead)
	{
		SetEvent( hForceRead);
		CloseHandle( hForceRead);
		return 0;
	}
	else
	{
		MessageBox( NULL, "email.nxlが見つかりませんねぇ。", "Force Chack", MB_ICONINFORMATION | MB_OK);
		return -1;
	}
}
