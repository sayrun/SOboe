#include <windows.h>
#include "AboutDlg.h"
#include "resource.h"

BOOL CALLBACK AboutDlg( HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_COMMAND:
		if( IDOK == wParam)EndDialog( hWndDlg, IDOK);
		if( IDCANCEL == wParam)EndDialog( hWndDlg, IDOK);
		return TRUE;
	}
	return FALSE;
}
