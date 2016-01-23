#include <windows.h>
#if(_WIN32_WINNT >= 0x0400)
#include <ws2tcpip.h>
#else
#include <winsock.h>
#endif
#include <process.h>
#include <LMCONS.H>
#include "NetEx_IF.h"
#include "Mltcstsrch.h"
#include "MltcstsrchSlv.h"
#include "EntryList.h"

#include "OrgSock.h"
/*
#define	_STEPDEBUG
*/

extern HANDLE			g_hModule;
extern MCSTNETORGDATA	g_stOrgData;
extern CRITICAL_SECTION g_csFileWrite;

static const char*	pcszMulticastEntry = "MulticastEntry";
static const char*	pcszSendEntryCommand = "SendEntryCommand";
static const char*	pcszEntryControlThread = "EntryControlThread";
static const char*	pcszListenThread = "ListenThread";

char				szErrorFormat[ 512];

void EntryControlThread( void* lpv)
{
	HANDLE	hInitializeEvent = *( HANDLE*)lpv;
#ifdef	_DEBUG
	OutputDebugString( "Start-EntryControlThread\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		CEntryList*		pcEntryList;
		CGroupList*		pcGroupList;
		UINT			unMultiPort;
		HANDLE			hStopEvent;
		char*			pszEntryName;
		unsigned long	ulMyAddress = 0;
		unsigned long	ulMiltAddress = 0;

		hStopEvent = g_stOrgData.hStopEvent;
		unMultiPort = g_stOrgData.unMultiPort;
		pcEntryList = g_stOrgData.pcEntryList;
		pcGroupList = g_stOrgData.pcGroupList;
		pszEntryName = g_stOrgData.pszEntryName;
		ulMyAddress = g_stOrgData.ulMyAddress;
		ulMiltAddress = g_stOrgData.ulRcvAddress;

		SOCKET				sMulticast = INVALID_SOCKET;
		struct sockaddr_in	stSkAddSrv;
		struct sockaddr		stSkAddFrm;
		struct sockaddr_in	stSkAddTo;
		int					nSkAddFrmSize;
		HANDLE				hWsaMctPort = NULL;
		HANDLE				ahEvents[ 3];
		ENTRYPACKET*		pstEntryPacket = NULL;
		int					nstEntryPacketSize = 0;
		DWORD				dwData;

		struct ip_mreq		mreq;

		// 自己アドレスの取得
		{
#ifdef	_STEPDEBUG
			OutputErrorString( "自己アドレスの取得", "EntryControlThread");
#endif
			char	szData[ 1024];
			for( int nIndex = 0; nIndex < _RETRY_COUNT; nIndex++)
			{
				if( WAIT_OBJECT_0 == WaitForSingleObject( hStopEvent, 0))
				{
#ifdef	_STEPDEBUG
					OutputErrorString( "WAIT_OBJECT_0 == WaitForSingleObject", "EntryControlThread");
#endif
					goto cleanup;
				}
#ifdef	_STEPDEBUG
				OutputErrorString( "オーソドックスな取得 - 1", "EntryControlThread");
#endif
				if( 0 == gethostname( szData, 1024))
				{
					// オーソドックスな取得
#ifdef	_STEPDEBUG
					OutputErrorString( "オーソドックスな取得 - 2", "EntryControlThread");
#endif
					PHOSTENT pHost = gethostbyname( szData);
					if( pHost)
					{
#ifdef	_STEPDEBUG
						OutputErrorString( "オーソドックスな取得 - 3", "EntryControlThread");
#endif
						if( AF_INET == pHost->h_addrtype && sizeof( unsigned long) == pHost->h_length)
						{
#ifdef	_STEPDEBUG
							OutputErrorString( "オーソドックスな取得 - 4", "EntryControlThread");
#endif
							CopyMemory( &ulMyAddress, *pHost->h_addr_list, sizeof( unsigned long));
						}
					}
#ifdef	_STEPDEBUG
					OutputErrorString( "オーソドックスな取得 - 5", "EntryControlThread");
#endif
				}
				if( 0 == ulMyAddress)
				{
#ifdef	_STEPDEBUG
					OutputErrorString( "ちょっと変わり種 - 1", "EntryControlThread");
#endif
					// ちょっと変わり種
					SOCKET	sGetMyAddr;
					struct sockaddr_in	stAddr;

					sGetMyAddr = socket( AF_INET, SOCK_DGRAM, 0);
					if( INVALID_SOCKET != sGetMyAddr)
					{
#ifdef	_STEPDEBUG
						OutputErrorString( "ちょっと変わり種 - 2", "EntryControlThread");
#endif
						stAddr.sin_family = AF_INET;
						stAddr.sin_port = htons( IPPORT_ECHO);
						stAddr.sin_addr.s_addr = inet_addr( "203.140.33.145");

						if( SOCKET_ERROR != connect( sGetMyAddr, ( struct sockaddr*)&stAddr, sizeof( struct sockaddr)))
						{
#ifdef	_STEPDEBUG
							OutputErrorString( "ちょっと変わり種 - 3", "EntryControlThread");
#endif
							int	nSize = sizeof( struct sockaddr);
							if( SOCKET_ERROR != getsockname( sGetMyAddr, ( struct sockaddr*)&stAddr, &nSize))
							{
								ulMyAddress = stAddr.sin_addr.s_addr;
							}
						}
						closesocket( sGetMyAddr);
					}
#ifdef	_STEPDEBUG
					OutputErrorString( "ちょっと変わり種 - 4", "EntryControlThread");
#endif
				}
			}
			if( 0 == ulMyAddress)
			{
				// 自己アドレスを取得できないので送信はデキナイ
				g_stOrgData.blReadySend = FALSE;
				goto cleanup;
			}
#ifdef	_STEPDEBUG
			else
			{
				OutputErrorString( "自己アドレスの取得 - E", "EntryControlThread");
				char	szWork[ 32];
				wsprintf( szWork, "ulMyAddress:0x%X", ulMyAddress);
				OutputErrorString( szWork, "EntryControlThread");
			}
#endif
			g_stOrgData.ulMyAddress = ulMyAddress;
		}

		// 受信用ソケット生成
		sMulticast = socket( PF_INET, SOCK_DGRAM, 0);
		if( INVALID_SOCKET == sMulticast)
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "SocketCreateErr[sMulticast]:%d", nError);
			OutputErrorString( szErrorFormat, pcszEntryControlThread);

			goto cleanup;
		}
		stSkAddSrv.sin_family		= AF_INET;
		stSkAddSrv.sin_addr.s_addr	= INADDR_ANY;
		stSkAddSrv.sin_port			= unMultiPort;
		if( SOCKET_ERROR == bind( sMulticast, ( struct sockaddr*)&stSkAddSrv, sizeof( struct sockaddr)))
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[sMulticast:bind]:%d", nError);
			OutputErrorString( szErrorFormat, pcszEntryControlThread);

			goto cleanup;
		}
		mreq.imr_multiaddr.s_addr = ulMiltAddress;
		mreq.imr_interface.s_addr = ulMyAddress;
		if( SOCKET_ERROR == setsockopt( sMulticast, IPPROTO_IP, IP_ADD_MEMBERSHIP, ( char*)&mreq, sizeof( mreq)))
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[IP_ADD_MEMBERSHIP]:%d", nError);
			OutputErrorString( szErrorFormat, pcszEntryControlThread);

			goto cleanup;
		}


		// 受信イベントの用意
		hWsaMctPort = CreateEvent( NULL, FALSE, FALSE, NULL);
		if( NULL == hWsaMctPort)goto cleanup;
		Org_WSAEventSelect( sMulticast, hWsaMctPort, FD_READ | FD_CLOSE);

		ahEvents[ 0] = hStopEvent;
		ahEvents[ 1] = hWsaMctPort;

		SetEvent( hInitializeEvent);
		// ポート受信
		while( TRUE)
		{
			dwData = Org_WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
			if( 0 == ( dwData - WSA_WAIT_EVENT_0))break;
			if( 1 == ( dwData - WSA_WAIT_EVENT_0))
			{
				ioctlsocket( sMulticast, FIONREAD, ( ULONG*)&nstEntryPacketSize);
				nstEntryPacketSize += 10;
				pstEntryPacket = ( ENTRYPACKET*)malloc( nstEntryPacketSize);

				nSkAddFrmSize = sizeof( stSkAddFrm);
				int	nErr = recvfrom( sMulticast, ( char*)pstEntryPacket, nstEntryPacketSize, 0, &stSkAddFrm, &nSkAddFrmSize);
				( ( struct sockaddr_in*)&stSkAddFrm)->sin_port = unMultiPort;
				if( SOCKET_ERROR != nErr)
				{
					if( !lstrcmp( pstEntryPacket->szHeader, _MULTICAST_ENTRYPACKET_HEADER))
					{
						if( pstEntryPacket->dwDomainKey == g_stOrgData.dwDomainKey)
						{
							switch( pstEntryPacket->nEntryCode)
							{
							case _ENTRY_START:
								// エントリを受信した！
								if( ulMyAddress != ( ( struct sockaddr_in*)&stSkAddFrm)->sin_addr.S_un.S_addr)
								{
									// 応答
									CopyMemory( &stSkAddTo, &stSkAddFrm, sizeof( struct sockaddr));
									SendEntryCommand( _ENTRY_ANSER, g_stOrgData.blAbsence, pszEntryName, ( struct sockaddr*)&stSkAddTo);
#ifdef	_DEBUG
									OutputDebugString( "SendAns-(M)\n");
#endif
								}
							case _ENTRY_ANSER:
								{
									if( pcEntryList->LockList())
									{
										char*	pszWork;
										int		nGroupCount = 0;
										int*	panGroupID = NULL;
										pszWork = ( char*)pstEntryPacket;
										if( sizeof( ENTRYPACKET) <= pstEntryPacket->nszGroup)
										{
											char*	pszGroup;
											pszGroup = &pszWork[ pstEntryPacket->nszGroup];
											nGroupCount = pcGroupList->CheckGroupCount( pszGroup);
											panGroupID = new int [ nGroupCount];
											pcGroupList->AddGroup( pszGroup, panGroupID, nGroupCount);
										}
										pcEntryList->AddEntry( &stSkAddFrm, pstEntryPacket, panGroupID, nGroupCount);
										pcEntryList->UnlockList();
									}
								}
								break;
							case _ENTRY_END:
								// 終了を受信した！
								if( pcEntryList->LockList())
								{
									char*	pszWork;
									char*	pszNickname;
									pszWork = ( char*)pstEntryPacket;
									pszNickname = &pszWork[ pstEntryPacket->nszNickname];
									pcEntryList->DelEntry( &stSkAddFrm, pszNickname);
									pcEntryList->UnlockList();
								}
								break;
							}
						}
					}
				}
				else
				{
					int nError = WSAGetLastError();
					wsprintf( szErrorFormat, "Err[sMulticast:recvfrom]:%d", nError);
					OutputErrorString( szErrorFormat, pcszEntryControlThread);
#ifdef	_DEBUG
					char	szMsg[ 128];
					wsprintf( szMsg, "%s:%d - EC:%d", __FILE__, __LINE__, nError);
					OutputDebugString( szMsg);
#endif
				}
				free( pstEntryPacket);
			}
		}

