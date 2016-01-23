#include <windows.h>
#include <lmcons.h>
#include <process.h>
#include "NetEx_IF.h"
#include "EntryList.h"
#include "StdNet.h"
#include "StdNetSlv.h"
#include "SettingDlg.h"
#include "AboutDlg.h"
#include "DInputDlg.h"
#if(_WIN32_WINNT < 0x0400)
#include "OrgSock.h"
#endif

/*
#define	_STEPDEBUG
*/

extern HANDLE			g_hModule;
extern CRITICAL_SECTION	g_csDInput;

STDNETORGDATA	g_stOrgData;

// protected
char*	pszProtcolName = "Standard Protocol";

// 機能確認
BOOL NetExSpecification( NXLSPECIFICATION* pDxlSpecification)
{
	pDxlSpecification->unLowerStructVersion = _NXL_IF_DATA_VER;
	pDxlSpecification->unUpperStructVersion = _NXL_IF_DATA_VER;
	lstrcpy( pDxlSpecification->szProtcolString, pszProtcolName);
	pDxlSpecification->unNxlID = _STDPROTCOL_ID;
	pDxlSpecification->unNxlStyle = _NXL_SENDRECEIVE | _NXL_ENTRYGROUP | _NXL_SUPPORTRESOLUTION | _NXL_SUPPORTINPUTENTRY;

	return TRUE;
}

// 設定変更用
BOOL NetExConfigure( HWND hWnd)
{
	DialogBox( ( HINSTANCE)g_hModule, "SETTINGDLG", hWnd, ( DLGPROC)SettingDlg);

	return TRUE;
}

// Copyright表示用
BOOL NetExAbout( HWND hWnd)
{
	DialogBox( ( HINSTANCE)g_hModule, "ABOUT", hWnd, ( DLGPROC)AboutDlg);

	return TRUE;
}

