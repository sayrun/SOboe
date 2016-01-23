#ifndef	__STDNETSLV_H__
#define	__STDNETSLV_H__

typedef	struct tagSRVDATA
{
	HANDLE			hStopEvent;
	SOCKET			sClient;
	RECEIVEDATA		lpfnRcvData;
	BOOL			blAbsence;
}SRVDATA;

typedef struct tagSENDDATAHEADER
{
	int		nSize;
	BYTE	abyData[ 1];
}SENDDATAHEADER;

#define	_ENTRY_START		1
#define	_ENTRY_ANSER		2
#define	_ENTRY_END			3

#define	_BROADCAST_ENTRYPACKET_HEADER	"OboeBct\0"
#define	_BROADCAST_PLUGIN_VERSION		0x00000100
typedef struct tagENTRYPACKET
{
	int		nSize;
	char	szHeader[ 8];	// "OboeBct\0"
	UINT	unPlugInVersion;
	int		nEntryCode;
	BOOL	blAbsence;
	int		nszNickname;
	int		nszGroup;
	int		nszLogonID;
	int		nszMachineName;
}ENTRYPACKET;

#define	_SR_TIMEOUT			( 60 * 1000)
#define	_RETRY_COUNT		5

// error code
#define	_REJECT	( _SEND_USER + 1)		// ëóêMêÊÇ™éÛêMÇãëî€

void EntryControlThread( void* pvParameter);
BOOL BroadcastEntry( int nEntry, BOOL blAbsence);
BOOL SendEntryCommand( int nEntry, BOOL blAbsence, struct sockaddr* stSockAdd, BOOL blBroadCast = FALSE);
void ListenThread( void* pvParameter);
void ReceiveDataThread( void* pvParameter);
int SendRawData( const ENTRYDATA* pstEntry, LPCVOID lpcvSendData, int nSendDataSize, DWORD dwTimeOut, HANDLE hCancelEvent);

void CleanupOrgData( void);
void OutputErrorString( LPCSTR lpcszErrMsg, LPCSTR lpcstrFunction);

class CGroupList;

BOOL GetLocalGroup( CGroupList* pcLocalGroupList);
int GetLocalParentID( LPCSTR lpcszSeed, int* panParent, int nSize, int nOffset, CGroupList* pcLocalGroupList);
int GetLocalName( LPCSTR lpcszSeed, char* pszLocalName, int nSize);
int GetSeed( int nIndex, char* pszSeed, int nSize);

#endif	//__STDNETSLV_H__
