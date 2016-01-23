#ifndef	__UPDATETHREAD_H__
#define	__UPDATETHREAD_H__

#define	WM_NOTIFYFROMTHREAD	WM_USER + 100
#define TN_MESSAGE			45
#define TN_PERCENT			46
#define	TN_QUIT				47

struct GETICONPARAM
{
	HWND hWnd;
	HANDLE hCancelEvent;
};

BOOL GetAppIniPath( LPTSTR lpszFilePath);
BOOL GetAppXmlPath( LPTSTR lpszFilePath);
BOOL GetUsrIniPath( LPTSTR lpszFilePath);

UINT __stdcall GetIconThread( void *pData);

#endif	//__UPDATETHREAD_H__
