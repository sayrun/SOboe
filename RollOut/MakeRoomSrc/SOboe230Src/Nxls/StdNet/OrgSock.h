#ifndef	__ORGSOCK_H__
#define	__ORGSOCK_H__

#if(_WIN32_WINNT >= 0x0400)
#define	Org_WSAEventSelect				WSAEventSelect
#define	Org_WSAEnumNetworkEvents		WSAEnumNetworkEvents
#define	Org_WSAWaitForMultipleEvents	WSAWaitForMultipleEvents
#else

typedef	struct tagSOCKEVENT
{
	SOCKET	s;
	HANDLE	hEvent;
	long	lSelectEvent;
	long	lNetEvent;
}SOCKEVENT;

#define		_MAX_SOCKEVENT		64

#define		FD_MAX_EVENTS		8
#define		WSAEVENT			HANDLE
#define		WSA_INFINITE		INFINITE
#define		WSA_WAIT_EVENT_0	WAIT_OBJECT_0
#define		WSA_WAIT_TIMEOUT	WAIT_TIMEOUT 

typedef struct _WSANETWORKEVENTS
{
	long lNetworkEvents;
	int iErrorCodes[FD_MAX_EVENTS];
} WSANETWORKEVENTS, FAR * LPWSANETWORKEVENTS;

void Initialize_OrgWsa( HWND hWnd);
void Uninitialize_OrgWsa( void);

int Org_WSAEventSelect( SOCKET s, WSAEVENT hEventObject, long lNetworkEvents);
int Org_WSAEnumNetworkEvents( SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
DWORD Org_WSAWaitForMultipleEvents( DWORD cEvents, const WSAEVENT FAR *lphEvents, BOOL fWaitAll, DWORD dwTimeOUT, BOOL fAlertable);

LRESULT CALLBACK OrgMainProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

#endif	//__ORGSOCK_H__
