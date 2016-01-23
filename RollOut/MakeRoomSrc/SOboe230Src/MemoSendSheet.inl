
inline int CMemoSendSheet::GetEntryCount( void)
{
	return m_cSendEditPage.GetEntryCount();
}

inline const CEntryData* CMemoSendSheet::GetEntryData( int nIndex)
{
	return m_cSendEditPage.GetEntryData( nIndex);
}

inline BOOL CMemoSendSheet::AddEntryData( const CEntryData* pcEntryData)
{
	return m_cSendEditPage.AddEntryData( pcEntryData);
}

inline BOOL CMemoSendSheet::IsDistributeEmpty( void)
{
	return ( 0 >= m_cSendEditPage.GetEntryCount());
}

inline BOOL CMemoSendSheet::IsDeleteAfterSend( void)
{
	return m_cSendEditPage.m_blDeleteAfterSend;
}

inline BOOL CMemoSendSheet::ForwardDxlID( UINT unDxlID)
{
	m_cSendEditPage.m_unDxlID = unDxlID;

	return CMemoEditSheet::ForwardDxlID( unDxlID);
}