// 初期化
BOOL InitializeNetEx( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData)
{
	BOOL	blResult = FALSE;

#ifdef	_STEPDEBUG
	OutputErrorString( "Enter", "InitializeNetEx");
#endif

#if(_WIN32_WINNT < 0x0400)
	Initialize_OrgWsa( hWnd);
#endif

	if( FALSE == g_stOrgData.blInitialized)
	{
		g_stOrgData.blAbsence = FALSE;

		g_stOrgData.hStopEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
		if( NULL == g_stOrgData.hStopEvent)goto errordetection;

		g_stOrgData.lpfnRcvData = lpfnRcvData;

		g_stOrgData.pcEntryList = new CEntryList;
		if( NULL == g_stOrgData.pcEntryList)goto errordetection;

		g_stOrgData.pcGroupList = new CGroupList;
		if( NULL == g_stOrgData.pcGroupList)goto errordetection;
		g_stOrgData.pcLocalGroupList = NULL;

		g_stOrgData.pszEntryName = new char [ lstrlen( lpcszEntryName) + 1];
		if( NULL == g_stOrgData.pszEntryName)goto errordetection;
		lstrcpy( g_stOrgData.pszEntryName, lpcszEntryName);

		char*	pszAnony = "Anonymous";
		char	szUser[ UNLEN + 1];
		DWORD	dwSize;
		dwSize = UNLEN + 1;
		if( !GetUserName( szUser, &dwSize))
		{
			lstrcpy( szUser, pszAnony);
		}
		g_stOrgData.pszLogonID = new char [ lstrlen( szUser) + 1];
		if( NULL == g_stOrgData.pszLogonID)goto errordetection;
		lstrcpy( g_stOrgData.pszLogonID, szUser);

		dwSize = MAX_COMPUTERNAME_LENGTH + 1;
		char	szDefMachine[ MAX_COMPUTERNAME_LENGTH + 1];
		if( !GetComputerName( szDefMachine, &dwSize))
		{
			lstrcpy( szDefMachine, pszAnony);
		}
		g_stOrgData.pszMachineName = new char [ lstrlen( szDefMachine) + 1];
		if( NULL == g_stOrgData.pszMachineName)goto errordetection;
		lstrcpy( g_stOrgData.pszMachineName, szDefMachine);

		{
			char*	pszAnony = "Anonymous";
			char	szUser[ UNLEN + 1];
			DWORD	dwSize;
			dwSize = UNLEN + 1;
			if( 0 >= GetUserName( szUser, &dwSize))
			{
				lstrcpy( szUser, pszAnony);
			}

			char	szProfilePath[ MAX_PATH];
			char	szDrive[ _MAX_DRIVE];
			char	szDir[ _MAX_DIR];
			GetModuleFileName( NULL, szProfilePath, _MAX_PATH);
			_splitpath( szProfilePath, szDrive, szDir, NULL, NULL);
			_makepath( szProfilePath, szDrive, szDir, "StdNet", "ini");

			g_stOrgData.unPort = GetPrivateProfileInt( szUser, "Port", 7127, szProfilePath);

			char	szWork[ 2048];
			GetPrivateProfileString( szUser, "Nickname", szUser, szWork, 2048, szProfilePath);
			g_stOrgData.pszNickname = new char [ lstrlen( szWork) + 1];
			if( NULL == g_stOrgData.pszNickname)goto errordetection;
			lstrcpy( g_stOrgData.pszNickname, szWork);

			GetPrivateProfileString( szUser, "Group", pszAnony, szWork, 2048, szProfilePath);
			g_stOrgData.pszGroup = new char [ lstrlen( szWork) + 1];
			if( NULL == g_stOrgData.pszGroup)goto errordetection;
			lstrcpy( g_stOrgData.pszGroup, szWork);

			g_stOrgData.nSeedType = GetPrivateProfileInt( szUser, "SeedType", 0, szProfilePath);
		}
		// グローバルデータ初期化正常完了
		g_stOrgData.blInitialized = TRUE;

		if( NULL != g_stOrgData.hStopEvent)
		{
			HANDLE hEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
			ResetEvent( hEvent);
			if( -1 != _beginthread( EntryControlThread, 0, &hEvent))
			{
				WaitForSingleObject( hEvent, 1000);
				ResetEvent( hEvent);
				if( -1 != _beginthread( ListenThread, 0, &hEvent))
				{
					WaitForSingleObject( hEvent, 1000);
					CloseHandle( hEvent);
					g_stOrgData.blActive = TRUE;
					g_stOrgData.blReadySend = TRUE;
					blResult = TRUE;
				}
			}
		}
	}
#ifdef	_STEPDEBUG
	OutputErrorString( "Return", "InitializeNetEx");
#endif
	return blResult;

errordetection:
#ifdef	_STEPDEBUG
	OutputErrorString( "errordetection", "InitializeNetEx");
#endif
	CleanupOrgData();
	return blResult;
}

// 終了
BOOL UnInitializeNetEx( HWND hWnd)
{
#ifdef	_STEPDEBUG
	OutputErrorString( "Enter", "UnInitializeNetEx");
#endif

#if(_WIN32_WINNT < 0x0400)
	Uninitialize_OrgWsa();
#endif

	if( g_stOrgData.blInitialized)
	{
#ifdef	_DEBUG
		OutputDebugString( "QueryExit-AllThread\n");
#endif
		SetEvent( g_stOrgData.hStopEvent);
		Sleep( 100);
#ifdef	_DEBUG
		OutputDebugString( "Exit-AllThread?\n");
#endif

		CleanupOrgData();

#ifdef	_STEPDEBUG
		OutputErrorString( "TRUE RETURN", "UnInitializeNetEx");
#endif

		return TRUE;
	}
#ifdef	_STEPDEBUG
		OutputErrorString( "TRUE FALSE", "UnInitializeNetEx");
#endif
	return FALSE;
}

// 状態の確認
DWORD GetNxlStatus( void)
{
	DWORD dwResult = 0;

	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.blActive)
		{
			dwResult |= _NXLSTATUS_ACTIVE;
			if( g_stOrgData.blReadyReceive)dwResult |= _NXLSTATUS_READY_RECEIVE;
			if( g_stOrgData.blReadySend)dwResult |= _NXLSTATUS_READY_SEND;
		}
	}

	return dwResult;
}

