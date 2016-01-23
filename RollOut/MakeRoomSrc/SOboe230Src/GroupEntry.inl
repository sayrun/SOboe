
inline CGroupData::CGroupData()
{
	m_nParentGroup = -1;
	m_cStrGroupName.Empty();
}

inline CGroupData::~CGroupData()
{
}

inline const CString& CGroupData::GetGroupName( void) const
{
	return m_cStrGroupName;
}

inline const int CGroupData::GetParentGroup( void) const
{
	return m_nParentGroup;
}

inline const int CGroupData::GetStatus( void) const
{
	return m_nGroupStatus;
}

inline CEntryData::CEntryData()
{
	m_pstEntryData = NULL;
	m_nEntryDataSize = 0;
}

inline CEntryData::~CEntryData()
{
	if( m_pstEntryData)
	{
		free( m_pstEntryData);
		m_pstEntryData = NULL;
	}
}


inline int CEntryData::GetEntryData( ENTRYDATA* pstEntryData, int nSize) const
{
	if( NULL == m_pstEntryData)return -1;
	if( NULL != pstEntryData && m_nEntryDataSize <= nSize)
	{
		CopyMemory( pstEntryData, m_pstEntryData, m_nEntryDataSize);
	}
	return m_nEntryDataSize;
}

inline int CEntryData::GetEntryDataSize( void) const
{
	return m_nEntryDataSize;
}


inline const ENTRYDATA* CEntryData::AccessEntryData() const
{
	return m_pstEntryData;
}

inline int CEntryData::GetStatus( void) const
{
	if( NULL == m_pstEntryData)return 0;
	return m_pstEntryData->nEntryStat;
}

inline int CEntryData::GetParentCount( void) const
{
	if( NULL == m_pstEntryData)return 0;
	return m_pstEntryData->nParentGroupCount;
}

inline int CEntryData::GetParent( int nIndex) const
{
	if( NULL == m_pstEntryData)return -1;
	if( nIndex >= m_pstEntryData->nParentGroupCount)return -1;
	if( nIndex < 0)return -1;

	int*	lpnData = ( int*)&( ( LPBYTE)m_pstEntryData)[ m_pstEntryData->nOffsetParentGroup];
	return lpnData[ nIndex];
}

inline CString CEntryData::GetEntryName( void) const
{
	CString	cStr;

	if( NULL != m_pstEntryData)
	{
		cStr = ( LPCSTR)&( ( LPBYTE)m_pstEntryData)[ m_pstEntryData->nOffsetEntryName];
	}

	return cStr;
}

inline const UINT CEntryData::GetNxlID( void) const
{
	if( NULL == m_pstEntryData)return 0;
	return m_pstEntryData->unNxlID;
}
