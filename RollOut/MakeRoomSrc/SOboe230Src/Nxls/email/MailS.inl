inline int CMailS::GetToCount( void) const
{
	return m_nToAddressCount;
}

inline LPCSTR CMailS::GetToAddress( int nIndex) const
{
	if( NULL == m_papszToAddress)return NULL;
	if( nIndex >= m_nToAddressCount)return NULL;

	return m_papszToAddress[ nIndex];
}

inline LPCSTR CMailS::GetToAddressRaw( int nIndex) const
{
	if( NULL == m_papszToAddressRaw)return NULL;
	if( nIndex >= m_nToAddressCount)return NULL;

	return m_papszToAddressRaw[ nIndex];
}

inline LPCSTR CMailS::GetFromAddress( void) const
{
	return m_pszFromAddress;
}

inline LPCSTR CMailS::GetSubject( void) const
{
	return m_pszSubject;
}

inline LPCSTR CMailS::GetBody( void) const
{
	return m_pszBody;
}

inline int CMailS::GetRawDataSize( void) const
{
	return m_nRawDataSize;
}

inline LPVOID CMailS::GetRawData( void) const
{
	return m_lpvRawData;
}
