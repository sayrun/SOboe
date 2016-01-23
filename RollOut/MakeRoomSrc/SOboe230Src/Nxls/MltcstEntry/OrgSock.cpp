#include <windows.h>
#include <winsock.h>
#include "OrgSock.h"

#if(_WIN32_WINNT < 0x0400)

extern HANDLE		g_hModule;

static UINT			g_unMsg = RegisterWindowMessage( "forOrgEventSelect");

static SOCKEVENT	g_stSockEvent[ _MAX_SOCKEVENT];

static HWND			g_hWndforOrgWsa = NULL;

static LPCSTR		g_lpcszClassName = "MulticastCheckWnd";

void Initialize_OrgWsa( HWND hWnd)
{
	if( NULL != g_hWndforOrgWsa)return;
	if( NULL == hWnd)return;

	for( int nIndex = 0; nIndex < _MAX_SOCKEVENT; nIndex++)
	{
		g_stSockEvent[ nIndex].s			= INVALID_SOCKET;
		g_stSockEvent[ nIndex].hEvent		= NULL;
		g_stSockEvent[ nIndex].lSelectEvent	= 0;
		g_stSockEvent[ nIndex].lNetEvent	= 0;
	}

	WNDCLASS wndcls;
	wndcls.style = 0;
	wndcls.lpfnWndProc = OrgMainProc;
	wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
	wndcls.hInstance = ( HINSTANCE)g_hModule;
	wndcls.hIcon = NULL;
	wndcls.hCursor = NULL;
	wndcls.hbrBackground = NULL;
	wndcls.lpszMenuName = NULL;
	wndcls.lpszClassName = g_lpcszClassName;

	ATOM at = RegisterClass(&wndcls);
	if( 0 != at)
	{
		g_hWndforOrgWsa = CreateWindow( g_lpcszClassName, "forOrgSelect", WS_CHILD, 0, 0, 0, 0, hWnd, ( HMENU)NULL, ( HINSTANCE)g_hModule, NULL);
#ifdef	_DEBUG
		if( NULL == g_hWndforOrgWsa)
		{
			char	sz[ 64];
			int		nError = GetLastError();
			wsprintf( sz, "CreateWindow_ERROR:%d\n", nError);
			OutputDebugString( sz);

			LPVOID lpMsgBuf;
			FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, nError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL );
			OutputDebugString( ( LPCSTR)lpMsgBuf);
			LocalFree( lpMsgBuf );
		}
		else
		{
			OutputDebugString( "¬Œ÷I\n");
		}
#endif
	}
#ifdef	_DEBUG
	else
	{
		char	sz[ 64];
		int		nError = GetLastError();
		wsprintf( sz, "RegisterClass_ERROR:%d\n", nError);
		OutputDebugString( sz);

		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, nError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL );
		OutputDebugString( ( LPCSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}
#endif
}

void Uninitialize_OrgWsa( void)
{
	if( NULL == g_hWndforOrgWsa)return;
	SendMessage( g_hWndforOrgWsa, WM_CLOSE, 0, 0L);
	g_hWndforOrgWsa = NULL;
	BOOL bl = UnregisterClass( g_lpcszClassName, ( HINSTANCE)g_hModule);
#ifdef	_DEBUG
	if( FALSE == bl)
	{
		int		nError = GetLastError();

		LPVOID lpMsgBuf;
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, nError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL );
		OutputDebugString( ( LPCSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}
#endif
}

int Org_WSAEventSelect( SOCKET s, WSAEVENT hEventObject, long lNetworkEvents)
{
	for( int nIndex = 0; nIndex < _MAX_SOCKEVENT; nIndex++)
	{
		if( s == g_stSockEvent[ nIndex].s)
		{
			if( 0 != lNetworkEvents)
			{
				g_stSockEvent[ nIndex].hEvent		= hEventObject;
				g_stSockEvent[ nIndex].lSelectEvent = lNetworkEvents;
			}
			else
			{
				g_stSockEvent[ nIndex].s			= INVALID_SOCKET;
				g_stSockEvent[ nIndex].hEvent		= NULL;
				g_stSockEvent[ nIndex].lSelectEvent	= 0;
				g_stSockEvent[ nIndex].lNetEvent	= 0;
			}
			WSAAsyncSelect( s, g_hWndforOrgWsa, g_unMsg, lNetworkEvents);
			return TRUE;
		}
	}
	for( nIndex = 0; nIndex < _MAX_SOCKEVENT; nIndex++)
	{
		if( INVALID_SOCKET == g_stSockEvent[ nIndex].s)
		{
			if( 0 != lNetworkEvents)
			{
				g_stSockEvent[ nIndex].s			= s;
				g_stSockEvent[ nIndex].hEvent		= hEventObject;
				g_stSockEvent[ nIndex].lSelectEvent = lNetworkEvents;

				WSAAsyncSelect( s, g_hWndforOrgWsa, g_unMsg, lNetworkEvents);
				return TRUE;
			}
		}
	}
	return FALSE;
}

int Org_WSAEnumNetworkEvents( SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents)
{
	for( int nIndex = 0; nIndex < _MAX_SOCKEVENT; nIndex++)
	{
		if( s == g_stSockEvent[ nIndex].s)
		{
			lpNetworkEvents->lNetworkEvents = g_stSockEvent[ nIndex].lNetEvent;
			return TRUE;
		}
	}
	return FALSE;
}

DWORD Org_WSAWaitForMultipleEvents( DWORD cEvents, const WSAEVENT FAR *lphEvents, BOOL fWaitAll, DWORD dwTimeOUT, BOOL fAlertable)
{
	return WaitForMultipleObjects( cEvents, lphEvents, fWaitAll, dwTimeOUT);
}

LRESULT CALLBACK OrgMainProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if( uMsg == g_unMsg)
	{
		for( int nIndex = 0; nIndex < _MAX_SOCKEVENT; nIndex++)
		{
			if( INVALID_SOCKET == g_stSockEvent[ nIndex].s)continue;
			if( ( SOCKET)wParam == g_stSockEvent[ nIndex].s)
			{
				if( g_stSockEvent[ nIndex].lSelectEvent & WSAGETSELECTEVENT( lParam))
				{
					g_stSockEvent[ nIndex].lNetEvent = WSAGETSELECTEVENT( lParam);
					SetEvent( g_stSockEvent[ nIndex].hEvent);
					break;
				}
			}
		}
		return 0;
	}
	return DefWindowProc( hWnd, uMsg, wParam, lParam);
}

#endif