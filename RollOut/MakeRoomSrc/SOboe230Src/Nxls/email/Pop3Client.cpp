#include "StdeMail.h"
#include "OrgSock.h"
#include "Pop3Client.h"
#include "RStreamEx.h"

CPop3Client::CPop3Client( bool blApop) : m_sSocket( INVALID_SOCKET)
											, m_hAbortEvent( NULL)
											, m_hSockEvent( NULL)
											, m_ulTimeOunt( 60 * 1000)
											, m_nMessageCount( 0)
											, m_panMassageSize( NULL)
											, m_pszAPOPKey( NULL)
											, m_blAPOP( blApop)
{
}

CPop3Client::~CPop3Client( void)
{
	CleanUp();
}

BOOL CPop3Client::Create( void)
{
	m_sSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( INVALID_SOCKET == m_sSocket)
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		goto errordetect;
	}

	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	m_hSockEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	if( NULL == m_hSockEvent)goto errordetect;
	m_hAbortEvent = CreateEvent( NULL, FALSE, FALSE, NULL);
	if( NULL == m_hAbortEvent)goto errordetect;

	Org_WSAEventSelect( m_sSocket, m_hSockEvent, FD_CONNECT | FD_READ | FD_CLOSE);

	m_nMessageCount = 0;
	if( NULL != m_panMassageSize)
	{
		delete [] m_panMassageSize;
		m_panMassageSize = NULL;
	}

	return TRUE;

errordetect:
	CleanUp();
	throw "ソケット作成異常終了";
	return FALSE;
}

BOOL CPop3Client::Connect( LPCSTR lpcszServerName, UINT unPort)
{
	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";

	HANDLE		ahEvents[ 2];
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// 接続先の設定
	sockaddr_in	stAddr;
	stAddr.sin_family = AF_INET;
	stAddr.sin_port = htons( unPort);
	stAddr.sin_addr.S_un.S_addr = inet_addr( lpcszServerName);
	if( INADDR_NONE == stAddr.sin_addr.S_un.S_addr)
	{
		hostent* pHost = gethostbyname( lpcszServerName);
		if( pHost)
		{
			if( AF_INET == pHost->h_addrtype && sizeof( unsigned long) <= pHost->h_length)
			{
				CopyMemory( &stAddr.sin_addr.S_un.S_addr, *pHost->h_addr_list, sizeof( unsigned long));
			}
		}
		if( INADDR_NONE == stAddr.sin_addr.S_un.S_addr)throw "ホスト名をアドレスに変換できなかった";
	}

	///////////////////////////////////////////////////////////////
	// 接続実行
	if( SOCKET_ERROR == connect( m_sSocket, ( sockaddr*)&stAddr, sizeof( sockaddr)))
	{
		if( WSAEWOULDBLOCK != WSAGetLastError())
		{
			char	szWork[ 128];
			int nCode = WSAGetLastError();
			wsprintf( szWork, "EC:%d\n", nCode);
			OutputDebugString( szWork);
			throw "接続は失敗した模様";
		}
	}
	/* 
	///////////////////////////////////////////////////////////////
	// 接続結果待ち
	DWORD	dwResult;
	dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
	if( WAIT_OBJECT_0 == dwResult)
	{
		WSANETWORKEVENTS	stNetEvents;
		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( 0 == ( FD_CONNECT & stNetEvents.lNetworkEvents))throw "接続は失敗した模様";
	}
	*/

	LPSTR lpszResult;

	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult( m_blAPOP ? &lpszResult : NULL))throw "受信結果異常";

	///////////////////////////////////////////////////////////////
	// APOPキーの取得
	if( m_blAPOP && NULL != lpszResult)
	{
		bool blFind = false;
		for( int nIndex = lstrlen( lpszResult) - 1; nIndex >= 0; nIndex--)
		{
			if( false == blFind)
			{
				if( '>' == lpszResult[ nIndex])
				{
					lpszResult[ nIndex + 1] = 0;
					blFind = true;
				}
			}
			else if( '<' == lpszResult[ nIndex])
			{
				int nLength = lstrlen( &lpszResult[ nIndex]);
				m_pszAPOPKey = new char[ nLength + 1];
				lstrcpy( m_pszAPOPKey, &lpszResult[ nIndex]);
				break;
			}
		}
	}

	return TRUE;
}

