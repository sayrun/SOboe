#ifndef	__EMAIL_H__
#define	__EMAIL_H__

#include "GroupList.h"
#include "EntryList.h"
#include "IDMapSubject.h"
#include "NetEx_IF.h"

typedef struct tagEMAILORGDATA
{
	BOOL	blInitialized;

	BOOL	blLogging;

	char	szMachineName[ MAX_COMPUTERNAME_LENGTH + 1];

	char	szMailAddres[ 256];
	char	szRealName[ 256];

	char	szAliasFileName[ _MAX_PATH];

	HANDLE	hStopEvent;

	char	szSmtpServerName[ 256];
	ULONG	ulAddress;
	UINT	unSmtpPort;
	char	szPop3ServerName[ 256];
	UINT	unPop3Port;
	char	szPop3ID[ 256];
	BOOL	blInvalidatePW;
	char	szPop3PW[ 256];
	ULONG	ulPop3Interval;
	BOOL	blPop3APOP;

	char	szSignaturePath[ 256];

	BOOL	blWithMyAddress;
	BOOL	blMbbtombc;
	BOOL	blFormatting;
	int		nCharCount;

	CGroupList*	pcGroupList;
	CEntryList*	pcEntryList;

	CIDMapSubject* pcIDMapSubject;

	RECEIVEDATA	lpfnReceiveData;
}EMAILORGDATA;

#define	_EMAILPLUGIN_ID	3

#define	_NOTFOUND_SERVER			_SEND_USER + 0

#endif	//__EMAIL_H__
