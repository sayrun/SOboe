inline const CString& CDrawExLib::GetPath( void)
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

	return FALSE;
}

inline HINSTANCE CDrawExLib::GetInstance( void) const
{
	return m_hInstance;
}

inline const CString& CDrawExLib::GetMenuString( void)
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

	return FALSE;
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

inline const CString& CDrawExLib::GetDefaultMenuString( void)
{
	return m_cStrDxlMenu;
}


inline UINT CDrawExLib::GetUnsupportMenu( void) const
{
	return m_unUnsupportMenu;
}

inline BOOL CDrawExLib::IsFunctionAssign( void)
{
	return ( 0 != m_unDXLID);
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
