#include "StdeMail.h"
#include "NetEx_IF.h"
#include "email.h"
#include "emailslv.h"
#include "EntryList.h"
#include "GroupList.h"
#include "OrgSock.h"
#include "Base64.h"
#include "RStream.hpp"
#include "KanjiUtl.h"
#include "resource.h"
#include "Dialogs.h"
#include "MailS.h"

#ifdef	_POP3_TOO
#include "Mail.h"
#include "Pop3Client.h"
#endif

extern HANDLE			g_hModule;
extern EMAILORGDATA		g_stOrgData;
CRITICAL_SECTION g_csFileWrite;

BOOL ReadAddress( LPCSTR lpcszFileName, CGroupList* pcGroupList, CEntryList* pcEntryList)
{
	if( 0 >= lstrlen( lpcszFileName))return FALSE;
	if( NULL == pcGroupList)return FALSE;
	if( NULL == pcEntryList)return FALSE;

	FILE*	pFile;

	char	lpszFileName[ _MAX_PATH];
	char*	p;
	DWORD dwResult = SearchPath( NULL, lpcszFileName, NULL, _MAX_PATH, lpszFileName, &p);
	if( 0 == dwResult)
	{
		lstrcpy( lpszFileName, lpcszFileName);
#ifdef	_DEBUG
		LPVOID lpMsgBuf;
		DWORD dwError = GetLastError();
		FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						dwError,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);

		OutputDebugString( ( LPCSTR)lpMsgBuf);
		OutputDebugString( "\n");

		LocalFree( lpMsgBuf );
#endif
	}

#ifdef	_DEBUG
	OutputDebugString( lpszFileName);
	OutputDebugString( "\n");
#endif

	pFile = fopen( lpszFileName, "r");
	if( NULL != pFile)
	{
		char	szData[ 512];
		int		nCurrentGroup = -1;

		while( fgets( szData, 512, pFile))
		{
			if( 0 < strlen( szData))
			{
				char*	pWork = &szData[ strspn( szData, "\r\n\t ")];
				if( '#' == pWork[0])continue;
				//if( NULL != strchr( szData, '@'))
				if( NULL != _mbschr( ( const unsigned char *)szData, '@'))
				{
					int nLen = lstrlen( pWork) - 1;
					for( ; nLen >= 0; nLen--)
					{
						if( '\r' == pWork[ nLen] ||
							'\n' == pWork[ nLen] ||
							'\t' == pWork[ nLen] ||
							' ' == pWork[ nLen] || 
							',' == pWork[ nLen])
						{
							pWork[ nLen] = '\0';
							continue;
						}
						break;
					}

					if( strlen( pWork))
					{
#ifdef	_DEBUG
						OutputDebugString( pWork);
						OutputDebugString( "\n");
#endif
						pcEntryList->AddEntry( pWork, nCurrentGroup);
					}
#ifdef	_DEBUG
					else
					{
						OutputDebugString( "ERROR\r\n");
						DebugBreak();
					}
#endif
				}
				else
				{
					int nLen = lstrlen( pWork) - 1;
					for( ; nLen >= 0; nLen--)
					{
						if( '\r' == pWork[ nLen] ||
							'\n' == pWork[ nLen] ||
							'\t' == pWork[ nLen] ||
							' ' == pWork[ nLen] || 
							',' == pWork[ nLen])
						{
							pWork[ nLen] = '\0';
							continue;
						}
						break;
					}
					nLen = lstrlen( pWork) - 1;
					if( ':' == pWork[ nLen])
					{
						pWork[ nLen] = 0;
						nCurrentGroup = pcGroupList->AddGroup( pWork);
					}
					else
					{
						pcGroupList->AddGroup( pWork, nCurrentGroup);
					}
				}
			}
		}
		fclose( pFile);
		return TRUE;
	}
	return FALSE;
}

int AsyncSend( SOCKET sClient, char* pszData, HANDLE hStopEvent, HANDLE hCancelEvent)
{
	int	nResult = _SEND_SUCCESS;

	HANDLE	hSyncEvent = NULL;
	hSyncEvent = CreateEvent( NULL, FALSE, FALSE, NULL);

	HANDLE	ahEvents[ 3];
	ahEvents[ 0] = hStopEvent;
	ahEvents[ 1] = hSyncEvent;
	ahEvents[ 2] = hCancelEvent;
	DWORD	dwEventCount;
	dwEventCount = ( NULL != hCancelEvent) ? 3 : 2;

	Org_WSAEventSelect( sClient, hSyncEvent, FD_WRITE | FD_CLOSE);
	if( SOCKET_ERROR == send( sClient, pszData, lstrlen( pszData), 0))
	{
		nResult = _SEND_ERROR;
	}
	else
	{
		DWORD	dwResult;
		// 応答受信
		dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, _SMTP_TIMEOUT, FALSE);
		if( WSA_WAIT_TIMEOUT == dwResult)
		{
			nResult = _SEND_TIMEOUT;
		}
		if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
		{
			nResult = _SEND_FORCEDEND;
		}
		else
		{
			if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
			{
				nResult = _SEND_SUCCESS;
			}
			else
			{
				if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					nResult = _SEND_CANCEL;
				}
			}
		}
	}

	if( NULL != hSyncEvent)
	{
		Org_WSAEventSelect( sClient, hSyncEvent, 0);
		CloseHandle( hSyncEvent);
	}

	return nResult;
}

LPCSTR	pcszAsyncResultCheck = "AsyncResultCheck";

