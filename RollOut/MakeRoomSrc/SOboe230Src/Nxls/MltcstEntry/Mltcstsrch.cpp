#include <windows.h>
#include <lmcons.h>
#include <process.h>
#include "NetEx_IF.h"
#include "GroupList.h"
#include "EntryList.h"
#include "Mltcstsrch.h"
#include "MltcstsrchSlv.h"
#include "OrgSock.h"
#include "SettingDlg.h"
#include "AboutDlg.h"
#include "DInputDlg.h"

extern HANDLE			g_hModule;
extern MCSTNETORGDATA	g_stOrgData;
extern CRITICAL_SECTION	g_csDInput;

// protected
char*	pszProtcolName = "Multicast Search";

// �@�\�m�F
BOOL NetExSpecification( NXLSPECIFICATION* pDxlSpecification)
{
	pDxlSpecification->unLowerStructVersion = _NXL_IF_DATA_VER;
	pDxlSpecification->unUpperStructVersion = _NXL_IF_DATA_VER;
	lstrcpy( pDxlSpecification->szProtcolString, pszProtcolName);
	pDxlSpecification->unNxlID = _MCST_PROTCOL_ID;
	pDxlSpecification->unNxlStyle = _NXL_SENDRECEIVE | _NXL_ENTRYGROUP | _NXL_SUPPORTRESOLUTION | _NXL_SUPPORTINPUTENTRY;

	return TRUE;
}

// �ݒ�ύX�p
BOOL NetExConfigure( HWND hWnd)
{
	DialogBox( ( HINSTANCE)g_hModule, "SETTINGDLG", hWnd, ( DLGPROC)SettingDlg);

	return TRUE;
}

// Copyright�\���p
BOOL NetExAbout( HWND hWnd)
{
	DialogBox( ( HINSTANCE)g_hModule, "ABOUT", hWnd, ( DLGPROC)AboutDlg);

	return TRUE;
}

