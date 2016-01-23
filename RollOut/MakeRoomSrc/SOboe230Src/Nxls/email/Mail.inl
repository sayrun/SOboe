inline LPCSTR CMail::GetMessageID( void) const
{
	return m_lpszMessageID;
}

inline LPCSTR CMail::GetMailer( void) const
{
	return m_lpszMailer;
}

inline LPCSTR CMail::GetFrom( void) const
{
	return m_lpszFrom;
}

inline LPCSTR CMail::GetSubject( void) const
{
	return m_lpszSubject;
}

inline LPCSTR CMail::GetBody( void) const
{
	return m_lpszBody;
}

inline int CMail::GetRawDataSize( void) const
{
	return m_nRawDaraSize;
}

inline BOOL CMail::GetRawData( LPBYTE lpabyRawData, int nSize) const
{
	if( NULL == lpabyRawData || 0 >= nSize || NULL == m_lpbyRawData)return FALSE;

	CopyMemory( lpabyRawData, m_lpbyRawData, min( nSize, GetRawDataSize()));
	return TRUE;
}
