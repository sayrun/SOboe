#include <windows.h>
#include <lmcons.h>
#include "SettingDlg.h"
#include "resource.h"

extern HANDLE	g_hModule;

static char		g_szIniFile[ _MAX_PATH];
static char*	g_pszAnony = "Anonymous";

static LPCSTR	alpcszSeedType[] =
{
	"disable",
	"nickname",
	"logon id",
	"computer name",
	"ip address"
};

BOOL OnInitDialog( HWND hWndDlg)
{
	char	szWork[ 128];
	UINT	unWork;

	char	szUser[ UNLEN + 1];
	char	szDrive[ _MAX_DRIVE];
	char	szDir[ _MAX_DIR];
	DWORD	dwSize;
	dwSize = UNLEN + 1;
	if( 0 >= GetUserName( szUser, &dwSize))
	{
		lstrcpy( szUser, g_pszAnony);
	}

	GetModuleFileName( NULL, g_szIniFile, _MAX_PATH);
	_splitpath( g_szIniFile, szDrive, szDir, NULL, NULL);
	_makepath( g_szIniFile, szDrive, szDir, "StdNet", "ini");

	char	szEntry[ 128];
	int nCount = GetPrivateProfileInt( szUser, "BcstAddrCnt", 1, g_szIniFile);
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		wsprintf( szEntry, "BcstAddr%d", nIndex + 1);
		GetPrivateProfileString( szUser, szEntry, "error", szWork, 128, g_szIniFile);
		if( !lstrcmp( szWork, "error"))
		{
			if( 0 == nIndex)
			{
				lstrcpy( szWork, "255.255.255.255");
			}
			else
			{
				break;
			}
		}
		SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_ADDSTRING, 0, ( LPARAM)szWork);
	}

	unWork = GetPrivateProfileInt( szUser, "Port", 7127, g_szIniFile);
	wsprintf( szWork, "%d", unWork);
	SetDlgItemText( hWndDlg, IDC_BCSTPORT, szWork);

	GetPrivateProfileString( szUser, "Nickname", szUser, szWork, 61, g_szIniFile);
	SetDlgItemText( hWndDlg, IDC_NICKNAME, szWork);

	GetPrivateProfileString( szUser, "Group", g_pszAnony, szWork, 61, g_szIniFile);
	SetDlgItemText( hWndDlg, IDC_GROUP, szWork);

	for( nIndex = 0; nIndex < 5; nIndex++)
	{
		SendDlgItemMessage( hWndDlg, IDC_SEEDTYPE, CB_ADDSTRING, 0, ( LPARAM)alpcszSeedType[ nIndex]);
	}
	int nSeedType = GetPrivateProfileInt( szUser, "SeedType", 0, g_szIniFile);
	nSeedType = ( 0 <= nSeedType && 4 >= nSeedType) ? nSeedType + 1 : 0;
	SendDlgItemMessage( hWndDlg, IDC_SEEDTYPE, CB_SETCURSEL, nSeedType, 0);

	char szNodelistPath[ _MAX_PATH];
	GetPrivateProfileString( szUser, "NodeListPath", "", szNodelistPath, _MAX_PATH, g_szIniFile);
	SetDlgItemText( hWndDlg, IDC_NODELISTPATH, szNodelistPath);
	EnableWindow( GetDlgItem( hWndDlg, IDC_NODELISTPATH), ( 0 < nSeedType));
	EnableWindow( GetDlgItem( hWndDlg, IDC_NODELISTSEL), ( 0 < nSeedType));

	return TRUE;
}

