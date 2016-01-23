#ifndef	__GROUPLIST_H__
#define	__GROUPLIST_H__

class CGroupList
{
public:
	CGroupList();
	~CGroupList();

	static int CheckGroupCount( LPCSTR lpcszGroupn);

	int AddGroup( LPCSTR lpcszGroupn, int* panGroupIDs, int nSize);

	int AddGroup( LPCSTR lpcszGroupn, int nParentID);
	int FindGroup( int nID);

	BOOL RemoveAll();

	int GetCount() const;
	const char* GetGroupName( int nIndex) const;
	int GetParentGroup( int nIndex) const;
protected:
	typedef struct tagGROUPLISTNODE
	{
		int				nstGroupDataSize;
		int				nID;
		int				nParentID;
		struct tagGROUPLISTNODE*	pNext;
		char			szGroupName[1];
	}GROUPLISTNODE;

	GROUPLISTNODE*	m_pTop;
	int				m_nGroupCount;
	int				m_nGroupID;
};

#endif	//__GROUPLIST_H__