// 終了処理
cleanup:
		if( INVALID_SOCKET != sMulticast)
		{
			if( NULL != hWsaMctPort)
			{
				Org_WSAEventSelect( sMulticast, hWsaMctPort, 0);
			}
			mreq.imr_multiaddr.s_addr = ulMiltAddress;
			mreq.imr_interface.s_addr = ulMyAddress;
			setsockopt( sMulticast, IPPROTO_IP, IP_DROP_MEMBERSHIP, ( char*)&mreq, sizeof( mreq));
			closesocket( sMulticast);
		}
		if( NULL != hWsaMctPort)CloseHandle( hWsaMctPort);
	}
#ifdef	_DEBUG
	OutputDebugString( "End-EntryControlThread\n");
#endif
	_endthread();
}

BOOL MulticastEntry( int nEntry, BOOL blAsence)
{
	if( !g_stOrgData.blInitialized)
	{
		OutputErrorString( "no init!g_stOrgData", pcszMulticastEntry);
		return FALSE;
	}

	BOOL	blResult = FALSE;
	SOCKET	sSend = INVALID_SOCKET;

	int	nSendSize;
	ENTRYPACKET*	pEntryPacket = NULL;

	nSendSize = sizeof( ENTRYPACKET);
	nSendSize += ( lstrlen( g_stOrgData.pszNickname) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszGroup) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszLogonID) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszMachineName) + 1);

	char*	pszWork = ( char*)malloc( nSendSize);
	if( NULL == pszWork)
	{
		OutputErrorString( "memory alloc erro!", pcszMulticastEntry);
		return FALSE;
	}

	pEntryPacket = ( ENTRYPACKET*)pszWork;
	int	nMasterOffset = sizeof( ENTRYPACKET);

	pEntryPacket->nszNickname = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszNickname], g_stOrgData.pszNickname);
	nMasterOffset += ( lstrlen( g_stOrgData.pszNickname) + 1);

	pEntryPacket->nszGroup = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszGroup], g_stOrgData.pszGroup);
	nMasterOffset += ( lstrlen( g_stOrgData.pszGroup) + 1);

	pEntryPacket->nszLogonID = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszLogonID], g_stOrgData.pszLogonID);
	nMasterOffset += ( lstrlen( g_stOrgData.pszLogonID) + 1);

	pEntryPacket->nszMachineName = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszMachineName], g_stOrgData.pszMachineName);
	nMasterOffset += ( lstrlen( g_stOrgData.pszMachineName) + 1);

	pEntryPacket->nSize				= nSendSize;
	lstrcpy( pEntryPacket->szHeader, _MULTICAST_ENTRYPACKET_HEADER);
	pEntryPacket->unPlugInVersion	= _MULTICAST_PLUGIN_VERSION;
	pEntryPacket->nEntryCode		= nEntry;
	pEntryPacket->blAbsence			= blAsence;
	pEntryPacket->dwDomainKey		= g_stOrgData.dwDomainKey;

	sSend = socket( PF_INET, SOCK_DGRAM, 0);
	if( INVALID_SOCKET != sSend)
	{
		struct sockaddr_in	stSkAddLcl;
		ZeroMemory( &stSkAddLcl, sizeof( struct sockaddr));
		stSkAddLcl.sin_family		= AF_INET;
		stSkAddLcl.sin_addr.s_addr	= g_stOrgData.ulMyAddress;
		stSkAddLcl.sin_port			= htons( 0);
		if( SOCKET_ERROR != bind( sSend, ( struct sockaddr*)&stSkAddLcl, sizeof( struct sockaddr)))
		{
			int	nTTL = g_stOrgData.nTTL;
			if( SOCKET_ERROR != setsockopt( sSend, IPPROTO_IP, IP_MULTICAST_TTL, ( const char*)&nTTL,  sizeof( int)))
			{
				DWORD	dwMyIPAddress = g_stOrgData.ulMyAddress;
				if( SOCKET_ERROR != setsockopt( sSend, IPPROTO_IP, IP_MULTICAST_IF, ( const char*)&dwMyIPAddress,  sizeof( DWORD)))
				{
					struct sockaddr_in	stSkAddTo;
					ZeroMemory( &stSkAddTo, sizeof( struct sockaddr));
					stSkAddTo.sin_family		= AF_INET;
					stSkAddTo.sin_addr.s_addr	= g_stOrgData.ulRcvAddress;
					stSkAddTo.sin_port			= g_stOrgData.unMultiPort;

					ULONG	ulBlock = TRUE;
					ioctlsocket( sSend, FIONBIO, &ulBlock);
					if( SOCKET_ERROR != sendto( sSend, ( char*)pEntryPacket, nSendSize, 0, ( struct sockaddr*)&stSkAddTo, sizeof( struct sockaddr)))
					{
						blResult = TRUE;
					}
					else
					{
						int nError = WSAGetLastError();
						wsprintf( szErrorFormat, "Err[sendto]:%d", nError);
						OutputErrorString( szErrorFormat, pcszMulticastEntry);
					}
				}
				else
				{
					int nError = WSAGetLastError();
					wsprintf( szErrorFormat, "Err[IP_MULTICAST_IF]:%d", nError);
					OutputErrorString( szErrorFormat, pcszMulticastEntry);
				}
			}
			else
			{
				int nError = WSAGetLastError();
				wsprintf( szErrorFormat, "Err[IP_MULTICAST_TTL]:%d", nError);
				OutputErrorString( szErrorFormat, pcszMulticastEntry);
			}
		}
		else
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[bind]:%d", nError);
			OutputErrorString( szErrorFormat, pcszMulticastEntry);
		}
		closesocket( sSend);
	}
	else
	{
		OutputErrorString( "SocketCreateErr", pcszMulticastEntry);
	}

	if( pEntryPacket)
	{
		free( pEntryPacket);
		pEntryPacket = NULL;
	}

	return blResult;
}