// エントリ開始
BOOL StartEntry( void)
{
#ifdef	_STEPDEBUG
	OutputErrorString( "Enter", "StartEntry");
#endif
#ifdef	_DEBUG
	OutputDebugString( "StartEntry\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		BroadcastEntry( _ENTRY_START, FALSE);
#ifdef	_STEPDEBUG
		OutputErrorString( "TRUE return", "StartEntry");
#endif
		return TRUE;
	}
#ifdef	_STEPDEBUG
	OutputErrorString( "FALSE return", "StartEntry");
#endif
	return FALSE;
}

// エントリ開始：留守通知付き
BOOL StartEntryEx( BOOL blAbsence)
{
#ifdef	_STEPDEBUG
	OutputErrorString( "Enter", "StartEntryEx");
#endif
#ifdef	_DEBUG
	OutputDebugString( "StartEntryEx\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		BroadcastEntry( _ENTRY_START, blAbsence);
#ifdef	_STEPDEBUG
		OutputErrorString( "TRUE return", "StartEntryEx");
#endif
		return TRUE;
	}
#ifdef	_STEPDEBUG
	OutputErrorString( "FALSE return", "StartEntryEx");
#endif
	return FALSE;
}

// 留守
BOOL AbsenceEntry( BOOL blAbsence)
{
	if( g_stOrgData.blInitialized)
	{
		g_stOrgData.blAbsence = blAbsence;
		BroadcastEntry( _ENTRY_ANSER, blAbsence);
		return TRUE;
	}
	return FALSE;
}

// エントリ終了
BOOL ExitEntry( void)
{
#ifdef	_STEPDEBUG
	OutputErrorString( "Enter", "ExitEntry");
#endif
	if( g_stOrgData.blInitialized)
	{
		int nCount = 0;
		struct sockaddr* pastSockAdd = NULL;;
		if( g_stOrgData.pcEntryList->LockList())
		{
			nCount = g_stOrgData.pcEntryList->GetCount();
			pastSockAdd = new struct sockaddr [ nCount];
			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				g_stOrgData.pcEntryList->GetEntryAdd( nIndex, &pastSockAdd[ nIndex], sizeof( struct sockaddr));
			}
			g_stOrgData.pcEntryList->RemoveAll();
			g_stOrgData.pcGroupList->RemoveAll();
			g_stOrgData.pcEntryList->UnlockList();
		}
		if( pastSockAdd)
		{
			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				SendEntryCommand( _ENTRY_END, 0, &pastSockAdd[ nIndex]);
			}
			delete [] pastSockAdd;
			pastSockAdd = NULL;
		}

#ifdef	_STEPDEBUG
		OutputErrorString( "TRUE return", "ExitEntry");
#endif
		return TRUE;
	}

#ifdef	_STEPDEBUG
	OutputErrorString( "FALSE return", "ExitEntry");
#endif
	return FALSE;
}

// エントリリスト更新の中断、取得の開始のため
BOOL LockEntryList( void)
{
	BOOL blResult = FALSE;
	if( g_stOrgData.blInitialized)
	{
		blResult = g_stOrgData.pcEntryList->LockList();
		if( g_stOrgData.pcLocalGroupList)
		{
			delete g_stOrgData.pcLocalGroupList;
			g_stOrgData.pcLocalGroupList = NULL;
		}
		if( 0 <= g_stOrgData.nSeedType && 3 >= g_stOrgData.nSeedType)
		{
			g_stOrgData.pcLocalGroupList = new CGroupList;
			GetLocalGroup( g_stOrgData.pcLocalGroupList);
		}
	}
	return blResult;
}

// エントリリスト更新の再開
BOOL UnlockEntryList( void)
{
	BOOL blResult = FALSE;
	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcLocalGroupList)
		{
			delete g_stOrgData.pcLocalGroupList;
			g_stOrgData.pcLocalGroupList = NULL;
		}
		blResult = g_stOrgData.pcEntryList->UnlockList();
	}
	return blResult;
}

