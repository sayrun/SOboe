#include "StdeMail.h"
#include "NetEx_IF.h"
#include "email.h"
#include "emailslv.h"
#include "property.h"
#include "Dialogs.h"
#include "resource.h"
#include "OrgSock.h"
#include "RStream.hpp"
#include "DInputDlg.h"
#include "MailS.h"
#ifdef	_POP3_TOO
#include "Pop3Part.h"
#endif

extern HANDLE			g_hModule;
extern EMAILORGDATA		g_stOrgData;
extern char*			g_pszAnony;

CRITICAL_SECTION		g_csDInput;

// protected
char*	pszProtcolName = "e-mail Protocol";

// 機能確認
BOOL NetExSpecification( NXLSPECIFICATION* pDxlSpecification)
{
	pDxlSpecification->unLowerStructVersion = _NXL_IF_DATA_VER;
	pDxlSpecification->unUpperStructVersion = _NXL_IF_DATA_VER;
	lstrcpy( pDxlSpecification->szProtcolString, pszProtcolName);
	pDxlSpecification->unNxlID = _EMAILPLUGIN_ID;
#ifdef	_POP3_TOO
	pDxlSpecification->unNxlStyle = _NXL_SENDRECEIVE | _NXL_ENTRYGROUP | _NXL_SUPPORTORIGINALICON | _NXL_SUPPORTBULKSEND;
#else
	pDxlSpecification->unNxlStyle = _NXL_SEND | _NXL_ENTRYGROUP | _NXL_SUPPORTORIGINALICON | _NXL_SUPPORTBULKSEND;
#endif

	return TRUE;
}

// 設定変更用
BOOL NetExConfigure( HWND hWnd)
{
	return eMailProperty( hWnd);
}

// Copyright表示用
BOOL NetExAbout( HWND hWnd)
{
	int nResult = DialogBox( ( HINSTANCE)g_hModule, MAKEINTRESOURCE( IDD_ABOUT), hWnd, ( DLGPROC)AboutDialogProc);

	return TRUE;
}

// 初期化
BOOL InitializeNetEx( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData)
{
#if(_WIN32_WINNT < 0x0400)
	Initialize_OrgWsa( hWnd);
#endif
	g_stOrgData.blInitialized = FALSE;
	g_stOrgData.hStopEvent = NULL;

	char	szIniFile[ _MAX_PATH];
	char	szUser[ UNLEN + 1];
	char	szDrive[ _MAX_DRIVE];
	char	szDir[ _MAX_DIR];
	DWORD	dwUnLen;
	dwUnLen = UNLEN + 1;
	if( 0 >= GetUserName( szUser, &dwUnLen))
	{
		lstrcpy( szUser, g_pszAnony);
	}
	GetModuleFileName( NULL, szIniFile, _MAX_PATH);
	_splitpath( szIniFile, szDrive, szDir, NULL, NULL);
	_makepath( szIniFile, szDrive, szDir, "email", "ini");

	char	szAddr[ 256];

	g_stOrgData.blLogging = GetPrivateProfileInt( szUser, "Logging", 0, szIniFile);

	GetPrivateProfileString( szUser, "SmtpServer", "", szAddr, 256, szIniFile);
	if( 0 >= lstrlen( szAddr))return FALSE;
	lstrcpy( g_stOrgData.szSmtpServerName, szAddr);

	g_stOrgData.unSmtpPort = htons( GetPrivateProfileInt( szUser, "SmtpPort", 25, szIniFile));
	if( 0 >= g_stOrgData.unSmtpPort)return FALSE;

#ifdef	_POP3_TOO
	BOOL	blEnablePop3;
	blEnablePop3 = GetPrivateProfileInt( szUser, "EnablePop3", 0, szIniFile);
	if( blEnablePop3)
	{
		g_stOrgData.ulAddress = INADDR_NONE;
		GetPrivateProfileString( szUser, "Pop3Server", "", szAddr, 256, szIniFile);
		if( 0 >= lstrlen( szAddr))
		{
			blEnablePop3 = FALSE;
		}
		else
		{
			lstrcpy( g_stOrgData.szPop3ServerName, szAddr);
		}

		g_stOrgData.unPop3Port = GetPrivateProfileInt( szUser, "Pop3Port", 0, szIniFile);
		if( 0 >= g_stOrgData.unPop3Port)blEnablePop3 = FALSE;

		GetPrivateProfileString( szUser, "Pop3User", "", szAddr, 256, szIniFile);
		if( 0 >= lstrlen( szAddr))
		{
			blEnablePop3 = FALSE;
		}
		else
		{
			lstrcpy( g_stOrgData.szPop3ID, szAddr);
		}

		// APOP
		g_stOrgData.blPop3APOP = ( 0 == GetPrivateProfileInt( szUser, "Pop3APop", 0, szIniFile)) ? FALSE : TRUE;

		// No save Password
		g_stOrgData.blInvalidatePW = GetPrivateProfileInt( szUser, "Pop3NoSave", 0, szIniFile);

		if( !g_stOrgData.blInvalidatePW)
		{
			char	szPassWord[ 128];
			ZeroMemory( szPassWord, 128);
			if( GetPrivateProfileStruct( szUser, "Pop3Pass", szPassWord, 128, szIniFile))
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
			}
			if( 0 < lstrlen( szPassWord))
			{
				lstrcpy( g_stOrgData.szPop3PW, szPassWord);
			}
			else
			{
				g_stOrgData.blInvalidatePW = TRUE;
			}
		}

		g_stOrgData.ulPop3Interval = GetPrivateProfileInt( szUser, "Pop3Interval", 30, szIniFile);
		if( 0 <= g_stOrgData.ulPop3Interval)
		{
			g_stOrgData.ulPop3Interval = max( 1, min( 120, g_stOrgData.ulPop3Interval));
		}
		else
		{
			g_stOrgData.ulPop3Interval = -1;
		}
		g_stOrgData.ulPop3Interval *= ( 1000 * 60);
		if( 0 >= g_stOrgData.ulPop3Interval)blEnablePop3 = FALSE;
	}