BOOL SendEntryCommand( int nEntry, BOOL blAsence, LPCSTR lpcszEntryName, struct sockaddr* pstSockAdd)
{
	if( !g_stOrgData.blInitialized)
	{
		OutputErrorString( "no init!g_stOrgData", pcszSendEntryCommand);
		return FALSE;
	}

	BOOL				blResult = FALSE;
	SOCKET				sSend = INVALID_SOCKET;
	
	int	nSendSize;
	ENTRYPACKET*	pEntryPacket = NULL;

	nSendSize = sizeof( ENTRYPACKET);
	nSendSize += ( lstrlen( g_stOrgData.pszNickname) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszGroup) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszLogonID) + 1);
	nSendSize += ( lstrlen( g_stOrgData.pszMachineName) + 1);

	char*	pszWork = ( char*)malloc( nSendSize);
	if( NULL == pszWork)
	{
		OutputErrorString( "memory alloc error!", pcszSendEntryCommand);
		return FALSE;
	}

	pEntryPacket = ( ENTRYPACKET*)pszWork;
	int	nMasterOffset = sizeof( ENTRYPACKET);

	pEntryPacket->nszNickname = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszNickname], g_stOrgData.pszNickname);
	nMasterOffset += ( lstrlen( g_stOrgData.pszNickname) + 1);

	pEntryPacket->nszGroup = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszGroup], g_stOrgData.pszGroup);
	nMasterOffset += ( lstrlen( g_stOrgData.pszGroup) + 1);

	pEntryPacket->nszLogonID = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszLogonID], g_stOrgData.pszLogonID);
	nMasterOffset += ( lstrlen( g_stOrgData.pszLogonID) + 1);

	pEntryPacket->nszMachineName = nMasterOffset;
	lstrcpy( &pszWork[ pEntryPacket->nszMachineName], g_stOrgData.pszMachineName);
	nMasterOffset += ( lstrlen( g_stOrgData.pszMachineName) + 1);


	pEntryPacket->nSize				= nSendSize;
	lstrcpy( pEntryPacket->szHeader, _MULTICAST_ENTRYPACKET_HEADER);
	pEntryPacket->unPlugInVersion	= _MULTICAST_PLUGIN_VERSION;
	pEntryPacket->nEntryCode		= nEntry;
	pEntryPacket->blAbsence			= blAsence;
	pEntryPacket->dwDomainKey		= g_stOrgData.dwDomainKey;

	// 送信用ソケット生成
	sSend = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if( INVALID_SOCKET != sSend)
	{
		struct sockaddr_in	stSkAddLcl;
		ZeroMemory( &stSkAddLcl, sizeof( struct sockaddr));
		stSkAddLcl.sin_family		= AF_INET;
		stSkAddLcl.sin_addr.s_addr	= INADDR_ANY;
		stSkAddLcl.sin_port			= htons( 0);
		if( SOCKET_ERROR != bind( sSend, ( struct sockaddr*)&stSkAddLcl, sizeof( struct sockaddr)))
		{
			BOOL	blLinger = TRUE;
			setsockopt( sSend, SOL_SOCKET, SO_LINGER, ( char*)&blLinger,  sizeof( BOOL));
			if( SOCKET_ERROR != sendto( sSend, ( char*)pEntryPacket, nSendSize, 0, pstSockAdd, sizeof( struct sockaddr)))
			{
				blResult = TRUE;
			}
			else
			{
				int nError = WSAGetLastError();
				wsprintf( szErrorFormat, "Err[sendto]:%d", nError);
				OutputErrorString( szErrorFormat, pcszSendEntryCommand);
			}
		}
		else
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[bind]:%d", nError);
			OutputErrorString( szErrorFormat, pcszSendEntryCommand);
		}
		closesocket( sSend);
	}
	else
	{
		OutputErrorString( "SocketCreateErr", pcszSendEntryCommand);
	}
	free( pEntryPacket);
	pEntryPacket = NULL;

	return blResult;
}

