#include "StdeMail.h"
#include "resource.h"
#include "property.h"

#define	_MAX_WORK	512

char*	g_pszAnony = "Anonymous";

static char*	pcszSmtpServer = "SmtpServer";
static char*	pcszSmtpPort = "SmtpPort";
static char*	pcszEnablePop3 = "EnablePop3";
static char*	pcszPop3Server = "Pop3Server";
static char*	pcszPop3Port = "Pop3Port";
static char*	pcszPop3User = "Pop3User";
static char*	pcszPop3Pass = "Pop3Pass";
static char*	pcszPop3Apop = "Pop3APop";
static char*	pcszPop3NoSave = "Pop3NoSave";
static char*	pcszPop3Interval = "Pop3Interval";
static char*	pcszMailAddress = "MailAddress";
static char*	pcszRealName = "RealName";
static char*	pcszAliasFile = "AliasFile";
static char*	pcszSignatureFile = "SignatureFile";
static char*	pcszLocalHost = "LocalHost";
static char*	pcszWithMyAddressInBCC = "WithMyAddressInBCC";
static char*	pcszEnableHan2Zen = "EnableHan2Zen";
static char*	pcszEnableFormatting = "EnableFormatting";
static char*	pcszCharCount = "CharCount";

static char*	pcszMsgboxCaption = "email.nxl";

extern HANDLE	g_hModule;

int GetInitFileName( char* pszIniFile, int nSize)
{
	char	szIniFile[ _MAX_PATH];
	char	szDrive[ _MAX_DRIVE];
	char	szDir[ _MAX_DIR];
	GetModuleFileName( NULL, szIniFile, _MAX_PATH);
	_splitpath( szIniFile, szDrive, szDir, NULL, NULL);
	_makepath( szIniFile, szDrive, szDir, "email", "ini");

	if( NULL != pszIniFile)
	{
		lstrcpyn( pszIniFile, szIniFile, nSize);
	}
	return lstrlen( szIniFile);
}

int GetIniUserName( char* pszUserName, int nSize)
{
	char	szUser[ UNLEN + 1];
	DWORD	dwUnLen = UNLEN + 1;
	if( 0 >= GetUserName( szUser, &dwUnLen))
	{
		lstrcpy( szUser, g_pszAnony);
	}

	if( NULL != pszUserName)
	{
		lstrcpyn( pszUserName, szUser, nSize);
	}

	return lstrlen( szUser);
}

BOOL PropSheetProc_OnNotifyHelp( HWND hWnd, LPHELPINFO lpHelpInfo)
{
	if( NULL != lpHelpInfo)
	{
		if( HELPINFO_WINDOW == lpHelpInfo->iContextType)
		{
			switch( lpHelpInfo->iCtrlId)
			{
			// Private
			case IDC_LOCALHOST:
			case IDC_MAILADDRESS:
			case IDC_REALNAME:
			case IDC_ADDRBOOK:
			case IDC_ADRBOKSEL:
			case IDC_SIGNATUREFILE:
			case IDC_SIGSEL:
			// SMTEP:
			case IDC_SMTPADDR:
			case IDC_SMTPPORT:
			case IDC_SENDSPECIFIC:
			case IDC_WITHMYADDRESS:
			case IDC_MBBTOMBC:
			case IDC_FORMATTING:
			case IDC_CHARCOUNT:
			// POP3
			case IDC_POP3:
			case IDC_POP3ADDR:
			case IDC_POP3PORT:
			case IDC_POP3LOGINID:
			case IDC_POP3LOGINPW:
			case IDC_NOSAVEPW:
			case IDC_POP3INTERVAL:
				WinHelp( hWnd, "email.hlp", HELP_CONTEXTPOPUP, 0x60000 | lpHelpInfo->iCtrlId);
			default:
				return TRUE;
			}
		}
	}
	else
	{
		WinHelp( hWnd, "email.hlp", HELP_CONTEXT, 1000/*HELP_MAIN*/);
	}
	return FALSE;
}