#endif

	g_stOrgData.blWithMyAddress = GetPrivateProfileInt( szUser, "WithMyAddressInBCC", 0, szIniFile);
	g_stOrgData.blMbbtombc = GetPrivateProfileInt( szUser, "EnableHan2Zen", 0, szIniFile);
	g_stOrgData.blFormatting = GetPrivateProfileInt( szUser, "EnableFormatting", 0, szIniFile);
	g_stOrgData.nCharCount = GetPrivateProfileInt( szUser, "CharCount", 70, szIniFile);
	if( 10 > g_stOrgData.nCharCount)g_stOrgData.nCharCount = 10;

	GetPrivateProfileString( szUser, "MailAddress", "", szAddr, 256, szIniFile);
	lstrcpy( g_stOrgData.szMailAddres, szAddr);

	GetPrivateProfileString( szUser, "RealName", "", szAddr, 256, szIniFile);
	lstrcpy( g_stOrgData.szRealName, szAddr);

	GetPrivateProfileString( szUser, "AliasFile", "", szAddr, 256, szIniFile);
	lstrcpy( g_stOrgData.szAliasFileName, szAddr);

	GetPrivateProfileString( szUser, "SignatureFile", "", szAddr, 256, szIniFile);
	lstrcpy( g_stOrgData.szSignaturePath, szAddr);

	GetPrivateProfileString( szUser, "HostName", "", g_stOrgData.szMachineName, MAX_COMPUTERNAME_LENGTH + 1, szIniFile);
	if( 0 >= lstrlen( g_stOrgData.szMachineName))
	{
		DWORD	dwSize = MAX_COMPUTERNAME_LENGTH + 1;
		if( !GetComputerName( g_stOrgData.szMachineName, &dwSize))
		{
			lstrcpy( g_stOrgData.szMachineName, "Default");
		}
		if( 0 >= lstrlen( g_stOrgData.szMachineName))return FALSE;
	}

	g_stOrgData.hStopEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	if( NULL == g_stOrgData.hStopEvent)return FALSE;

	g_stOrgData.pcGroupList = new CGroupList;
	if( NULL == g_stOrgData.pcGroupList)return FALSE;
	g_stOrgData.pcEntryList = new CEntryList;
	if( NULL == g_stOrgData.pcEntryList)return FALSE;

	g_stOrgData.pcIDMapSubject = new CIDMapSubject;
	if( NULL == g_stOrgData.pcIDMapSubject)return FALSE;

#ifdef	_POP3_TOO
	if( blEnablePop3)
	{
		g_stOrgData.lpfnReceiveData = lpfnRcvData;
		if( -1 != _beginthread( CycleCheckThread, 0, NULL))
		{
			/*
			g_stOrgData.blActive = TRUE;
			g_stOrgData.blReadySend = TRUE;
			*/
		}
	}
#endif

	g_stOrgData.blInitialized = TRUE;
	return TRUE;
}