void ListenThread( void*lpv)
{
	HANDLE	hInitializeEvent = *( HANDLE*)lpv;

	SOCKET				sListen = INVALID_SOCKET;
	SOCKET				sClient = INVALID_SOCKET;
	struct sockaddr_in	stSkAddSrv;
	struct sockaddr_in	stSkAddCli;
	HANDLE				ahEvents[2];
	DWORD				dwWaitResult;
	HANDLE			hWsaAccept = NULL;
	int					nLen;

#ifdef	_DEBUG
	OutputDebugString( "Start-ListenThread\n");
#endif
	if( g_stOrgData.blInitialized)
	{
		UINT		unPort;
		HANDLE		hStopEvent;
		RECEIVEDATA	lpfnRcvData;
		hStopEvent = g_stOrgData.hStopEvent;
		lpfnRcvData = g_stOrgData.lpfnRcvData;
		unPort = g_stOrgData.unMultiPort;
		////////////////////////////////////////////////////////
		//
		// 初期化開始
		//
		// ソケットの作成
		sListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( INVALID_SOCKET == sListen)
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "SocketCreateErr[sListen]:%d", nError);
			OutputErrorString( szErrorFormat, pcszListenThread);

			goto cleanup;
		}
		// ソケットのバインド
		ZeroMemory( &stSkAddSrv, sizeof( stSkAddSrv));
		stSkAddSrv.sin_family		= AF_INET;
		stSkAddSrv.sin_addr.s_addr	= htons( INADDR_ANY);
		stSkAddSrv.sin_port			= unPort;
		if( SOCKET_ERROR == bind( sListen, ( struct sockaddr*)&stSkAddSrv, sizeof( stSkAddSrv)))
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[bind]:%d", nError);
			OutputErrorString( szErrorFormat, pcszListenThread);

			goto cleanup;
		}
		if( 0 > listen( sListen, 4))
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[listen]:%d", nError);
			OutputErrorString( szErrorFormat, pcszListenThread);

			goto cleanup;
		}
		hWsaAccept = CreateEvent( NULL, FALSE, FALSE, NULL);
		if( NULL == hWsaAccept)
		{
			int nError = WSAGetLastError();
			wsprintf( szErrorFormat, "Err[CreateEvent]:%d", nError);
			OutputErrorString( szErrorFormat, pcszListenThread);

			goto cleanup;
		}
		//
		// 初期終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// アクセプト待ちループ開始
		//
		Org_WSAEventSelect( sListen, hWsaAccept, FD_ACCEPT);
		ahEvents[ 0] = hStopEvent;
		ahEvents[ 1] = hWsaAccept;

		// ほんとはさぁ排他制御とかしなきゃイケナイのかもしれないけど……
		g_stOrgData.blReadyReceive = TRUE;
		SetEvent( hInitializeEvent);
		while( 1)
		{
			dwWaitResult = Org_WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
			if( 0 == ( dwWaitResult - WSA_WAIT_EVENT_0))break;
			if( 1 == ( dwWaitResult - WSA_WAIT_EVENT_0))
			{
				nLen = sizeof( stSkAddCli);
				sClient = accept( sListen, ( struct sockaddr*)&stSkAddCli, &nLen);
				if( INVALID_SOCKET != sClient)
				{
					SRVDATA*	pstSrvData;

					pstSrvData = ( SRVDATA*)malloc( sizeof( SRVDATA));
					pstSrvData->hStopEvent		= hStopEvent;
					pstSrvData->sClient			= sClient;
					pstSrvData->lpfnRcvData		= lpfnRcvData;
					pstSrvData->blAbsence		= g_stOrgData.blAbsence;
					if( -1 == _beginthread( ReceiveDataThread, 0, ( void*)pstSrvData))
					{
						free( pstSrvData);
					}
				}
				else
				{
					int nError = WSAGetLastError();
					wsprintf( szErrorFormat, "Err[accept]:%d", nError);
					OutputErrorString( szErrorFormat, pcszListenThread);
				}
			}
		}
		//
		// アクセプト待ちループ終了
		//
		////////////////////////////////////////////////////////

// 終了処理
cleanup:
		// ほんとはさぁ排他制御とかしなきゃイケナイのかもしれないけど……
		g_stOrgData.blReadyReceive = FALSE;

		if( INVALID_SOCKET != sListen)
		{
			if( NULL != hWsaAccept)
			{
				Org_WSAEventSelect( sListen, hWsaAccept, 0);
			}
			closesocket( sListen);
		}
		if( NULL != hWsaAccept)CloseHandle( hWsaAccept);
	}
#ifdef	_DEBUG
	OutputDebugString( "End-ListenThread\n");
#endif
	_endthread();
}