// グループ数を取得
int GetGroupCount( void)
{
	int	nResult = 0;
	if( g_stOrgData.blInitialized)
	{
		nResult += g_stOrgData.pcGroupList->GetCount();
	}
	if( g_stOrgData.pcLocalGroupList)
	{
		nResult += g_stOrgData.pcLocalGroupList->GetCount();
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
		if( g_stOrgData.pcGroupList->GetCount() > nIndex)
		{
			int	nNameSize = lstrlen( g_stOrgData.pcGroupList->GetGroupName( nIndex)) + 1;
			int nDataSize = sizeof( GROUPDATA) + nNameSize;
			nResult = nDataSize;
			if( NULL != pstGroupData && nDataSize <= nSize)
			{
				pstGroupData->nSize = nDataSize;
				pstGroupData->unStructVersion = _GROUPSTRUCT_VER100;
				pstGroupData->nParentGroup = g_stOrgData.pcGroupList->GetParentGroup( nIndex);
				pstGroupData->nGroupStatus = 0;
				pstGroupData->nOffsetGroupName = sizeof( GROUPDATA);
				lstrcpy( &( ( LPSTR)pstGroupData)[pstGroupData->nOffsetGroupName], g_stOrgData.pcGroupList->GetGroupName( nIndex));
			}
		}
		else
		{
			int nLocalIndex = nIndex - g_stOrgData.pcGroupList->GetCount();
			if( g_stOrgData.pcLocalGroupList->GetCount() > nLocalIndex)
			{
				int	nNameSize = lstrlen( g_stOrgData.pcLocalGroupList->GetGroupName( nLocalIndex)) + 1;
				int nDataSize = sizeof( GROUPDATA) + nNameSize;
				nResult = nDataSize;
				if( NULL != pstGroupData && nDataSize <= nSize)
				{
					pstGroupData->nSize = nDataSize;
					pstGroupData->unStructVersion = _GROUPSTRUCT_VER100;
					pstGroupData->nParentGroup = g_stOrgData.pcLocalGroupList->GetParentGroup( nLocalIndex);
					if( 0 <= pstGroupData->nParentGroup)pstGroupData->nParentGroup += g_stOrgData.pcGroupList->GetCount();
					pstGroupData->nGroupStatus = 0;
					pstGroupData->nOffsetGroupName = sizeof( GROUPDATA);
					lstrcpy( &( ( LPSTR)pstGroupData)[pstGroupData->nOffsetGroupName], g_stOrgData.pcLocalGroupList->GetGroupName( nLocalIndex));
				}
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

	int	nResult = -1;

	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcEntryList->GetCount() >= nIndex)
		{
			char*	pszLocalname = NULL;
			char*	pszSeed = NULL;
			int nLocalParentCount = 0;
			int	nLocalNameLen = 0;
			if( g_stOrgData.pcLocalGroupList)
			{
				int nLen = GetSeed( nIndex, NULL, 0);
				if( 0 < nLen)
				{
					pszSeed = new char [ nLen + 1];
					if( pszSeed)GetSeed( nIndex, pszSeed, nLen + 1);
				}
				nLocalParentCount += GetLocalParentID( pszSeed, NULL, 0, 0, g_stOrgData.pcLocalGroupList);
				nLocalNameLen = GetLocalName( pszSeed, NULL, 0);
				if( 0 < nLocalNameLen)
				{
					pszLocalname = new char [ nLocalNameLen + 1];
					GetLocalName( pszSeed, pszLocalname, nLocalNameLen + 1);
				}
			}

			int	nNameSize = g_stOrgData.pcEntryList->GetEntryName( nIndex, NULL, 0, pszLocalname) + 1;
			int nDataSize = sizeof( ENTRYDATA) + nNameSize + sizeof( struct sockaddr);
			int nParentCount = g_stOrgData.pcEntryList->GetParentIDCount( nIndex);

			if( 0 < nParentCount || 0 < nLocalParentCount)
			{
				nDataSize += sizeof( int) * ( nParentCount + nLocalParentCount);
			}
			nResult = nDataSize;

			if( NULL != pstEntry && nDataSize <= nSize)
			{
				pstEntry->nSize = nDataSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _STDPROTCOL_ID;
				pstEntry->nEntryStat = g_stOrgData.pcEntryList->GetEntryStatus( nIndex);
				g_stOrgData.pcEntryList->GetEntryAdd( nIndex, ( struct sockaddr*)&( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)], sizeof( struct sockaddr));
				pstEntry->nOffsetEntryName = sizeof( ENTRYDATA) + sizeof( struct sockaddr);
				g_stOrgData.pcEntryList->GetEntryName( nIndex, ( LPSTR)&( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA) + sizeof( struct sockaddr)], nNameSize, pszLocalname);
				if( 0 < nParentCount)
				{
					pstEntry->nParentGroupCount = nParentCount;
					pstEntry->nOffsetParentGroup = pstEntry->nOffsetEntryName + nNameSize;
					g_stOrgData.pcEntryList->GetParentID( nIndex, ( int*)&( ( LPBYTE)pstEntry)[ pstEntry->nOffsetParentGroup], nParentCount);
				}
				else
				{
					pstEntry->nParentGroupCount = 0;
					pstEntry->nOffsetParentGroup = -1;
				}
				if( 0 < nLocalParentCount)
				{
					pstEntry->nParentGroupCount += nLocalParentCount;
					pstEntry->nOffsetParentGroup = pstEntry->nOffsetEntryName + nNameSize;
					int nOffset = pstEntry->nOffsetParentGroup;
					if( 0 < nParentCount)nOffset += ( sizeof( int) * nParentCount) ;
					int nGroupCount = g_stOrgData.pcGroupList->GetCount();
					GetLocalParentID( pszSeed, ( int*)&( ( LPBYTE)pstEntry)[ nOffset], nLocalParentCount, nGroupCount, g_stOrgData.pcLocalGroupList);
				}
			}
			if( pszSeed)
			{
				delete [] pszSeed;
				pszSeed = NULL;
			}
			if( pszLocalname)
			{
				delete [] pszLocalname;
				pszLocalname = NULL;
			}
		}
	}
	return nResult;
}