int AsyncResultCheck( SOCKET sClient, char* pszData, HANDLE hStopEvent, HANDLE hCancelEvent)
{
	if( g_stOrgData.blLogging)OutputErrorString( "ENTER", pcszAsyncResultCheck);

	int nResult = _SEND_SUCCESS;

	HANDLE	hSyncEvent = NULL;
	hSyncEvent = CreateEvent( NULL, FALSE, FALSE, NULL);

	HANDLE	ahEvents[ 3];
	ahEvents[ 0] = hStopEvent;
	ahEvents[ 1] = hSyncEvent;
	ahEvents[ 2] = hCancelEvent;
	DWORD	dwEventCount;
	dwEventCount = ( NULL != hCancelEvent) ? 3 : 2;

	char*	lpszData = NULL;

	Org_WSAEventSelect( sClient, hSyncEvent, FD_READ | FD_CLOSE);

	DWORD		dwResult;
	ULONG		ulSize;
	int			nReceiveLen;
	CRStream	cRstream;
	// 応答受信
	while( true)
	{
		dwResult = Org_WSAWaitForMultipleEvents( dwEventCount, ahEvents, FALSE, _SMTP_TIMEOUT, FALSE);
		if( WSA_WAIT_TIMEOUT == dwResult)
		{
			if( g_stOrgData.blLogging)OutputErrorString( "TIMEOUT!", pcszAsyncResultCheck);
			nResult = _SEND_TIMEOUT;
			break;
		}
		else
		{
			if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
			{
				nResult = _SEND_FORCEDEND;
				break;
			}
			else
			{
				if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
				{
					WSANETWORKEVENTS	stNetEvents;;
					ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
					Org_WSAEnumNetworkEvents( sClient, hSyncEvent, &stNetEvents);
					if( FD_CLOSE & stNetEvents.lNetworkEvents)goto cleanup;

					// 受信データサイズの取得
					ioctlsocket( sClient, FIONREAD, &ulSize);

					ulSize += 10;
					lpszData = new char [ ulSize];
					ZeroMemory( lpszData, ulSize);
					ulSize = recv( sClient, lpszData, ulSize, 0);
					lpszData[ ulSize] = 0;
					nReceiveLen = cRstream.AddReceiveString( lpszData);
					delete [] lpszData;
					lpszData = NULL;

					if( 4 <= nReceiveLen)	// 4 = lstrlen( "XXX "); XXX は result codeね
					{
						bool blChecked = false;
						do
						{
							lpszData = new char [ nReceiveLen + 1];
							cRstream.GetHeadString( lpszData, nReceiveLen + 1);
							if( g_stOrgData.blLogging)
							{
								char	szError[ 128];
								wsprintf( szError, "Result=%s", lpszData);
								OutputErrorString( szError, pcszAsyncResultCheck);
							}
							if( ' ' == lpszData[ 3])
							{
								nResult = strncmp( lpszData, pszData, lstrlen( pszData)) ? _SEND_ERROR : _SEND_SUCCESS;
								blChecked = true;
								if( g_stOrgData.blLogging)
								{
									char	szError[ 128];
									wsprintf( szError, "Check=[%s]", pszData);
									OutputErrorString( szError, pcszAsyncResultCheck);
								}
								break;
							}
							delete [] lpszData;
							lpszData = NULL;

							nReceiveLen = cRstream.GetHeadStringLength();
						}while( 0 < nReceiveLen);
						if( true == blChecked)break;

						/* マルチラインに対応していなかったですよ……テヘ
						lpszData = new char [ nReceiveLen + 1];
						cRstream.GetHeadString( lpszData, nReceiveLen + 1);
						nResult = strncmp( lpszData, pszData, lstrlen( pszData)) ? _SEND_ERROR : _SEND_SUCCESS;

						if( g_stOrgData.blLogging)
						{
							char	szError[ 128];
							wsprintf( szError, "Result=%s:Check=%s", lpszData, pszData);
							OutputErrorString( szError, pcszAsyncResultCheck);
						}
						break;
						*/
					}
				}
				else
				{
					if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
					{
						nResult = _SEND_CANCEL;
						break;
					}
				}
			}
		}
	}

cleanup:
	if( NULL != hSyncEvent)
	{
		Org_WSAEventSelect( sClient, hSyncEvent, 0);
		CloseHandle( hSyncEvent);
	}
	if( lpszData)
	{
		delete [] lpszData;
		lpszData = NULL;
	}

	if( g_stOrgData.blLogging)OutputErrorString( "REUTRN", pcszAsyncResultCheck);

	return nResult;
}