// ������
BOOL InitializeNetEx( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData)
{
	BOOL	blResult = FALSE;

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

			OFSTRUCT	of;
			if( HFILE_ERROR != OpenFile( "Mltcstsrch.ini", &of, OF_READ | OF_EXIST))
			{
				g_stOrgData.unMultiPort = htons( GetPrivateProfileInt( szUser, "McstPort", 7129, of.szPathName));
				char	szWork[ 2048];
				GetPrivateProfileString( szUser, "McstAddr", "239.191.38.82", szWork, 2048, of.szPathName);
				g_stOrgData.ulRcvAddress = inet_addr( szWork);
				ULONG ulCheckWork = ntohl( g_stOrgData.ulRcvAddress);
				// 0x000000e0 = inet_addr( "224.0.0.0");
				// 0xffffffef = inet_addr( "239.255.255.255");
				if( ULONG( 0xE0000000) > ulCheckWork && ULONG( 0xEFFFFFFF) < ulCheckWork)goto errordetection;
				GetPrivateProfileString( szUser, "McstIF", "0.0.0.0", szWork, 2048, of.szPathName);
				g_stOrgData.ulMyAddress = inet_addr( szWork);
				g_stOrgData.nTTL = GetPrivateProfileInt( szUser, "McstTTL", 15, of.szPathName);

				GetPrivateProfileString( szUser, "Nickname", szUser, szWork, 2048, of.szPathName);
				g_stOrgData.pszNickname = new char [ lstrlen( szWork) + 1];
				if( NULL == g_stOrgData.pszNickname)goto errordetection;
				lstrcpy( g_stOrgData.pszNickname, szWork);

				GetPrivateProfileString( szUser, "Group", pszAnony, szWork, 2048, of.szPathName);
				g_stOrgData.pszGroup = new char [ lstrlen( szWork) + 1];
				if( NULL == g_stOrgData.pszGroup)goto errordetection;
				lstrcpy( g_stOrgData.pszGroup, szWork);

				WORD	wDomainKeyU = GetPrivateProfileInt( szUser, "DomainKeyUw", 0, of.szPathName);
				WORD	wDomainKeyL = GetPrivateProfileInt( szUser, "DomainKeyLw", 0, of.szPathName);
				g_stOrgData.dwDomainKey = MAKELONG( wDomainKeyL, wDomainKeyU);

				g_stOrgData.nSeedType = GetPrivateProfileInt( szUser, "SeedType", 0, of.szPathName);
			}
			else
			{
				g_stOrgData.unMultiPort = htons( 7129);
				g_stOrgData.ulRcvAddress = inet_addr( "239.191.38.82");
				g_stOrgData.ulMyAddress = 0;
				g_stOrgData.nTTL = 15;

				g_stOrgData.pszNickname = new char [ lstrlen( szUser) + 1];
				if( NULL == g_stOrgData.pszNickname)goto errordetection;
				lstrcpy( g_stOrgData.pszNickname, szUser);

				g_stOrgData.pszGroup = new char [ lstrlen( pszAnony) + 1];
				if( NULL == g_stOrgData.pszGroup)goto errordetection;
				lstrcpy( g_stOrgData.pszGroup, pszAnony);

				g_stOrgData.dwDomainKey = 0;
				g_stOrgData.nSeedType = 0;
			}
		}
		// �O���[�o���f�[�^���������튮��
		g_stOrgData.blInitialized = TRUE;

		if( NULL != g_stOrgData.hStopEvent)
		{
			HANDLE hEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
			ResetEvent( hEvent);
			// ��M�҂��X���b�h�̋N��
			if( -1 != _beginthread( EntryControlThread, 0, &hEvent))
			{
				WaitForSingleObject( hEvent, 1000);
				ResetEvent( hEvent);
				// �G���g����M�X���b�h�̊J�n
				if( -1 != _beginthread( ListenThread, 0, &hEvent))
				{
					// �X���b�h�J�n�҂�WAIT
					WaitForSingleObject( hEvent, 1000);
					CloseHandle( hEvent);
					g_stOrgData.blActive = TRUE;
					g_stOrgData.blReadySend = TRUE;
					blResult = TRUE;
				}
				else
				{
					goto errordetection;
				}
			}
			else
			{
				goto errordetection;
			}
		}
	}
	return blResult;

errordetection:
	CleanupOrgData();
	return FALSE;
}

// �I��
BOOL UnInitializeNetEx( HWND hWnd)
{
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

		return TRUE;
	}
	return FALSE;
}

// ��Ԃ̊m�F
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

// �G���g���J�n
BOOL StartEntry( void)
{
#ifdef	_DEBUG
	OutputDebugString( "StartEntry\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		MulticastEntry( _ENTRY_START, FALSE);
		return TRUE;
	}
	return FALSE;
}

// �G���g���J�n�F����ʒm�t��
BOOL StartEntryEx( BOOL blAbsence)
{
#ifdef	_DEBUG
	OutputDebugString( "StartEntry\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		MulticastEntry( _ENTRY_START, blAbsence);
		return TRUE;
	}
	return FALSE;
}

// ����
BOOL AbsenceEntry( BOOL blAbsence)
{
	if( g_stOrgData.blInitialized)
	{
		g_stOrgData.blAbsence = blAbsence;
		MulticastEntry( _ENTRY_ANSER, blAbsence);
		return TRUE;
	}
	return FALSE;
}

// �G���g���I��
BOOL ExitEntry( void)
{
	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcEntryList->LockList())
		{
			g_stOrgData.pcEntryList->RemoveAll();
			g_stOrgData.pcGroupList->RemoveAll();
			g_stOrgData.pcEntryList->UnlockList();
		}
		MulticastEntry( _ENTRY_END, FALSE);
		return TRUE;
	}
	return FALSE;
}

// �G���g�����X�g�X�V�̒��f�A�擾�̊J�n�̂���
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

// �G���g�����X�g�X�V�̍ĊJ
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

