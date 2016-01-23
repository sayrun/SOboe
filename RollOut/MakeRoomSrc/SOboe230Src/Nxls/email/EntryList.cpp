#include "StdeMail.h"
#include "NetEx_IF.h"
#include "emailslv.h"
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

BOOL CEntryList::AddEntry( LPCSTR lpcszAddress, int nGroupID)
{
	ENTRYLISTNODE*	pstEntryListNode;

	int	nSize = sizeof( ENTRYLISTNODE) + lstrlen( lpcszAddress) + 1;
	pstEntryListNode = ( ENTRYLISTNODE*)malloc( nSize);
	if( NULL == pstEntryListNode)return FALSE;

	lstrcpy( pstEntryListNode->szEntryName, lpcszAddress);
	pstEntryListNode->nstEntryDataSize = nSize;
	pstEntryListNode->nGroupID = nGroupID;
	pstEntryListNode->pNext = NULL;

	if( NULL == pTop)
	{
		pTop = pstEntryListNode;
	}
	else
	{
		////////////////////////////////////////////////////////
		//
		// 一致検査と更新の開始
		//
		ENTRYLISTNODE*	pstNode;
		pstNode = pTop;
		do
		{
			if( !lstrcmp( pstNode->szEntryName, lpcszAddress))
			{
				if( pstNode->nGroupID == nGroupID)
				{
					pstEntryListNode->pNext = pstNode->pNext;
					free( pstNode);
					pstNode = pstEntryListNode;
					return TRUE;
				}
			}
			pstNode = pstNode->pNext;
		}while( pstNode);
/*
		do
		{
			if( !lstrcmp( ( *ppstNode)->szEntryName, lpcszAddress))
			{
				if( ( *ppstNode)->nGroupID == nGroupID)
				{
					pstEntryListNode->pNext = ( *ppstNode)->pNext;
					free( *ppstNode);
					( *ppstNode) = pstEntryListNode;
					return TRUE;
				}
			}
			ppstNode = &( ( *ppstNode)->pNext);
		}while( ( *ppstNode));
		*/
		//
		// 一致検査と更新の終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 新規追加の開始
		//
		pstNode = pTop;

		while( pstNode->pNext)
		{
			pstNode = pstNode->pNext;
		}

		pstNode->pNext = pstEntryListNode;
		//
		// 新規追加の終了
		//
		////////////////////////////////////////////////////////
	}

	nEntryCount++;

	return TRUE;
}

BOOL CEntryList::RemoveEntry( LPCSTR lpcszAddress)
{
	ENTRYLISTNODE*	pstNode;
	ENTRYLISTNODE**	ppstPrev;

	pstNode = pTop;
	ppstPrev = &pTop;
	for( int nPos = 0; nPos < nEntryCount; nPos++)
	{
		if( !lstrcmp( pstNode->szEntryName, lpcszAddress))
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

int CEntryList::GetCount( void)
{
	return nEntryCount;
}

int CEntryList::GetEntryAdd( int nIndex, LPSTR lpszEntryAdd, int nLength)
{
	return GetEntryName( nIndex, lpszEntryAdd, nLength);
}

int CEntryList::GetEntryName( int nIndex, LPSTR lpszEntryName, int nLength)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}

	if( NULL == lpszEntryName || 0 >= nLength)
	{
		return lstrlen( pstNode->szEntryName);
	}

	lstrcpyn( lpszEntryName, pstNode->szEntryName, nLength);

	return lstrlen( lpszEntryName);
}

int CEntryList::GetParentGroup( int nIndex)
{
	ENTRYLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)return -1;
	}
	return pstNode->nGroupID;
}
