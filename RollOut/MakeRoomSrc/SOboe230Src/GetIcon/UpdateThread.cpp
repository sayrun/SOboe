#include "stdafx.h"
#include "UpdateThread.h"
#include "GetIconDlg.h"

//#define _LOGING

void TrimmingStr( LPTSTR lpszData)
{
	int nLength = lstrlen( lpszData);
	if( 0 < nLength)
	{
		int nStartIndex = 0;

		// 空白以外になるまでまわれ！
		while( TEXT( '\r') == lpszData[ nStartIndex] ||
				TEXT( '\n') == lpszData[ nStartIndex] ||
				TEXT( '\t') == lpszData[ nStartIndex] ||
				TEXT( ' ') == lpszData[ nStartIndex])nStartIndex++;

		int nEndIndex = nLength;

		// 空白以外になるまで0にする
		while( 0 < nEndIndex &&
				( TEXT( '\r') == lpszData[ nEndIndex] ||
					TEXT( '\n') == lpszData[ nEndIndex] ||
					TEXT( '\t') == lpszData[ nEndIndex] ||
					TEXT( ' ') == lpszData[ nEndIndex] ||
					0 == lpszData[ nEndIndex]))
		{
			nEndIndex--;
		}

		MoveMemory( lpszData, &lpszData[ nStartIndex], nLength - nStartIndex);
		lpszData[ nEndIndex - nStartIndex + 1] = 0;
	}
}

