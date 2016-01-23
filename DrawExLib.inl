inline const CString& CDrawExLib::GetPath( void) const
{
	return *m_pcStrDxlPath;
}

inline BOOL CDrawExLib::SetPath( LPCSTR lpcsz)
{
	if( NULL == m_pcStrDxlPath)
	{
		m_pcStrDxlPath = new CString;
	}
	*m_pcStrDxlPath = lpcsz;

	return TRUE;
}

inline HINSTANCE CDrawExLib::GetInstance( void) const
{
	return m_hInstance;
}

inline const CString& CDrawExLib::GetMenuString( void) const
{
	return *m_pcStrMenuString;
}

inline BOOL CDrawExLib::SetMenuString( LPCSTR lpcsz)
{
	if( NULL == m_pcStrMenuString)
	{
		m_pcStrMenuString = new CString;
	}
	*m_pcStrMenuString = lpcsz;

	return TRUE;
}

inline UINT CDrawExLib::GetDataVersion( void) const
{
	return m_unDxlDataVersion;
}

inline UINT CDrawExLib::GetDxlID( void) const
{
	return m_unDXLID;
}

inline UINT CDrawExLib::GetDrawType( void) const
{
	return m_unDrawType;
}

inline UINT CDrawExLib::GetDxlStyle( void) const
{
	return m_unStyle;
}

inline BOOL CDrawExLib::IsFunctionAssign( void) const
{
	return ( 0 != m_unDXLID);
}

inline BOOL CDrawExLib::IsSupportEditHelper( void) const
{
	return ( 0 != ( m_unStyle & _DXL_EDITHEPLER));
}

inline const int CDrawExLib::UseStart( void)
{
	m_nUseCount++;
	return m_nUseCount;
}

inline const int CDrawExLib::UseEnd( void)
{
	m_nUseCount--;
	return m_nUseCount;
}

inline int CDrawExLib::GetUseCount( void) const
{
	return m_nUseCount;
}

inline BOOL CDrawExLib::IsSupportExSend( void) const
{
	return ( 0 == ( m_unStyle & _DXL_NOEXSEND));
}

inline BOOL CDrawExLib::IsSupportNativeData( void) const
{
	return ( 0 != ( m_unStyle & _DXL_NATIVEDATA));
}