void ReceiveDataThread( void* pvParameter)
{
	SRVDATA*		pstSrvData;
	SOCKET			sClient = INVALID_SOCKET;
	HANDLE			hStopEvent = NULL;
	RECEIVEDATA		lpfnRcvData;
	int				nResult = 0;
	int				nLen;
	int				nRcvLen;
	char*			pchRcvBuffer = NULL;
	DWORD			dwWaitReslut;
	BOOL			blAbsence = FALSE;

#ifdef	_DEBUG
	OutputDebugString( "Start-ReceiveDataThread\n");
#endif
	if( pvParameter)
	{
		////////////////////////////////////////////////////////
		//
		// 初期化開始
		//
		pstSrvData		= ( SRVDATA*)pvParameter;
		sClient			= pstSrvData->sClient;
		hStopEvent		= pstSrvData->hStopEvent;
		lpfnRcvData		= pstSrvData->lpfnRcvData;
		blAbsence		= pstSrvData->blAbsence;
		free( pvParameter);

		HANDLE	hWsaCloseRead;
		hWsaCloseRead = CreateEvent( NULL, FALSE, FALSE, NULL);
		WSANETWORKEVENTS	stNetEvents;
		HANDLE	ahEvents[2];
		ahEvents[ 0] = hStopEvent;
		ahEvents[ 1] = hWsaCloseRead;
		//
		// 初期化開始
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 接続の応答
		//
		char*	pAns = "Hi,Oboe!";
		Org_WSAEventSelect( sClient, hWsaCloseRead, FD_WRITE | FD_CLOSE);
		if( SOCKET_ERROR == send( sClient, ( LPCSTR)pAns, lstrlen( pAns) + 1, 0))
		{
			if( WSAEWOULDBLOCK != WSAGetLastError())
			{
				goto cleanup;
			}
		}
		// 送信完了の確認
		while( TRUE)
		{
			dwWaitReslut = Org_WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
			if( 0 == ( dwWaitReslut - WSA_WAIT_EVENT_0))goto cleanup;
			if( 1 == ( dwWaitReslut - WSA_WAIT_EVENT_0))
			{
				ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
				Org_WSAEnumNetworkEvents( sClient, hWsaCloseRead, &stNetEvents);
				if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;
				break;
			}
		}
		//
		// 接続の応答
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// データ受信の開始
		//
		Org_WSAEventSelect( sClient, hWsaCloseRead, FD_READ | FD_CLOSE);
		while( 1)
		{
			dwWaitReslut = Org_WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
			if( 0 == ( dwWaitReslut - WSA_WAIT_EVENT_0))break;
			if( 1 == ( dwWaitReslut - WSA_WAIT_EVENT_0))
			{
				ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
				Org_WSAEnumNetworkEvents( sClient, hWsaCloseRead, &stNetEvents);
				if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

				struct sockaddr stPeerAddr;
				int	nData = sizeof( struct sockaddr);
				getpeername( sClient, &stPeerAddr, &nData);

				ioctlsocket( sClient, FIONREAD, ( ULONG*)&nLen);
				pchRcvBuffer = new char [ nLen];
				nRcvLen = recv( sClient, pchRcvBuffer, nLen, 0);
				if( SOCKET_ERROR == nRcvLen)
				{
					int nError = WSAGetLastError();
					if( WSAEWOULDBLOCK == nError)
					{
						Sleep( 100);
						continue;
					}
					break;
				}
				else
				{
					SENDDATAHEADER*	pstSendHeader = ( SENDDATAHEADER*)pchRcvBuffer;
					if( pstSendHeader->nSize <= nLen)
					{
						ENTRYDATA*	pEntryData = NULL;
						int			nEntryDataSize = 0;
						if( g_stOrgData.pcEntryList->LockList())
						{
							int	nCount = g_stOrgData.pcEntryList->GetCount();
							for( int nIndex = 0; nIndex < nCount; nIndex++)
							{
								struct sockaddr	stEntry;
								g_stOrgData.pcEntryList->GetEntryAdd( nIndex, &stEntry, sizeof( struct sockaddr));
								if( ( ( struct sockaddr_in*)&stPeerAddr)->sin_addr.S_un.S_addr == ( ( struct sockaddr_in*)&stEntry)->sin_addr.S_un.S_addr)
								{
									int nNameSize = g_stOrgData.pcEntryList->GetEntryName( nIndex, NULL, 0) + 1;
									nEntryDataSize = sizeof( ENTRYDATA) + sizeof( struct sockaddr)+ nNameSize;
									pEntryData = ( ENTRYDATA*)malloc( nEntryDataSize);
									if( pEntryData)
									{
										pEntryData->nSize				= nEntryDataSize;
										pEntryData->unStructVersion		= _ENTRYSTRUCT_VER100;
										pEntryData->unNxlID				= _MCST_PROTCOL_ID;
										pEntryData->nEntryStat			= 1;
										pEntryData->nParentGroupCount	= 0;
										pEntryData->nOffsetParentGroup	= -1;
										pEntryData->nOffsetEntryName	= sizeof( ENTRYDATA) + sizeof( struct sockaddr);
										g_stOrgData.pcEntryList->GetEntryAdd( nIndex, ( struct sockaddr*)&( ( LPBYTE)pEntryData)[ sizeof( ENTRYDATA)], sizeof( struct sockaddr));
										g_stOrgData.pcEntryList->GetEntryName( nIndex, ( LPSTR)&( ( LPBYTE)pEntryData)[ pEntryData->nOffsetEntryName], nNameSize);
									}
									break;
								}
							}
							g_stOrgData.pcEntryList->UnlockList();
						}
						nResult = lpfnRcvData( pEntryData, nEntryDataSize, pstSendHeader->abyData, pstSendHeader->nSize);
						nResult = ( FALSE == nResult) ? _SEND_ERROR : ( ( blAbsence) ? _SEND_ABSENCE : _SEND_SUCCESS);
						if( pEntryData)
						{
							free( pEntryData);
						}
					}
					delete [] pchRcvBuffer;
					pchRcvBuffer = NULL;
					break;
				}
			}
		}
		//
		// データ受信の終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 受信結果の応答
		//
		Org_WSAEventSelect( sClient, hWsaCloseRead, FD_CLOSE);
		if( SOCKET_ERROR == send( sClient, ( LPCSTR)&nResult, sizeof( int), 0))
		{
			if( WSAEWOULDBLOCK != WSAGetLastError())
			{
				goto cleanup;
			}
		}
		// 相手から切断されるのを待つ
		while( TRUE)
		{
			dwWaitReslut = Org_WSAWaitForMultipleEvents( 2, ahEvents, FALSE, WSA_INFINITE, FALSE);
			if( 0 == ( dwWaitReslut - WSA_WAIT_EVENT_0))goto cleanup;
			if( 1 == ( dwWaitReslut - WSA_WAIT_EVENT_0))
			{
				ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
				Org_WSAEnumNetworkEvents( sClient, hWsaCloseRead, &stNetEvents);
				break;
			}
		}
		//
		// 受信結果の応答
		//
		////////////////////////////////////////////////////////

// 終了処理
cleanup:
		if( pchRcvBuffer)
		{
			delete [] pchRcvBuffer;
			pchRcvBuffer = NULL;
		}

		if( INVALID_SOCKET != sClient)
		{
			if( NULL != hWsaCloseRead)
			{
				Org_WSAEventSelect( sClient, hWsaCloseRead, 0);
			}
			closesocket( sClient);
		}
		if( NULL != hWsaCloseRead)
		{
			CloseHandle( hWsaCloseRead);
		}
	}
#ifdef	_DEBUG
	OutputDebugString( "End-ReceiveDataThread\n");
#endif
	_endthread();
}