BOOL SmtpPageProc_OnInitDialog( HWND hWnd)
{
	char	szUser[ UNLEN + 1];
	GetIniUserName( szUser, UNLEN + 1);

	char	szIniFile[ _MAX_PATH];
	GetInitFileName( szIniFile, _MAX_PATH);

	int		nWork;
	BOOL	blWork;
	char	szWork[ _MAX_WORK];
	UINT	unPort;

	GetPrivateProfileString( szUser, pcszSmtpServer, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_SMTPADDR, szWork);
	}

	unPort = GetPrivateProfileInt( szUser, pcszSmtpPort, 25, szIniFile);
	wsprintf( szWork, "%d", unPort);
	SetDlgItemText( hWnd, IDC_SMTPPORT, szWork);

	// with my address in BCC.
	blWork = GetPrivateProfileInt( szUser, pcszWithMyAddressInBCC, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_WITHMYADDRESS, blWork ? BST_CHECKED : BST_UNCHECKED);

	// enable exchange han to zen.
	blWork = GetPrivateProfileInt( szUser, pcszEnableHan2Zen, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_MBBTOMBC, blWork ? BST_CHECKED : BST_UNCHECKED);

	// enable formatting
	blWork = GetPrivateProfileInt( szUser, pcszEnableFormatting, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_FORMATTING, blWork ? BST_CHECKED : BST_UNCHECKED);
		// char count
		nWork = GetPrivateProfileInt( szUser, pcszCharCount, 70, szIniFile);
		if( 10 > nWork)nWork = 10;
		wsprintf( szWork, "%d", nWork);
		SetDlgItemText( hWnd, IDC_CHARCOUNT, szWork);
		EnableWindow( GetDlgItem( hWnd, IDC_CHARCOUNT), blWork);

	return TRUE;
}

BOOL SmtpPageProc_OnNotifyApply( HWND hWnd, LPARAM lParam)
{
	int		nWork;
	BOOL	blWork;
	char	szWork[ _MAX_WORK];

	char	szUser[ UNLEN + 1];
	char	szIniFile[ _MAX_PATH];
	GetIniUserName( szUser, UNLEN + 1);
	GetInitFileName( szIniFile, _MAX_PATH);

	// SMTP Address
	GetDlgItemText( hWnd, IDC_SMTPADDR, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszSmtpServer, szWork, szIniFile);

	// SMTP Port
	GetDlgItemText( hWnd, IDC_SMTPPORT, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszSmtpPort, szWork, szIniFile);

	// with my address in BCC.
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_WITHMYADDRESS));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszWithMyAddressInBCC, szWork, szIniFile);

	// enable exchange han to zen.
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_MBBTOMBC));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszEnableHan2Zen, szWork, szIniFile);

	// enable formatting
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_FORMATTING));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszEnableFormatting, szWork, szIniFile);

	// char count
	if( blWork)
	{
		GetDlgItemText( hWnd, IDC_CHARCOUNT, szWork, _MAX_WORK);
		nWork = atoi( szWork);
		if( 10 > nWork)nWork = 10;
		WritePrivateProfileString( szUser, pcszCharCount, szWork, szIniFile);
	}

	return TRUE;
}

BOOL SmtpPageProc_OnKillActive( HWND hWnd, LPARAM lParam)
{
	BOOL	blWork;
	char	szWork[ _MAX_WORK];

	// SMTP Address
	GetDlgItemText( hWnd, IDC_SMTPADDR, szWork, _MAX_WORK);
	if( 0 >= lstrlen( szWork))
	{
		MessageBox( hWnd, "SMTPｻｰﾊﾞが設定されていません", pcszMsgboxCaption, MB_OK);
		SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
		return TRUE;
	}

	// SMTP Port
	UINT unPort = GetDlgItemInt( hWnd, IDC_SMTPPORT, &blWork, FALSE);
	if( FALSE == blWork)
	{
		MessageBox( hWnd, "SMTPﾎﾟｰﾄ番号が不整です", pcszMsgboxCaption, MB_OK);
		SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
		return TRUE;
	}

	return TRUE;
}

