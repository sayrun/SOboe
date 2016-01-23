#include <windows.h>
#include <process.h>
#include "NetEx_IF.h"
#include "StdNetSlv.h"
#include "EntryList.h"

CEntryList::CEntryList()
{
	pTop = NULL;
	nEntryCount = 0;
	hLock = CreateSemaphore( NULL, 1, 1, NULL);
}

CEntryList::~CEntryList()
{
	CloseHandle( hLock);
	RemoveAll();
}

void CEntryList::RemoveAll( void)
{
	ENTRYLISTNODE*	pstNode;
	ENTRYLISTNODE*	pstNodeNext;
	pstNode = pTop;

	while( pstNode)
	{
		pstNodeNext = pstNode->pNext;
		free( pstNode);
		pstNode = pstNodeNext;
	}
	pTop = NULL;
	nEntryCount = 0;
}

BOOL CEntryList::LockList( DWORD dwMilliseconds)
{
	if( WAIT_OBJECT_0 == WaitForSingleObject( hLock, dwMilliseconds))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CEntryList::UnlockList()
{
	ReleaseSemaphore( hLock, 1, NULL); 
	return TRUE;
}

BOOL CEntryList::AddEntry( struct sockaddr* pstSockAdd, ENTRYPACKET* const cpEntryPacket, int* panGroupID, int nGroupCount)
{
	int	nSize = sizeof( ENTRYLISTNODE) + cpEntryPacket->nSize + ( sizeof( int) * nGroupCount) + 1;	// +1に意味はない
	BYTE*	pabyData = ( BYTE*)malloc( nSize);
	if( NULL == pabyData)return FALSE;

	ENTRYLISTNODE*	pstEntryListNode;
	pstEntryListNode = ( ENTRYLISTNODE*)pabyData;

	pstEntryListNode->nstEntryDataSize = nSize;

	CopyMemory( &pstEntryListNode->stSockAdd, pstSockAdd, sizeof( struct sockaddr));
	int		nOffset = sizeof( ENTRYLISTNODE);
	pstEntryListNode->nGroupCount = nGroupCount;
	if( 0 < nGroupCount)
	{
		pstEntryListNode->nGroupOffset = nOffset;
		CopyMemory( &pabyData[ pstEntryListNode->nGroupOffset], panGroupID, sizeof( int) * nGroupCount);
		nOffset += sizeof( int) * nGroupCount;
	}
	else
	{
		pstEntryListNode->nGroupOffset = 0;
	}
	pstEntryListNode->nEntryPacketOffset = nOffset;
	CopyMemory( &pabyData[ pstEntryListNode->nEntryPacketOffset], cpEntryPacket, cpEntryPacket->nSize);
	nOffset += cpEntryPacket->nSize;
	pstEntryListNode->pNext = NULL;

	if( NULL == pTop)
	{
#ifdef	_DEBUG
		OutputDebugString( "New-Entry\n");
#endif
		pTop = pstEntryListNode;
	}
	else
	{
		////////////////////////////////////////////////////////
		//
		// 一致検査と更新の開始
		//
		ENTRYLISTNODE**	ppstNode;
		ppstNode = &pTop;

		struct sockaddr_in*	pstAddIn;
		struct sockaddr_in*	pstAddInNew = ( struct sockaddr_in*)pstSockAdd;
		do
		{
			pstAddIn = ( struct sockaddr_in*)&( *ppstNode)->stSockAdd;
			if( pstAddIn->sin_addr.S_un.S_addr == pstAddInNew->sin_addr.S_un.S_addr)
			{
				pstEntryListNode->pNext = ( *ppstNode)->pNext;
				free( *ppstNode);
				( *ppstNode) = pstEntryListNode;
#ifdef	_DEBUG
				OutputDebugString( "Modify-Entry\n");
#endif
				return TRUE;
			}
			ppstNode = &( ( *ppstNode)->pNext);
		}while( ( *ppstNode));
		//
		// 一致検査と更新の終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 新規追加の開始
		//
		ENTRYLISTNODE*	pstNode;
		pstNode = pTop;

		while( pstNode->pNext)
		{
			pstNode = pstNode->pNext;
		}

		pstNode->pNext = pstEntryListNode;
#ifdef	_DEBUG
		OutputDebugString( "Add-Entry\n");
#endif
		//
		// 新規追加の終了
		//
		////////////////////////////////////////////////////////
	}

	nEntryCount++;

	return TRUE;
}

BOOL CEntryList::DelEntry( struct sockaddr* pstSockAdd, LPCSTR lpszEntryName)
{
	ENTRYLISTNODE*	pstNode;
	ENTRYLISTNODE**	ppstPrev;

	pstNode = pTop;
	ppstPrev = &pTop;
	for( int nPos = 0; nPos < nEntryCount; nPos++)
	{
		if( ( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_addr == ( ( struct sockaddr_in*)pstSockAdd)->sin_addr.S_un.S_addr)
		{
#ifdef	_DEBUG
			OutputDebugString( "Del-Entry\n");
#endif
			*ppstPrev = pstNode->pNext;
			free(  pstNode);
			nEntryCount--;
			return TRUE;
		}
		ppstPrev = &(pstNode->pNext);
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return FALSE;
	}
	return FALSE;
}

int CEntryList::GetCount( void)
{
	return nEntryCount;
}

int CEntryList::GetEntryAdd( int nIndex, struct sockaddr* pstSockAdd, int nSize)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return FALSE;
	}

	if( NULL == pstSockAdd || 0 >= nSize)
	{
		return sizeof( struct sockaddr);
	}

	int nCopySize = min( sizeof( struct sockaddr), nSize);
	CopyMemory( pstSockAdd, &pstNode->stSockAdd, nCopySize);

	return nCopySize;
}