// 終了
BOOL UnInitializeNetEx( HWND hWnd)
{
#if(_WIN32_WINNT < 0x0400)
	Uninitialize_OrgWsa();
#endif

#ifdef	_DEBUG
	OutputDebugString( "QueryExit-AllThread\n");
#endif
	SetEvent( g_stOrgData.hStopEvent);
	Sleep( 100);
#ifdef	_DEBUG
	OutputDebugString( "Exit-AllThread?\n");
#endif

	if( g_stOrgData.hStopEvent)
	{
		CloseHandle( g_stOrgData.hStopEvent);
		g_stOrgData.hStopEvent = NULL;
	}
	if( g_stOrgData.pcGroupList)
	{
		delete g_stOrgData.pcGroupList;
		g_stOrgData.pcGroupList = NULL;
	}
	if( g_stOrgData.pcEntryList)
	{
		delete g_stOrgData.pcEntryList;
		g_stOrgData.pcEntryList = NULL;
	}
	if( g_stOrgData.pcIDMapSubject)
	{
		delete g_stOrgData.pcIDMapSubject;
		g_stOrgData.pcIDMapSubject = NULL;
	}

	return TRUE;
}

// 状態の確認
DWORD GetNxlStatus( void)
{
	DWORD	dwResult = 0;
	if( g_stOrgData.blInitialized)
	{
		dwResult |= _NXLSTATUS_ACTIVE;
		if( 0 < lstrlen( g_stOrgData.szMailAddres))
		{
			dwResult |= _NXLSTATUS_READY_SEND;
		}
	}
	return dwResult;
}

// エントリ開始
BOOL StartEntry( void)
{
	if( g_stOrgData.blInitialized)
	{
		return TRUE;
	}
	return FALSE;
}

// 留守
BOOL AbsenceEntry( BOOL blAbsence)
{
	if( g_stOrgData.blInitialized)
	{
		return TRUE;
	}
	return FALSE;
}

// エントリ終了
BOOL ExitEntry( void)
{
	if( g_stOrgData.blInitialized)
	{
		return TRUE;
	}
	return FALSE;
}

// エントリリスト更新の中断、取得の開始のため
BOOL LockEntryList( void)
{
	if( g_stOrgData.blInitialized)
	{
		ReadAddress( g_stOrgData.szAliasFileName, g_stOrgData.pcGroupList, g_stOrgData.pcEntryList);
		return TRUE;
	}
	return FALSE;
}

// エントリリスト更新の再開
BOOL UnlockEntryList( void)
{
	if( g_stOrgData.blInitialized)
	{
		g_stOrgData.pcEntryList->RemoveAll();
		g_stOrgData.pcGroupList->RemoveAll();
		return TRUE;
	}
	return FALSE;
}

// エントリーイメージの取得
BOOL GetEntryImage( IMAGEDATA* lpImageData)
{
	HBITMAP	hBitmap = LoadBitmap( ( HINSTANCE)g_hModule, MAKEINTRESOURCE( IDB_STATUS));
	if( NULL != hBitmap)
	{
		lpImageData->stSize.cx = 12;
		lpImageData->stSize.cy = 12;
		lpImageData->nCount = 3;
		lpImageData->hImage = hBitmap;
		lpImageData->clrMask = RGB( 0xFF, 0xFF, 0xFF);
		return TRUE;
	}
	return FALSE;
}

// グループ数を取得
int GetGroupCount( void)
{
	int nResult = 0;

	if( g_stOrgData.blInitialized)
	{
		nResult = g_stOrgData.pcGroupList->GetCount();
	}
	return nResult;
}

// グループ情報を取得
int GetGroupData( int nIndex, GROUPDATA* pstGroupData, int nSize)
{
	if( 0 > nIndex)return -1;

	int	nResult = -1;

	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcGroupList->GetCount() >= nIndex)
		{
			int	nNameSize = lstrlen( g_stOrgData.pcGroupList->GetGroupName( nIndex)) + 1;
			int nDataSize = sizeof( GROUPDATA) + nNameSize;
			nResult = nDataSize;
			if( NULL != pstGroupData && nDataSize <= nSize)
			{
				pstGroupData->nSize = nDataSize;
				pstGroupData->unStructVersion = _GROUPSTRUCT_VER100;
				pstGroupData->nParentGroup = g_stOrgData.pcGroupList->GetParentGroupID( nIndex);
				pstGroupData->nGroupStatus = 1;
				pstGroupData->nOffsetGroupName = sizeof( GROUPDATA);
				lstrcpy( &( ( LPSTR)pstGroupData)[pstGroupData->nOffsetGroupName], g_stOrgData.pcGroupList->GetGroupName( nIndex));
			}
		}
	}
	return nResult;
}