BOOL SmtpPageProc_OnNotify( HWND hDlg, LPARAM lParam)
{
	LPPSHNOTIFY	pstPsNotify;

	pstPsNotify = ( LPPSHNOTIFY)lParam;
	switch( pstPsNotify->hdr.code)
	{
	case PSN_KILLACTIVE:
		return SmtpPageProc_OnKillActive( hDlg, lParam);
	case PSN_APPLY:
		return SmtpPageProc_OnNotifyApply( hDlg, lParam);
	case PSN_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)pstPsNotify->lParam);
	}
	return TRUE;
}

BOOL CALLBACK SmtpPageProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_INITDIALOG:
		return SmtpPageProc_OnInitDialog( hDlg);

	case WM_NOTIFY:
		return SmtpPageProc_OnNotify( hDlg, lParam);

	case WM_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)lParam);
	}
	return FALSE;
}

BOOL Pop3PageProc_OnInitDialog( HWND hWnd)
{
	char	szUser[ UNLEN + 1];
	char	szIniFile[ _MAX_PATH];
	GetIniUserName( szUser, UNLEN + 1);
	GetInitFileName( szIniFile, _MAX_PATH);

	char	szWork[ _MAX_WORK];
	UINT	unPort;

	BOOL blEnablePop3 = GetPrivateProfileInt( szUser, pcszEnablePop3, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_POP3, ( blEnablePop3 ? BST_CHECKED : BST_UNCHECKED));
	EnableWindow( GetDlgItem( hWnd, IDC_POP3ADDR), blEnablePop3);
	EnableWindow( GetDlgItem( hWnd, IDC_POP3PORT), blEnablePop3);
	EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINID), blEnablePop3);
	EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINPW), blEnablePop3);
#if(_WIN32_WINNT >= 0x0400)
	EnableWindow( GetDlgItem( hWnd, IDC_APOP), blEnablePop3);
#else
	EnableWindow( GetDlgItem( hWnd, IDC_APOP), FALSE);
	ShowWindow( GetDlgItem( hWnd, IDC_APOP), SW_HIDE);
#endif
	EnableWindow( GetDlgItem( hWnd, IDC_NOSAVEPW), blEnablePop3);
	EnableWindow( GetDlgItem( hWnd, IDC_POP3INTERVAL), blEnablePop3);

	GetPrivateProfileString( szUser, pcszPop3Server, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_POP3ADDR, szWork);
	}

	unPort = GetPrivateProfileInt( szUser, pcszPop3Port, 110, szIniFile);
	wsprintf( szWork, "%d", unPort);
	SetDlgItemText( hWnd, IDC_POP3PORT, szWork);

	GetPrivateProfileString( szUser, pcszPop3User, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_POP3LOGINID, szWork);
	}

	// Interval
	int nWork = GetPrivateProfileInt( szUser, pcszPop3Interval, 30, szIniFile);
	wsprintf( szWork, "%d", nWork);
	SetDlgItemText( hWnd, IDC_POP3INTERVAL, szWork);

	// APOP
	BOOL blWork = GetPrivateProfileInt( szUser, pcszPop3Apop, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_APOP, ( blWork ? BST_CHECKED : BST_UNCHECKED));

	// No save Password
	blWork = GetPrivateProfileInt( szUser, pcszPop3NoSave, 0, szIniFile);
	CheckDlgButton( hWnd, IDC_NOSAVEPW, ( blWork ? BST_CHECKED : BST_UNCHECKED));

	if( blWork)
	{
		EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINPW), FALSE);
	}
	else
	{
		char	szPassWord[ 128];
		if( GetPrivateProfileStruct(szUser, pcszPop3Pass, szPassWord, 128, szIniFile))
		{
			BOOL blFlipFlop = FALSE;
			DWORD dwKey = 0x7D15217B;
			DWORD dwData;
			for( int nIndex = ( 128 / 4) - 1; nIndex >= 0; nIndex--)
			{
				dwData = *( ( DWORD*)&szPassWord[ nIndex * 4]);
				dwData ^= dwKey;
				dwKey = dwData;
				*( ( DWORD*)&szPassWord[ nIndex * 4]) = dwData;
				if( blFlipFlop)dwKey = MAKELONG( HIWORD( dwKey), LOWORD( dwKey));
				blFlipFlop = !blFlipFlop;
			}
			// "文字列"に変更
			for( nIndex = 128 - 1; nIndex >= 0; nIndex++)
			{
				if( 0 == szPassWord[ nIndex])break;
				szPassWord[ nIndex] = 0;
			}
			SetDlgItemText( hWnd, IDC_POP3LOGINPW, szPassWord);
		}
	}

	return TRUE;
}