int CEntryList::GetEntryName( int nIndex, LPSTR lpszEntryName, int nLength, LPCSTR lpcszAlias)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	LPCSTR	lpcsz;
	if( NULL == lpcszAlias)
	{
		BYTE*	pabyNode = ( BYTE*)pstNode;
		ENTRYPACKET*	pEntryPacket = (ENTRYPACKET*)&( ( ( BYTE*)pabyNode)[ pstNode->nEntryPacketOffset]);
		lpcsz = ( char*)&( ( BYTE*)pEntryPacket)[ pEntryPacket->nszNickname];
	}
	else
	{
		lpcsz = lpcszAlias;
	}
	int nWorkLength = lstrlen( lpcsz) + 18;	// " (192.168.255.255)"
	if( NULL == lpszEntryName || nWorkLength >= nLength)
	{
		return nWorkLength;
	}

	wsprintf( lpszEntryName, "%s (%d.%d.%d.%d)", lpcsz,
												( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b1,
												( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b2,
												( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b3,
												( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b4);
/*
	if( NULL == lpszEntryName || 0 >= nLength)
	{
		return lstrlen( &( pstNode->achEntryPacket[ ( ( ENTRYPACKET*)pstNode->achEntryPacket)->nszNickname]));
	}

	lstrcpyn( lpszEntryName, &( pstNode->achEntryPacket[ ( ( ENTRYPACKET*)pstNode->achEntryPacket)->nszNickname]), nLength);
*/

	return lstrlen( lpszEntryName);
}

int CEntryList::GetParentID( int nIndex, int* panParentIDs, int nSize)
{
	int	nParentNum = -1;

	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}

	if( NULL != pstNode)
	{
		int nCopyCount = min( pstNode->nGroupCount, nSize);
		if( 0 < nCopyCount && NULL != panParentIDs)
		{
			CopyMemory( panParentIDs, ( int*)&( ( BYTE*)pstNode)[ pstNode->nGroupOffset], sizeof( int) * nCopyCount);
		}
	}

	return nParentNum;
}

int CEntryList::GetParentIDCount( int nIndex)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}

	return ( NULL != pstNode) ? pstNode->nGroupCount : 0;
}

int CEntryList::GetEntryStatus( int nIndex)
{
	int	nStatus = 0;

	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}

	if( NULL != pstNode)
	{
		const ENTRYPACKET* pstPacket;
		pstPacket = ( const ENTRYPACKET*)&( ( BYTE*)pstNode)[ pstNode->nEntryPacketOffset];
		nStatus = ( FALSE == pstPacket->blAbsence) ? _STATUS_NORMAL : _STATUS_ABSENCE;
	}

	return nStatus;
}

int CEntryList::GetUserName( int nIndex, LPSTR lpUserName, int nSize)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	const ENTRYPACKET* pstPacket;
	pstPacket = ( const ENTRYPACKET*)&( ( BYTE*)pstNode)[ pstNode->nEntryPacketOffset];
	int nLen = lstrlen( ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszLogonID]);
	if( NULL == lpUserName || nLen >= nSize)return nLen;

	lstrcpyn( lpUserName, ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszLogonID], nSize);

	return lstrlen( lpUserName);
}

int CEntryList::GetComputerName( int nIndex, LPSTR lpComputerName, int nSize)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	const ENTRYPACKET* pstPacket;
	pstPacket = ( const ENTRYPACKET*)&( ( BYTE*)pstNode)[ pstNode->nEntryPacketOffset];

	int nLen = lstrlen( ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszMachineName]);
	if( NULL == lpComputerName || nLen >= nSize)return nLen;

	lstrcpyn( lpComputerName, ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszMachineName], nSize);

	return lstrlen( lpComputerName);
}

int	CEntryList::GetNickName(int nIndex, LPSTR lpNicName, int nSize)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	const ENTRYPACKET* pstPacket;
	pstPacket = ( const ENTRYPACKET*)&( ( BYTE*)pstNode)[ pstNode->nEntryPacketOffset];

	int nLen = lstrlen( ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszNickname]);
	if( NULL == lpNicName || nLen >= nSize)return nLen;

	lstrcpyn( lpNicName, ( char*)&( ( BYTE*)pstPacket)[ pstPacket->nszNickname], nSize);

	return lstrlen( lpNicName);
}

int	CEntryList::GetIpAddr(int nIndex, LPSTR lpIpAddress, int nSize)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	int nLen = 16;	// lstrlen( "255.255.255.255") + 1;
	if( NULL == lpIpAddress || nLen >= nSize)return nLen;

	wsprintf( lpIpAddress, "%d.%d.%d.%d", ( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b1,
											( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b2,
											( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b3,
											( ( struct sockaddr_in*)&pstNode->stSockAdd)->sin_addr.S_un.S_un_b.s_b4);

	return lstrlen( lpIpAddress);
}