// リストに存在するエントリー個数取得
int GetEntryCount( void)
{
	int nResult = 0;

	if( g_stOrgData.blInitialized)
	{
		nResult = g_stOrgData.pcEntryList->GetCount();
	}
	return nResult;
}

// エントリー情報の取得
int GetEntryData( int nIndex, ENTRYDATA* pstEntry, int nSize)
{
	if( 0 > nIndex)return -1;
/*
#ifdef	_DEBUG
	char	szData[ 128];
	wsprintf( szData, "%d-%d\n", nIndex, nSize);
	OutputDebugString( szData);
#endif
*/

	int	nResult = -1;

	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcEntryList->GetCount() >= nIndex)
		{
			int nGroupID = g_stOrgData.pcEntryList->GetParentGroup( nIndex);
			int	nNameSize = g_stOrgData.pcEntryList->GetEntryName( nIndex, NULL, 0) + 1;
			int nAddrSize = g_stOrgData.pcEntryList->GetEntryAdd( nIndex, NULL, 0) + 1;
			int nDataSize = sizeof( ENTRYDATA) + nNameSize + nAddrSize;

			if( 0 <= nGroupID)nDataSize += sizeof( int);
			nResult = nDataSize;

			if( NULL != pstEntry && nDataSize <= nSize)
			{
				pstEntry->nSize = nDataSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _EMAILPLUGIN_ID;
				pstEntry->nEntryStat = 2;
				g_stOrgData.pcEntryList->GetEntryAdd( nIndex, ( LPSTR)&( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)], nAddrSize);
				pstEntry->nOffsetEntryName = sizeof( ENTRYDATA) + nAddrSize;
				g_stOrgData.pcEntryList->GetEntryName( nIndex, ( LPSTR)&( ( LPBYTE)pstEntry)[ pstEntry->nOffsetEntryName], nNameSize);
				if( 0 <= nGroupID)
				{
					pstEntry->nParentGroupCount = 1;
					pstEntry->nOffsetParentGroup = sizeof( ENTRYDATA) + nAddrSize + nNameSize;
					*( ( LPINT)&( ( LPBYTE)pstEntry)[ pstEntry->nOffsetParentGroup]) = nGroupID;
				}
				else
				{
					pstEntry->nParentGroupCount = 0;
					pstEntry->nOffsetParentGroup = -1;
				}
			}
		}
	}
	return nResult;
}

// エントリ情報の比較
int CompareEntryData( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize)
{
	if( _EMAILPLUGIN_ID == pstEntryDst->unNxlID && _EMAILPLUGIN_ID == pstEntrySrc->unNxlID)
	{
		int nMinSize = sizeof( ENTRYDATA);
		if( nMinSize <= nDstSize && nMinSize <= nSrcSize)
		{
			LPCSTR lpcsz1 = ( LPSTR)&( ( LPBYTE)pstEntryDst)[ sizeof( ENTRYDATA)];
			LPCSTR lpcsz2 = ( LPSTR)&( ( LPBYTE)pstEntrySrc)[ sizeof( ENTRYDATA)];

			return !lstrcmp( lpcsz1, lpcsz2);
		}
	}
	return FALSE;
}

// エントリ情報の表示
int GetEntryInfo( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize)
{
	return 0;
}

// エントリーの直接取得
int InputEntry( HWND hWnd, HGLOBAL* phEntry)
{
	int nResult = IDCANCEL;

	EnterCriticalSection( &g_csDInput);

	if( IDOK == DialogBox( ( HINSTANCE)g_hModule, MAKEINTRESOURCE( IDD_DINPUTDLG), hWnd, ( DLGPROC)DInputDlg))
	{
		int nLength = lstrlen( DInputDlg_GetAddress());
		if( 0 < nLength)
		{
			int nDataSize = sizeof( ENTRYDATA) + ( ( nLength + 1) * 2);

			*phEntry = GlobalAlloc( GHND, nDataSize);
			ENTRYDATA*	pstEntry = ( ENTRYDATA*)GlobalLock( *phEntry);

			if( NULL != pstEntry)
			{
				pstEntry->nSize = nDataSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _EMAILPLUGIN_ID;
				pstEntry->nEntryStat = 2;
				lstrcpyn( ( LPSTR)&( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)], DInputDlg_GetAddress(), nLength + 1);
				pstEntry->nOffsetEntryName = sizeof( ENTRYDATA) + nLength + 1;
				lstrcpyn( ( LPSTR)&( ( LPBYTE)pstEntry)[ pstEntry->nOffsetEntryName], DInputDlg_GetAddress(), nLength + 1);
				pstEntry->nParentGroupCount = 0;
				pstEntry->nOffsetParentGroup = -1;

				nResult = IDOK;

				GlobalUnlock( *phEntry);
			}
		}
	}

	LeaveCriticalSection( &g_csDInput);

	return nResult;
}

