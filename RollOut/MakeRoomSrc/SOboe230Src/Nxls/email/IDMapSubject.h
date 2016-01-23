#ifndef	__PACLETLIST_H__
#define	__PACLETLIST_H__

class CIDMapSubject
{
public:
	CIDMapSubject( int nMaxCount = 30);
	~CIDMapSubject( void);

	int AddSubject( LPCSTR lpcszID, LPCSTR lpcszSubject);
	int GetCount( void) const;
	LPCSTR GetID( int nIndex) const;
	LPCSTR GetSubject( int nIndex) const;
protected:
	struct NODE
	{
		int		nSize;

		NODE*	pNext;

		int		nIDOffset;
		int		nSubjectOffset;
	};

	NODE*	m_pTop;
	int		m_nNodeCount;
	int		m_nMaxCount;
};

#endif	__PACLETLIST_H__