// エントリ情報の比較
int CompareEntryData( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize)
{
	if( _STDPROTCOL_ID == pstEntryDst->unNxlID && _STDPROTCOL_ID == pstEntrySrc->unNxlID)
	{
		int nMinSize = sizeof( ENTRYDATA) + sizeof( struct sockaddr);
		if( nMinSize <= nDstSize && nMinSize <= nSrcSize)
		{
			const struct sockaddr_in*	pstDstAddr;
			const struct sockaddr_in*	pstSrcAddr;

			pstDstAddr = ( struct sockaddr_in*)&( ( ( LPBYTE)pstEntryDst)[ sizeof( ENTRYDATA)]);
			pstSrcAddr = ( struct sockaddr_in*)&( ( ( LPBYTE)pstEntrySrc)[ sizeof( ENTRYDATA)]);

			if( pstDstAddr->sin_addr.S_un.S_addr == pstSrcAddr->sin_addr.S_un.S_addr)return TRUE;
		}
	}
	return FALSE;
}

// エントリ情報の表示
int GetEntryInfo( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize)
{
	if( _STDPROTCOL_ID == pstEntry->unNxlID)
	{
		int nMinSize = sizeof( ENTRYDATA) + sizeof( struct sockaddr);
		if( nMinSize <= nSize)
		{
			if( NULL == lpszEntryInfo || ( 31 + ( UNLEN + 1) + ( MAX_COMPUTERNAME_LENGTH + 1)) > nEntryInfoSize)return ( 31 + ( UNLEN + 1) + ( MAX_COMPUTERNAME_LENGTH + 1));

			char	szUser[ UNLEN + 1];
			char	szComp[ MAX_COMPUTERNAME_LENGTH + 1];

			lstrcpy( szUser, "Invalidate");
			lstrcpy( szComp, "Invalidate");

			const struct sockaddr_in*	pstAddr;
			pstAddr = ( struct sockaddr_in*)&( ( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)]);

			struct sockaddr		stWorkAdd;
			for( int nIndex = 0; nIndex < g_stOrgData.pcEntryList->GetCount(); nIndex++)
			{
				g_stOrgData.pcEntryList->GetEntryAdd( nIndex, &stWorkAdd, sizeof( struct sockaddr));
				if( ( ( struct sockaddr_in*)&stWorkAdd)->sin_addr.S_un.S_addr == pstAddr->sin_addr.S_un.S_addr)
				{
					g_stOrgData.pcEntryList->GetUserName( nIndex, szUser, UNLEN + 1);
					g_stOrgData.pcEntryList->GetComputerName( nIndex, szComp, MAX_COMPUTERNAME_LENGTH + 1);
					break;
				}
			}

			wsprintf( lpszEntryInfo, "IP Addr : %d.%d.%d.%d\r\nUSER : %s\r\nMachine : %s",
									pstAddr->sin_addr.S_un.S_un_b.s_b1,
									pstAddr->sin_addr.S_un.S_un_b.s_b2,
									pstAddr->sin_addr.S_un.S_un_b.s_b3,
									pstAddr->sin_addr.S_un.S_un_b.s_b4,
									szUser,
									szComp);

			return lstrlen( lpszEntryInfo);
		}
	}
	return 0;
}