BOOL OnOK( HWND hWndDlg)
{
	char	szErrMsg[ 1024];
	char	szBcstAddr[ 128];
	char	szWork[ 128];
	char	szNickname[ 61];
	char	szGroup[ 1024];
	UINT	unPort;
	char	szUser[ UNLEN + 1];
	DWORD	dwSize;

	dwSize = UNLEN + 1;
	if( 0 >= GetUserName( szUser, &dwSize))
	{
		lstrcpy( szUser, g_pszAnony);
	}

	GetDlgItemText( hWndDlg, IDC_BCSTPORT, szWork, 128);
	unPort = atoi( szWork);
	if( 0 >= unPort)
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_BCSTPORT, szErrMsg, 1024);
		MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	GetDlgItemText( hWndDlg, IDC_NICKNAME, szNickname, 61);
	if( 0 >= lstrlen( szNickname))
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_NICKNAME, szErrMsg, 1024);
		MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	GetDlgItemText( hWndDlg, IDC_GROUP, szGroup, 1024);

	wsprintf( szWork, "%d", unPort);
	WritePrivateProfileString( szUser, "Port", szWork, g_szIniFile);

	WritePrivateProfileString( szUser, "Nickname", szNickname, g_szIniFile);
	WritePrivateProfileString( szUser, "Group", szGroup, g_szIniFile);

	char	szEntry[ 128];
	// 前のデータを無効化する
	int nCount = GetPrivateProfileInt( szUser, "BcstAddrCnt", 0, g_szIniFile);
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		wsprintf( szEntry, "BcstAddr%d", nIndex + 1);
		WritePrivateProfileString( szUser, szEntry, "", g_szIniFile);
	}
	
	nCount = SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETCOUNT, 0, 0);
	wsprintf( szWork, "%d", nCount);
	WritePrivateProfileString( szUser, "BcstAddrCnt", szWork, g_szIniFile);
	for( nIndex = 0; nIndex < nCount; nIndex++)
	{
		wsprintf( szEntry, "BcstAddr%d", nIndex + 1);
		SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETTEXT, nIndex, ( LPARAM)szBcstAddr);
		WritePrivateProfileString( szUser, szEntry, szBcstAddr, g_szIniFile);
	}

	int nSeedType = SendDlgItemMessage( hWndDlg, IDC_SEEDTYPE, CB_GETCURSEL, 0, 0);
	if( 0 <= nSeedType)nSeedType--;
	wsprintf( szWork, "%d", nSeedType);
	WritePrivateProfileString( szUser, "SeedType", szWork, g_szIniFile);

	char szNodelistPath[ _MAX_PATH];
	GetDlgItemText( hWndDlg, IDC_NODELISTPATH, szNodelistPath, _MAX_PATH);
	WritePrivateProfileString( szUser, "NodeListPath", szNodelistPath, g_szIniFile);

	EndDialog( hWndDlg, IDOK);
	return TRUE;
}

BOOL OnCancel( HWND hWndDlg)
{
	EndDialog( hWndDlg, IDCANCEL);
	return TRUE;
}

BOOL OnOpenHelp( HWND hWndDlg)
{
	WinHelp( hWndDlg, "StdNet.hlp", HELP_CONTEXT , HELP_MAIN);
	return TRUE;
}

BOOL OnHelp( HWND hWndDlg, LPHELPINFO lpHelpInfo)
{
	if( HELPINFO_WINDOW == lpHelpInfo->iContextType)
	{
		switch( lpHelpInfo->iCtrlId)
		{
		case IDC_BCSTADDR:
		case IDC_ADD:
		case IDC_ADDRLIST:
		case IDC_BCSTPORT:
		case IDC_NICKNAME:
		case IDC_GROUP:
		case IDC_SEEDTYPE:
		case IDC_NODELISTPATH:
		case IDC_NODELISTSEL:
			WinHelp( hWndDlg, "StdNet.hlp", HELP_CONTEXTPOPUP, 0x60000 | lpHelpInfo->iCtrlId);
		default:
			return TRUE;
		}
	}
	return FALSE;
}

BOOL OnAdd( HWND hWndDlg)
{
	char	szWork[ 64];
	GetDlgItemText( hWndDlg, IDC_BCSTADDR, szWork, 64);
	ULONG	ulAddr;

	if( 0 < lstrlen( szWork))
	{
		char	szEntry[ 32];
		char	szBcstAddr[ 128];

		ulAddr = inet_addr( szWork);
		int nCount = SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETCOUNT, 0, 0);
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			wsprintf( szEntry, "BcstAddr%d", nIndex + 1);
			SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETTEXT, nIndex, ( LPARAM)szBcstAddr);
			if( ulAddr == inet_addr( szBcstAddr))
			{
				return TRUE;
			}
		}
		SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_ADDSTRING, 0, ( LPARAM)szWork);
	}

	return TRUE;
}