BOOL GetAppIniPath( LPTSTR lpszFilePath)
{
	if( lpszFilePath)
	{
		GetModuleFileName( NULL, lpszFilePath, MAX_PATH);

		int nLength = lstrlen( lpszFilePath);
		while( nLength)
		{
			if( TEXT( '\\') == lpszFilePath[ nLength])
			{
				lstrcpy( &lpszFilePath[ nLength + 1], TEXT( "GetIcon.ini"));
				break;
			}
			nLength--;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL GetAppXmlPath( LPTSTR lpszFilePath)
{
	if( lpszFilePath)
	{
		GetModuleFileName( NULL, lpszFilePath, MAX_PATH);

		int nLength = lstrlen( lpszFilePath);
		while( nLength)
		{
			if( TEXT( '\\') == lpszFilePath[ nLength])
			{
				lstrcpy( &lpszFilePath[ nLength + 1], TEXT( "GetIcon.xml"));
				break;
			}
			nLength--;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL GetUsrIniPath( LPTSTR lpszFilePath)
{
	if( lpszFilePath)
	{
		GetModuleFileName( NULL, lpszFilePath, MAX_PATH);

		int nLength = lstrlen( lpszFilePath);
		while( nLength)
		{
			if( TEXT( '\\') == lpszFilePath[ nLength])
			{
				LPTSTR lpszUserName = NULL;
				DWORD dwLength = 0;
				GetUserName( NULL, &dwLength);
				if( 0 < nLength)
				{
					lpszUserName = new TCHAR[ dwLength + 10];
					if( NULL != lpszUserName)
					{
						GetUserName( lpszUserName, &dwLength);
					}
				}

				if( NULL != lpszUserName)
				{
					lstrcpyn( &lpszFilePath[ nLength + 1], lpszUserName, ( MAX_PATH - ( nLength + 1)) - 5);
					lstrcat( &lpszFilePath[ nLength + 1], TEXT( ".ini"));
					delete [] lpszUserName;
				}
				else
				{
					lstrcpy( &lpszFilePath[ nLength + 1], TEXT( "GetIcon.ini"));
				}
				break;
			}
			nLength--;
		}
		return TRUE;
	}
	return FALSE;
}

static LPCTSTR lpcszMAIN = TEXT( "MAIN");
static LPCTSTR lpcszICON = TEXT( "ICON");
static LPCTSTR lpcszLastUpdate = TEXT( "lastupdate");
static LPCTSTR lpcszDefaultURL = TEXT( "http://www.system-lab.co.jp/~Tomo/cgi-bin/Oboe/icon.cgi");
static LPCTSTR lpcszNewStation = TEXT( "newlocate");

void GetStationURL( LPTSTR lpszURL, DWORD dwSize)
{
	TCHAR szIniPath[ MAX_PATH];
	GetAppIniPath( szIniPath);

	GetPrivateProfileString( lpcszMAIN, TEXT( "URL"), lpcszDefaultURL, lpszURL, dwSize, szIniPath);
}

void SetStationURL( LPTSTR lpszURL)
{
	if( NULL != lpszURL && 0 < lstrlen( lpszURL))
	{
		TCHAR szIniPath[ MAX_PATH];
		GetAppIniPath( szIniPath);

		WritePrivateProfileString( lpcszMAIN, TEXT( "URL"), lpszURL, szIniPath);
	}
}

static LPCTSTR lpcszAgetnt = TEXT( "GetIcon/0.03");
static TCHAR szUserID[ 128] = TEXT( "");
static TCHAR szUserPW[ 128] = TEXT( "");

HLOCAL GetServerData( LPCTSTR lpcszURL)
{
	HLOCAL hResult = NULL;

	HINTERNET hInetSession = NULL;
	HINTERNET hInetConnect = NULL;
	HINTERNET hInetHttp = NULL;


	hInetSession = ::InternetOpen( lpcszAgetnt, PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
	if( NULL == hInetSession)goto leavepoint;

	TCHAR szHost[256];
	TCHAR szPath[256];
	URL_COMPONENTS stComp;
	ZeroMemory( &stComp, sizeof( URL_COMPONENTS));
	stComp.dwStructSize = sizeof( URL_COMPONENTS);
	stComp.lpszHostName = szHost;
	stComp.dwHostNameLength = 256;
	stComp.lpszUrlPath = szPath;
	stComp.dwUrlPathLength = 256;
	::InternetCrackUrl( lpcszURL, lstrlen( lpcszURL), 0, &stComp);

	while( true)
	{
		hInetConnect = ::InternetConnect( hInetSession, szHost, INTERNET_DEFAULT_HTTP_PORT, lstrlen( szUserID) ? szUserID : NULL, lstrlen( szUserPW) ? szUserPW : NULL, INTERNET_SERVICE_HTTP, 0, 95);
		if( NULL == hInetConnect)goto leavepoint;

		hInetHttp = ::HttpOpenRequest( hInetConnect, TEXT( "GET"), szPath, TEXT( "HTTP/1.1"), NULL, NULL, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_ASYNC, 95);
		if( NULL == hInetConnect)goto leavepoint;

		if( ::HttpSendRequest( hInetHttp, NULL, 0, NULL, 0))
		{
			TCHAR szMsg[ 1024];
			DWORD dwError = 0;
			DWORD dwLength = 1024;
			DWORD dwIndex = 0;
			dwLength = 1024;
			::HttpQueryInfo( hInetHttp, HTTP_QUERY_STATUS_CODE, szMsg, &dwLength, &dwIndex);

			int nResult = atoi( szMsg);
			if( HTTP_STATUS_PROXY_AUTH_REQ == nResult || HTTP_STATUS_DENIED == nResult)
			{
				::InternetCloseHandle( hInetHttp);
				hInetHttp = NULL;
				::InternetCloseHandle( hInetConnect);
				hInetConnect = NULL;


				CLoginDialog cLoginDlg( szUserID, szUserPW);

				if( IDOK == cLoginDlg.DoModal())
				{
					lstrcpy( szUserID, cLoginDlg.GetID());
					lstrcpy( szUserPW, cLoginDlg.GetPW());
					continue;
				}
				else
				{
					break;
				}
			}

			// 正常な読み出し！
			DWORD dwAllocSize = 512;
			DWORD dwGetSize = 0;
			HLOCAL hLocal = LocalAlloc( LHND, dwAllocSize);
			if( hLocal)
			{
				BYTE abyData[ 512];
				DWORD dwReadSize = 0;
				LPBYTE lpbyWork;
				BOOL blResult;

				while( TRUE)
				{
					blResult = ::InternetReadFile( hInetHttp, abyData, 511, &dwReadSize);
					if( blResult && 0 == dwReadSize)
					{
						hResult = hLocal;
						hLocal = NULL;
						break;
					}

					// 領域拡張の必要性があるか？
					if( dwAllocSize <= ( dwGetSize + dwReadSize))
					{
						dwAllocSize += dwReadSize + 1;
						hLocal = LocalReAlloc( hLocal, dwAllocSize, LHND);
					}

					lpbyWork = ( LPBYTE)LocalLock( hLocal);
					if( lpbyWork)
					{
						CopyMemory( &lpbyWork[ dwGetSize], abyData, dwReadSize);
						dwGetSize += dwReadSize;
						LocalUnlock( hLocal);
					}
				}

				if( hLocal)LocalFree( hLocal);
			}

			break;
		}
	}

leavepoint:
	if( hInetHttp)
	{
		::InternetCloseHandle( hInetHttp);
		hInetHttp = NULL;
	}
	if( hInetConnect)
	{
		::InternetCloseHandle( hInetConnect);
		hInetConnect = NULL;
	}
	if( hInetSession)
	{
		::InternetCloseHandle( hInetSession);
		hInetSession = NULL;
	}

	/*
	if( NULL != hResult)
	{
		HANDLE hFile = CreateFile( "sasasasasas", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if( INVALID_HANDLE_VALUE != hFile)
		{
			LPCSTR lpBuffer = ( LPCSTR)LocalLock( hResult);
			DWORD dwLength = lstrlen( lpBuffer);
			DWORD dwSize;
			WriteFile( hFile, lpBuffer, dwLength, &dwSize, NULL);
			LocalUnlock( hResult);

			CloseHandle( hFile);
		}
	}*/

	return hResult;


	/*
	HLOCAL hResult = NULL;

	HINTERNET hInetSession = ::InternetOpen( TEXT( "GetIcon/0.02"), PRE_CONFIG_INTERNET_ACCESS, NULL, NULL, 0);
	if( hInetSession)
	{
		HINTERNET hInetURL = ::InternetOpenUrl( hInetSession, lpcszURL, NULL, 0, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_ASYNC, 0);
		if( hInetURL)
		{
			DWORD dwAllocSize = 512;
			DWORD dwGetSize = 0;
			HLOCAL hLocal = LocalAlloc( LHND, dwAllocSize);
			if( hLocal)
			{
				BYTE abyData[ 512];
				DWORD dwReadSize = 0;
				LPBYTE lpbyWork;
				BOOL blResult;

				while( TRUE)
				{
					blResult = ::InternetReadFile( hInetURL, abyData, 511, &dwReadSize);
					if( blResult && 0 == dwReadSize)
					{
						hResult = hLocal;
						hLocal = NULL;
						break;
					}

					// 領域拡張の必要性があるか？
					if( dwAllocSize <= ( dwGetSize + dwReadSize))
					{
						dwAllocSize += dwReadSize + 1;
						hLocal = LocalReAlloc( hLocal, dwAllocSize, LHND);
					}

					lpbyWork = ( LPBYTE)LocalLock( hLocal);
					if( lpbyWork)
					{
						CopyMemory( &lpbyWork[ dwGetSize], abyData, dwReadSize);
						dwGetSize += dwReadSize;
						LocalUnlock( hLocal);
					}
				}

				if( hLocal)LocalFree( hLocal);
			}

			::InternetCloseHandle( hInetURL);
		}
#ifdef _DEBUG
		else
		{
			TCHAR szDebug[ 512];
			DWORD dwError = GetLastError();
			wsprintf( szDebug, "dwError = 0x%04X", dwError);
			::OutputDebugString( szDebug);
		}
#endif
		::InternetCloseHandle( hInetSession);
	}

	return hResult;
	*/
}

BOOL SetCurrentDate( SYSTEMTIME& stTime, LPCTSTR lpcszOboeGakiIni)
{
	TCHAR szWork[ 32];

	wsprintf( szWork, TEXT( "%d/%d/%d"), stTime.wYear, stTime.wMonth, stTime.wDay);
	WritePrivateProfileString( lpcszMAIN, lpcszLastUpdate, szWork, lpcszOboeGakiIni);

	return TRUE;
}

BOOL GetCurrentDate( SYSTEMTIME& stTime, LPCTSTR lpcszOboeGakiIni)
{
	TCHAR szGetWork[ 16];
	int nLength = GetPrivateProfileString( lpcszMAIN, lpcszLastUpdate, TEXT( ""), szGetWork, 16, lpcszOboeGakiIni);

	int nYear = 1900;
	int nMonth = 1;
	int nDay = 1;
	if( 0 < nLength)
	{
		for( int nIndex = 0; nIndex <= nLength; nIndex++)
		{
			if( !isdigit( szGetWork[ nIndex]))
			{
				szGetWork[ nIndex] = 0;
				nYear = atoi( szGetWork);
				nIndex++;
				break;
			}
		}
		LPTSTR lpszWork = &szGetWork[ nIndex];
		for( ; nIndex <= nLength; nIndex++)
		{
			if( !isdigit( szGetWork[ nIndex]))
			{
				szGetWork[ nIndex] = 0;
				nMonth = atoi( lpszWork);
				nIndex++;
				break;
			}
		}
		lpszWork = &szGetWork[ nIndex];
		for( ; nIndex <= nLength; nIndex++)
		{
			if( !isdigit( szGetWork[ nIndex]))
			{
				szGetWork[ nIndex] = 0;
				nDay = atoi( lpszWork);
				break;
			}
		}
	}

	ZeroMemory( &stTime, sizeof( SYSTEMTIME));
	stTime.wYear = nYear;
	stTime.wMonth = nMonth;
	stTime.wDay = nDay;

	return TRUE;
}

BOOL GetNewStationURL( LPTSTR lpszNewStation, int nSize, LPCTSTR lpcszMainIni)
{
	LPSTR lpszFind = strstr( lpcszMainIni, lpcszNewStation);
	if( NULL != lpszFind)
	{
		int nStart = int( lpszFind - lpcszMainIni);
		nStart += lstrlen( lpcszNewStation) + 1;

		int nLength = lstrlen( &lpcszMainIni[ nStart]);
		for( int nIndex = 0; nIndex < nLength; nIndex++)
		{
			 if( TEXT( '\n') == lpcszMainIni[ nStart + nIndex])break;
		}
		lstrcpyn( lpszNewStation, &lpcszMainIni[ nStart], min( nSize, nIndex));
		TrimmingStr( lpszNewStation);

		return TRUE;
	}
	return FALSE;
}

BOOL GetServerDate( SYSTEMTIME& stTime, LPCTSTR lpcszMainIni)
{
	LPSTR lpszFind = strstr( lpcszMainIni, lpcszLastUpdate);
	if( NULL != lpszFind)
	{
		int nFind = int( lpszFind - lpcszMainIni);

		TCHAR szGetWork[ 16];

		 for( int nIndex = 0; nIndex < 15; nIndex++)
		 {
			 szGetWork[ nIndex] = lpcszMainIni[ nFind + ( lstrlen( lpcszLastUpdate) + 1) + nIndex];
			 if( 0 == szGetWork[ nIndex])break;
			 if( TEXT( '\n') == szGetWork[ nIndex])break;
		 }
		 szGetWork[ nIndex] = 0;

		int nYear = 0;
		int nMonth = 0;
		int nDay = 0;
		int nLength = lstrlen( szGetWork);
		if( 0 < nLength)
		{
			for( int nIndex = 0; nIndex <= nLength; nIndex++)
			{
				if( !isdigit( szGetWork[ nIndex]))
				{
					szGetWork[ nIndex] = 0;
					nYear = atoi( szGetWork);
					nIndex++;
					break;
				}
			}
			LPTSTR lpszWork = &szGetWork[ nIndex];
			for( ; nIndex <= nLength; nIndex++)
			{
				if( !isdigit( szGetWork[ nIndex]))
				{
					szGetWork[ nIndex] = 0;
					nMonth = atoi( lpszWork);
					nIndex++;
					break;
				}
			}
			lpszWork = &szGetWork[ nIndex];
			for( ; nIndex <= nLength; nIndex++)
			{
				if( !isdigit( szGetWork[ nIndex]))
				{
					szGetWork[ nIndex] = 0;
					nDay = atoi( lpszWork);
					break;
				}
			}
		}

		ZeroMemory( &stTime, sizeof( SYSTEMTIME));
		stTime.wYear = nYear;
		stTime.wMonth = nMonth;
		stTime.wDay = nDay;

		return TRUE;
	}
	return FALSE;
}

int GetIconCount( LPCTSTR lpcszMainIni)
{
	int nCount = 0;

	LPCTSTR lpcszCount = TEXT( "count");
	LPSTR lpszFind = strstr( lpcszMainIni, lpcszCount);
	if( NULL != lpszFind)
	{
		int nStart = int( lpszFind - lpcszMainIni);

		nStart += 6;	//	6 = lstrlen( lpcszCount) + 1;
		int nLength = lstrlen( lpcszMainIni);
		for( int nIndex = nStart; nIndex <= nLength; nIndex++)
		{
			if( !isdigit( lpcszMainIni[ nIndex]))
			{
				LPTSTR lpszWork = new TCHAR[ ( nIndex + 1) - nStart];
				if( lpszWork)
				{
					lstrcpyn( lpszWork, &lpcszMainIni[ nStart], ( nIndex - nStart) + 1);
					nCount = atoi( lpszWork);
					delete [] lpszWork;
				}
				break;
			}
		}
	}
	return nCount;
}

#define	CRCPOLY 0x2e0b10b7UL

DWORD CRC32( int nSize, LPCTSTR pData)
{
	DWORD	dwResult = UINT_MAX;

	for( int nIndex = 0; nIndex < nSize; nIndex++)
	{
		dwResult ^= ( DWORD)pData[ nIndex] << ( 32 - ( CHAR_BIT * sizeof( TCHAR)));
		for( int nBit = 0; nBit < ( CHAR_BIT * sizeof( TCHAR)); nBit++)
		{
			if( dwResult & 0x80000000UL)
			{
				dwResult = ( dwResult << 1) ^ CRCPOLY;
			}
			else
			{
				dwResult <<= 1;
			}
		}
	}

	return ~dwResult & UINT_MAX;
}

BOOL WriteIconData( int nIndex, LPCTSTR lpcszIconData)
{
	if( NULL == strstr( lpcszIconData, TEXT( "error=-1")))
	{
		// データソースと同じサイズのエリアを作業エリアとして確保
		int nLength = lstrlen( lpcszIconData);
		LPSTR lpszWorkArea = new TCHAR[ nLength + 1];
		if( lpszWorkArea)
		{
			TCHAR szProFile[ MAX_PATH];
			if( GetAppIniPath( szProFile))
			{
				LPCTSTR lpcszApp = TEXT( "ICON");
				LPCTSTR lpcszName = TEXT( "name");
				LPCTSTR lpcszAuthor = TEXT( "author");
				LPCTSTR lpcszEmail = TEXT( "email");
				LPCTSTR lpcszWeb = TEXT( "web");
				LPCTSTR lpcszData = TEXT( "data");
				LPCTSTR lpcszCrc = TEXT( "crc");

				LPTSTR lpszStopPoint;
				LPTSTR lpszFind;

				// CRC
				lpszFind = strstr( lpcszIconData, lpcszCrc);
				DWORD dwCRC = -1;
				if( NULL != lpszFind)
				{
					int nStart = int( lpszFind - lpcszIconData);

					nStart += lstrlen( lpcszCrc) + 1;
					lpszFind = strchr( &lpcszIconData[ nStart], TEXT( '\n'));
					if( NULL != lpszFind)
					{
						int nSize = int( lpszFind - &lpcszIconData[ nStart]);

						lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
						lpszWorkArea[ nSize] = 0;

						TrimmingStr( lpszWorkArea);
						dwCRC = strtoul( lpszWorkArea, &lpszStopPoint, 16);
					}
				}

				// データ
				lpszWorkArea[ 0] = 0;
				lpszFind = strstr( lpcszIconData, lpcszData);
				if( NULL != lpszFind)
				{
					int nStart = int( lpszFind - lpcszIconData);
					nStart += lstrlen( lpcszData) + 1;
					lpszFind = strchr( &lpcszIconData[ nStart], TEXT('\n'));
					if( NULL != lpszFind)
					{
						int nSize = int( lpszFind - &lpcszIconData[ nStart]);

						lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
						TrimmingStr( lpszWorkArea);
					}
				}
				DWORD dwCheck = CRC32( lstrlen( lpszWorkArea), lpszWorkArea);
				

				// アイコンデータのサイズは64文字！
				if( 64 == lstrlen( lpszWorkArea) && dwCRC == dwCheck)
				{
					TCHAR szEntry[ 128];
					TCHAR szWork[ 3];
					BYTE abyData[ 32];
					ULONG ulData;
					for( int nImgIndex = 0; nImgIndex < lstrlen( lpszWorkArea); nImgIndex += 2)
					{
						lstrcpyn( szWork, &lpszWorkArea[ nImgIndex], 3);
						ulData = strtol( szWork, &lpszStopPoint, 16);
						abyData[ nImgIndex / 2] = LOBYTE( LOWORD( ulData));
					}
					wsprintf( szEntry, TEXT( "icon%ddata"), nIndex);
					WritePrivateProfileStruct( lpcszApp, szEntry, abyData, 32, szProFile);

					// アイコン名称
					lstrcpy( lpszWorkArea, TEXT( "NONAME"));
					lpszFind = strstr( lpcszIconData, lpcszName);
					if( NULL != lpszFind)
					{
						int nStart = int( lpszFind - lpcszIconData);
						nStart += lstrlen( lpcszName) + 1;

						lpszFind = strchr( &lpcszIconData[ nStart], TEXT('\n'));
						if( NULL != lpszFind)
						{
							int nSize = int( lpszFind - &lpcszIconData[ nStart]);
							lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
							TrimmingStr( lpszWorkArea);
						}
					}
					wsprintf( szEntry, TEXT( "icon%dname"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, lpszWorkArea, szProFile);

					// アイコン作者
					lstrcpy( lpszWorkArea, TEXT( "NONAME"));
					lpszFind = strstr( lpcszIconData, lpcszAuthor);
					if( NULL != lpszFind)
					{
						int nStart = int( lpszFind - lpcszIconData);
						nStart += lstrlen( lpcszAuthor) + 1;

						lpszFind = strchr( &lpcszIconData[ nStart], TEXT('\n'));
						if( NULL != lpszFind)
						{
							int nSize = int( lpszFind - &lpcszIconData[ nStart]);
							lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
							TrimmingStr( lpszWorkArea);
						}
					}
					wsprintf( szEntry, TEXT( "icon%dauthor"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, lpszWorkArea, szProFile);

					// 作者メイル
					lstrcpy( lpszWorkArea, TEXT( ""));
					lpszFind = strstr( lpcszIconData, lpcszEmail);
					if( NULL != lpszFind)
					{
						int nStart = int( lpszFind - lpcszIconData);
						nStart += lstrlen( lpcszEmail) + 1;

						lpszFind = strchr( &lpcszIconData[ nStart], TEXT('\n'));
						if( NULL != lpszFind)
						{
							int nSize = int( lpszFind - &lpcszIconData[ nStart]);
							lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
							TrimmingStr( lpszWorkArea);
						}
					}
					wsprintf( szEntry, TEXT( "icon%demail"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, lpszWorkArea, szProFile);

					// 作者サイト
					lstrcpy( lpszWorkArea, TEXT( ""));
					lpszFind = strstr( lpcszIconData, lpcszWeb);
					if( NULL != lpszFind)
					{
						int nStart = int( lpszFind - lpcszIconData);
						nStart += lstrlen( lpcszWeb) + 1;

						lpszFind = strchr( &lpcszIconData[ nStart], TEXT('\n'));
						if( NULL != lpszFind)
						{
							int nSize = int( lpszFind - &lpcszIconData[ nStart]);
							lstrcpyn( lpszWorkArea, &lpcszIconData[ nStart], nSize + 1);
							TrimmingStr( lpszWorkArea);
						}
					}
					wsprintf( szEntry, TEXT( "icon%dwab"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, lpszWorkArea, szProFile);

					wsprintf( szEntry, TEXT( "icon%denable"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, TEXT( "1"), szProFile);
				}
				else
				{
					TCHAR szEntry[ 128];
					wsprintf( szEntry, TEXT( "icon%denable"), nIndex);
					WritePrivateProfileString( lpcszApp, szEntry, TEXT( "0"), szProFile);
				}
				return TRUE;
			}

			delete [] lpszWorkArea;
		}
	}
	return FALSE;
}

BOOL TestResult( HWND hWnd, LPCTSTR lpcszResult)
{
	if( NULL == strstr( lpcszResult, TEXT( "error=0")))
	{
		LPCTSTR lpcszInfo = TEXT( "info=");
		LPSTR lpszFind = strstr( lpcszResult, lpcszInfo);
		if( NULL != lpszFind)
		{
			int nStart = int( lpszFind - lpcszResult);
			nStart += lstrlen( lpcszInfo);

			lpszFind = strchr( &lpcszResult[ nStart], TEXT( '\n'));
			if( NULL != lpszFind)
			{
				int nSize = int( lpszFind - &lpcszResult[ nStart]);
				LPTSTR lpszMsg = new TCHAR [ nSize + 1] ;
				if( lpszMsg)
				{
					lstrcpyn( lpszMsg, &lpcszResult[ nStart], nSize + 1);
					::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)lpszMsg);
					delete [] lpszMsg;
					return FALSE;
				}
			}
		}
		::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "不明なエラーが発生"));
		return FALSE;
	}
	return TRUE;
}

BOOL UpdateIcon( HWND hWnd, LPCTSTR lpcszStationURL, int nIndex)
{
	BOOL blResult = FALSE;

	TCHAR szProFile[ MAX_PATH];
	if( GetAppIniPath( szProFile))
	{
		LPCTSTR lpcszApp = TEXT( "ICON");
		TCHAR szEntery[ 32];
		wsprintf( szEntery, TEXT( "icon%denable"), nIndex);
		BOOL blEnable = GetPrivateProfileInt( lpcszApp, szEntery, 0, szProFile);
		if( FALSE == blEnable)
		{
			// 取得する必要あり
			int nCmdSize = lstrlen( lpcszStationURL) + 20;
			LPTSTR lpszCmd = new TCHAR[ nCmdSize];
			if( lpszCmd)
			{
				wsprintf( lpszCmd, TEXT( "%s?icon+%d"), lpcszStationURL, nIndex);

				HLOCAL hLocal = GetServerData( lpszCmd);
				if( NULL != hLocal)
				{
					LPCTSTR lpcszWork = ( LPCTSTR)LocalLock( hLocal);
					if( lpcszWork)
					{
#ifdef _LOGING
						HANDLE hFile = CreateFile( szEntery, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if( INVALID_HANDLE_VALUE != hFile)
						{
							DWORD dwWrited = 0;
							WriteFile( hFile, lpcszWork, lstrlen( lpcszWork), &dwWrited, NULL);
							CloseHandle( hFile);
						}
#endif
						if( TestResult( hWnd, lpcszWork))
						{
							WriteIconData( nIndex, lpcszWork);
							blResult = TRUE;
						}
						LocalUnlock( hLocal);
					}
					LocalFree( hLocal);
					hLocal = NULL;
				}

				delete [] lpszCmd;
			}
		}
	}
	return blResult;
}

BOOL GetStatus( LPCTSTR lpcszMainIni, int nIndex, BOOL& blEnable)
{
	BOOL blResult = FALSE;

	blEnable = FALSE;

	TCHAR szEntry[ 32];
	wsprintf( szEntry, TEXT( "icon%d="), nIndex);
	LPCTSTR lpcszFind = strstr( lpcszMainIni, szEntry);
	if( NULL != lpcszFind)
	{
		int nStart = int( lpcszFind - lpcszMainIni);
		nStart += lstrlen( szEntry);
		lpcszFind = strchr( &lpcszMainIni[ nStart], TEXT( '\n'));
		int nLength = ( lpcszFind - &lpcszMainIni[ nStart]);
		if( 0 > nLength)nLength = lstrlen( &lpcszMainIni[ nStart]);
		for( int nIndex = nStart; nIndex <= ( nStart + nLength); nIndex++)
		{
			if( !isdigit( lpcszMainIni[ nIndex]))
			{
				LPTSTR lpszWork = new TCHAR [ ( nIndex - nStart) + 1];
				if( NULL != lpszWork)
				{
					lstrcpyn( lpszWork, &lpcszMainIni[ nStart], ( nIndex - nStart) + 1);
					blEnable = atoi( lpszWork);
					blResult = TRUE;

					delete [] lpszWork;
				}
				break;
			}
		}
		/*
		int nFileName = cStrMainIni.Find( TEXT( ","), nStart);
		if( 0 <= nFileName)
		{
			nFileName++;
			cStrFileName = cStrMainIni.Mid( nFileName, nLength - nFileName);
			cStrFileName.TrimLeft();
			cStrFileName.TrimRight();
			if( cStrFileName.IsEmpty())
			{
				blEnable = FALSE;
				blResult = FALSE;
			}
		}*/
	}
	return blResult;
}

BOOL DisableIcon( int nIndex)
{
	TCHAR szProFile[ MAX_PATH];
	if( GetAppIniPath( szProFile))
	{
		LPCTSTR lpcszApp = TEXT( "ICON");

		TCHAR szEntry[ 32];
		wsprintf( szEntry, TEXT( "icon%denable"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( "0"), szProFile);
		wsprintf( szEntry, TEXT( "icon%dname"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( ""), szProFile);
		wsprintf( szEntry, TEXT( "icon%dauthor"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( ""), szProFile);
		wsprintf( szEntry, TEXT( "icon%demail"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( ""), szProFile);
		wsprintf( szEntry, TEXT( "icon%dwab"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( ""), szProFile);
		wsprintf( szEntry, TEXT( "icon%ddata"), nIndex);
		WritePrivateProfileString( lpcszApp, szEntry, TEXT( ""), szProFile);

		return TRUE;
	}
	return FALSE;
}

BOOL UpdateIcons( HWND hWnd, LPCTSTR lpcszStationURL, LPCTSTR lpcszMainIni, HANDLE hCancelEvent)
{
	int nCount = GetIconCount( lpcszMainIni);
	TCHAR szProFile[ MAX_PATH];
	if( GetAppIniPath( szProFile))
	{
		// 日付をチェックするのはちょっと問題ありみたい。
//		SYSTEMTIME stServerTime;
//		SYSTEMTIME stLocalTime;
//		GetServerDate( stServerTime, lpcszMainIni);
//		GetCurrentDate( stLocalTime, szProFile);
//		if( stServerTime.wYear != stLocalTime.wYear || stServerTime.wMonth != stLocalTime.wMonth || stServerTime.wDay != stLocalTime.wDay)
		{
			LPCTSTR lpcszApp = TEXT( "ICON");
			TCHAR szEntry[ 32];
			wsprintf( szEntry, TEXT( "%d"), nCount);
			WritePrivateProfileString( lpcszApp, TEXT( "count"), szEntry, szProFile);

			TCHAR szMessage[ 64];
			BOOL blEnable;
			for( int nIndex = 1; nIndex <= nCount; nIndex++)
			{
				if( WAIT_OBJECT_0 == WaitForSingleObject( hCancelEvent, 0))return FALSE;
				GetStatus( lpcszMainIni, nIndex, blEnable);
				if( blEnable)
				{
					if( UpdateIcon( hWnd, lpcszStationURL, nIndex))
					{
						wsprintf( szMessage, "No.%dのアイコンデータを更新しました", nIndex);
						::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)szMessage);
					}
				}
				else
				{
					DisableIcon( nIndex);
				}
			}
//			SetCurrentDate( stServerTime, szProFile);
		}

		TCHAR szStationURL[ MAX_PATH];
		if( GetNewStationURL( szStationURL, MAX_PATH, lpcszMainIni))
		{
			SetStationURL( szStationURL);
		}
	}
	return TRUE;
}

// 酷い実装だって指摘は甘んじて受けます。``r(///)
UINT __stdcall GetIconThread( void *pData)
{
	GETICONPARAM* pGetIConParam = ( GETICONPARAM*)pData;

	HWND hWnd = NULL;
	HANDLE hEvent= NULL;
	BOOL blResult = FALSE;
	HLOCAL hLocal = NULL;

	if( NULL != pGetIConParam)
	{
		hEvent = NULL;
		hWnd = pGetIConParam->hWnd;
		HANDLE hProcess = GetCurrentProcess();
		if( DuplicateHandle( hProcess, pGetIConParam->hCancelEvent, hProcess, &hEvent, EVENT_ALL_ACCESS, TRUE, DUPLICATE_SAME_ACCESS))
		{
			::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "アイコンデータ更新処理開始"));

			// URL取得
			TCHAR szURL[ 512];
			GetStationURL( szURL, 512);

			// マスタini取得
			TCHAR szStartion[ 512];
			wsprintf( szStartion, "%s?ini", szURL);

			::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "更新ソースを取得します。"));
			hLocal = GetServerData( szStartion);
			if( NULL == hLocal)
			{
				::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "更新ソースが取得できませんでした。"));
				goto errordetect;
			}
			::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "更新ソースを取得しました。"));
			if( WAIT_OBJECT_0 == WaitForSingleObject( hEvent, 0))
			{
				::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "取得処理が中断されました。"));
				goto errordetect;
			}

			LPCTSTR lpcszMainIni = ( LPCTSTR)LocalLock( hLocal);
			if( lpcszMainIni)
			{
#ifdef _LOGING
				HANDLE hFile = CreateFile( "master.ini", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				if( INVALID_HANDLE_VALUE != hFile)
				{
					DWORD dwWrited = 0;
					WriteFile( hFile, lpcszMainIni, lstrlen( lpcszMainIni), &dwWrited, NULL);
					CloseHandle( hFile);
				}
#endif
				if( NULL != strstr( lpcszMainIni, TEXT( "busy")))
				{
					::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "サーバのデータファイルがビジーです"));
				}
				else
				{
					if( TestResult( hWnd, lpcszMainIni))
					{
						::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "更新ソースに基づいてアイコンデータの収集を開始します。"));
						if( UpdateIcons( hWnd, szURL, lpcszMainIni, hEvent))
						{
							blResult = TRUE;
							::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "アイコンデータ更新処理終了"));
						}
						else
						{
							if( WAIT_OBJECT_0 == WaitForSingleObject( hEvent, 0))
							{
								::SendMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_MESSAGE, ( LPARAM)TEXT( "取得処理が中断されました。"));
							}
						}
					}
				}
				LocalUnlock( hLocal);
				hLocal = NULL;
			}
		}
	}

errordetect:
	if( NULL != hWnd)::PostMessage( hWnd, WM_NOTIFYFROMTHREAD, TN_QUIT, blResult);
	if( NULL != hEvent)CloseHandle( hEvent);
	if( NULL != hLocal)LocalFree( hLocal);
	_endthreadex( 0);
	return 0;
}
