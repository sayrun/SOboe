#include <windows.h>
#include "DInputDlg.h"
#include "resource.h"

static char	g_szAddrress[ 256];

BOOL CALLBACK DInputDlg( HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_INITDIALOG:
		return DInputDlg_OnInitDialog( hWndDlg);
	case WM_COMMAND:
		if( IDOK == wParam)DInputDlg_OnOK( hWndDlg);
		if( IDCANCEL == wParam)EndDialog( hWndDlg, IDCANCEL);
		return TRUE;
	}
	return FALSE;
}

BOOL DInputDlg_OnInitDialog( HWND hWndDlg)
{
	g_szAddrress[ 0] = 0;

	return TRUE;
}

BOOL DInputDlg_OnOK( HWND hWndDlg)
{
	GetDlgItemText( hWndDlg, IDC_EDITIPADD, g_szAddrress, 255);

	return EndDialog( hWndDlg, IDOK);
}

LPCSTR GetDInputString( void)
{
	return g_szAddrress;
}