int AcyncSendBase64Encode( SOCKET sClient, LPCSTR lpcszHeader, LPCSTR lpcszData, HANDLE hStopEvent, HANDLE hCancelEvent)
{
	int		nResult = -1;

	char	szEncoded[ 256];
	char	szNormalWork[ 256];
	int		nNormalWork = 0;
	int		nLen = lstrlen( lpcszData);

	lstrcpy( szNormalWork, lpcszHeader);
	nNormalWork = lstrlen( szNormalWork);
	int nIndex = 0;

	while( TRUE)
	{
		////////////////////////////////////////////////////////////////////////
		// 与えられた文字列から適度な長さの文字列を拾い出す
		//
		// 漢字かな？
		if( _ismbblead( lpcszData[ nIndex]) && _ismbbtrail( lpcszData[ nIndex + 1]))
		{
			// おっ、一応漢字みたいだ
			// なので、漢字つながりで文字列を拾い出してくる
			nIndex += MimeEncode( szEncoded, lpcszData, nIndex, _MAX_SUBJECT_LEN);
		}
		else
		{
			// ASCII
			// なので、ASCIIつながりで文字列を拾い出してくる
			ZeroMemory( szEncoded, 256);
			for( int nEncodeIndex = 0; nEncodeIndex < _MAX_SUBJECT_LEN; nEncodeIndex++)
			{
				// 漢字かな？
				if( _ismbblead( lpcszData[ nIndex]) && _ismbbtrail( lpcszData[ nIndex + 1]))
				{
					// 漢字が出たので取得処理は終える
					break;
				}
				szEncoded[ nEncodeIndex] = lpcszData[ nIndex];
				nIndex++;
				// 所謂特殊文字で優先区切りとする。
				// TO/FROM以外の処理では意味ないが、実害も無いのでよしとするか。
				if( NULL != strchr( "@() ;,", lpcszData[ nIndex - 1]))break;
			}
		}
		//
		// 与えられた文字列から適度な長さの文字列を拾い出す
		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// 取り出した文字列を送信エリアに格納
		//
		// 新たに発生した文字列を追加すると送信文字列が長くなりすぎる？
		if( _MAX_SUBJECT_LEN < ( lstrlen( szNormalWork) + lstrlen( szEncoded)))
		{
			// 未送信データが有れば
			//
			// なぜ１以上であるかというとですな、最初に強制的に' 'を入れちゃっているンです
			// 本当はダサダサって思うんだけど、我慢しよう。
			// 後から挿入することにすると、ワークエリアが必要になっちゃうから……
			if( 1 < nNormalWork)
			{
				lstrcat( szNormalWork, "\r\n");
				nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
				if( 0 > nResult)goto cleanup;
			}
			ZeroMemory( szNormalWork, 256);
			szNormalWork[ 0] = ' ';
			nNormalWork = 1;
		}
		// 新規発生分を既存文字列に追加
		lstrcat( szNormalWork, szEncoded);
		nNormalWork = lstrlen( szNormalWork);
		//
		// 取り出した文字列を送信エリアに格納
		////////////////////////////////////////////////////////////////////////


		////////////////////////////////////////////////////////////////////////
		// 未処理データが無くなれば残りを送信して終わる
		//
		if( nLen <= nIndex)
		{
			// なぜ１以上であるかというとですな、最初に強制的に' 'を入れちゃっているンです
			// 本当はダサダサって思うんだけど、我慢しよう。
			// 後から挿入することにすると、ワークエリアが必要になっちゃうから……
			if( 1 < nNormalWork)
			{
				lstrcat( szNormalWork, "\r\n");
				nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
				if( 0 > nResult)goto cleanup;
			}
			break;
		}
		//
		// 未処理データが無くなれば残りを送信して終わる
		////////////////////////////////////////////////////////////////////////
	}
	nResult = 0;

cleanup:
	return nResult;

/* 旧版のコード上記コードはこのコードが基に……
	while( TRUE)
	{
		// 漢字かな？
		if( _ismbblead( lpcszData[ nIndex]))
		{
			// おっ、一応漢字みたいだ
			if( _ismbbtrail( lpcszData[ nIndex + 1]))
			{
				nIndex += MimeEncode( szEncoded, lpcszData, nIndex, _MAX_SUBJECT_LEN);
				if( _MAX_SUBJECT_LEN < ( lstrlen( szNormalWork) + lstrlen( szEncoded)))
				{
					if( lstrlen( szNormalWork))
					{
						lstrcat( szNormalWork, "\r\n");
						nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
						if( 0 > nResult)goto cleanup;
					}
					ZeroMemory( szNormalWork, 256);
					szNormalWork[ 0] = ' ';
					nNormalWork = 1;
					lstrcat( szNormalWork, szEncoded);
					lstrcat( szNormalWork, "\r\n");
					nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
					if( 0 > nResult)goto cleanup;
					ZeroMemory( szNormalWork, 256);
					szNormalWork[ 0] = ' ';
					nNormalWork = 1;
					continue;
//					lstrcat( szEncoded, "\r\n");
//					nResult = AsyncSend( sClient, szEncoded, hStopEvent, hCancelEvent);
//					if( 0 > nResult)goto cleanup;
//					ZeroMemory( szNormalWork, 256);
//					szNormalWork[ 0] = ' ';
//					nNormalWork = 1;
//					continue;
				}
				else
				{
					lstrcat( szNormalWork, szEncoded);
					nNormalWork = lstrlen( szNormalWork);
					continue;
				}
			}
		}

		ZeroMemory( szEncoded, 256);
		for( int nEncodeIndex = 0; nEncodeIndex < _MAX_SUBJECT_LEN; nEncodeIndex++)
		{
			// 漢字かな？
			if( _ismbblead( lpcszData[ nIndex]))
			{
				// おっ、一応漢字みたいだ
				if( _ismbbtrail( lpcszData[ nIndex + 1]))
				{
					break;
				}
			}
			szEncoded[ nEncodeIndex] = lpcszData[ nIndex];
			nIndex++;
			if( NULL != strchr( "@() ;,", lpcszData[ nIndex - 1]))break;
		}

		if( _MAX_SUBJECT_LEN < ( lstrlen( szNormalWork) + lstrlen( szEncoded)))
		{
			if( lstrlen( szNormalWork))
			{
				lstrcat( szNormalWork, "\r\n");
				nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
				if( 0 > nResult)goto cleanup;
			}
			ZeroMemory( szNormalWork, 256);
			szNormalWork[ 0] = ' ';
			nNormalWork = 1;
			lstrcat( szNormalWork, szEncoded);
			nNormalWork = lstrlen( szNormalWork);
//			lstrcat( szNormalWork, "\r\n");
//			nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
//			if( 0 > nResult)goto cleanup;
//			ZeroMemory( szNormalWork, 256);
//			szNormalWork[ 0] = ' ';
//			nNormalWork = 1;
		}
		else
		{
			lstrcat( szNormalWork, szEncoded);
			nNormalWork = lstrlen( szNormalWork);
		}
		if( nLen <= nIndex)
		{
			// なぜ１以上であるかというとですな、最初に強制的に' 'を入れちゃっているンです
			// 本当はダサダサって思うんだけど、我慢しよう。
			// 後から挿入することにすると、ワークエリアが必要になっちゃうから……
			if( 1 < lstrlen( szNormalWork))
			{
				lstrcat( szNormalWork, "\r\n");
				nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
				if( 0 > nResult)goto cleanup;
				ZeroMemory( szNormalWork, 256);
				szNormalWork[ 0] = ' ';
				nNormalWork = 1;
			}
			break;
		}

//		szNormalWork[ nNormalWork] = lpcszData[ nIndex];
//		nNormalWork++;
//		nIndex++;
//		if( _MAX_SUBJECT_LEN < lstrlen( szNormalWork) || nLen < nIndex)
//		{
//			// なぜ１以上であるかというとですな、最初に強制的に' 'を入れちゃっているンです
//			// 本当はダサダサって思うんだけど、我慢しよう。
//			// 後から挿入することにすると、ワークエリアが必要になっちゃうから……
//			if( 1 < lstrlen( szNormalWork))
//			{
//				lstrcat( szNormalWork, "\r\n");
//				nResult = AsyncSend( sClient, szNormalWork, hStopEvent, hCancelEvent);
//				if( 0 > nResult)goto cleanup;
//				ZeroMemory( szNormalWork, 256);
//				szNormalWork[ 0] = ' ';
//				nNormalWork = 1;
//			}
//		}
//		if( nLen < nIndex)break;
	}
	nResult = 0;

cleanup:
	return nResult;
	*/
}

