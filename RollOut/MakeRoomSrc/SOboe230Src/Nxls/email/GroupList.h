#ifndef	__GROUPLIST_H__
#define	__GROUPLIST_H__

class CGroupList
{
public:
	CGroupList();
	~CGroupList();

	int AddGroup( LPCSTR lpcszGroup, int nParentGroupID = -1);
	int FindGroup( int nID);

	BOOL RemoveAll();

	int GetCount() const;
	const char* GetGroupName( int nIndex) const;
	int GetParentGroupID( int nIndex) const;
protected:
	typedef struct tagGROUPLISTNODE
	{
		int				nstGroupDataSize;
		int				nParentGroupID;
		int				nID;
		struct tagGROUPLISTNODE*	pNext;
		char			szGroupName[1];
	}GROUPLISTNODE;

	GROUPLISTNODE*	pTop;
	int				nGroupCount;
	int				nGroupID;
};

#endif	//__GROUPLIST_H__