BOOL Pop3PageProc_OnNotifyApply( HWND hWnd, LPARAM lParam)
{
	char	szUser[ UNLEN + 1];
	char	szIniFile[ _MAX_PATH];
	GetIniUserName( szUser, UNLEN + 1);
	GetInitFileName( szIniFile, _MAX_PATH);

	BOOL	blWork;
	char	szWork[ _MAX_WORK];

	// POP3
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_POP3));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszEnablePop3, szWork, szIniFile);

	// POP3 Address
	GetDlgItemText( hWnd, IDC_POP3ADDR, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszPop3Server, szWork, szIniFile);

	// POP3 Port
	UINT unPort = GetDlgItemInt( hWnd, IDC_POP3PORT, &blWork, FALSE);
	wsprintf( szWork, "%d", unPort);
	WritePrivateProfileString( szUser, pcszPop3Port, szWork, szIniFile);

	// UserID
	GetDlgItemText( hWnd, IDC_POP3LOGINID, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszPop3User, szWork, szIniFile);

	// Interval
	int nInterval = GetDlgItemInt( hWnd, IDC_POP3INTERVAL, &blWork, TRUE);
	wsprintf( szWork, "%d", nInterval);
	WritePrivateProfileString( szUser, pcszPop3Interval, szWork, szIniFile);

	// No save Password
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_NOSAVEPW));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszPop3NoSave, szWork, szIniFile);

	// PassWord
	if( blWork)
	{
		WritePrivateProfileStruct( szUser, pcszPop3Pass, NULL, 0, szIniFile);
	}
	else
	{
		char	szPassWord[ 128];

		GetDlgItemText( hWnd, IDC_POP3LOGINPW, szWork, _MAX_WORK);
		int nStringSize = lstrlen( szWork) + 1;
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		srand( ( stSysTime.wDay * 24 + stSysTime.wHour) * 60 + stSysTime.wMinute);
		for( int nIndex = nStringSize; nIndex < ( _MAX_WORK - sizeof( int)); nIndex += sizeof( int))
		{
			*( ( int*)&szWork[ nIndex]) = rand();
		}
		CopyMemory( szPassWord, szWork, 128);

		BOOL	blFlipFlop = FALSE;
		DWORD	dwKey = 0x7D15217B;
		DWORD	dwData;
		for( nIndex = ( 128 / 4) - 1; nIndex >= 0; nIndex--)
		{
			dwData = *( ( DWORD*)&szPassWord[ nIndex * 4]);
			dwData ^= dwKey;
			dwKey = *( ( DWORD*)&szPassWord[ nIndex * 4]);
			*( ( DWORD*)&szPassWord[ nIndex * 4]) = dwData;
			if( blFlipFlop)dwKey = MAKELONG( HIWORD( dwKey), LOWORD( dwKey));
			blFlipFlop = !blFlipFlop;
		}
		WritePrivateProfileStruct( szUser, pcszPop3Pass, szPassWord, 128, szIniFile);
	}

	// APOP
	blWork = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_APOP));
	wsprintf( szWork, "%d", blWork);
	WritePrivateProfileString( szUser, pcszPop3Apop, szWork, szIniFile);


	return TRUE;
}