int SendRawData( const ENTRYDATA* pstEntry, LPCVOID lpcvSendData, int nSendDataSize, DWORD dwTimeOut, HANDLE hCancelEvent)
{
	int					nResult = _SEND_ERROR;

	SOCKET				sClient = INVALID_SOCKET;
	HANDLE				hWsaCnctRcv = NULL;
	struct sockaddr_in	stSkAddSrv;
	DWORD				dwResult;
	WSANETWORKEVENTS	stNetEvents;
	BYTE*				pabyData = NULL;
	SENDDATAHEADER*		pstSendRawData;
	int					nSize;
	HANDLE				ahEvents[ 3];
	DWORD				dwEventCount;

	if( g_stOrgData.blInitialized)
	{
		////////////////////////////////////////////////////////
		//
		// 初期化開始
		//
		sClient = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( INVALID_SOCKET == sClient)
		{
			goto cleanup;
		}
		hWsaCnctRcv = CreateEvent( NULL, FALSE, FALSE, NULL);
		if( NULL == hWsaCnctRcv)goto cleanup;

		ahEvents[ 0] = g_stOrgData.hStopEvent;
		ahEvents[ 1] = hWsaCnctRcv;
		ahEvents[ 2] = hCancelEvent;
		dwEventCount = ( NULL != hCancelEvent) ? 3 : 2;
		//
		// 初期化終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// コネクト開始
		//
		Org_WSAEventSelect( sClient, hWsaCnctRcv, FD_CONNECT | FD_CLOSE);
		CopyMemory( &stSkAddSrv, &( ( LPBYTE)pstEntry)[ sizeof( ENTRYDATA)], sizeof( struct sockaddr));
		if( SOCKET_ERROR == connect( sClient, ( struct sockaddr*)&stSkAddSrv, sizeof( struct sockaddr)))
		{
			if( WSAEWOULDBLOCK != WSAGetLastError())
			{
				goto cleanup;
			}
		}
		// コネクトの確認
		while( TRUE)
		{
			dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, dwTimeOut, FALSE);
			if( WSA_WAIT_TIMEOUT == dwResult)
			{
				nResult = _SEND_TIMEOUT;
				goto cleanup;
			}
			else
			{
				if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					nResult = _SEND_FORCEDEND;
					goto cleanup;
				}
				else
				{
					if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
					{
						ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
						Org_WSAEnumNetworkEvents( sClient, hWsaCnctRcv, &stNetEvents);
						if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;
						break;
					}
					else
					{
						if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
						{
							nResult = _SEND_CANCEL;
							goto cleanup;
						}
					}
				}
			}
		}
		//
		// コネクト終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 応答待ち
		//
		Org_WSAEventSelect( sClient, hWsaCnctRcv, FD_READ | FD_CLOSE);
		// 応答受信
		while( TRUE)
		{
			dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, dwTimeOut, FALSE);
			if( WSA_WAIT_TIMEOUT == dwResult)
			{
				nResult = _SEND_TIMEOUT;
				goto cleanup;
			}
			else
			{
				if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					nResult = _SEND_FORCEDEND;
					goto cleanup;
				}
				else
				{
					if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
					{
						ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
						Org_WSAEnumNetworkEvents( sClient, hWsaCnctRcv, &stNetEvents);
						if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;

						// 受信データサイズの取得
						ULONG	ulSize;
						ioctlsocket( sClient, FIONREAD, &ulSize);

						ulSize += 10;
						char*	lpszData = new char [ ulSize];
						ZeroMemory( lpszData, ulSize);
						ulSize = recv( sClient, lpszData, ulSize, 0);
						lpszData[ ulSize] = 0;
						if( lstrcmp( lpszData, "Hi,Oboe!"))
						{
							delete [] lpszData;
							goto cleanup;
						}
						delete [] lpszData;
						break;
					}
					else
					{
						if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
						{
							nResult = _SEND_CANCEL;
							goto cleanup;
						}
					}
				}
			}
		}
		//
		// 応答待ち
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 送信開始
		//
		nSize = sizeof( SENDDATAHEADER) + nSendDataSize;
		pabyData = new BYTE [ nSize];
		pstSendRawData = ( SENDDATAHEADER*)pabyData;
		pstSendRawData->nSize = nSize;
		CopyMemory( pstSendRawData->abyData, lpcvSendData, nSendDataSize);

		Org_WSAEventSelect( sClient, hWsaCnctRcv, FD_WRITE | FD_CLOSE);
		if( SOCKET_ERROR == send( sClient, ( LPCSTR)pstSendRawData, nSize, 0))
		{
			if( WSAEWOULDBLOCK != WSAGetLastError())
			{
				goto cleanup;
			}
		}
		// 送信完了の確認
		while( TRUE)
		{
			dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, dwTimeOut, FALSE);
			if( WSA_WAIT_TIMEOUT == dwResult)
			{
				nResult = _SEND_TIMEOUT;
				goto cleanup;
			}
			else
			{
				if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					nResult = _SEND_FORCEDEND;
					goto cleanup;
				}
				else
				{
					if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
					{
						ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
						Org_WSAEnumNetworkEvents( sClient, hWsaCnctRcv, &stNetEvents);
						if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;
						break;
					}
					else
					{
						if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
						{
							nResult = _SEND_CANCEL;
							goto cleanup;
						}
					}
				}
			}
		}
		//
		// 送信終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 送信結果待ち開始
		//
		Org_WSAEventSelect( sClient, hWsaCnctRcv, FD_READ | FD_CLOSE);
		// 応答受信
		while( TRUE)
		{
			dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, dwTimeOut, FALSE);
			if( WSA_WAIT_TIMEOUT == dwResult)
			{
				nResult = _SEND_TIMEOUT;
				goto cleanup;
			}
			else
			{
				if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					nResult = _SEND_FORCEDEND;
					goto cleanup;
				}
				else
				{
					if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
					{
						ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
						Org_WSAEnumNetworkEvents( sClient, hWsaCnctRcv, &stNetEvents);
						if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;

						// 受信データサイズの取得
						ULONG	ulSize;
						ioctlsocket( sClient, FIONREAD, &ulSize);

						ulSize += 10;
						char*	lpszData = new char [ ulSize];
						ZeroMemory( lpszData, ulSize);
						ulSize = recv( sClient, lpszData, ulSize, 0);
						lpszData[ ulSize] = 0;
						if( SOCKET_ERROR == ulSize)
						{
							delete [] lpszData;
							int nError = WSAGetLastError();
							if( WSAEWOULDBLOCK == nError)
							{
								Sleep( 100);
								continue;
							}
							goto cleanup;
						}
						if( sizeof( int) <= ulSize)
						{
							CopyMemory( &nResult, lpszData, sizeof( int));
						}
						delete [] lpszData;
						break;
					}
					else
					{
						if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
						{
							nResult = _SEND_CANCEL;
							goto cleanup;
						}
					}
				}
			}
		}
		//
		// 送信結果待ち開始
		//
		////////////////////////////////////////////////////////