#if(_WIN32_WINNT >= 0x0400)
LPSTR CPop3Client::CreateDigest( LPCSTR lpcszPass)
{
	LPSTR lpszWork = NULL;
	LPSTR lpszResult = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	LPBYTE lpbySigneture = NULL;

	try
	{
		lpszWork = new char[ lstrlen( m_pszAPOPKey) + lstrlen( lpcszPass) + 1];
		lstrcpy( lpszWork, m_pszAPOPKey);
		lstrcat( lpszWork, lpcszPass);

		// デフォルト鍵コンテナの取得
		if( !CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, 0))
		{
			if( NTE_BAD_KEYSET == GetLastError())
			{
				/* デフォルト鍵コンテナの生成 */
				if( !CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				{
					DWORD dwError = GetLastError();
					char szError[ 64];

					wsprintf( szError, "error 0x%08X", dwError);

					throw szError;
				}
			}
		}

		// ハッシュ値の生成
		if( !CryptCreateHash( hProv, CALG_MD5, 0, 0, &hHash))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		DWORD dwSize = lstrlen( lpszWork);
		if( !CryptHashData( hHash, reinterpret_cast<const BYTE*>( lpszWork), dwSize, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		DWORD dwSignLen;
		if( !CryptGetHashParam( hHash, HP_HASHVAL, NULL, &dwSignLen, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		lpbySigneture = new BYTE[ dwSignLen];
		if( !CryptGetHashParam( hHash, HP_HASHVAL, lpbySigneture, &dwSignLen, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		lpszResult = new char[ ( dwSignLen * 2) + 1];
		if( NULL != lpszResult)
		{
			for( DWORD dwIndex = 0; dwIndex < dwSignLen; dwIndex++)
			{
				wsprintf( &lpszResult[ ( dwIndex * 2)], "%02x", lpbySigneture[ dwIndex]);
			}
		}
	}
	catch( char* pszError)
	{
		OutputDebugString( pszError);
	}

	if( NULL != lpszWork)
	{
		delete [] lpszWork;
		lpszWork = NULL;
	}
	if( NULL != lpbySigneture)
	{
		delete [] lpbySigneture;
		lpbySigneture = NULL;
	}
	if( NULL != hHash)
	{
		CryptDestroyHash( hHash);
		hHash = NULL;
	}
	if( NULL != hProv)
	{
		CryptReleaseContext( hProv, 0);
		hProv = NULL;
	}

	return lpszResult;
}
#else

CAdvApi32::CAdvApi32( void) : m_hAdvapi32DLL( NULL)
								,m_lpfnCryptAcquireContext( NULL)
								,m_lpfnCryptCreateHash( NULL)
								,m_lpfnCryptHashData( NULL)
								,m_lpfnCryptGetHashParam( NULL)
								,m_lpfnCryptDestroyHash( NULL)
								,m_lpfnCryptReleaseContext( NULL)
{
	m_hAdvapi32DLL = LoadLibrary( "Advapi32.dll");
	if( NULL != m_hAdvapi32DLL)
	{
		m_lpfnCryptAcquireContext = reinterpret_cast< LPCRYPTACQUIRECONTEXT>( GetProcAddress( m_hAdvapi32DLL, "CryptAcquireContextA"));
		m_lpfnCryptCreateHash = reinterpret_cast< LPCRYPTCREATEHASH>( GetProcAddress( m_hAdvapi32DLL, "CryptCreateHash"));
		m_lpfnCryptHashData = reinterpret_cast< LPCRYPTHASHDATA>( GetProcAddress( m_hAdvapi32DLL, "CryptHashData"));
		m_lpfnCryptGetHashParam = reinterpret_cast< LPCRYPTGETHASHPARAM>( GetProcAddress( m_hAdvapi32DLL, "CryptGetHashParam"));
		m_lpfnCryptDestroyHash = reinterpret_cast< LPCRYPTDESTROYHASH>( GetProcAddress( m_hAdvapi32DLL, "CryptDestroyHash"));
		m_lpfnCryptReleaseContext = reinterpret_cast< LPCRYPTRELEASECONTEXT>( GetProcAddress( m_hAdvapi32DLL, "CryptReleaseContext"));

		if( NULL == m_lpfnCryptAcquireContext
			|| NULL == m_lpfnCryptCreateHash
			|| NULL == m_lpfnCryptHashData
			|| NULL == m_lpfnCryptGetHashParam
			|| NULL == m_lpfnCryptDestroyHash
			|| NULL == m_lpfnCryptReleaseContext)
		{
			FreeLibrary( m_hAdvapi32DLL);
			m_hAdvapi32DLL = NULL;
			return;
		}
	}
}

CAdvApi32::~CAdvApi32( void)
{
	if( NULL != m_hAdvapi32DLL)
	{
		FreeLibrary( m_hAdvapi32DLL);
		m_hAdvapi32DLL = NULL;
	}
}

CAdvApi32 CPop3Client::m_cAdvApi32;

LPSTR CPop3Client::CreateDigest( LPCSTR lpcszPass)
{
	if( !m_cAdvApi32.IsEnable())
	{
		return NULL;
	}

	LPSTR lpszWork = NULL;
	LPSTR lpszResult = NULL;
	HCRYPTPROV hProv = NULL;
	HCRYPTHASH hHash = NULL;
	LPBYTE lpbySigneture = NULL;

	try
	{
		lpszWork = new char[ lstrlen( m_pszAPOPKey) + lstrlen( lpcszPass) + 1];
		lstrcpy( lpszWork, m_pszAPOPKey);
		lstrcat( lpszWork, lpcszPass);

		// デフォルト鍵コンテナの取得
		if( !m_cAdvApi32.CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, 0))
		{
			if( NTE_BAD_KEYSET == GetLastError())
			{
				/* デフォルト鍵コンテナの生成 */
				if( !m_cAdvApi32.CryptAcquireContext( &hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
				{
					DWORD dwError = GetLastError();
					char szError[ 64];

					wsprintf( szError, "error 0x%08X", dwError);

					throw szError;
				}
			}
		}

		// ハッシュ値の生成
		if( !m_cAdvApi32.CryptCreateHash( hProv, CALG_MD5, 0, 0, &hHash))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		DWORD dwSize = lstrlen( lpszWork);
		if( !m_cAdvApi32.CryptHashData( hHash, reinterpret_cast<const BYTE*>( lpszWork), dwSize, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		DWORD dwSignLen;
		if( !m_cAdvApi32.CryptGetHashParam( hHash, HP_HASHVAL, NULL, &dwSignLen, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		lpbySigneture = new BYTE[ dwSignLen];
		if( !m_cAdvApi32.CryptGetHashParam( hHash, HP_HASHVAL, lpbySigneture, &dwSignLen, 0))
		{
			DWORD dwError = GetLastError();
			char szError[ 64];

			wsprintf( szError, "error 0x%08X", dwError);

			throw szError;
		}

		lpszResult = new char[ ( dwSignLen * 2) + 1];
		if( NULL != lpszResult)
		{
			for( DWORD dwIndex = 0; dwIndex < dwSignLen; dwIndex++)
			{
				wsprintf( &lpszResult[ ( dwIndex * 2)], "%02x", lpbySigneture[ dwIndex]);
			}
		}
	}
	catch( char* pszError)
	{
		OutputDebugString( pszError);
	}

	if( NULL != lpszWork)
	{
		delete [] lpszWork;
		lpszWork = NULL;
	}
	if( NULL != lpbySigneture)
	{
		delete [] lpbySigneture;
		lpbySigneture = NULL;
	}
	if( NULL != hHash)
	{
		m_cAdvApi32.CryptDestroyHash( hHash);
		hHash = NULL;
	}
	if( NULL != hProv)
	{
		m_cAdvApi32.CryptReleaseContext( hProv, 0);
		hProv = NULL;
	}

	return lpszResult;
}
#endif

BOOL CPop3Client::LogInAPOPMode( LPCSTR lpcszUser, LPCSTR lpcszPass)
{
	LPSTR lpszDigest = CreateDigest( lpcszPass);
	if( NULL == lpszDigest)
	{
		throw "APOPのDigestが生成できない";
	}

	///////////////////////////////////////////////////////////////
	// APOP送信
	char*	pszApop = "APOP %s %s\r\n";
	int nWorkLen = lstrlen( pszApop) + lstrlen( lpcszUser) + lstrlen( lpszDigest);
	char* pszWork = new char [ nWorkLen];
	if( NULL == pszWork)
	{
		delete [] lpszDigest;
		throw "作業エリアが取得できない";
	}
	wsprintf( pszWork, pszApop, lpcszUser, lpszDigest);
	if( SOCKET_ERROR == send( m_sSocket, pszWork, lstrlen( pszWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		delete [] lpszDigest;
		throw "APOP送信失敗";
	}
	delete [] pszWork;
	delete [] lpszDigest;

	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult())throw "受信結果異常";

	///////////////////////////////////////////////////////////////
	// サーバの状態取得
	if( FALSE == GetPopStatus())throw "サーバ状態チェック異常";

	return TRUE;
}

BOOL CPop3Client::LogInNormalMode( LPCSTR lpcszUser, LPCSTR lpcszPass)
{
#ifdef	_DEBUG
	OutputDebugString( "LogIn:Enter\n");
#endif

	char*	pszWork = NULL;

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";

	///////////////////////////////////////////////////////////////
	// USER送信
	char*	pszUser = "user %s\r\n";
	int nWorkLen = lstrlen( pszUser) + lstrlen( lpcszUser);
	pszWork = new char [ nWorkLen];
	if( NULL == pszWork)throw "作業エリアが取得できない";
	wsprintf( pszWork, pszUser, lpcszUser);
	if( SOCKET_ERROR == send( m_sSocket, pszWork, lstrlen( pszWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "user名送信失敗";
	}
	delete [] pszWork;

	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult())throw "受信結果異常";

	///////////////////////////////////////////////////////////////
	// USER送信
	char*	pszPass = "pass %s\r\n";
	nWorkLen = lstrlen( pszPass) + lstrlen( lpcszPass);
	pszWork = new char [ nWorkLen];
	if( NULL == pszWork)throw "作業エリアが取得できない";
	wsprintf( pszWork, pszPass, lpcszPass);
	if( SOCKET_ERROR == send( m_sSocket, pszWork, lstrlen( pszWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "password送信失敗";
	}
	delete [] pszWork;
	pszWork = NULL;

	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult())throw "受信結果異常";

	///////////////////////////////////////////////////////////////
	// サーバの状態取得
	if( FALSE == GetPopStatus())throw "サーバ状態チェック異常";

#ifdef	_DEBUG
	OutputDebugString( "LogIn:Leave\n");
#endif
	return TRUE;
}

BOOL CPop3Client::LogIn( LPCSTR lpcszUser, LPCSTR lpcszPass)
{
	if( m_blAPOP)
	{
		return LogInAPOPMode( lpcszUser, lpcszPass);
	}
	else
	{
		return LogInNormalMode( lpcszUser, lpcszPass);
	}
}

int CPop3Client::GetMessageCount( void)
{
	return m_nMessageCount;
}

int CPop3Client::GetMessageSize( int nIndex)
{
	return ( ( NULL != m_panMassageSize && ( 0 <= nIndex && nIndex < m_nMessageCount)) ? m_panMassageSize[ nIndex] : 0);
}

BOOL CPop3Client::TestMailer( int nIndex, LPSTR lpszMail)
{
	CRStreamEx	cRstreamEx;
	char*		pszTop = "top %d 0\r\n";
	char		szWork[ 32];
	HANDLE		ahEvents[ 2];
	DWORD		dwResult;

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// コマンド作成
	wsprintf( szWork, pszTop, nIndex + 1);

	///////////////////////////////////////////////////////////////
	// TOP送信
	if( SOCKET_ERROR == send( m_sSocket, szWork, lstrlen( szWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "topコマンド送信失敗";
	}

	///////////////////////////////////////////////////////////////
	// 応答受信
	WSANETWORKEVENTS	stNetEvents;
	ULONG				ulSize;
	LPSTR				lpszData;
	int					nReceiveLen;
	BOOL				blFlag = FALSE;
	while( true)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

		// 受信データサイズの取得
		if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";

		ulSize += 10;
		lpszData = new char [ ulSize];
		ZeroMemory( lpszData, ulSize);
		ulSize = recv( m_sSocket, lpszData, ulSize, 0);
		lpszData[ ulSize] = 0;
		nReceiveLen = cRstreamEx.AddReceiveString( lpszData);
		delete [] lpszData;
		lpszData = NULL;

		char*	pszMailer = "X-Mailer: ";
		int		nMailer = lstrlen( pszMailer);
		char*	pszWork;
		while( 0 < nReceiveLen)
		{
			pszWork = new char[ nReceiveLen + 1];
			cRstreamEx.GetHeadString( pszWork, nReceiveLen + 1);
			if( blFlag)
			{
				if( '.' == pszWork[ 0] && '.' != pszWork[ 1])
				{
					delete [] pszWork;
					return FALSE;
				}
				else
				{
					if( 'X' == pszWork[ 0])
					{
						if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, nMailer, pszMailer, nMailer))
						{
							if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ nMailer], lstrlen( lpszMail), lpszMail, lstrlen( lpszMail)))
							{
								delete [] pszWork;
								return TRUE;
							}
						}
					}
				}
			}
			else
			{
				if( '+' == pszWork[ 0])
				{
					blFlag = TRUE;
				}
			}
			delete [] pszWork;
			nReceiveLen = cRstreamEx.GetHeadStringLength();
		}
	}

	return FALSE;
}

// 本当はこちらを利用したかったが、popサーバからのlist応答で、サイズが正しくないケースがあるようなので、未使用
int CPop3Client::GetMessageBody( int nIndex, LPSTR lpszMail, int nSize)
{
	int			nReceiveSize = -1;
	CRStreamEx	cRstreamEx;
	char*		pszRetr = "retr %d\r\n";
	char		szWork[ 32];
	HANDLE		ahEvents[ 2];
	DWORD		dwResult;

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// コマンド作成
	wsprintf( szWork, pszRetr, nIndex + 1);

	///////////////////////////////////////////////////////////////
	// RETR送信
	if( SOCKET_ERROR == send( m_sSocket, szWork, lstrlen( szWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "retrコマンド送信失敗";
	}

	///////////////////////////////////////////////////////////////
	// 応答受信
	WSANETWORKEVENTS	stNetEvents;
	ULONG ulSize;
	BOOL				blFlag = FALSE;
	int nDataIndex = 0;
	char szRecvBuf[ 1024];
	int nRecvSize;
	char szLineBuf[ 1024];
	int nLineIndex = 0;

	lpszMail[ 0] = 0;
	int nMailSize = 0;

	while( true)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

		// 受信データサイズの取得
		ulSize = 0;
		if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";

		nRecvSize = recv( m_sSocket, szRecvBuf, 1024, 0);
		szRecvBuf[ nRecvSize] = 0;
		for( int nIndex = 0; nIndex < nRecvSize; nIndex++)
		{
			szLineBuf[ nLineIndex] = szRecvBuf[ nIndex];
			nLineIndex++;

			if( '\n' == szRecvBuf[ nIndex])
			{
				szLineBuf[ nLineIndex] = 0;
				nLineIndex = 0;

				if( FALSE == blFlag)
				{
					if( '+' == szLineBuf[ 0])
					{
						blFlag = TRUE;
					}
				}
				else
				{
					if( '.' == szLineBuf[ 0])
					{
						if( '.' != szLineBuf[ 1])
						{
							return nMailSize;
						}
						else
						{
							// コスト高いけど出現頻度が低いし……
							MoveMemory( szLineBuf, &szLineBuf[ 1], lstrlen( &szLineBuf[ 1]) + 1);
						}
					}

					int nLineLen = lstrlen( szLineBuf);
					if( nSize > ( nMailSize + nLineLen))
					{
						lstrcpy( &lpszMail[ nMailSize], szLineBuf);
						nMailSize += nLineLen;
					}
				}
			}
		}

		/*

		ulSize += 10;
		lpszData = new char [ ulSize];
		ZeroMemory( lpszData, ulSize);
		ulSize = recv( m_sSocket, lpszData, ulSize, 0);
		lpszData[ ulSize] = 0;
		nReceiveLen = cRstreamEx.AddReceiveString( lpszData);
		delete [] lpszData;
		lpszData = NULL;

		char* pszWork;
		char* pszCount;
		char* pszSrcHeader;
		while( 0 < nReceiveLen)
		{
			pszWork = new char[ nReceiveLen + 1];
			cRstreamEx.GetHeadString( pszWork, nReceiveLen + 1);
			if( blFlag)
			{
				pszSrcHeader = pszWork;
				if( '.' == pszWork[ 0])
				{
					if( '.' == pszWork[ 1])
					{
						// 先頭の.二つはターミネータでないことの意味で、
						// 本来は一つの.なのだ！
						pszSrcHeader = &pszWork[ 1];
					}
					else
					{
						delete [] pszWork;
						return lstrlen( lpszMail);
					}
				}
				lstrcat( lpszMail, pszSrcHeader);
			}
			else
			{
				if( '+' == pszWork[ 0])
				{
					blFlag = TRUE;
					strtok( pszWork, " ");
					strtok( NULL, " ");
					pszCount = strtok( NULL, " ");
					nReceiveSize = atoi( pszCount);
					if( NULL != lpszMail && 0 < nSize)ZeroMemory( lpszMail, nSize);
				}
			}
			delete [] pszWork;
			nReceiveLen = cRstreamEx.GetHeadStringLength();
		}
		*/
	}

	return nSize;
}

// クラス内部でアロケートして、外部で開放というエレガントじゃない実装に泣き(T^T)
int CPop3Client::GetMessageBody( int nIndex, LPSTR* lppszMail)
{
	int			nReceiveSize = -1;
	CRStreamEx	cRstreamEx;
	char*		pszRetr = "retr %d\r\n";
	char		szWork[ 32];
	HANDLE		ahEvents[ 2];
	DWORD		dwResult;

	*lppszMail = NULL;

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// コマンド作成
	wsprintf( szWork, pszRetr, nIndex + 1);

	///////////////////////////////////////////////////////////////
	// RETR送信
	if( SOCKET_ERROR == send( m_sSocket, szWork, lstrlen( szWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "retrコマンド送信失敗";
	}

	///////////////////////////////////////////////////////////////
	// 応答受信
	WSANETWORKEVENTS	stNetEvents;
	ULONG ulSize;
	BOOL				blFlag = FALSE;

	// RFC的に998なので、1024で固定してみますた。
	int nDataIndex = 0;
	char szRecvBuf[ 1024];
	int nRecvSize;
	char szLineBuf[ 1024];
	int nLineIndex = 0;

	// うお！ここだけSTL……ｶｺﾜﾙｲ
	std::string strMailBuf;

	while( true)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

		// 受信データサイズの取得
		ulSize = 0;
		if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";

		nRecvSize = recv( m_sSocket, szRecvBuf, 1024, 0);
		szRecvBuf[ nRecvSize] = 0;
		for( int nIndex = 0; nIndex < nRecvSize; nIndex++)
		{
			szLineBuf[ nLineIndex] = szRecvBuf[ nIndex];
			nLineIndex++;

			if( '\n' == szRecvBuf[ nIndex])
			{
				szLineBuf[ nLineIndex] = 0;
				nLineIndex = 0;

				if( FALSE == blFlag)
				{
					if( '+' == szLineBuf[ 0])
					{
						blFlag = TRUE;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					if( '.' == szLineBuf[ 0])
					{
						if( '.' != szLineBuf[ 1])
						{
							*lppszMail = new char[ strMailBuf.length() + 1];
							lstrcpy( *lppszMail, strMailBuf.c_str());

							return strMailBuf.length();
						}
						else
						{
							// コスト高いけど出現頻度が低いし……
							MoveMemory( szLineBuf, &szLineBuf[ 1], lstrlen( &szLineBuf[ 1]) + 1);
						}
					}
					strMailBuf += szLineBuf;
				}
			}
		}
	}

	return 0;
}

BOOL CPop3Client::DeleteMessage( int nIndex)
{
	char*		pszRetr = "dele %d\r\n";
	char		szWork[ 32];

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";

	///////////////////////////////////////////////////////////////
	// コマンド作成
	wsprintf( szWork, pszRetr, nIndex + 1);

	///////////////////////////////////////////////////////////////
	// LIST送信
	if( SOCKET_ERROR == send( m_sSocket, szWork, lstrlen( szWork), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "listコマンド送信失敗";
	}
	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult())throw "受信結果異常";

	return TRUE;
}

BOOL CPop3Client::Quit( void)
{
	char*		pszRetr = "quit\r\n";

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";

	///////////////////////////////////////////////////////////////
	// QUIT送信
	if( SOCKET_ERROR == send( m_sSocket, pszRetr, lstrlen( pszRetr), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "listコマンド送信失敗";
	}
	/* QUIT送信後は解答を待たないでもいいンじゃないの？だって、切られるんだよ。あっちから。
	///////////////////////////////////////////////////////////////
	// 応答受信
	if( FALSE == CheckResult())throw "受信結果異常";
	*/

	return TRUE;
}

BOOL CPop3Client::Disconnect( void)
{
	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// 終了
	CleanUp();
	return TRUE;
}

HANDLE CPop3Client::GetAbortEvent( void) const
{
	return m_hAbortEvent;
}

void CPop3Client::CleanUp( void)
{
	m_nMessageCount = 0;
	if( NULL != m_panMassageSize)
	{
		delete [] m_panMassageSize;
		m_panMassageSize = NULL;
	}

	if( INVALID_SOCKET != m_sSocket)
	{
		if( NULL != m_hSockEvent)Org_WSAEventSelect( m_sSocket, m_hSockEvent, 0);
		closesocket( m_sSocket);
		m_sSocket = INVALID_SOCKET;
	}
	if( NULL == m_hAbortEvent)
	{
		CloseHandle( m_hAbortEvent);
		m_hAbortEvent = NULL;
	}
	if( NULL != m_hSockEvent)
	{
		CloseHandle( m_hSockEvent);
		m_hSockEvent = NULL;
	}
	if( NULL != m_pszAPOPKey)
	{
		delete [] m_pszAPOPKey;
		m_pszAPOPKey = NULL;
	}
}

BOOL CPop3Client::CheckResult( LPSTR* lppszResult)
{
	HANDLE		ahEvents[ 2];
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	DWORD				dwResult;
	CRStreamEx			cRstreamEx;
	WSANETWORKEVENTS	stNetEvents;
	ULONG				ulSize;
	LPSTR				lpszData;
	int					nReceiveLen;

	BOOL				blResult = FALSE;
	BOOL				blChecked = FALSE;

	if( NULL != lppszResult)
	{
		*lppszResult = NULL;
	}

	while( true)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;
		if( FD_READ & stNetEvents.lNetworkEvents)
		{
			// 受信データサイズの取得
			ulSize = 0;
			if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";
			if( 0 < ulSize)
			{
				lpszData = new char [ ulSize + 10];
				if( NULL == lpszData)throw "受信領域取得異常";
				ZeroMemory( lpszData, ulSize + 10);
				ulSize = recv( m_sSocket, lpszData, ulSize, 0);
				if( SOCKET_ERROR == ulSize)
				{
					delete [] lpszData;
					throw "受信データ取得異常";
				}
				lpszData[ ulSize] = 0;
				nReceiveLen = cRstreamEx.AddReceiveString( lpszData);
				char*	pszWork;
				while( 0 < nReceiveLen)
				{
					pszWork = new char[ nReceiveLen + 1];
					cRstreamEx.GetHeadString( pszWork, nReceiveLen + 1);
#ifdef	_DEBUG
					OutputDebugString( pszWork);
					OutputDebugString( "\n");
#endif
					if( '+' == pszWork[ 0])
					{
						blResult = TRUE;
						blChecked = TRUE;
						nReceiveLen = 0;

						if( NULL != lppszResult)
						{
							*lppszResult = new char[ lstrlen( pszWork) + 1];
							lstrcpy( *lppszResult, pszWork);
						}
					}
					else
					{
						if( '-' == pszWork[ 0])
						{
							blResult = TRUE;
							blChecked = TRUE;
							nReceiveLen = 0;
						}
						else
						{
							nReceiveLen = cRstreamEx.GetHeadStringLength();
						}
					}
					delete [] pszWork;
				}
				// 結果のチェックは終了？
				if( blChecked)break;
			}
		}
	}

	return blResult;
}

BOOL CPop3Client::GetPopStatus( void)
{
#ifdef	_DEBUG
	OutputDebugString( "GetPopStatus:Enter\n");
#endif
	BOOL blResult = FALSE;

	m_nMessageCount = CheckPopStat();
	if( NULL != m_panMassageSize)
	{
		delete [] m_panMassageSize;
		m_panMassageSize = NULL;
	}

	if( 0 < m_nMessageCount)
	{
		m_panMassageSize = new int [ m_nMessageCount];
		if( m_panMassageSize)
		{
			ZeroMemory( m_panMassageSize, sizeof( int) * m_nMessageCount);
			if( CheckPopList( m_panMassageSize, m_nMessageCount))
			{
				blResult = TRUE;
			}
			else
			{
				m_nMessageCount = 0;
				if( NULL != m_panMassageSize)
				{
					delete [] m_panMassageSize;
					m_panMassageSize = NULL;
				}
			}
		}
	}
	else
	{
		if( 0 == m_nMessageCount)blResult = TRUE;
	}

#ifdef	_DEBUG
	char szWork[ 64];
	wsprintf( szWork, "MessageCount:%d\n", m_nMessageCount);
	OutputDebugString( szWork);
	for( int nIndex = 0; nIndex < m_nMessageCount; nIndex++)
	{
		wsprintf( szWork, "MessageSize[ %d]:%d\n", nIndex, m_panMassageSize[ nIndex]);
		OutputDebugString( szWork);
	}

	OutputDebugString( "GetPopStatus:Leave\n");
#endif
	return blResult;
}

int CPop3Client::CheckPopStat( void)
{
#ifdef	_DEBUG
	OutputDebugString( "CPop3Client::CheckPopStat:Enter\n");
#endif
	int			nCount = 0;

	DWORD		dwResult;
	char*		pszList = "stat\r\n";
	CRStreamEx	cRstreamEx;
	HANDLE		ahEvents[ 2];

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// LIST送信
	if( SOCKET_ERROR == send( m_sSocket, pszList, lstrlen( pszList), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "statコマンド送信失敗";
	}

	///////////////////////////////////////////////////////////////
	// 応答受信
	WSANETWORKEVENTS	stNetEvents;
	ULONG				ulSize;
	LPSTR				lpszData;
	int					nReceiveLen;
	while( true)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

		// 受信データサイズの取得
		if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";

		ulSize += 10;
		lpszData = new char [ ulSize];
		ZeroMemory( lpszData, ulSize);
		ulSize = recv( m_sSocket, lpszData, ulSize, 0);
		lpszData[ ulSize] = 0;
		nReceiveLen = cRstreamEx.AddReceiveString( lpszData);
		delete [] lpszData;
		lpszData = NULL;

		if( 0 < nReceiveLen)
		{
			char*	pszWork;
			char*	pszCount;
			pszWork = new char[ nReceiveLen + 1];
			cRstreamEx.GetHeadString( pszWork, nReceiveLen + 1);
			if( '+' == pszWork[ 0])
			{
				strtok( pszWork, " ");
				pszCount = strtok( NULL, " ");
				nCount = atoi( pszCount);
			}
			delete [] pszWork;

			break;
		}
	}

#ifdef	_DEBUG
	OutputDebugString( "CPop3Client::CheckPopStat:Leave\n");
#endif
	return nCount;
}

BOOL CPop3Client::CheckPopList( int *panSize, int nCount)
{
#ifdef	_DEBUG
	OutputDebugString( "CPop3Client::CheckPopList:Enter\n");
#endif
	BOOL		blResult = FALSE;
	CRStreamEx	cRstreamEx;
	char*		pszList = "list\r\n";
	HANDLE		ahEvents[ 2];
	DWORD		dwResult;

	if( INVALID_SOCKET == m_sSocket)throw "ソケットが不整";
	///////////////////////////////////////////////////////////////
	// イヴェントの設定
	ahEvents[ 0] = m_hSockEvent;
	ahEvents[ 1] = m_hAbortEvent;

	///////////////////////////////////////////////////////////////
	// LIST送信
	if( SOCKET_ERROR == send( m_sSocket, pszList, lstrlen( pszList), 0))
	{
#ifdef	_DEBUG
		char	szWork[ 128];
		int nCode = WSAGetLastError();
		wsprintf( szWork, "EC:%d\n", nCode);
		OutputDebugString( szWork);
#endif
		throw "listコマンド送信失敗";
	}

	///////////////////////////////////////////////////////////////
	// 応答受信
	WSANETWORKEVENTS	stNetEvents;
	ULONG				ulSize;
	LPSTR				lpszData;
	int					nReceiveLen;
	char*				pszWork;
	BOOL				blSuccess = FALSE;
	BOOL				blCheckEnd = FALSE;
	while( !blCheckEnd)
	{
		dwResult = WaitForMultipleObjects( 2, ahEvents, FALSE, m_ulTimeOunt);
		if( WAIT_OBJECT_0 != dwResult)throw "コマンド待ち異常";

		ZeroMemory( &stNetEvents, sizeof( WSANETWORKEVENTS));
		Org_WSAEnumNetworkEvents( m_sSocket, m_hSockEvent, &stNetEvents);
		if( FD_CLOSE & stNetEvents.lNetworkEvents)break;

		// 受信データサイズの取得
		if( SOCKET_ERROR == ioctlsocket( m_sSocket, FIONREAD, &ulSize))throw "受信データサイズ取得異常";

		ulSize += 10;
		lpszData = new char [ ulSize];
		ZeroMemory( lpszData, ulSize);
		ulSize = recv( m_sSocket, lpszData, ulSize, 0);
		lpszData[ ulSize] = 0;
		nReceiveLen = cRstreamEx.AddReceiveString( lpszData);
		delete [] lpszData;
		lpszData = NULL;

		while( 0 < nReceiveLen)
		{
			pszWork = new char[ nReceiveLen + 1];
			cRstreamEx.GetHeadString( pszWork, nReceiveLen + 1);
#ifdef	_DEBUG
			OutputDebugString( "CPop3Client::CheckPopList:pszWork = ");
			OutputDebugString( pszWork);
			OutputDebugString( "\r\n");
#endif
			if( !blSuccess)
			{
				if( '+' == pszWork[ 0])
				{
					blSuccess = TRUE;
					blResult = TRUE;
				}
				else
				{
					if( '-' == pszWork[ 0])
					{
						blCheckEnd = TRUE;
						break;
					}
				}
			}
			else
			{
				if( '.' == pszWork[ 0])
				{
					blCheckEnd = TRUE;
					break;
				}
				else
				{
					LPSTR lpszWork = strtok( pszWork, " ");
					int nIndex = atoi( lpszWork);
					if( 1 <= nIndex && nIndex <= nCount)
					{
						nIndex--;
						lpszWork = strtok( NULL, " ");
						panSize[ nIndex] = atoi( lpszWork);
					}
				}
			}
			delete [] pszWork;

			nReceiveLen = cRstreamEx.GetHeadStringLength();
		}
	}

#ifdef	_DEBUG
	OutputDebugString( "CPop3Client::CheckPopList:Leave\n");
#endif
	return blResult;
}