BOOL Pop3PageProc_OnKillActive( HWND hWnd, LPARAM lParam)
{
	char	szWork[ _MAX_WORK];
	BOOL	blWork;

	// POP3 Address
	if( IsWindowEnabled( GetDlgItem( hWnd, IDC_POP3ADDR)))
	{
		GetDlgItemText( hWnd, IDC_POP3ADDR, szWork, _MAX_WORK);
		if( 0 >= lstrlen( szWork))
		{
			MessageBox( hWnd, "Pop3ｻｰﾊﾞが設定されていません", pcszMsgboxCaption, MB_OK);
			SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
			return TRUE;
		}
	}
	// POP3 Port
	if( IsWindowEnabled( GetDlgItem( hWnd, IDC_POP3PORT)))
	{
		UINT unPort = GetDlgItemInt( hWnd, IDC_POP3PORT, &blWork, FALSE);
		if( FALSE == blWork)
		{
			MessageBox( hWnd, "Pop3のﾎﾟｰﾄ番号が不正です", pcszMsgboxCaption, MB_OK);
			SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
			return TRUE;
		}
	}
	// Interval
	if( IsWindowEnabled( GetDlgItem( hWnd, IDC_POP3INTERVAL)))
	{
		int nInterval = GetDlgItemInt( hWnd, IDC_POP3INTERVAL, &blWork, TRUE);
		if( FALSE == blWork || ( -1 != nInterval && ( 0 >= nInterval || 120 < nInterval)))
		{
			MessageBox( hWnd, "ﾁｪｯｸ周期が不整です", pcszMsgboxCaption, MB_OK);
			SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
			return TRUE;
		}
	}

	return TRUE;
}

BOOL Pop3PageProc_OnNotify( HWND hDlg, LPARAM lParam)
{
	LPPSHNOTIFY	pstPsNotify;

	pstPsNotify = ( LPPSHNOTIFY)lParam;
	switch( pstPsNotify->hdr.code)
	{
	case PSN_KILLACTIVE:
		return Pop3PageProc_OnKillActive( hDlg, lParam);
	case PSN_APPLY:
		return Pop3PageProc_OnNotifyApply( hDlg, lParam);
	case PSN_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)pstPsNotify->lParam);
	}
	return TRUE;
}

BOOL Pop3PageProc_OnCommand( HWND hWnd, int nCode, int nID)
{
	switch ( nID)
	{
	case IDC_NOSAVEPW:
		if( BN_CLICKED == nCode)
		{
			EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINPW), ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_POP3)) && ( BST_CHECKED != IsDlgButtonChecked( hWnd, IDC_NOSAVEPW)));
		}
		return TRUE;
	case IDC_POP3:
		if( BN_CLICKED == nCode)
		{
			BOOL blEnable = ( BST_CHECKED == IsDlgButtonChecked( hWnd, IDC_POP3));
			EnableWindow( GetDlgItem( hWnd, IDC_POP3ADDR), blEnable);
			EnableWindow( GetDlgItem( hWnd, IDC_POP3PORT), blEnable);
			EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINID), blEnable);
			EnableWindow( GetDlgItem( hWnd, IDC_POP3LOGINPW), blEnable && ( BST_CHECKED != IsDlgButtonChecked( hWnd, IDC_NOSAVEPW)));
#if(_WIN32_WINNT >= 0x0400)
			EnableWindow( GetDlgItem( hWnd, IDC_APOP), blEnable);
#endif
			EnableWindow( GetDlgItem( hWnd, IDC_NOSAVEPW), blEnable);
			EnableWindow( GetDlgItem( hWnd, IDC_POP3INTERVAL), blEnable);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK Pop3PageProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_INITDIALOG:
		return Pop3PageProc_OnInitDialog( hDlg);

	case WM_NOTIFY:
		return Pop3PageProc_OnNotify( hDlg, lParam);

	case WM_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)lParam);

	case WM_COMMAND:
		return Pop3PageProc_OnCommand( hDlg, HIWORD( wParam), LOWORD(wParam));
	}
	return FALSE;
}

