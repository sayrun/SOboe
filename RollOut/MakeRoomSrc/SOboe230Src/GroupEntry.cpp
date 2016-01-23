#include "stdafx.h"
#include "SOboe.h"
#include "GroupEntry.h"

BOOL CGroupData::SetGroupData( const GROUPDATA* pstGroupData, int nSize)
{
	if( NULL == pstGroupData)return FALSE;
	if( nSize < pstGroupData->nSize)return FALSE;
	if( _GROUPSTRUCT_VER100 < pstGroupData->unStructVersion)return FALSE;

	m_nParentGroup = pstGroupData->nParentGroup;
	m_nGroupStatus = pstGroupData->nGroupStatus;
	m_cStrGroupName = &( ( LPCSTR)pstGroupData)[ pstGroupData->nOffsetGroupName];

	if( m_cStrGroupName.IsEmpty())
	{
		m_nParentGroup = -1;
		return FALSE;
	}
	return TRUE;
}

BOOL CEntryData::SetEntryData( const ENTRYDATA* pstEntryData, int nSize)
{
	if( NULL == pstEntryData)return FALSE;
	if( nSize < pstEntryData->nSize)return FALSE;
	if( _ENTRYSTRUCT_VER100 < pstEntryData->unStructVersion)return FALSE;

	if( m_pstEntryData)
	{
		free( m_pstEntryData);
		m_pstEntryData = NULL;
		m_nEntryDataSize = 0;
	}

	m_pstEntryData = ( ENTRYDATA*)malloc( nSize);
	if( m_pstEntryData)
	{
		m_nEntryDataSize = nSize;
		CopyMemory( m_pstEntryData, pstEntryData, nSize);
		return TRUE;
	}
	return FALSE;
}

BOOL CEntryData::operator==( const CEntryData& cEntryData) const
{
	if( NULL == m_pstEntryData && NULL == cEntryData.m_pstEntryData)return TRUE;
	if( NULL == m_pstEntryData)return FALSE;
	if( NULL == cEntryData.m_pstEntryData)return FALSE;
//	if( m_pstEntryData->nSize != cEntryData.m_pstEntryData->nSize)return FALSE;
	if( GetNxlID() != cEntryData.GetNxlID())return FALSE;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	CNetExLib*	pcNetExLib;
	pcNetExLib = pcSOboe->GetNetExLib( pcSOboe->FindNxlID( GetNxlID()));
	if( pcNetExLib)
	{
		return pcNetExLib->CompareEntryData( *this, cEntryData);
	}
	return FALSE;
}

const CEntryData& CEntryData::operator=( const CEntryData& cEntryData)
{
	if( m_pstEntryData)
	{
		free( m_pstEntryData);
		m_pstEntryData = NULL;
		m_nEntryDataSize = 0;
	}

	if( NULL != cEntryData.m_pstEntryData)
	{
		if( 0 < cEntryData.m_nEntryDataSize)
		{
			m_pstEntryData = ( ENTRYDATA*)malloc( cEntryData.m_nEntryDataSize);
			if( m_pstEntryData)
			{
				m_nEntryDataSize = cEntryData.m_nEntryDataSize;
				CopyMemory( m_pstEntryData, cEntryData.m_pstEntryData, cEntryData.m_nEntryDataSize);
			}
		}
	}
	return *this;
}
