#include "stdafx.h"
#include "GetIconDlg.h"
#include "GetIconApi.h"

int DoUpdateIcon( HWND hWnd)
{
	return GetIconDlg_DoModal( hWnd);
}