BOOL PrivatePageProc_OnInitDialog( HWND hWnd)
{
	char	szWork[ _MAX_WORK];
	char	szUser[ UNLEN + 1];
	char	szIniFile[ _MAX_PATH];
	GetIniUserName( szUser, UNLEN + 1);
	GetInitFileName( szIniFile, _MAX_PATH);

	GetPrivateProfileString( szUser, pcszMailAddress, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_MAILADDRESS, szWork);
	}

	GetPrivateProfileString( szUser, pcszRealName, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_REALNAME, szWork);
	}

	GetPrivateProfileString( szUser, pcszAliasFile, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_ADDRBOOK, szWork);
	}

	GetPrivateProfileString( szUser, pcszSignatureFile, "", szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_SIGNATUREFILE, szWork);
	}

	DWORD	dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	char	szDefMachine[ MAX_COMPUTERNAME_LENGTH + 1];
	if( !GetComputerName( szDefMachine, &dwSize))
	{
		lstrcpy( szDefMachine, "Default");
	}
	GetPrivateProfileString( szUser, pcszLocalHost, szDefMachine, szWork, _MAX_WORK, szIniFile);
	if( 0 < lstrlen( szWork))
	{
		SetDlgItemText( hWnd, IDC_LOCALHOST, szWork);
	}

	return TRUE;
}

BOOL PrivatePageProc_OnNotifyApply( HWND hWnd)
{
	char	szWork[ _MAX_WORK];
	char	szUser[ UNLEN + 1];
	char	szIniFile[ _MAX_PATH];
	GetIniUserName( szUser, UNLEN + 1);
	GetInitFileName( szIniFile, _MAX_PATH);

	// e-mail address
	GetDlgItemText( hWnd, IDC_MAILADDRESS, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszMailAddress, szWork, szIniFile);

	// real name
	GetDlgItemText( hWnd, IDC_REALNAME, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszRealName, szWork, szIniFile);

	// address book
	GetDlgItemText( hWnd, IDC_ADDRBOOK, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszAliasFile, szWork, szIniFile);

	// signature file
	GetDlgItemText( hWnd, IDC_SIGNATUREFILE, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszSignatureFile, szWork, szIniFile);

	// local host name
	GetDlgItemText( hWnd, IDC_LOCALHOST, szWork, _MAX_WORK);
	WritePrivateProfileString( szUser, pcszLocalHost, szWork, szIniFile);

	return TRUE;
}

BOOL PrivatePageProc_OnKillActive( HWND hWnd)
{
	char	szWork[ _MAX_WORK];
	// e-mail address
	GetDlgItemText( hWnd, IDC_MAILADDRESS, szWork, _MAX_WORK);
	if( 0 >= lstrlen( szWork))
	{
		MessageBox( hWnd, "emailアドレスが設定されていません", pcszMsgboxCaption, MB_OK);
		SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
		return TRUE;
	}

	// local host name
	GetDlgItemText( hWnd, IDC_LOCALHOST, szWork, _MAX_WORK);
	if( 0 >= lstrlen( szWork))
	{
		MessageBox( hWnd, "ローカルホスト名が設定されていません", pcszMsgboxCaption, MB_OK);
		SetWindowLong( hWnd, DWL_MSGRESULT, TRUE);
		return TRUE;
	}

	return TRUE;
}

BOOL PrivatePageProc_OnNotify( HWND hDlg, LPARAM lParam)
{
	LPPSHNOTIFY	pstPsNotify;

	pstPsNotify = ( LPPSHNOTIFY)lParam;
	switch( pstPsNotify->hdr.code)
	{
	case PSN_KILLACTIVE:
		return PrivatePageProc_OnKillActive( hDlg);
	case PSN_APPLY:
		return PrivatePageProc_OnNotifyApply( hDlg);
	case PSN_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)pstPsNotify->lParam);
	}
	return TRUE;
}

BOOL PrivatePageProc_OnAdrBkSelFile( HWND hwndDlg)
{
	OPENFILENAME ofn;
	char szFile[ _MAX_PATH];
	GetDlgItemText( hwndDlg, IDC_ADDRBOOK, szFile, _MAX_WORK);
	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME));
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hwndDlg;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFilter = "DEN8 Alias file\0*.ali\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Display the Open dialog box. 
	if( GetOpenFileName( &ofn))
	{
		SetDlgItemText( hwndDlg, IDC_ADDRBOOK, ofn.lpstrFile);
		return TRUE;
	}
	return FALSE;
}