static LPCSTR pcszSendMemoData = "SendMemoData";

// データ送信
int SendMemoData( const ENTRYDATA* pstEntry, const SENDDATA* pstSendData, int nSendDataSize, HANDLE hCancelEvent, HGLOBAL hExt)
{
	int nResult = 0;

	SendMemoDataInBluk( &pstEntry, &nResult, 1, pstSendData, nSendDataSize, hCancelEvent);

	return nResult;
}


static LPCSTR pcszSendMemoDataInBluk = "SendMemoDataInBluk";

// データ送信（一括
int SendMemoDataInBluk( const ENTRYDATA** pstEntry, int* panResult, int nCount, const SENDDATA* pstSendData, int nSendDataSize, HANDLE hCancelEvent)
{
	if( g_stOrgData.blLogging)OutputErrorString( "ENTER", pcszSendMemoDataInBluk);

	int nResult = _SEND_ERROR;
	if( g_stOrgData.blInitialized)
	{
		CMailS				cMail;

		if( g_stOrgData.blLogging)OutputErrorString( "START", pcszSendMemoDataInBluk);
		////////////////////////////////////////////////////////
		//
		// メイルフォーマット作成開始
		//
		if( !cMail.SetToAddress( pstEntry, nCount))goto cleanup;
		if( !cMail.SetFromAddress( g_stOrgData.szMailAddres, g_stOrgData.szRealName))goto cleanup;
		if( !cMail.SetMailBody( pstSendData, nSendDataSize, g_stOrgData.szSignaturePath))goto cleanup;
		//
		// メイルフォーマット作成終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		// 送信の実施
		nResult = SendMail( cMail, hCancelEvent);

		if( g_stOrgData.blLogging)OutputErrorString( "END", pcszSendMemoDataInBluk);
	}


	if( g_stOrgData.blLogging)OutputErrorString( "RETURN", pcszSendMemoDataInBluk);

cleanup:
	////////////////////////////////////////////////////////
	// 結果の格納
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		panResult[ nIndex] = nResult;
	}

	return nResult;
}

static LPCSTR pcszSendResultData = "SendResultData";

// 応答送信
int SendResultData( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize)
{
	if( g_stOrgData.blLogging)OutputErrorString( "ENTER", pcszSendResultData);

	int nResult = _SEND_ERROR;
	if( g_stOrgData.blInitialized)
	{
		CMailS				cMail;

		if( g_stOrgData.blLogging)OutputErrorString( "START", pcszSendResultData);
		////////////////////////////////////////////////////////
		//
		// メイルフォーマット作成開始
		//
		if( !cMail.SetToAddress( &pstEntry, 1))goto cleanup;
		if( !cMail.SetFromAddress( g_stOrgData.szMailAddres, g_stOrgData.szRealName))goto cleanup;
		if( !cMail.SetMailBody( pResultData, nResultDataSize, g_stOrgData.szSignaturePath))goto cleanup;
		//
		// メイルフォーマット作成終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 過去の受信データでＩＤに相当するSubject検索
		//
		if( g_stOrgData.pcIDMapSubject)
		{
			for( int nIndex = 0; nIndex < g_stOrgData.pcIDMapSubject->GetCount(); nIndex++)
			{
				if( !lstrcmp( g_stOrgData.pcIDMapSubject->GetID( nIndex), cMail.GetSubject()))
				{
					char	szWork[ 128];
					wsprintf( szWork, "Re: %s", g_stOrgData.pcIDMapSubject->GetSubject( nIndex));
					cMail.SetSubject( szWork);
					break;
				}
			}
		}
		//
		// 過去の受信データでＩＤに相当するSubject検索
		//
		////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////
		// 送信の実施
		nResult = SendMail( cMail, NULL);

		if( g_stOrgData.blLogging)OutputErrorString( "END", pcszSendResultData);
	}


	if( g_stOrgData.blLogging)OutputErrorString( "RETURN", pcszSendResultData);

cleanup:
	return nResult;
}

char* pszNotFound = "メイル・サーバに接続できませんでした";

// 固有エラーメッセージ
int GetErrorMsg( int nEcode, char* pszErrorMsg, int nSize)
{
	if( nEcode != _NOTFOUND_SERVER)return -1;
	int nLen = lstrlen( pszNotFound);
	if( NULL == pszErrorMsg || nLen >= nSize)return nLen;

	lstrcpy( pszErrorMsg, pszNotFound);

	return nLen;
}
