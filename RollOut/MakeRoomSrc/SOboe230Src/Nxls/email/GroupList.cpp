#include "StdeMail.h"
#include "GroupList.h"

CGroupList::CGroupList()
{
	pTop		= NULL;
	nGroupCount	= 0;
	nGroupID	= 0;
}

CGroupList::~CGroupList()
{
	RemoveAll();
}

int CGroupList::AddGroup( LPCSTR lpcszGroup, int nParentGroupID)
{
	if( 0 >= lstrlen( lpcszGroup))return -1;

	if( NULL == pTop)
	{
#ifdef	_DEBUG
		OutputDebugString( "New-Group-");
		OutputDebugString( lpcszGroup);
		OutputDebugString( "\n");
#endif
		int	nSize = sizeof( GROUPLISTNODE) + lstrlen( lpcszGroup) + 1;
		GROUPLISTNODE*	pGroup = ( GROUPLISTNODE*)malloc( nSize);
		if( pGroup)
		{
			pGroup->nstGroupDataSize	= nSize;
			pGroup->pNext = NULL;
			pGroup->nID = nGroupID;
			nGroupID++;
			lstrcpy( pGroup->szGroupName, lpcszGroup);
			pTop = pGroup;

			nGroupCount++;
			return pGroup->nID;
		}
	}
	else
	{
		////////////////////////////////////////////////////////
		//
		// ’Ç‰Á‚ÌŠJŽn
		//
		GROUPLISTNODE*	pstNode;
		GROUPLISTNODE*	pstNodeNext;
		pstNodeNext = pTop;

		do
		{
			pstNode = pstNodeNext;
#ifdef	_DEBUG
			OutputDebugString( "Entry1:");
			OutputDebugString( pstNode->szGroupName);
			OutputDebugString( "\n");
			OutputDebugString( "Entry2:");
			OutputDebugString( lpcszGroup);
			OutputDebugString( "\n");
#endif
			if( !lstrcmp( pstNode->szGroupName, lpcszGroup))
			{
				return pstNode->nID;
			}
			pstNodeNext = pstNode->pNext;
		}while( pstNodeNext);

#ifdef	_DEBUG
		OutputDebugString( "Add-Entry:");
		OutputDebugString( lpcszGroup);
		OutputDebugString( "\n");
#endif
		int	nSize = sizeof( GROUPLISTNODE) + lstrlen( lpcszGroup) + 1;
		GROUPLISTNODE*	pGroup = ( GROUPLISTNODE*)malloc( nSize);
		if( pGroup)
		{
			pGroup->nstGroupDataSize	= nSize;
			pGroup->pNext = NULL;
			pGroup->nParentGroupID = nParentGroupID;
			pGroup->nID = nGroupID;
			nGroupID++;
			lstrcpy( pGroup->szGroupName, lpcszGroup);
			pstNode->pNext = pGroup;

			nGroupCount++;
			return pGroup->nID;
		}
		//
		// ’Ç‰Á‚ÌI—¹
		//
		////////////////////////////////////////////////////////
	}

	return -1;
}

int CGroupList::FindGroup( int nID)
{
	if( 0 > nID)return -1;

	int	nIndex = -1;

	GROUPLISTNODE*	pstNode;
	pstNode = pTop;

	while( pstNode)
	{
		nIndex++;
		if( pstNode->nID == nID)
		{
			return nIndex;
		}
		pstNode = pstNode->pNext;
	}

	return nIndex;
}


BOOL CGroupList::RemoveAll()
{
	GROUPLISTNODE*	pstNode;
	GROUPLISTNODE*	pstNodeNext;
	pstNode = pTop;

	while( pstNode)
	{
		pstNodeNext = pstNode->pNext;
		free( pstNode);
		pstNode = pstNodeNext;
	}
	pTop = NULL;
	nGroupCount = 0;
	nGroupID = 0;

	return TRUE;
}

CGroupList::GetCount() const
{
	return nGroupCount;
}

const char* CGroupList::GetGroupName( int nIndex) const
{
	GROUPLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}

	if( pstNode)
	{
		return pstNode->szGroupName;
	}
	return NULL;
}

int CGroupList::GetParentGroupID( int nIndex) const
{
	GROUPLISTNODE*	pstNode;
	pstNode = pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}
	return pstNode->nParentGroupID;
}
