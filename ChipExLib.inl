inline CChipExLib::CChipExLib()
{
	m_cStrMenu.Empty();
	m_cStrPath.Empty();

	m_hInstance = NULL;
}

inline CChipExLib::~CChipExLib()
{
}

inline BOOL CChipExLib::SetIndex( int nIndex)
{
	m_nIndex = nIndex;

	return TRUE;
}

inline BOOL CChipExLib::SetPath( const CString& cStrPath)
{
	m_cStrPath = cStrPath;

	return !m_cStrPath.IsEmpty();
}

inline BOOL CChipExLib::SetMenu( const CString& cStrMenu)
{
	m_cStrMenu = cStrMenu;

	return !m_cStrMenu.IsEmpty();
}

inline const CString& CChipExLib::GetMenu( void)
{
	return m_cStrMenu;
}

// égópíÜÅH
inline BOOL CChipExLib::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoCall);

	blResult = cSlk.Lock( 0);

	return !blResult;
}
