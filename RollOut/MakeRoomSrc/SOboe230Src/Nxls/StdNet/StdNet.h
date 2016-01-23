#ifndef	__STDNET_H__
#define	__STDNET_H__

#define	_STDPROTCOL_ID	1

#include "EntryList.h"
#include "GroupList.h"
typedef struct tagSTDNETORGDATA
{
	BOOL			blInitialized;
	BOOL			blActive;
	BOOL			blReadyReceive;
	BOOL			blReadySend;
	HANDLE			hStopEvent;
	UINT			unPort;
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
}STDNETORGDATA;

#endif	//__STDNET_H__