BOOL OnAddrList( HWND hWndDlg, WORD wAction, HWND hWnd)
{
	if( hWnd == GetDlgItem( hWndDlg, IDC_ADDRLIST))
	{
		int nIndex = SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETCURSEL, 0, 0);
		if( 0 <= nIndex)
		{
			switch( wAction)
			{
			case LBN_DBLCLK:
				SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_DELETESTRING, nIndex, 0);
				break;
			case LBN_SELCHANGE:
				char	szBcstAddr[ 128];
				SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_GETTEXT, nIndex, ( LPARAM)szBcstAddr);
				SetDlgItemText( hWndDlg, IDC_BCSTADDR, szBcstAddr);
				break;
			case LBN_KILLFOCUS:
				SendDlgItemMessage( hWndDlg, IDC_ADDRLIST, LB_SETCURSEL, -1, 0);
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
	}

	return FALSE;
}

BOOL OnSeedTypeSelChange( HWND hWnd)
{
	bool blEnable = ( 0 < SendDlgItemMessage( hWnd, IDC_SEEDTYPE, CB_GETCURSEL, 0, 0));
	EnableWindow( GetDlgItem( hWnd, IDC_NODELISTPATH), blEnable);
	EnableWindow( GetDlgItem( hWnd, IDC_NODELISTSEL), blEnable);

	return TRUE;
}

BOOL OnNodeListSel( HWND hWnd)
{
	DWORD dwLen = GetCurrentDirectory( 0, NULL);
	char*	pszFolder;
	dwLen++;
	pszFolder = new char[ dwLen];
	GetCurrentDirectory( dwLen, pszFolder);

	OPENFILENAME	stOFN;
	char			szPath[ _MAX_PATH];
	ZeroMemory( szPath, _MAX_PATH);
	ZeroMemory( &stOFN, sizeof( OPENFILENAME));

	stOFN.lStructSize		= sizeof( stOFN);
	stOFN.lpstrFile			= szPath;
	stOFN.nMaxFile			= _MAX_PATH;
	stOFN.lpstrDefExt		= "ini";
	stOFN.Flags				= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	stOFN.hInstance			= 0;
	stOFN.lpfnHook			= NULL;
	stOFN.lpstrFilter		= "nodelist(*.ini)\0*.ini\0"; 
	stOFN.hwndOwner			= hWnd; 

	if( GetOpenFileName( &stOFN))
	{
		SetDlgItemText( hWnd, IDC_NODELISTPATH, szPath);
	}
	SetCurrentDirectory( pszFolder);
	delete [] pszFolder;

	return TRUE;
}

BOOL CALLBACK SettingDlg( HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch( uMsg)
	{
	case WM_INITDIALOG:
		return OnInitDialog( hWndDlg);
	case WM_COMMAND:
		if( CBN_SELCHANGE == HIWORD( wParam))
		{
			if( IDC_SEEDTYPE == LOWORD( wParam))return OnSeedTypeSelChange( hWndDlg);
		}
		else
		{
			if( IDOK == LOWORD( wParam))return OnOK( hWndDlg);
			if( IDCANCEL == LOWORD( wParam))return OnCancel( hWndDlg);
			if( IDC_OPENHELP == LOWORD( wParam))return OnOpenHelp( hWndDlg);
			if( IDC_ADD == LOWORD( wParam))return OnAdd( hWndDlg);
			if( IDC_NODELISTSEL == LOWORD( wParam))return OnNodeListSel( hWndDlg);
			if( IDC_ADDRLIST == LOWORD( wParam))return OnAddrList( hWndDlg, HIWORD( wParam), ( HWND)lParam);
		}
		break;
	case WM_HELP:
		return OnHelp( hWndDlg, ( LPHELPINFO)lParam);
	}
	return FALSE;
}
