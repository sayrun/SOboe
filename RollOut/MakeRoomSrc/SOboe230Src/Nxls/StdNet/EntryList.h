#ifndef	__ENTRYLIST_H__
#define	__ENTRYLIST_H__

#include "StdNetSlv.h"

class CEntryList
{
public:
	CEntryList();
	~CEntryList();

	BOOL LockList( DWORD dwMilliseconds = -1);
	BOOL UnlockList( void);

	BOOL AddEntry( struct sockaddr* pstSockAdd, ENTRYPACKET* const cpEntryPacket, int* panGroupID, int nGroupCount);
	BOOL DelEntry( struct sockaddr* pstSockAdd, LPCSTR lpszEntryName);
	int GetCount( void);
	int GetEntryAdd( int nIndex, struct sockaddr* pstSockAdd, int nSize);
	int GetEntryName( int nIndex, LPSTR lpszEntryName, int nLength, LPCSTR lpcszAlias = NULL);
	int GetParentID( int nIndex, int* panParentIDs, int nSize);
	int GetParentIDCount( int nIndex);
	int GetEntryStatus( int nIndex);
	int GetUserName( int nIndex, LPSTR lpUserName, int nSize);
	int GetComputerName( int nIndex, LPSTR lpComputerName, int nSize);
	int	GetNickName(int nIndex, LPSTR lpNicName, int nSize);
	int	GetIpAddr(int nIndex, LPSTR lpIpAddress, int nSize);

	void RemoveAll( void);

protected:
	typedef struct tagENTRYLISTNODE
	{
		int				nstEntryDataSize;

		struct tagENTRYLISTNODE*	pNext;

		struct sockaddr stSockAdd;
		int				nGroupCount;
		int				nGroupOffset;
		int				nEntryPacketOffset;
	}ENTRYLISTNODE;

	ENTRYLISTNODE*	pTop;
	int				nEntryCount;
	HANDLE			hLock;
};

#endif	//__ENTRYLIST_H__