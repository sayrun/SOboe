class CStrList
{
public:
	CStrList( void);
	~CStrList( void);

	BOOL SetStreamData( LPCSTR lpcszData);

	int GetCount( void) const;
	LPCSTR GetLine( int nIndex) const;
protected:
	void CleanUp( void);
protected:
	char*	m_pszWorkArea;
	int*	m_panLineHeads;
	int		m_nLineCount;
};