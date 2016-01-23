#include <windows.h>
#include "GroupList.h"

CGroupList::CGroupList()
{
	m_pTop		= NULL;
	m_nGroupCount	= 0;
	m_nGroupID	= 0;
}

CGroupList::~CGroupList()
{
	RemoveAll();
}

int CGroupList::CheckGroupCount( LPCSTR lpcszGroupn)
{
	if( NULL == lpcszGroupn)return 0;

	int nResult = 0;

	if( 0 < lstrlen( lpcszGroupn))
	{
		int nIndex = 0;
		while( TRUE)
		{
			if( '&' == lpcszGroupn[ nIndex])
			{
				if( '&' == lpcszGroupn[ nIndex + 1] || '>' == lpcszGroupn[ nIndex + 1])
				{
					nIndex += 2;
					continue;
				}
				if( 0 != lpcszGroupn[ nIndex + 1])
				{
					nResult++;
				}
			}
			if( 0 == lpcszGroupn[ nIndex])
			{
				nResult++;
				break;
			}
			nIndex++;
		}
	}

	return nResult;
}

int CGroupList::AddGroup( LPCSTR lpcszGroupn, int* panGroupIDs, int nSize)
{
	if( NULL == lpcszGroupn)return 0;

	int nGroupCount = CheckGroupCount( lpcszGroupn);
	if( 0 < nGroupCount)
	{
		int nLength = lstrlen( lpcszGroupn);
		char*	pszWork;
		pszWork = new char [ nLength + 1];
		if( pszWork)
		{
			int	nParentID = -1;
			int nWorkIndex = 0;
			int nGroupIdIndex = 0;
			for( int nIndex = 0; nIndex <= nLength; nIndex++)
			{
				pszWork[ nWorkIndex] = lpcszGroupn[ nIndex];
				if( '&' == lpcszGroupn[ nIndex])
				{
					if( '>' == lpcszGroupn[ nIndex + 1] || '&' == lpcszGroupn[ nIndex + 1])
					{
						pszWork[ nWorkIndex] = lpcszGroupn[ nIndex + 1];
						nWorkIndex++;
						nIndex++;
						continue;
					}
					else
					{
						pszWork[ nWorkIndex] = 0;
					}
				}
				else
				{
					if( '>' == lpcszGroupn[ nIndex])
					{
						pszWork[ nWorkIndex] = 0;
					}
				}
				if( 0 == pszWork[ nWorkIndex])
				{
					nWorkIndex = 0;
					nParentID = AddGroup( pszWork, nParentID);
					if( 0 == lpcszGroupn[ nIndex])
					{
						panGroupIDs[ nGroupIdIndex] = nParentID;
						nGroupIdIndex++;
					}
					if( '&' == lpcszGroupn[ nIndex] && '&' != lpcszGroupn[ nIndex + 1])
					{
						if( NULL != panGroupIDs && nSize > nGroupIdIndex)
						{
							panGroupIDs[ nGroupIdIndex] = nParentID;
							nGroupIdIndex++;
						}
						nParentID = -1;
					}
				}
				else
				{
					nWorkIndex++;
				}
			}
		}
	}
	return nGroupCount;
}

int CGroupList::AddGroup( LPCSTR lpcszGroup, int nParentID)
{
	if( NULL == m_pTop)
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
			pGroup->nID = m_nGroupID;
			pGroup->nParentID = ( 0 <= nParentID && m_nGroupID > nParentID) ? nParentID : -1;
			m_nGroupID++;
			lstrcpy( pGroup->szGroupName, lpcszGroup);
			m_pTop = pGroup;

			m_nGroupCount++;
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
		pstNodeNext = m_pTop;

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
				if( pstNode->nParentID == nParentID)
				{
					return pstNode->nID;
				}
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
			pGroup->nID = m_nGroupID;
			pGroup->nParentID = ( 0 <= nParentID && m_nGroupID > nParentID) ? nParentID : -1;
			m_nGroupID++;
			lstrcpy( pGroup->szGroupName, lpcszGroup);
			pstNode->pNext = pGroup;

			m_nGroupCount++;
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
	pstNode = m_pTop;

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
	pstNode = m_pTop;

	while( pstNode)
	{
		pstNodeNext = pstNode->pNext;
		free( pstNode);
		pstNode = pstNodeNext;
	}
	m_pTop = NULL;
	m_nGroupCount = 0;
	m_nGroupID = 0;

	return TRUE;
}

CGroupList::GetCount() const
{
	return m_nGroupCount;
}

const char* CGroupList::GetGroupName( int nIndex) const
{
	GROUPLISTNODE*	pstNode;
	pstNode = m_pTop;
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

int CGroupList::GetParentGroup( int nIndex) const
{
	GROUPLISTNODE*	pstNode;
	pstNode = m_pTop;
	for( int nPos = 0; nPos < nIndex; nPos++)
	{
		pstNode = pstNode->pNext;
		if( NULL == pstNode)break;
	}

	if( pstNode)
	{
		return pstNode->nParentID;
	}
	return -1;
}
