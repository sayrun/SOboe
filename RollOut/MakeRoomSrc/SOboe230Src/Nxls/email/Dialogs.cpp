#include "StdeMail.h"
#include "Resource.h"
#include "Dialogs.h"

BOOL CALLBACK AboutDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		if( IDOK == wParam)
		{
			EndDialog( hwndDlg, IDOK);
		}
		if( IDCANCEL == wParam)
		{
			EndDialog( hwndDlg, IDCANCEL);
		}
		return TRUE;
	}
	return FALSE;
}

#ifdef	_POP3_TOO
BOOL CALLBACK PWInputDialogProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_INITDIALOG:
		{
			int		nData = GetWindowLong( hwndDlg, DWL_USER);

			RECT	stRect;
			SIZE	stSize;
			GetWindowRect( hwndDlg, &stRect);
			stSize.cx = stRect.right - stRect.left;
			stSize.cy = stRect.bottom - stRect.top;
			SetWindowPos( hwndDlg, HWND_TOPMOST, ( GetSystemMetrics( SM_CXSCREEN) - stSize.cx) / 2, ( GetSystemMetrics( SM_CYSCREEN) - stSize.cy) / 2, stSize.cx, stSize.cy, SWP_NOSIZE);
			SetWindowLong( hwndDlg, DWL_USER, lParam);
			DWORD dwECode = GetLastError();
			char	szText[ 128];
			wsprintf( szText, "%d\n", dwECode);
			OutputDebugString( szText);

			SetForegroundWindow( hwndDlg);
		}
		return TRUE;
	case WM_COMMAND:
		if( IDOK == wParam)
		{
			LPSTR	lpsz;
			lpsz = ( LPSTR)GetWindowLong( hwndDlg, DWL_USER);
			GetDlgItemText( hwndDlg, IDC_POP3LOGINPW, lpsz, 128);
			EndDialog( hwndDlg, IDOK);
		}
		if( IDCANCEL == wParam)
		{
			EndDialog( hwndDlg, IDCANCEL);
		}
		return TRUE;
	}
	return FALSE;
}
#endif