int AsyncSendData( SOCKET sClient, CMailS& cMail, HANDLE hStopEvent, HANDLE hCancelEvent)
{
	int	nResult = _SEND_SUCCESS;
	int nIndex;

#ifdef	_POP3_TOO
	BOOL	blMultiPart = ( 0 < cMail.GetRawDataSize());
#else
	BOOL	blMultiPart = FALSE;
#endif

	char	szDelimiter[ 70];
	LPSTR	lpszExchangeBodyWork = NULL;
	LPSTR	lpszExchangeSubjectWork = NULL;
	int		nExchangeWork;
	LPCSTR	lpcszBodyWorkPointer = cMail.GetBody();
	LPCSTR	lpcszSubjectWorkPointer = cMail.GetSubject();
	// 半角からの変換と任意文字数への改行ナリ！：Subject
	nExchangeWork = CheckExchangeBodyLen( lpcszSubjectWorkPointer, TRUE);
	if( 0 < nExchangeWork)
	{
		lpszExchangeSubjectWork = new char [ nExchangeWork + 1];
		if( 0 < ExchangeBody( lpszExchangeSubjectWork, nExchangeWork + 1, lpcszSubjectWorkPointer, TRUE))
		{
			lpcszSubjectWorkPointer = lpszExchangeSubjectWork;
		}
	}
	// 半角からの変換と任意文字数への改行ナリ！：本文
	nExchangeWork = CheckExchangeBodyLen( lpcszBodyWorkPointer);
	if( 0 < nExchangeWork)
	{
		lpszExchangeBodyWork = new char [ nExchangeWork + 1];
		if( 0 < ExchangeBody( lpszExchangeBodyWork, nExchangeWork + 1, lpcszBodyWorkPointer))
		{
			lpcszBodyWorkPointer = lpszExchangeBodyWork;
		}
	}

	int nBodySize = CheckSjis2JisLen( lpcszBodyWorkPointer);
	char*	pBody = NULL;
	if( 0 < nBodySize)
	{
		pBody = new char [ nBodySize + 1];
		Sjis2Jis( pBody, nBodySize + 1, lpcszBodyWorkPointer);
	}

	char szWork[ 512];

	// MIME送信
	lstrcpy( szWork, "MIME-Version: 1.0\r\n");
	nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
	if( 0 > nResult)goto cleanup;
	// Content-Type送信
	if( blMultiPart)
	{
		// デリミタ作成
		do
		{
			wsprintf( szDelimiter, "SOboe_email%X", GetTickCount());
		}while( strstr( lpcszBodyWorkPointer, szDelimiter));
		wsprintf( szWork, "Content-Type: multipart/mixed; boundary=\"%s\"\r\n", szDelimiter);
	}
	else
	{
		lstrcpy( szWork, "Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
	}
	nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
	if( 0 > nResult)goto cleanup;

	// From送信
	nResult = AcyncSendBase64Encode( sClient, "From: ", cMail.GetFromAddress(), hStopEvent, hCancelEvent);
	if( 0 > nResult)goto cleanup;
	// To送信
	for( nIndex = 0; nIndex < cMail.GetToCount(); nIndex++)
	{
		lstrcpy( szWork, cMail.GetToAddressRaw( nIndex));
		if( ( nIndex + 1) < cMail.GetToCount())
		{
			lstrcat( szWork, ",");
		}
		nResult = AcyncSendBase64Encode( sClient, ( 0 == nIndex) ? "To: " : "    ", szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;
	}

	// 送信時間の送信
	{
		char*	apszDayOfWeek[ 7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		char*	apszMonth[ 12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		TIME_ZONE_INFORMATION	stTimeZone;
		GetTimeZoneInformation( &stTimeZone);
		wsprintf( szWork, "%s, %d %s %d %02d:%02d:%02d %c%02d%02d", apszDayOfWeek[ stSysTime.wDayOfWeek], stSysTime.wDay, apszMonth[ stSysTime.wMonth - 1], stSysTime.wYear, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, ( 0 > stTimeZone.Bias) ? '+' : '-' ,abs( stTimeZone.Bias / 60), abs( stTimeZone.Bias % 60));
		nResult = AcyncSendBase64Encode( sClient, "Date: ", szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;
	}

	// SUBJECTの送信だよ！
	nResult = AcyncSendBase64Encode( sClient, "Subject: ", lpcszSubjectWorkPointer, hStopEvent, hCancelEvent);
	if( 0 > nResult)goto cleanup;

	// X-Mailer送信
	lstrcpy( szWork, "X-Mailer: OboeGaki-email.nxl Ver.0.06\r\n\r\n");
	nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
	if( 0 > nResult)goto cleanup;

	if( blMultiPart)
	{
		lstrcpy( szWork, "This is a MIME multipart message.\r\n\r\n");
		nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;

		wsprintf( szWork, "--%s\r\n", szDelimiter);
		lstrcat( szWork, "Content-Type: text/plain; charset=\"iso-2022-jp\"\r\n");
		lstrcat( szWork, "Content-Transfer-Encoding: 7bit\r\n\r\n");
		nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;
	}

	int nWorkPos;
	nWorkPos = 0;
	ZeroMemory( szWork, 512);
	BOOL	blMaxChar;
	blMaxChar = FALSE;
	for( nIndex = 0; nIndex <= lstrlen( pBody); nIndex++)
	{
		if( '\r' == pBody[ nIndex])continue;
		if( '\n' == pBody[ nIndex] || '\0' == pBody[ nIndex] || FALSE != blMaxChar || nWorkPos >= 509)
		{
			if( '.' == szWork[ 0] && '\0' == szWork[ 1])
			{
				szWork[ nWorkPos] = '.';
				nWorkPos++;
			}
			lstrcat( szWork, "\r\n");
			nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
			if( 0 > nResult)goto cleanup;
			ZeroMemory( szWork, 512);
			nWorkPos = 0;
			blMaxChar = FALSE;
			continue;
		}
		szWork[ nWorkPos] = pBody[ nIndex];
		nWorkPos++;
	}

	if( blMultiPart)
	{
		wsprintf( szWork, "\r\n--%s\r\n", szDelimiter);
		lstrcat( szWork, "Content-Type: application/octet-stream; name=\"SOboeChip.bin\"\r\n");
		lstrcat( szWork, "Content-Transfer-Encoding: Base64\r\n");
		lstrcat( szWork, "Content-Disposition: attachment; filename=\"SOboeChip.bin\"\r\n\r\n");
		nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;

		int nLength = Base64Encode( NULL, ( const BYTE*)cMail.GetRawData(), cMail.GetRawDataSize());
		char*	pszBase64 = new char [ nLength + 1];
		Base64Encode( pszBase64, ( const BYTE*)cMail.GetRawData(), cMail.GetRawDataSize());
		int	nLine = 0;
		for( int nIndex = 0; nIndex < nLength + 1; nIndex++)
		{
			szWork[ nLine] = pszBase64[ nIndex];
			nLine++;
			if( 64 <= nLine || 0 == pszBase64[ nIndex + 1])
			{
				szWork[ nLine] = '\r';
				nLine++;
				szWork[ nLine] = '\n';
				nLine++;
				szWork[ nLine] = 0;
				nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
				if( 0 > nResult)goto cleanup;
				nLine = 0;
			}
		}

		wsprintf( szWork, "\r\n--%s--\r\n", szDelimiter);
		nResult = AsyncSend( sClient, szWork, hStopEvent, hCancelEvent);
		if( 0 > nResult)goto cleanup;
	}

	nResult = _SEND_SUCCESS;

cleanup:
	if( pBody)
	{
		delete [] pBody;
		pBody = NULL;
	}
	if( lpszExchangeBodyWork)
	{
		delete [] lpszExchangeBodyWork;
		lpszExchangeBodyWork = NULL;
	}
	if( lpszExchangeSubjectWork)
	{
		delete [] lpszExchangeSubjectWork;
		lpszExchangeSubjectWork = NULL;
	}
	return nResult;
}

// blSubjectはSubjectの変換用さ！
int CheckExchangeBodyLen( LPCSTR lpcszSrc, BOOL blSubject/*=FALSE*/)
{
	int		nLen = lstrlen( lpcszSrc);
	BOOL	blNowKanji = FALSE;
	BOOL	bl2ndCode;
	int		nCharCount = 0;
	BOOL	blNoCount = FALSE;

	int nResult = nLen;

	if( '>' == lpcszSrc[ 0] || !g_stOrgData.blFormatting)blNoCount = TRUE;
	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		if( g_stOrgData.blFormatting && !blSubject)
		{
			if( g_stOrgData.nCharCount <= nCharCount)
			{
				nResult += 2;	// 2 = lstrlen( "\r\n");
				nCharCount = 0;
			}
			if( '\r' == lpcszSrc[ nIndex])continue;
			if( '\n' == lpcszSrc[ nIndex])
			{
				nCharCount = 0;
				blNoCount = FALSE;
				if( '>' == lpcszSrc[ nIndex + 1])blNoCount = TRUE;
			}
		}
		// 漢字かな？
		if( _ismbblead( lpcszSrc[ nIndex]))
		{
			// おっ、一応漢字みたいだ
			if( _ismbbtrail( lpcszSrc[ nIndex + 1]))
			{
				if( !blNoCount && !blSubject)nCharCount += 2;
				nIndex++;
				continue;
			}
		}
		if( g_stOrgData.blMbbtombc)
		{
			if( 0xa0 <= ( BYTE)lpcszSrc[ nIndex] && 0xdf >= ( BYTE)lpcszSrc[ nIndex])
			{
				bl2ndCode = FALSE;
				if( 0 < nIndex)
				{
					bl2ndCode = _ismbblead( lpcszSrc[ nIndex - 1]);
				}
				if( FALSE == bl2ndCode)
				{
					if( !blNoCount)nCharCount += 2;
					nResult++;
					continue;
				}
			}
		}
		if( !blNoCount && !blSubject)nCharCount++;
	}

	return nResult + 1;
}

// blSubjectはSubjectの変換用さ！
int ExchangeBody( LPSTR lpszDist, int nSize, LPCSTR lpcszSrc, BOOL blSubject/*=FALSE*/)
{
	BOOL	blNowKanji = FALSE;
	int		nLen = lstrlen( lpcszSrc);
	int		nWritePos = 0;
	BOOL	bl2ndCode = FALSE;

	int		nCharCount = 0;
	BOOL	blNoCount = FALSE;

	ZeroMemory( lpszDist, nSize);
	if( '>' == lpcszSrc[ 0] || !g_stOrgData.blFormatting)blNoCount = TRUE;
	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		if( g_stOrgData.blFormatting && !blSubject)
		{
			if( '\r' == lpcszSrc[ nIndex])continue;
			if( '\n' == lpcszSrc[ nIndex])
			{
				nCharCount = 0;
				blNoCount = FALSE;
				if( '>' == lpcszSrc[ nIndex + 1])blNoCount = TRUE;
			}
			if( g_stOrgData.nCharCount <= nCharCount)
			{
				if( ( nWritePos + 2) >= nSize)goto memory_size_error;
				lpszDist[ nWritePos] = '\r';
				nWritePos++;
				lpszDist[ nWritePos] = '\n';
				nWritePos++;
				nCharCount = 0;
				if( '>' == lpcszSrc[ nIndex])blNoCount = TRUE;
			}
		}
		// 漢字かな？
		if( _ismbblead( lpcszSrc[ nIndex]))
		{
			// おっ、一応漢字みたいだ
			if( _ismbbtrail( lpcszSrc[ nIndex + 1]))
			{
				if( ( nWritePos + 1) >= nSize)goto memory_size_error;
				lpszDist[ nWritePos] = lpcszSrc[ nIndex];
				nWritePos++;
				lpszDist[ nWritePos] = lpcszSrc[ nIndex + 1];
				nWritePos++;
				if( !blNoCount && !blSubject)nCharCount += 2;
				nIndex++;
				continue;
			}
		}
		if( g_stOrgData.blMbbtombc)
		{
			if( 0xa0 <= ( BYTE)lpcszSrc[ nIndex] && 0xdf >= ( BYTE)lpcszSrc[ nIndex])
			{
				bl2ndCode = FALSE;
				if( 0 < nIndex)
				{
					bl2ndCode = _ismbblead( lpcszSrc[ nIndex - 1]);
				}
				if( FALSE == bl2ndCode)
				{
					WORD wChar = ( BYTE)lpcszSrc[ nIndex];
					wChar = _mbbtombc( wChar);

					if( ( nWritePos + 2) >= nSize)goto memory_size_error;
					lpszDist[ nWritePos] = HIBYTE( wChar);
					nWritePos++;
					lpszDist[ nWritePos] = LOBYTE( wChar);
					nWritePos++;
					if( !blNoCount)nCharCount += 2;
					continue;
				}
			}
		}
		if( ( nWritePos + 1) >= nSize)goto memory_size_error;
		lpszDist[ nWritePos] = lpcszSrc[ nIndex];
		nWritePos++;
		if( !blNoCount && !blSubject)nCharCount++;
	}

	return lstrlen( lpszDist);
memory_size_error:
	ZeroMemory( lpszDist, nSize);
	return CheckSjis2JisLen( lpcszSrc);
}

void OutputErrorString( LPCSTR lpcszErrMsg, LPCSTR lpcstrFunction)
{
	EnterCriticalSection( &g_csFileWrite);

	char	szPath[ _MAX_PATH];
	if( GetModuleFileName( ( HINSTANCE)g_hModule, szPath, _MAX_PATH))
	{
		char drive[ _MAX_DRIVE];
		char dir[ _MAX_DIR];
		char fname[ _MAX_FNAME];
		_splitpath( szPath, drive, dir, fname, NULL);
		_makepath( szPath, drive, dir, fname, "err");

		//HFILE		hFile;
		//OFSTRUCT	of;
		//hFile = OpenFile( szPath, &of, OF_CREATE | OF_WRITE | OF_PROMPT);
		HANDLE	hFile;
		hFile = CreateFile( szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
		if( INVALID_HANDLE_VALUE != hFile)
		{
			SYSTEMTIME stSysTime;
			GetLocalTime( &stSysTime);

			char	szWork[ 64];
			wsprintf( szWork, "[%02d/%02d %02d:%02d:%02d] - ", stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond);

			char*	pszCRLF = "\r\n";
			char*	pszIN = " : ";
			DWORD	dwDummy;

			SetFilePointer( hFile, 0, NULL, FILE_END);
			WriteFile( hFile, szWork, lstrlen( szWork), &dwDummy, NULL);
			WriteFile( hFile, lpcszErrMsg, lstrlen( lpcszErrMsg), &dwDummy, NULL);
			WriteFile( hFile, pszIN, lstrlen( pszIN), &dwDummy, NULL);
			WriteFile( hFile, lpcstrFunction, lstrlen( lpcstrFunction), &dwDummy, NULL);
			WriteFile( hFile, pszCRLF, lstrlen( pszCRLF), &dwDummy, NULL);
			CloseHandle( hFile);
		}
	}

#ifdef	_DEBUG
	OutputDebugString( lpcszErrMsg);
	OutputDebugString( " : ");
	OutputDebugString( lpcstrFunction);
	OutputDebugString( "\r\n");
#endif

	LeaveCriticalSection( &g_csFileWrite);
}

static LPCSTR	pcszSendMail = "SendMail";

int SendMail( CMailS& cMail, HANDLE hCancelEvent)
{
	int nResult = _SEND_ERROR;
	if( g_stOrgData.blInitialized)
	{
		ULONG				ulSmtpAddress = INADDR_NONE;
		SOCKET				sClient = INVALID_SOCKET;
		HANDLE				hWsaCnctRcv = NULL;
		struct sockaddr_in	stSmtpSrvr;
		char				szWork[ 1024];
		PHOSTENT			pHost;
		int					nIndex;
		CRStream			cRstream;

		////////////////////////////////////////////////////////
		//
		// 初期化開始
		//
		if( g_stOrgData.blLogging)OutputErrorString( "初期化開始", pcszSendMail);
		sClient = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( INVALID_SOCKET == sClient)
		{
			goto cleanup;
		}
		hWsaCnctRcv = CreateEvent( NULL, FALSE, FALSE, NULL);
		if( NULL == hWsaCnctRcv)goto cleanup;
		// サーバのアドレス取得
		if( g_stOrgData.blLogging)OutputErrorString( "サーバのアドレス取得開始", pcszSendMail);
		pHost = gethostbyname( g_stOrgData.szSmtpServerName);
		if( pHost)
		{
			if( AF_INET == pHost->h_addrtype && sizeof( unsigned long) <= pHost->h_length)
			{
				CopyMemory( &ulSmtpAddress, *pHost->h_addr_list, sizeof( unsigned long));
			}
		}
		if( g_stOrgData.blLogging)
		{
			char	szError[ 128];
			wsprintf( szError, "SMTP[%s:0x%X]", g_stOrgData.szSmtpServerName, ulSmtpAddress);
			OutputErrorString( szError, pcszSendMail);
		}
		if( INADDR_NONE == ulSmtpAddress)
		{
#if(_WIN32_WINNT < 0x0400)
			ulSmtpAddress = inet_addr( g_stOrgData.szSmtpServerName);
			if( INADDR_NONE == ulSmtpAddress)
			{
				nResult = _NOTFOUND_SERVER;
				goto cleanup;
			}
#else
			nResult = _NOTFOUND_SERVER;
			goto cleanup;
#endif
		}
		if( g_stOrgData.blLogging)OutputErrorString( "初期化終了", pcszSendMail);
		//
		// 初期化終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// コネクト開始
		//
		if( g_stOrgData.blLogging)OutputErrorString( "コネクト開始", pcszSendMail);

		{
			HANDLE	ahEvents[ 3];
			DWORD	dwResult;

			Org_WSAEventSelect( sClient, hWsaCnctRcv, FD_CONNECT | FD_CLOSE);
			ahEvents[ 0] = g_stOrgData.hStopEvent;
			ahEvents[ 1] = hWsaCnctRcv;
			ahEvents[ 2] = hCancelEvent;

			stSmtpSrvr.sin_family			= PF_INET;
			stSmtpSrvr.sin_port				= g_stOrgData.unSmtpPort;
			stSmtpSrvr.sin_addr.S_un.S_addr	= ulSmtpAddress;
			if( SOCKET_ERROR == connect( sClient, ( struct sockaddr*)&stSmtpSrvr, sizeof( struct sockaddr)))
			{
				if( WSAEWOULDBLOCK != WSAGetLastError())
				{
					goto cleanup;
				}
			}
			// コネクトの確認
			dwResult = Org_WSAWaitForMultipleEvents( ( ( NULL != hCancelEvent) ? 3 : 2), ahEvents, FALSE, _SMTP_TIMEOUT, FALSE);
			if( WSA_WAIT_TIMEOUT == dwResult)
			{
				nResult = _NOTFOUND_SERVER;
				goto cleanup;
			}
			if( 0 == ( dwResult - WSA_WAIT_EVENT_0))
			{
				nResult = _SEND_FORCEDEND;
				goto cleanup;
			}
			if( 1 == ( dwResult - WSA_WAIT_EVENT_0))
			{
				WSANETWORKEVENTS	stNetEvents;;
				ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
				Org_WSAEnumNetworkEvents( sClient, hWsaCnctRcv, &stNetEvents);
				if( FD_CLOSE & stNetEvents.lNetworkEvents)
				{
					nResult = _SEND_ERROR;
					goto cleanup;
				}
			}
			if( 2 == ( dwResult - WSA_WAIT_EVENT_0))
			{
				nResult = _SEND_CANCEL;
				goto cleanup;
			}
		}
		if( g_stOrgData.blLogging)OutputErrorString( "コネクト完了", pcszSendMail);
		//
		// コネクト終了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// 応答待ち
		//
		if( g_stOrgData.blLogging)OutputErrorString( "応答待ち開始", pcszSendMail);
		nResult = AsyncResultCheck( sClient, "220", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "応答待ち終了", pcszSendMail);
		//
		// 応答待ち
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// HELO送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "HELO送信", pcszSendMail);
		wsprintf( szWork, "HELO %s\r\n", g_stOrgData.szMachineName);
		nResult = AsyncSend( sClient, szWork, g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		nResult = AsyncResultCheck( sClient, "250", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "HELO送信完了", pcszSendMail);
		//
		// HELO送信
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// MAILコマンド送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "MAILコマンド送信", pcszSendMail);
		wsprintf( szWork, "MAIL FROM:<%s>\r\n", g_stOrgData.szMailAddres);
		nResult = AsyncSend( sClient, szWork, g_stOrgData.hStopEvent, hCancelEvent);
		if( g_stOrgData.blLogging)OutputErrorString( szWork, pcszSendMail);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		nResult = AsyncResultCheck( sClient, "250", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "MAILコマンド送信完了", pcszSendMail);
		//
		// MAILコマンド送信
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// RCPTコマンド送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "RCPTコマンド送信", pcszSendMail);
		for( nIndex = 0; nIndex < cMail.GetToCount(); nIndex++)
		{
			wsprintf( szWork, "RCPT TO:<%s>\r\n", cMail.GetToAddress( nIndex));
			if( g_stOrgData.blLogging)OutputErrorString( szWork, pcszSendMail);
			nResult = AsyncSend( sClient, szWork, g_stOrgData.hStopEvent, hCancelEvent);
			if( _SEND_SUCCESS != nResult)goto cleanup;
			nResult = AsyncResultCheck( sClient, "250", g_stOrgData.hStopEvent, hCancelEvent);
			if( _SEND_SUCCESS != nResult)goto cleanup;
		}
		if( g_stOrgData.blWithMyAddress)
		{
			if( g_stOrgData.blLogging)OutputErrorString( "with my address in BCC", pcszSendMail);

			wsprintf( szWork, "RCPT TO:<%s>\r\n", g_stOrgData.szMailAddres);
			if( g_stOrgData.blLogging)OutputErrorString( szWork, pcszSendMail);
			nResult = AsyncSend( sClient, szWork, g_stOrgData.hStopEvent, hCancelEvent);
			if( _SEND_SUCCESS != nResult)goto cleanup;
			nResult = AsyncResultCheck( sClient, "250", g_stOrgData.hStopEvent, hCancelEvent);
			if( _SEND_SUCCESS != nResult)goto cleanup;
		}

		if( g_stOrgData.blLogging)OutputErrorString( "RCPTコマンド送信完了", pcszSendMail);
		//
		// RCPTコマンド送信
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// DATAコマンド送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "DATAコマンド送信", pcszSendMail);
		nResult = AsyncSend( sClient, "DATA\r\n", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		nResult = AsyncResultCheck( sClient, "354", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "DATAコマンド完了", pcszSendMail);
		//
		// DATAコマンド送信
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// DATA送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "DATA送信", pcszSendMail);
		nResult = AsyncSendData( sClient, cMail, g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "DATA完了", pcszSendMail);
		//
		// DATA送信
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// DATA送信完了
		//
		if( g_stOrgData.blLogging)OutputErrorString( "EOD[.]送信", pcszSendMail);
		nResult = AsyncSend( sClient, ".\r\n", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		nResult = AsyncResultCheck( sClient, "250", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "EOD[.]送信完了", pcszSendMail);
		//
		// DATA送信完了
		//
		////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////
		//
		// QUITコマンド送信
		//
		if( g_stOrgData.blLogging)OutputErrorString( "QUITコマンド送信", pcszSendMail);
		nResult = AsyncSend( sClient, "QUIT\r\n", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		nResult = AsyncResultCheck( sClient, "221", g_stOrgData.hStopEvent, hCancelEvent);
		if( _SEND_SUCCESS != nResult)goto cleanup;
		if( g_stOrgData.blLogging)OutputErrorString( "QUITコマンド送信完了", pcszSendMail);
		//
		// QUITコマンド送信
		//
		////////////////////////////////////////////////////////
		if( g_stOrgData.blLogging)OutputErrorString( "送信全行程終了", pcszSendMail);
		nResult = _SEND_SUCCESS;
cleanup:
		if( g_stOrgData.blLogging)OutputErrorString( "クリーンアップ処理", pcszSendMail);
		if( INVALID_SOCKET != sClient)
		{
			closesocket( sClient);
			sClient = INVALID_SOCKET;
		}
	}
	return nResult;
}