cleanup:
		////////////////////////////////////////////////////////
		//
		// 終了処理開始
		//
		if( INVALID_SOCKET != sClient)
		{
			if( NULL != hWsaCnctRcv)
			{
				Org_WSAEventSelect( sClient, hWsaCnctRcv, 0);
			}
			closesocket( sClient);
		}
		if( NULL != hWsaCnctRcv)CloseHandle( hWsaCnctRcv);
		if( pabyData)delete [] pabyData;
		//
		// 終了処理終了
		//
		////////////////////////////////////////////////////////
	}

	return nResult;
}

void CleanupOrgData( void)
{
	g_stOrgData.blInitialized = FALSE;
	g_stOrgData.blActive = FALSE;
	g_stOrgData.blReadyReceive = FALSE;
	g_stOrgData.blReadySend = FALSE;
	if( g_stOrgData.hStopEvent)
	{
		CloseHandle( g_stOrgData.hStopEvent);
		g_stOrgData.hStopEvent = NULL;
	}
	g_stOrgData.ulRcvAddress = 0;
	g_stOrgData.ulMyAddress = 0;
	g_stOrgData.unMultiPort = 0;
	g_stOrgData.nTTL = 0;
	g_stOrgData.blAbsence = FALSE;
	g_stOrgData.lpfnRcvData = NULL;
	if( g_stOrgData.pcEntryList)
	{
		delete g_stOrgData.pcEntryList;
		g_stOrgData.pcEntryList = NULL;
	}
	if( g_stOrgData.pcGroupList)
	{
		delete g_stOrgData.pcGroupList;
		g_stOrgData.pcGroupList = NULL;
	}
	if( g_stOrgData.pszEntryName)
	{
		delete g_stOrgData.pszEntryName;
		g_stOrgData.pszEntryName = NULL;
	}
	if( g_stOrgData.pszNickname)
	{
		delete g_stOrgData.pszNickname;
		g_stOrgData.pszNickname = NULL;
	}
	if( g_stOrgData.pszGroup)
	{
		delete g_stOrgData.pszGroup;
		g_stOrgData.pszGroup = NULL;
	}
}

