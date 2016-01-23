#ifndef	__ENTRYLIST_H__
#define	__ENTRYLIST_H__

class CEntryList
{
public:
	CEntryList();
	~CEntryList();

	BOOL LockList( DWORD dwMilliseconds = -1);
	BOOL UnlockList( void);

	BOOL AddEntry( LPCSTR lpcszAddress, int nGroupID);
	BOOL RemoveEntry( LPCSTR lpcszAddress);

	void RemoveAll( void);

	int GetCount( void);
	int GetEntryAdd( int nIndex, LPSTR lpszEntryAdd, int nLength);
	int GetEntryName( int nIndex, LPSTR lpszEntryName, int nLength);

	int GetParentGroup( int nIndex);

protected:
	typedef struct tagENTRYLISTNODE
	{
		int				nstEntryDataSize;

		struct tagENTRYLISTNODE*	pNext;

		int				nGroupID;
		char			szEntryName[1];
	}ENTRYLISTNODE;

	ENTRYLISTNODE*	pTop;
	int				nEntryCount;
	HANDLE			hLock;
};

#endif	//__ENTRYLIST_H__