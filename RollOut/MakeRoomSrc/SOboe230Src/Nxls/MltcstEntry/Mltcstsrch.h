#ifndef	__STDNET_H__
#define	__STDNET_H__

#define	_MCST_PROTCOL_ID	2

#include "GroupList.h"
#include "EntryList.h"

typedef struct tagMCSTNETORGDATA
{
	BOOL			blInitialized;
	BOOL			blActive;
	BOOL			blReadyReceive;
	BOOL			blReadySend;
	HANDLE			hStopEvent;
	ULONG			ulRcvAddress;
	ULONG			ulMyAddress;
	UINT			unMultiPort;
	int				nTTL;
	DWORD			dwDomainKey;
	BOOL			blAbsence;
	int				nSeedType;
	RECEIVEDATA		lpfnRcvData;
	CEntryList*		pcEntryList;
	CGroupList*		pcGroupList;
	CGroupList*		pcLocalGroupList;
	char*			pszEntryName;
	char*			pszNickname;
	char*			pszGroup;
	char*			pszLogonID;
	char*			pszMachineName;
}MCSTNETORGDATA;

#endif	//__STDNET_H__