BOOL PrivatePageProc_OnSinBkSelFile( HWND hwndDlg)
{
	OPENFILENAME ofn;
	char szFile[ _MAX_PATH];
	GetDlgItemText( hwndDlg, IDC_SIGNATUREFILE, szFile, _MAX_WORK);
	// Initialize OPENFILENAME
	ZeroMemory( &ofn, sizeof( OPENFILENAME));
	ofn.lStructSize = sizeof( OPENFILENAME);
	ofn.hwndOwner = hwndDlg;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFilter = "DEN8 Signature file\0*.sig\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	// Display the Open dialog box. 
	if( GetOpenFileName( &ofn))
	{
		SetDlgItemText( hwndDlg, IDC_SIGNATUREFILE, ofn.lpstrFile);
		return TRUE;
	}
	return FALSE;
}

BOOL CALLBACK PrivatePageProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message )
	{
	case WM_INITDIALOG:
		return PrivatePageProc_OnInitDialog( hDlg);

	case WM_NOTIFY:
		return PrivatePageProc_OnNotify( hDlg, lParam);

	case WM_HELP:
		return PropSheetProc_OnNotifyHelp( hDlg, ( LPHELPINFO)lParam);

	case WM_COMMAND:
		if( BN_CLICKED == HIWORD( wParam))
		{
			switch( LOWORD( wParam))
			{
			case IDC_ADRBOKSEL:
				return PrivatePageProc_OnAdrBkSelFile( hDlg);
			case IDC_SIGSEL:
				return PrivatePageProc_OnSinBkSelFile( hDlg);
			}
		}

	}
	return FALSE;
}

#ifdef	_POP3_TOO
#define	_PAGE_COUNT	3
#else
#define	_PAGE_COUNT	2
#endif


int eMailProperty( HWND hWnd)
{
	int nResult = IDCANCEL;

	PROPSHEETPAGE*	pastPP = NULL;
	PROPSHEETHEADER	stPS;

	ZeroMemory( &stPS, sizeof( PROPSHEETHEADER));
	stPS.dwSize = sizeof( PROPSHEETHEADER);
	stPS.dwFlags = PSH_PROPSHEETPAGE | PSH_HASHELP | PSH_NOAPPLYNOW | PSH_PROPTITLE;
	stPS.hwndParent = hWnd;
	stPS.hInstance = ( HINSTANCE)g_hModule;
	stPS.nPages = _PAGE_COUNT;
	stPS.pszCaption = pcszMsgboxCaption;
	pastPP = new PROPSHEETPAGE [ _PAGE_COUNT];
	stPS.ppsp = pastPP;
	if( pastPP)
	{
		ZeroMemory( pastPP, sizeof( PROPSHEETPAGE) * _PAGE_COUNT);

		pastPP[ 0].dwSize = sizeof( PROPSHEETPAGE);
		pastPP[ 0].dwFlags = PSP_HASHELP;
		pastPP[ 0].hInstance = ( HINSTANCE)g_hModule;
		pastPP[ 0].pszTemplate = MAKEINTRESOURCE( IDD_PRIVATEPAGE);
		pastPP[ 0].pfnDlgProc = PrivatePageProc;

		pastPP[ 1].dwSize = sizeof( PROPSHEETPAGE);
		pastPP[ 1].dwFlags = PSP_HASHELP;
		pastPP[ 1].hInstance = ( HINSTANCE)g_hModule;
		pastPP[ 1].pszTemplate = MAKEINTRESOURCE( IDD_SMTPPAGE);
		pastPP[ 1].pfnDlgProc = SmtpPageProc;
#ifdef	_POP3_TOO
		pastPP[ 2].dwSize = sizeof( PROPSHEETPAGE);
		pastPP[ 2].dwFlags = PSP_HASHELP;
		pastPP[ 2].hInstance = ( HINSTANCE)g_hModule;
		pastPP[ 2].pszTemplate = MAKEINTRESOURCE( IDD_POPPAGE);
		pastPP[ 2].pfnDlgProc = Pop3PageProc;
#endif
		nResult = PropertySheet( &stPS);
		delete [] pastPP;
	}

	return nResult;
}