void OutputErrorString( LPCSTR lpcszErrMsg, LPCSTR lpcstrFunction)
{
	EnterCriticalSection( &g_csFileWrite);

	char	szPath[ _MAX_PATH];
	if( GetModuleFileName( ( HINSTANCE)g_hModule, szPath, _MAX_PATH))
	{
		char drive[ _MAX_DRIVE];
		char dir[ _MAX_DIR];
		char fname[ _MAX_FNAME];
		_splitpath( szPath, drive, dir, fname, NULL);
		_makepath( szPath, drive, dir, fname, "err");

		//HFILE		hFile;
		//OFSTRUCT	of;
		//hFile = OpenFile( szPath, &of, OF_CREATE | OF_WRITE | OF_PROMPT);
		HANDLE	hFile;
		hFile = CreateFile( szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
		if( INVALID_HANDLE_VALUE != hFile)
		{
			SYSTEMTIME stSysTime;
			GetLocalTime( &stSysTime);

			char	szWork[ 64];
			wsprintf( szWork, "[%02d/%02d %02d:%02d:%02d] - ", stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);

			char*	pszCRLF = "\r\n";
			char*	pszIN = " : ";
			DWORD	dwDummy;
			SetFilePointer( hFile, 0, NULL, FILE_END);
			WriteFile( hFile, szWork, lstrlen( szWork), &dwDummy, NULL);
			WriteFile( hFile, lpcszErrMsg, lstrlen( lpcszErrMsg), &dwDummy, NULL);
			WriteFile( hFile, pszIN, lstrlen( pszIN), &dwDummy, NULL);
			WriteFile( hFile, lpcstrFunction, lstrlen( lpcstrFunction), &dwDummy, NULL);
			WriteFile( hFile, pszCRLF, lstrlen( pszCRLF), &dwDummy, NULL);
			CloseHandle( hFile);
		}
	}

	LeaveCriticalSection( &g_csFileWrite);
}

BOOL GetNodeListPath( LPSTR lpcszNodeList)
{
	char*	pszAnony = "Anonymous";
	char	szUser[ UNLEN + 1];
	DWORD	dwSize;
	dwSize = UNLEN + 1;
	if( 0 >= GetUserName( szUser, &dwSize))
	{
		lstrcpy( szUser, pszAnony);
	}
	char	szProfilePath[ _MAX_PATH];
	char	szDrive[ _MAX_DRIVE];
	char	szDir[ _MAX_DIR];
	GetModuleFileName( NULL, szProfilePath, _MAX_PATH);
	_splitpath( szProfilePath, szDrive, szDir, NULL, NULL);
	_makepath( szProfilePath, szDrive, szDir, "StdNet", "ini");

	WIN32_FIND_DATA	stData;
	HANDLE			hFFF;

	char	szNodeListPath[ _MAX_PATH];
	// NoselistのPATH
	if( 0 < GetPrivateProfileString( szUser, "NodeListPath", "", szNodeListPath, _MAX_PATH, szProfilePath))
	{
		hFFF = FindFirstFile( szNodeListPath, &stData);
		if( INVALID_HANDLE_VALUE != hFFF)
		{
			FindClose( hFFF);
			lstrcpy( lpcszNodeList, szNodeListPath);
			return TRUE;
		}
	}

	char*			pszNodeListFname = "NodeList.ini";

	GetWindowsDirectory( szNodeListPath, MAX_PATH);
	if( '\\'!= szNodeListPath[ lstrlen( szNodeListPath) - 1])lstrcat( szNodeListPath, "\\");
	lstrcat( szNodeListPath, pszNodeListFname);
	hFFF = FindFirstFile( szNodeListPath, &stData);
	if( INVALID_HANDLE_VALUE != hFFF)
	{
		FindClose( hFFF);
		lstrcpy( lpcszNodeList, szNodeListPath);
		return TRUE;
	}

	GetSystemDirectory( szNodeListPath, MAX_PATH);
	if( '\\'!= szNodeListPath[ lstrlen( szNodeListPath) - 1])lstrcat( szNodeListPath, "\\");
	lstrcat( szNodeListPath, pszNodeListFname);
	hFFF = FindFirstFile( szNodeListPath, &stData);
	if( INVALID_HANDLE_VALUE != hFFF)
	{
		FindClose( hFFF);
		lstrcpy( lpcszNodeList, szNodeListPath);
		return TRUE;
	}

	if( 0 < SearchPath( NULL, pszNodeListFname, NULL, _MAX_PATH, lpcszNodeList, NULL))
	{
		return TRUE;
	}

	lstrcpy( lpcszNodeList, pszNodeListFname);
	return FALSE;
}

BOOL GetLocalGroup( CGroupList* pcLocalGroupList)
{
	if( NULL == pcLocalGroupList)return FALSE;

	char	szStrNodeListPath[ _MAX_PATH];
	if( GetNodeListPath( szStrNodeListPath))
	{
		char*	pcszGroup = "GROUP";
		char*	pcszCount = "COUNT";
		int nLocalGroupCount = GetPrivateProfileInt( pcszGroup, pcszCount, 0, szStrNodeListPath);
		if( 0 < nLocalGroupCount)
		{
			char	szEntry[ 32];
			char	szWork[ 256];
			char*	pszGroupName;
			char*	pszParent;
			int		nGroup;
			int*	pnGroupID = NULL;

			pnGroupID = new int [ nLocalGroupCount];
			if( pnGroupID)
			{
				FillMemory( pnGroupID, sizeof( int) * nLocalGroupCount, -1);
				for( int nIndex = 0; nIndex < nLocalGroupCount; nIndex++)
				{
					wsprintf( szEntry, "GROUP%d", nIndex + 1);
					GetPrivateProfileString( pcszGroup, szEntry, "error", szWork, 256, szStrNodeListPath);
					pszGroupName = strtok( szWork, ",");
					pszParent = strtok( NULL, ",");
					if( NULL != pszParent)
					{
						nGroup = atoi( pszParent);
						pnGroupID[ nIndex] = pcLocalGroupList->AddGroup( pszGroupName, ( 0 < nGroup) ? pnGroupID[ nGroup - 1] : -1);
					}
					else
					{
						pnGroupID[ nIndex] = pcLocalGroupList->AddGroup( pszGroupName, -1);
					}
				}
				delete [] pnGroupID;
				return FALSE;
			}
		}
	}
	return FALSE;
}

int GetLocalParentID( LPCSTR lpcszSeed, int* panParent, int nSize, int nOffset, CGroupList* pcLocalGroupList)
{
	int	nResult = 0;

	char	szStrNodeListPath[ _MAX_PATH];
	if( GetNodeListPath( szStrNodeListPath))
	{
		char	szData[ 1024];
		GetPrivateProfileString( "MACHINE", "DEFAULT", "", szData, 1024, szStrNodeListPath);
		GetPrivateProfileString( "MACHINE", lpcszSeed, szData, szData, 1024, szStrNodeListPath);
		if( 0 < lstrlen( szData))
		{
			LPSTR lpszName;
			LPSTR lpszGroup;
			lpszName = szData;
			for( int nIndex = 0; nIndex < lstrlen( szData); nIndex++)
			{
				if( ',' == szData[ nIndex])
				{
					szData[ nIndex] = 0;
					lpszGroup = &szData[ nIndex + 1];
				}
			}
			if( NULL != lpszGroup)
			{
				char*	pszParent = lpszGroup;
				int nGroupLen = lstrlen( lpszGroup) + 1;
				for( int nIndex = 0; nIndex < nGroupLen; nIndex++)
				{
					if( '&' == lpszGroup[ nIndex] || 0 == lpszGroup[ nIndex])
					{
						lpszGroup[ nIndex] = 0;
						if( 0 < lstrlen( pszParent))
						{
							int nGroup = atoi( pszParent);
							if( 0 < nGroup)
							{
								nGroup--;
								if( NULL != pcLocalGroupList->GetGroupName( nGroup))
								{
									if( NULL != panParent)
									{
										if( nSize > nResult)panParent[ nResult] = ( 0 >= nOffset) ? nGroup : nGroup + nOffset;
									}
									nResult++;
								}
							}
						}
						pszParent = &lpszGroup[ nIndex + 1];
					}
				}
			}
		}
	}
	return nResult;
}

int GetLocalName( LPCSTR lpcszSeed, char* pszLocalName, int nSize)
{
	int	nResult = 0;

	char	szStrNodeListPath[ _MAX_PATH];
	if( GetNodeListPath( szStrNodeListPath))
	{
		char	szData[ 1024];
		GetPrivateProfileString( "MACHINE", "DEFAULT", "", szData, 1024, szStrNodeListPath);
		GetPrivateProfileString( "MACHINE", lpcszSeed, szData, szData, 1024, szStrNodeListPath);
		if( 0 < lstrlen( szData))
		{
			for( int nIndex = 0; nIndex < lstrlen( szData); nIndex++)
			{
				if( ',' == szData[ nIndex])
				{
					szData[ nIndex] = 0;
					break;
				}
			}
			nResult = lstrlen( szData);
			if( NULL != pszLocalName && nSize > nResult)
			{
				lstrcpy( pszLocalName, szData);
			}
		}
	}
	return nResult;
}

// エントリのグループ化処理用データの取得
int GetSeed( int nIndex, char* pszSeed, int nSize)
{
	if( 0 > nIndex)return -1;

	int	nResult = -1;

	if( g_stOrgData.blInitialized)
	{
		if( g_stOrgData.pcEntryList->GetCount() >= nIndex)
		{
			int	nDataSize;
			switch( g_stOrgData.nSeedType)
			{
			case 0:	// NicName
			default:
				nDataSize = g_stOrgData.pcEntryList->GetNickName( nIndex, NULL, 0) + 1;
				break;
			case 1:	// UserName
				nDataSize = g_stOrgData.pcEntryList->GetUserName( nIndex, NULL, 0) + 1;
				break;
			case 2:	// MachineName
				nDataSize = g_stOrgData.pcEntryList->GetComputerName( nIndex, NULL, 0) + 1;
				break;
			case 3:	// IP Address
				nDataSize = g_stOrgData.pcEntryList->GetIpAddr( nIndex, NULL, 0) + 1;
				break;
			}
			nResult = nDataSize;

			if( NULL != pszSeed && nDataSize <= nSize)
			{
				switch( g_stOrgData.nSeedType)
				{
				case 0:	// NicName
				default:
					g_stOrgData.pcEntryList->GetNickName( nIndex, pszSeed, nSize);
					break;
				case 1:	// UserName
					g_stOrgData.pcEntryList->GetUserName( nIndex, pszSeed, nSize);
					break;
				case 2:	// MachineName
					g_stOrgData.pcEntryList->GetComputerName( nIndex, pszSeed, nSize);
					break;
				case 3:	// IP Address
					g_stOrgData.pcEntryList->GetIpAddr( nIndex, pszSeed, nSize);
					break;
				}
			}
		}
	}
	return nResult;
}
