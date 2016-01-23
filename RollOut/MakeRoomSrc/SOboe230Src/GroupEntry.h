#ifndef	__GROUPENTRY_H__
#define	__GROUPENTRY_H__

#include "NetEx_IF.h"

class CGroupData
{
public:
	CGroupData();
	~CGroupData();

	BOOL SetGroupData( const GROUPDATA* pstGroupData, int nSize);

	const CString& GetGroupName( void) const;
	const int GetParentGroup( void) const;
	const int GetStatus( void) const;
protected:
	int		m_nParentGroup;
	int		m_nGroupStatus;
	CString	m_cStrGroupName;
};

class CEntryData
{
public:
	CEntryData();
	~CEntryData();

	BOOL SetEntryData( const ENTRYDATA* pstEntryData, int nSize);
	int GetEntryData( ENTRYDATA* pstEntryData, int nSize) const;
	int GetEntryDataSize( void) const;
	const ENTRYDATA* AccessEntryData() const;

	int GetStatus( void) const;
	int GetParentCount( void) const;
	int GetParent( int nIndex) const;

	CString GetEntryName( void) const;

	const UINT GetNxlID( void) const;

	BOOL operator==( const CEntryData& cEntryData) const;
	const CEntryData& operator=( const CEntryData& cEntryData);
protected:
	ENTRYDATA*	m_pstEntryData;
	int			m_nEntryDataSize;
};

#include "GroupEntry.inl"
#endif	//__GROUPENTRY_H__