// �O���[�v�����擾
int GetGroupCount( void)
{
	int	nResult = 0;
	if( g_stOrgData.blInitialized)
	{
		nResult = g_stOrgData.pcGroupList->GetCount();
	}
	if( g_stOrgData.pcLocalGroupList)
	{
		nResult += g_stOrgData.pcLocalGroupList->GetCount();
	}
	return nResult;
}

// �O���[�v�����擾
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

// �G���g�����̔�r
int CompareEntryData( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize)
{
	if( _MCST_PROTCOL_ID == pstEntryDst->unNxlID && _MCST_PROTCOL_ID == pstEntrySrc->unNxlID)
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

// �G���g�����̕\��
int GetEntryInfo( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize)
{
	if( _MCST_PROTCOL_ID == pstEntry->unNxlID)
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

// ���X�g�ɑ��݂���G���g�����擾
int GetEntryCount( void)
{
	int nResult = 0;

	if( g_stOrgData.blInitialized)
	{
		nResult = g_stOrgData.pcEntryList->GetCount();
	}
	return nResult;
}

// �G���g�����̎擾
int GetEntryData( int nIndex, ENTRYDATA* pstEntry, int nSize)
{
	if( 0 > nIndex)return -1;

	int	nResult = -1;

	if( g_stOrgData.blInitialized)
	{
		// ���݂�`�A�Ȃ񂩁A���\����Ȋ����ɂȂ��Ă��܂��܂����B(/-;)
		if( g_stOrgData.pcEntryList->GetCount() >= nIndex)
		{
			// �ŏ��ɁANodeList����G���A�X�Ƃ����Q�b�g�I
			char*	pszLocalname = NULL;
			char*	pszSeed = NULL;
			int nLocalParentCount = 0;
			int nLocalNameLen = 0;
			if( g_stOrgData.pcLocalGroupList)
			{
				int nLen = GetSeed( nIndex, NULL, 0);
				if( 0 < nLen)
				{
					pszSeed = new char [ nLen + 1];
					if( pszSeed)GetSeed( nIndex, pszSeed, nLen + 1);
				}
				nLocalParentCount = GetLocalParentID( pszSeed, NULL, 0, 0, g_stOrgData.pcLocalGroupList);
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
				// ���̂ւ�Ȃ�āA�{�N������̃C�������c�c
				pstEntry->nSize = nDataSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _MCST_PROTCOL_ID;
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

// �G���g���[�̒��ڎ擾
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
				stAddress.sin_port = htons( g_stOrgData.unMultiPort);
				stAddress.sin_addr.S_un.S_addr = ulAdd;

				pstEntry->nSize = nSize;
				pstEntry->unStructVersion = _ENTRYSTRUCT_VER100;
				pstEntry->unNxlID = _MCST_PROTCOL_ID;
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

// �f�[�^���M
int SendMemoData( const ENTRYDATA* pstEntry, const SENDDATA* pstSendData, int nSendDataSize, HANDLE hCancelEvent, HGLOBAL hExt)
{
	return SendRawData( pstEntry, pstSendData, nSendDataSize, ( 60 * 1000), hCancelEvent);
}

// �������M
int SendResultData( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent)
{
	return SendRawData( pstEntry, pResultData, nResultDataSize, WSA_INFINITE, hCancelEvent);
}

// �ŗL�G���[���b�Z�[�W
int GetErrorMsg( int nEcode, char* pszErrorMsg, int nSize)
{
	// ���̊֐��͂Ȃ��Ă������������ǂˁB
	// ����������Ƃ����G���[��Ԃ��悤�ɂ��邩������Ȃ����炱�����Ă��邾��
	if( _REJECT == nEcode)
	{
		char*	pszReject = "��M���ŋ��ۂ���܂����B";
		if( NULL != pszErrorMsg && 0 != nSize)
		{
			lstrcpyn( pszErrorMsg, pszReject, nSize);
			return lstrlen( pszErrorMsg);
		}
		return lstrlen( pszReject);
	}
	return -1;
}
