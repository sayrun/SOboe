#include "StdeMail.h"
#include "DInputDlg.h"
#include "resource.h"

static char	g_szAddrress[ 256];

BOOL DInputDlg_OnHelp( HWND hWndDlg, LPHELPINFO lpHelpInfo)
{
	if( HELPINFO_WINDOW == lpHelpInfo->iContextType)
	{
		switch( lpHelpInfo->iCtrlId)
		{
		case IDC_EDITDADDRESS:
			WinHelp( hWndDlg, "email.hlp", HELP_CONTEXTPOPUP, 0x60000 | lpHelpInfo->iCtrlId);
		default:
			return TRUE;
		}
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
	GetDlgItemText( hWndDlg, IDC_EDITDADDRESS, g_szAddrress, 255);

	return EndDialog( hWndDlg, IDOK);
}

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
	case WM_HELP:
		return DInputDlg_OnHelp( hWndDlg, ( LPHELPINFO)lParam);
	}
	return FALSE;
}

LPCSTR DInputDlg_GetAddress( void)
{
	return g_szAddrress;
}
