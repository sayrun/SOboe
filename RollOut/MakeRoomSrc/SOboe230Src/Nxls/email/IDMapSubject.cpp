#include "StdeMail.h"
#include "IDMapSubject.h"

CIDMapSubject::CIDMapSubject( int nMaxCount)
{
	m_nMaxCount = nMaxCount;
	m_nNodeCount = 0;
	m_pTop = NULL;
}

CIDMapSubject::~CIDMapSubject( void)
{
	NODE*	pNode = m_pTop;
	NODE*	pNodeTemp;
	while( pNode)
	{
		pNodeTemp = pNode->pNext;
		free( pNode);
		pNode = pNodeTemp;
	}
}

BOOL CIDMapSubject::AddSubject( LPCSTR lpcszID, LPCSTR lpcszSubject)
{
	if( NULL == lpcszID)return FALSE;
	if( NULL == lpcszSubject)return FALSE;

	BOOL	blResult = FALSE;
	NODE*	pNode;
	int	nSize = sizeof( NODE) + ( lstrlen( lpcszID) + 1) + ( lstrlen( lpcszSubject) + 1);
	NODE*	pNewNode = ( NODE*)malloc( nSize);
	if( pNewNode)
	{
		int	nOffset = sizeof( NODE);

		pNewNode->nSize = nSize;
		pNewNode->nIDOffset = nOffset;
		lstrcpy( &( ( char*)pNewNode)[ pNewNode->nIDOffset], lpcszID);
		nOffset += ( lstrlen( lpcszID) + 1);
		pNewNode->nSubjectOffset = nOffset;
		lstrcpy( &( ( char*)pNewNode)[ pNewNode->nSubjectOffset], lpcszSubject);
		nOffset += ( lstrlen( lpcszSubject) + 1);
		pNewNode->pNext = NULL;

		if( NULL == m_pTop)
		{
			m_pTop = pNewNode;
		}
		else
		{
			pNode = m_pTop;
			while( pNode->pNext)pNode = pNode->pNext;
			pNode->pNext = pNewNode;
		}
		blResult = TRUE;

		m_nNodeCount++;
		if( m_nNodeCount >= m_nMaxCount)
		{
			int nDiff = m_nNodeCount - m_nMaxCount;
			pNode = m_pTop;
			for( int nIndex = 0; nIndex < nDiff; nIndex++)
			{
				m_pTop = pNode->pNext;
				free( pNode);
				m_nNodeCount--;
				if( NULL == m_pTop)break;
			}
		}
	}

	return blResult;
}

int CIDMapSubject::GetCount( void) const
{
	return m_nNodeCount;
}

LPCSTR CIDMapSubject::GetID( int nIndex) const
{
	if( nIndex >= m_nNodeCount)return NULL;
	if( 0 >= m_nNodeCount)return NULL;

	NODE*	pNode = m_pTop;
	for( int nIndexW = 0; nIndexW < nIndex; nIndexW++)
	{
		pNode = pNode->pNext;
		if( NULL == pNode)return NULL;
	}

	if( ( int)sizeof( NODE) > pNode->nIDOffset)return NULL;

	return &( ( char*)pNode)[ pNode->nIDOffset];
}

LPCSTR CIDMapSubject::GetSubject( int nIndex) const
{
	if( nIndex >= m_nNodeCount)return NULL;
	if( 0 >= m_nNodeCount)return NULL;

	NODE*	pNode = m_pTop;
	for( int nIndexW = 0; nIndexW < nIndex; nIndexW++)
	{
		pNode = pNode->pNext;
		if( NULL == pNode)return NULL;
	}

	if( ( int)sizeof( NODE) > pNode->nSubjectOffset)return NULL;

	return &( ( char*)pNode)[ pNode->nSubjectOffset];
}