// エントリーの直接取得
int InputEntry( HWND hWnd, HGLOBAL* phEntry)
{
	int nResult = IDCANCEL;

	EnterCriticalSection( &g_csDInput);

	if( IDOK == DialogBox( ( HINSTANCE)g_hModule, "DINPUTDLG", hWnd, ( DLGPROC)DInputDlg))
	{
		ULONG ulAdd = inet_addr( GetDInputString());
		if( INADDR_NONE != ulAdd)
		{
			int nSize = sizeof( ENTRYDATA) + sizeof( struct sockaddr) + lstrlen( GetDInputString()) + 1;
			*phEntry = GlobalAlloc( GHND, nSize);

			ENTRYDATA*	pstEntry = ( ENTRYDATA*)GlobalLock( *phEntry);

			if( NULL != pstEntry)
			{
				struct sockaddr_in	stAddress;

				stAddress.sin_family = AF_INET;
				stAddress.sin_port = htons( g_stOrgData.unPort);
				stAddress.sin_addr.S_un.S_addr = ulAdd;

				pstEntry->nSize = nSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _STDPROTCOL_ID;
				pstEntry->nEntryStat = 1;
				CopyMemory( &( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)], &stAddress, sizeof( struct sockaddr));
				pstEntry->nOffsetEntryName = sizeof( ENTRYDATA) + sizeof( struct sockaddr);
				lstrcpy( ( LPSTR)&( ( LPBYTE)pstEntry)[ pstEntry->nOffsetEntryName], GetDInputString());
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

// データ送信
int SendMemoData( const ENTRYDATA* pstEntry, const SENDDATA* pstSendData, int nSendDataSize, HANDLE hCancelEvent)
{
	return SendRawData( pstEntry, pstSendData, nSendDataSize, ( 60 * 1000), hCancelEvent);
}

// 応答送信
int SendResultData( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent)
{
	return SendRawData( pstEntry, pResultData, nResultDataSize, WSA_INFINITE, hCancelEvent);
}

// 固有エラーメッセージ
int GetErrorMsg( int nEcode, char* pszErrorMsg, int nSize)
{
	// この関数はなくてもいいンだけどね。
	// 将来きちんとしたエラーを返すようにするかもしれないからこうしてあるだけ
	if( _REJECT == nEcode)
	{
		char*	pszReject = "受信側で拒否されました。";
		if( NULL != pszErrorMsg && 0 != nSize)
		{
			lstrcpyn( pszErrorMsg, pszReject, nSize);
			return lstrlen( pszErrorMsg);
		}
		return lstrlen( pszReject);
	}
	return -1;
}
