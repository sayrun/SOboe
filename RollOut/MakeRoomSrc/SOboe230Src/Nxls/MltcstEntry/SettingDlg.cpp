#include <windows.h>
#include <lmcons.h>
#include "SettingDlg.h"
#include "resource.h"

extern HANDLE	g_hModule;

typedef struct tagBADDR
{
	BYTE	b1;
	BYTE	b2;
	BYTE	b3;
	BYTE	b4;
}BADDR;

typedef union tagIPADDR
{
	ULONG	ulAddr;
	BADDR	bAddr;
}IPADDR;

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
	char	szWork[ 1024];
	UINT	unWork;
	int		nWork;

	char	szUser[ UNLEN + 1];
	char	szDrive[ _MAX_DRIVE];
	char	szDir[ _MAX_DIR];
	DWORD	dwSize;
	dwSize = UNLEN + 1;
	if( !GetUserName( szUser, &dwSize))
	{
		lstrcpy( szUser, g_pszAnony);
	}

	GetModuleFileName( NULL, g_szIniFile, _MAX_PATH);
	_splitpath( g_szIniFile, szDrive, szDir, NULL, NULL);
	_makepath( g_szIniFile, szDrive, szDir, "Mltcstsrch", "ini");

	GetPrivateProfileString( szUser, "McstAddr", "239.191.38.82", szWork, 128, g_szIniFile);
	if( INADDR_NONE == inet_addr( szWork) || 0 >= lstrlen( szWork))
	{
		lstrcpy( szWork, "234.191.38.82");
	}
	SetDlgItemText( hWndDlg, IDC_MCSTADDR, szWork);

	GetPrivateProfileString( szUser, "McstIF", "0.0.0.0", szWork, 128, g_szIniFile);
	if( INADDR_NONE == inet_addr( szWork))
	{
		lstrcpy( szWork, "0.0.0.0");
	}
	SetDlgItemText( hWndDlg, IDC_MCSTIF, szWork);

	unWork = GetPrivateProfileInt( szUser, "McstPort", 7129, g_szIniFile);
	wsprintf( szWork, "%d", unWork);
	SetDlgItemText( hWndDlg, IDC_MCSTPORT, szWork);

	nWork = GetPrivateProfileInt( szUser, "McstTTL", 1, g_szIniFile);
	wsprintf( szWork, "%d", nWork);
	SetDlgItemText( hWndDlg, IDC_MCSTTTL, szWork);

	GetPrivateProfileString( szUser, "Nickname", szUser, szWork, 61, g_szIniFile);
	SetDlgItemText( hWndDlg, IDC_NICKNAME, szWork);

	GetPrivateProfileString( szUser, "Group", g_pszAnony, szWork, 1024, g_szIniFile);
	SetDlgItemText( hWndDlg, IDC_GROUP, szWork);

	for( int nIndex = 0; nIndex < 5; nIndex++)
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
	char	szMcstAddr[ 128];
	char	szMcstIF[ 128];
	char	szWork[ 128];
	char	szNickname[ 61];
	char	szGroup[ 1024];
	UINT	unPort;
	int		nTTL;
	char	szUser[ UNLEN + 1];
	DWORD	dwSize;
	IPADDR	stIpAddr;
	int		nSeedType;

	dwSize = UNLEN + 1;
	if( !GetUserName( szUser, &dwSize))
	{
		lstrcpy( szUser, g_pszAnony);
	}

	GetDlgItemText( hWndDlg, IDC_MCSTADDR, szMcstAddr, 128);
	if( INADDR_NONE == inet_addr( szMcstAddr))
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_MCSTADDR, szErrMsg, 1024);
		MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}
	else
	{
		stIpAddr.ulAddr = inet_addr( szMcstAddr);
		// 224.0.0.0 - 239.255.255.255
		if( 224 > stIpAddr.bAddr.b1 || 239 < stIpAddr.bAddr.b1)
		{
			LoadString( ( HINSTANCE)g_hModule, IDS_ILLEGAL_MCSTADDR, szErrMsg, 1024);
			MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
			return TRUE;
		}
	}

	GetDlgItemText( hWndDlg, IDC_MCSTIF, szMcstIF, 128);
	if( INADDR_NONE == inet_addr( szMcstIF))
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_MCSTIF, szErrMsg, 1024);
		MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	GetDlgItemText( hWndDlg, IDC_MCSTPORT, szWork, 128);
	unPort = atoi( szWork);
	if( 0 >= unPort)
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_MCSTPORT, szErrMsg, 1024);
		MessageBox( hWndDlg, szErrMsg, "data error", MB_OK | MB_ICONWARNING);
		return TRUE;
	}

	GetDlgItemText( hWndDlg, IDC_MCSTTTL, szWork, 128);
	nTTL = atoi( szWork);
	if( 0 > nTTL)
	{
		LoadString( ( HINSTANCE)g_hModule, IDS_NA_MCSTTTL, szErrMsg, 1024);
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
	nSeedType = SendDlgItemMessage( hWndDlg, IDC_SEEDTYPE, CB_GETCURSEL, 0, 0);
	if( 0 <= nSeedType)nSeedType--;

	wsprintf( szWork, "%d", unPort);
	WritePrivateProfileString( szUser, "McstPort", szWork, g_szIniFile);
	WritePrivateProfileString( szUser, "McstAddr", szMcstAddr, g_szIniFile);
	WritePrivateProfileString( szUser, "McstIF", szMcstIF, g_szIniFile);
	wsprintf( szWork, "%d", nTTL);
	WritePrivateProfileString( szUser, "McstTTL", szWork, g_szIniFile);

	WritePrivateProfileString( szUser, "Nickname", szNickname, g_szIniFile);
	WritePrivateProfileString( szUser, "Group", szGroup, g_szIniFile);

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
	WinHelp( hWndDlg, "Mltcstsrch.hlp", HELP_CONTEXT , HELP_MAIN);
	return TRUE;
}

BOOL OnHelp( HWND hWndDlg, LPHELPINFO lpHelpInfo)
{
	if( HELPINFO_WINDOW == lpHelpInfo->iContextType)
	{
		switch( lpHelpInfo->iCtrlId)
		{
		case IDC_MCSTADDR:
		case IDC_MCSTIF:
		case IDC_MCSTPORT:
		case IDC_MCSTTTL:
		case IDC_NICKNAME:
		case IDC_GROUP:
		case IDC_SEEDTYPE:
		case IDC_NODELISTPATH:
		case IDC_NODELISTSEL:
			WinHelp( hWndDlg, "Mltcstsrch.hlp", HELP_CONTEXTPOPUP, 0x60000 | lpHelpInfo->iCtrlId);
		default:
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
			if( IDC_NODELISTSEL == LOWORD( wParam))return OnNodeListSel( hWndDlg);
		}
		break;
	case WM_HELP:
		return OnHelp( hWndDlg, ( LPHELPINFO)lParam);
	}
	return FALSE;
}
