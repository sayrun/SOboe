// ForceCheck.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
 	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B
	HANDLE	hForceRead = OpenEvent( EVENT_MODIFY_STATE, FALSE, "email|nxl#ForceRead");

	if( NULL != hForceRead)
	{
		SetEvent( hForceRead);
		CloseHandle( hForceRead);
		return 0;
	}
	else
	{
		MessageBox( NULL, "email.nxl��������܂���˂��B", "Force Chack", MB_ICONINFORMATION | MB_OK);
		return -1;
	}
}
