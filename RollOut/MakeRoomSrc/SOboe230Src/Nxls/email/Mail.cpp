#include "StdeMail.h"
#include "Mail.h"
#include "Base64.h"
#include "RStreamEx.h"
#include "KanjiUtl.h"
#include "StrList.h"

CMail::CMail( LPCSTR lpcszMail) : m_lpHead( NULL)
									, m_lpBody( NULL)
									, m_blExchanged( FALSE)
									, m_lpszMessageID( NULL)
									, m_lpszMailer( NULL)
									, m_lpszFrom( NULL)
									, m_lpszSubject( NULL)
									, m_lpszMBoundary( NULL)
									, m_lpszBody( NULL)
									, m_blBase64Encoding( FALSE)
									, m_lpbyRawData( NULL)
									, m_nRawDaraSize( 0)
{
	SetMailSource( lpcszMail);
}

CMail::~CMail( void)
{
	if( NULL != m_lpHead)
	{
		delete [] m_lpHead;
		m_lpHead = NULL;
	}
	if( NULL != m_lpBody)
	{
		delete [] m_lpBody;
		m_lpBody = NULL;
	}
	if( NULL != m_lpszMessageID)
	{
		delete [] m_lpszMessageID;
		m_lpszMessageID = NULL;
	}
	if( NULL != m_lpszMailer)
	{
		delete [] m_lpszMailer;
		m_lpszMailer = NULL;
	}
	if( NULL != m_lpszFrom)
	{
		delete [] m_lpszFrom;
		m_lpszFrom = NULL;
	}
	if( NULL != m_lpszSubject)
	{
		delete [] m_lpszSubject;
		m_lpszSubject = NULL;
	}
	if( NULL != m_lpszMBoundary)
	{
		delete [] m_lpszMBoundary;
		m_lpszMBoundary = NULL;
	}
	if( NULL != m_lpszBody)
	{
		delete [] m_lpszBody;
		m_lpszBody = NULL;
	}
	if( NULL != m_lpbyRawData)
	{
		delete [] m_lpbyRawData;
		m_lpbyRawData = NULL;
	}
}

BOOL CMail::SetMailSource( LPCSTR lpcszMail)
{
	if( NULL == lpcszMail)return FALSE;

	char*	lpszMail = new char [ lstrlen( lpcszMail) + 1];
	if( lpszMail)
	{
		lstrcpy( lpszMail, lpcszMail);

		char*	pHead = lpszMail;
		char*	pBody = NULL;
		int nLen = lstrlen( lpszMail);
		for( int nIndex = 0; nIndex < nLen; nIndex++)
		{
			if( '\n' == lpszMail[ nIndex])
			{
				while( ( ' ' == lpszMail[ nIndex + 1] || '\t' == lpszMail[ nIndex + 1]) && 0 != lpszMail[ nIndex + 1])nIndex++;
				if( '\r' == lpszMail[ nIndex + 1])
				{
					if( '\n' == lpszMail[ nIndex + 2])
					{
						lpszMail[ nIndex + 1] = 0;
						pBody = &lpszMail[ nIndex + 3];
						break;
					}
				}
			}
		}

		if( NULL != m_lpHead)delete [] m_lpHead;
		m_lpHead = new char [ lstrlen( pHead) + 1];
		lstrcpy( m_lpHead, pHead);
		if( NULL != m_lpBody)delete [] m_lpBody;
		m_lpBody = new char [ lstrlen( pBody) + 1];
		lstrcpy( m_lpBody, pBody);

		char*	pszMessageID = "Message-Id: ";
		int		nMessageID = lstrlen( pszMessageID);
		char*	pszMailer = "X-Mailer: ";
		int		nMailer = lstrlen( pszMailer);

		char*	pszWork = strtok( pHead, "\r\n");
		while( pszWork)
		{
			if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, nMessageID, pszMessageID, nMessageID))
			{
#ifdef	_DEBUG
				OutputDebugString( &pszWork[ nMessageID]);
				OutputDebugString( "\r\n");
#endif
				if( m_lpszMessageID)delete [] m_lpszMessageID;
				m_lpszMessageID = new char[ lstrlen( &pszWork[ nMessageID]) + 1];
				lstrcpy( m_lpszMessageID, &pszWork[ nMessageID]);
			}
			else
			{
				if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, nMailer, pszMailer, nMailer))
				{
#ifdef	_DEBUG
					OutputDebugString( &pszWork[ nMailer]);
					OutputDebugString( "\r\n");
#endif
					if( m_lpszMailer)delete [] m_lpszMailer;
					m_lpszMailer = new char[ lstrlen( &pszWork[ nMailer]) + 1];
					lstrcpy( m_lpszMailer, &pszWork[ nMailer]);
				}
			}
			pszWork = strtok( NULL, "\r\n");
		}

		delete [] lpszMail;
	}

	m_blExchanged = FALSE;
	return TRUE;
}

BOOL CMail::DoDecode( void)
{
	if( m_blExchanged)return TRUE;
	if( NULL == m_lpHead)return FALSE;
	if( NULL == m_lpBody)return FALSE;

	if( DecodeHeader())
	{
		if( DecodeBody())
		{
			m_blExchanged = true;
			return TRUE;
		}
	}
	return FALSE;
}

char* CMail::sm_pszFrom = "From: ";
int CMail::sm_nFrom = lstrlen( CMail::sm_pszFrom);
char* CMail::sm_pszSubject = "Subject: ";
int CMail::sm_nSubject = lstrlen( CMail::sm_pszSubject);
char* CMail::sm_pszMultiPart = "Content-Type: multipart/mixed;";
int CMail::sm_nMultiPart = lstrlen( CMail::sm_pszMultiPart);
char* CMail::sm_pszBoundary = "boundary";
int CMail::sm_nBoundary = lstrlen( CMail::sm_pszBoundary);
char* CMail::sm_pszEncoding = "Content-Transfer-Encoding: ";
int CMail::sm_nEncoding = lstrlen( CMail::sm_pszEncoding);
char* CMail::sm_pszBase64 = "Base64";
int CMail::sm_nBase64 = lstrlen( CMail::sm_pszBase64);

BOOL CMail::DecodeHeader( void)
{
	char*	pszWork = strtok( m_lpHead, "\r\n");
	while( pszWork)
	{
		if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, sm_nSubject, sm_pszSubject, sm_nSubject))
		{
			char	szData[ 640];
			MimeDecode( szData, 640, &pszWork[ sm_nSubject]);
			pszWork = strtok( NULL, "\r\n");
			while( pszWork && ( ' ' == pszWork[ 0] || '\t' == pszWork[ 0]))
			{
				char	szData2[ 128];
				MimeDecode( szData2, 128, &pszWork[ 1]);
				lstrcat( szData, szData2);
				pszWork = strtok( NULL, "\r\n");
			}
#ifdef	_DEBUG
			OutputDebugString( "SUBJECT:");
			OutputDebugString( szData);
			OutputDebugString( "\r\n");
#endif
			if( m_lpszSubject)delete [] m_lpszSubject;
			m_lpszSubject = new char[ lstrlen( szData) + 1];
			lstrcpy( m_lpszSubject, szData);
			continue;
		}
		else
		{
			if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, sm_nFrom, sm_pszFrom, sm_nFrom))
			{
#ifdef	_DEBUG
				OutputDebugString( &pszWork[ sm_nFrom]);
				OutputDebugString( "\r\n");
#endif
				char	szData[ 128];
				MimeDecode( szData, 128, &pszWork[ sm_nFrom]);
				pszWork = strtok( NULL, "\r\n");
				while( pszWork && ( ' ' == pszWork[ 0] || '\t' == pszWork[ 0]))
				{
					char	szData2[ 128];
					MimeDecode( szData2, 128, &pszWork[ 1]);
					lstrcat( szData, szData2);
					pszWork = strtok( NULL, "\r\n");
				}
#ifdef	_DEBUG
				OutputDebugString( "FROM:");
				OutputDebugString( szData);
				OutputDebugString( "\r\n");
#endif
				if( m_lpszFrom)delete [] m_lpszFrom;
				m_lpszFrom = new char[ lstrlen( szData) + 1];
				lstrcpy( m_lpszFrom, szData);
				continue;
			}
			else
			{
				// MIME用のBOUNDARY取り出し
				if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, sm_nMultiPart, sm_pszMultiPart, sm_nMultiPart))
				{
					int nIndex = sm_nMultiPart;
					while( true)
					{
						for( ; 0 != pszWork[ nIndex]; nIndex++)
						{
							if( 'B' == pszWork[ nIndex] || 'b' == pszWork[ nIndex])
							{
								if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ nIndex], sm_nBoundary, sm_pszBoundary, sm_nBoundary))
								{
									for( ; 0 != pszWork[ nIndex]; nIndex++)
									{
										if( '\"' == pszWork[ nIndex])
										{
											if( m_lpszMBoundary)delete [] m_lpszMBoundary;
											m_lpszMBoundary = new char [ lstrlen( &pszWork[ nIndex + 1]) + 1];
											lstrcpy( m_lpszMBoundary, &pszWork[ nIndex + 1]);
											int nLength = lstrlen( m_lpszMBoundary);
											for( nIndex = 0; nIndex < nLength; nIndex++)
											{
												if( '\"' == m_lpszMBoundary[ nIndex])
												{
													m_lpszMBoundary[ nIndex] = 0;
													break;
												}
											}
											break;
										}
									}
									break;
								}
							}
						}
						if( NULL != m_lpszMBoundary)break;
						pszWork = strtok( NULL, "\r\n");
						nIndex = 0;
					}
				}
				else
				{
					// プレーンテキストでのBase64エンコードチェック
					if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, pszWork, sm_nEncoding, sm_pszEncoding, sm_nEncoding))
					{
						// 本当は複数行チェックと化しなきゃないんだけど、チョット手抜きね
#ifdef	_DEBUG
						OutputDebugString( &pszWork[ sm_nEncoding]);
						OutputDebugString( "\r\n");
#endif
						if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ sm_nEncoding], sm_nBase64, sm_pszBase64, sm_nBase64))
						{
							m_blBase64Encoding = TRUE;
						}
					}
				}
			}
		}
		pszWork = strtok( NULL, "\r\n");
	}
	return TRUE;
}

char* CMail::sm_pszPlain = "text/plain;";
int CMail::sm_nPlain = lstrlen( CMail::sm_pszPlain);
char* CMail::sm_pszRawData = "application/octet-stream";
int CMail::sm_nRawData = lstrlen( CMail::sm_pszRawData);
char* CMail::sm_pszCharset = "charset";
int CMail::sm_nCharset = lstrlen( CMail::sm_pszCharset);
WORD CMail::sm_wBS = '--';
char* CMail::sm_pszTextType = "Content-Type: ";
int CMail::sm_nTextType = lstrlen( sm_pszTextType);
char* CMail::sm_pszChipData = "SOboeChip.bin";
int CMail::sm_nChipData = lstrlen( CMail::sm_pszChipData);

BOOL CMail::DecodeBody( void)
{
	if( m_lpszMBoundary)
	{
		CStrList	cStrList;

		CRStreamEx	cStrmPlainText;
		CRStreamEx	cStrmChipData;
		cStrList.SetStreamData( m_lpBody);

		LPCSTR	lpcszWork;
		LPCSTR	lpcszTemp;
		int		nExchnSize;
		char*	pszExchnWork;
		for( int nIndex = 0; nIndex < cStrList.GetCount(); nIndex++)
		{
			lpcszWork = cStrList.GetLine( nIndex);
			if( NULL != lpcszWork)
			{
				int nChar = 0;
				while(  ' ' == lpcszWork[ nChar] || '\t' == lpcszWork[ nChar])nChar++;
				lpcszWork = &lpcszWork[ nChar];
			}
			if( sm_wBS == *( ( WORD*)&lpcszWork[ 0]) && CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &lpcszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))
			{
				lpcszWork = cStrList.GetLine( ++nIndex);
				if( NULL != lpcszWork)
				{
					int nChar = 0;
					while(  ' ' == lpcszWork[ nChar] || '\t' == lpcszWork[ nChar])nChar++;
					lpcszWork = &lpcszWork[ nChar];
				}
				// コンテキストタイプ記述行？
				lpcszTemp = strstr( lpcszWork, sm_pszTextType);
				if( NULL != lpcszTemp)
				{
					lpcszWork = lpcszTemp;
					while( true)
					{
						// プレーンテキスト？
						lpcszTemp = strstr( lpcszWork, sm_pszPlain);
						if( NULL != lpcszTemp)
						{
							lpcszWork = lpcszTemp;
							while( true)
							{
								// エンコードされてない？
								lpcszTemp = strstr( lpcszWork, sm_pszCharset);
								if( NULL != lpcszTemp)
								{
									// パート部引き出し：つまり最初の改行探し
									nIndex++;
									for( ; nIndex < cStrList.GetCount(); nIndex++)
									{
										lpcszWork = cStrList.GetLine( nIndex);
										if( NULL != lpcszWork)
										{
											int nChar = 0;
											while(  ' ' == lpcszWork[ nChar] || '\t' == lpcszWork[ nChar])nChar++;
											lpcszWork = &lpcszWork[ nChar];
										}
										if( '\r' == lpcszWork[ 0])break;
									}
									// 以下本文
									nIndex++;
									for( ; nIndex < cStrList.GetCount(); nIndex++)
									{
										lpcszWork = cStrList.GetLine( nIndex);
										if( sm_wBS == *( ( WORD*)&lpcszWork[ 0]) && CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &lpcszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))
										{
											// エンコードされていないテキストの取得完了
											nIndex--;	// もう一度検出して欲しいら、ずるいけど、－１しておく
											break;
										}
										else
										{
											// SHIFT JISに変換しながら格納
											nExchnSize = CheckJis2SjisLen( lpcszWork) + 1;
											pszExchnWork = new char[ nExchnSize];
											if( pszExchnWork)
											{
												Jis2Sjis( pszExchnWork, nExchnSize, lpcszWork);
												cStrmPlainText.AddReceiveString( pszExchnWork);
												delete [] pszExchnWork;
												pszExchnWork = NULL;
											}
										}
									}
									break;
								}
								else
								{
									lpcszWork = cStrList.GetLine( nIndex + 1);
									if( ' ' != lpcszWork[ 0] && '\t' != lpcszWork[ 0])break;
									nIndex++;
								}
							}
							break;
						}
						else
						{
							// バイナリ？
							lpcszTemp = strstr( lpcszWork, sm_pszRawData);
							if( NULL != lpcszTemp)
							{
								lpcszWork = lpcszTemp;
								while( true)
								{
									// 覚え書のデータ？
									lpcszTemp = strstr( lpcszWork, sm_pszChipData);
									if( NULL != lpcszTemp)
									{
										// パート部引き出し：つまり最初の改行探し
										nIndex++;
										for( ; nIndex < cStrList.GetCount(); nIndex++)
										{
											lpcszWork = cStrList.GetLine( nIndex);
											if( NULL != lpcszWork)
											{
												int nChar = 0;
												while(  ' ' == lpcszWork[ nChar] || '\t' == lpcszWork[ nChar])nChar++;
												lpcszWork = &lpcszWork[ nChar];
											}
											if( '\r' == lpcszWork[ 0])break;
										}
										nIndex++;
										for( ; nIndex < cStrList.GetCount(); nIndex++)
										{
											lpcszWork = cStrList.GetLine( nIndex);
											if( sm_wBS == *( ( WORD*)&lpcszWork[ 0]) && CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &lpcszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))
											{
												// エンコードされていないテキストの取得完了
												nIndex--;	// もう一度検出して欲しいら、ずるいけど、－１しておく
												break;
											}
											else
											{
#ifdef	_DEBUG
												OutputDebugString( lpcszWork);
#endif
												if( '\r' == lpcszWork[ 0])continue;

												nExchnSize = lstrlen( lpcszWork) + 1;
												pszExchnWork = new char [ nExchnSize];
												lstrcpy( pszExchnWork, lpcszWork);
												// お尻のトリミング（力技
												int nTailPos = lstrlen( pszExchnWork) - 1;
												while( 0 < nTailPos)
												{
													if( '\n' == pszExchnWork[ nTailPos])
													{
														pszExchnWork[ nTailPos] = 0;
														nTailPos--;
														if( 0 >= nTailPos)break;
													}
													if( '\r' == pszExchnWork[ nTailPos])
													{
														pszExchnWork[ nTailPos] = 0;
														nTailPos--;
														if( 0 >= nTailPos)break;
													}
													if( '\r' != pszExchnWork[ nTailPos] &&
														'\n' != pszExchnWork[ nTailPos])
													{
														break;
													}
												}
												cStrmChipData.AddReceiveString( pszExchnWork);
												delete [] pszExchnWork;
												pszExchnWork = NULL;
											}
										}
										break;
									}
									else
									{
										lpcszWork = cStrList.GetLine( nIndex + 1);
										if( ' ' != lpcszWork[ 0] && '\t' != lpcszWork[ 0])break;
										nIndex++;
									}
								}
								break;
							}
							else
							{
								lpcszWork = cStrList.GetLine( nIndex + 1);
								if( ' ' != lpcszWork[ 0] && '\t' != lpcszWork[ 0])break;
							}
						}
					}
				}
			}
		}
		m_lpszBody = new char [ lstrlen( cStrmPlainText.GetWork()) + 1];
		lstrcpy( m_lpszBody, cStrmPlainText.GetWork());

		int nSize = Base64Decode( NULL, 0, cStrmChipData.GetWork());
		m_lpbyRawData = new BYTE [ nSize];
		m_nRawDaraSize = Base64Decode( m_lpbyRawData, nSize, cStrmChipData.GetWork());
		if( 0 >= m_nRawDaraSize)
		{
			delete [] m_lpbyRawData;
			m_lpbyRawData = NULL;
		}
#ifdef	_DEBUG
		else
		{
			DWORD	dwLen;
			HANDLE hFile = CreateFile( "CHIP.bin", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
			WriteFile( hFile, m_lpbyRawData, m_nRawDaraSize, &dwLen, NULL);
			CloseHandle( hFile);
		}
#endif
	}
	else
	{
		BOOL blEncodingBase64 = FALSE;

		if( m_lpBody && 0 < lstrlen( m_lpBody))
		{
			LPTSTR lpszExchangeWork = NULL;
			LPTSTR lpszExchangeTemp = m_lpBody;
			// Base64で処理されていれば、それを先に戻す
			if( m_blBase64Encoding)
			{
				int nSize = Base64Decode( NULL, 0, m_lpBody);
				lpszExchangeWork = new char [ nSize + 1];
				if( lpszExchangeWork)
				{
					Base64Decode( ( LPBYTE)lpszExchangeWork, nSize + 1, m_lpBody);
					lpszExchangeWork[ nSize] = 0;
					lpszExchangeTemp = lpszExchangeWork;
				}
			}

			int nLen = CheckJis2SjisLen( lpszExchangeTemp);
			if( m_lpszBody)delete [] m_lpszBody;
			m_lpszBody = new char[ nLen + 1];
			Jis2Sjis( m_lpszBody, nLen + 1, lpszExchangeTemp);

			if( lpszExchangeWork)
			{
				delete [] lpszExchangeWork;
			}
		}
	}
	return TRUE;
}

/*
char* CMail::sm_pszPlain = "Content-Type: text/plain;";
int CMail::sm_nPlain = lstrlen( sm_pszPlain);
char* CMail::sm_pszRawData = "Content-Type: application/octet-stream; name=\"SOboeChip.bin\"";
int CMail::sm_nRawData = lstrlen( sm_pszRawData);
char* CMail::sm_pszCharset = "charset";
int CMail::sm_nCharset = lstrlen( sm_pszCharset);
WORD CMail::sm_wBS = '--';

BOOL CMail::DecodeBody( void)
{
	char*	pszWork = NULL;
	try
	{
		if( m_lpszMBoundary)
		{
			CRStreamEx	cRStreamDst;
			CRStreamEx	cRStream( lstrlen( m_lpBody) + 1);
			cRStream.AddReceiveString( m_lpBody);

			char*	pszTextBody;
			int		nTextBody;

			// プレーンテキストだけの拾い読み

			// パートの始まり探し
			int	nLength = cRStream.GetHeadStringLength();
			if( 0 >= nLength)throw -1;
			while( nLength)
			{
				pszWork = new char[ nLength + 1];
				if( NULL == pszWork)throw -1;
				cRStream.GetHeadString( pszWork, nLength + 1);
				if( sm_wBS == *( ( WORD*)&pszWork[ 0]) &&
					CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))
				{
					break;
				}
				delete [] pszWork;
				pszWork = NULL;
			}

			// パートのヘッダ探し
			nLength = cRStream.GetHeadStringLength();
			if( 0 >= nLength)throw -1;
			while( nLength)
			{
				pszWork = new char[ nLength + 1];
				if( NULL == pszWork)throw - 1;

				cRStream.GetHeadString( pszWork, nLength + 1);
				if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ 0], sm_nPlain, sm_pszPlain, sm_nPlain))
				{
					break;
				}
				delete [] pszWork;
				pszWork = NULL;
			}
			if( NULL == pszWork)throw -1;

			// charsetを探せ！
			while( pszWork)
			{
				for( int nIndex = 0; 0 != pszWork[ nIndex]; nIndex++)
				{
					if( 'C' == pszWork[ nIndex] || 'c' == pszWork[ nIndex])
					{
						if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ nIndex], sm_nCharset, sm_pszCharset, sm_nCharset))
						{
							delete [] pszWork;
							pszWork = NULL;
							nLength = cRStream.GetHeadStringLength();
							while( nLength)
							{
								pszWork = new char[ nLength + 1];
								if( NULL == pszWork)throw -1;
								cRStream.GetHeadString( pszWork, nLength + 1);
								if( '\r' == pszWork[ 0])
								{
									delete [] pszWork;
									pszWork = NULL;
									break;
								}
								nLength = cRStream.GetHeadStringLength();
							}
							break;
						}
					}
				}

				if( NULL == pszWork)break;

				delete [] pszWork;
				pszWork = NULL;
				nLength = cRStream.GetHeadStringLength();
				if( 0 >= nLength)throw -1;

				pszWork = new char[ nLength + 1];
				if( NULL == pszWork)throw - 1;
				cRStream.GetHeadString( pszWork, nLength + 1);
				if( sm_wBS == *( ( WORD*)&pszWork[ 0])&&
					CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))throw -1;
			}

			// 本文ＧＥＴだぜぇ！
			nLength = cRStream.GetHeadStringLength();
			while( nLength)
			{
				pszWork = new char[ nLength + 1];
				if( pszWork)
				{
					cRStream.GetHeadString( pszWork, nLength + 1);
					if( sm_wBS == *( ( WORD*)&pszWork[ 0])&&
						CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &pszWork[ 2], lstrlen( m_lpszMBoundary), m_lpszMBoundary, lstrlen( m_lpszMBoundary)))
					{
						break;
					}
					nTextBody = CheckJis2SjisLen( pszWork);
					pszTextBody = new char[ nTextBody + 1];
					if( pszTextBody)
					{
						Jis2Sjis( pszTextBody, nTextBody + 1, pszWork);
						cRStreamDst.AddReceiveString( pszTextBody);
						delete [] pszTextBody;
					}
					delete [] pszWork;
					pszWork = NULL;
					nLength = cRStream.GetHeadStringLength();
				}
			}
			if( m_lpszBody)delete [] m_lpszBody;
			m_lpszBody = new char[ lstrlen( cRStreamDst.GetWork()) + 1];
			lstrcpy( m_lpszBody, cRStreamDst.GetWork());
		}
		else
		{
			if( m_lpBody && 0 < lstrlen( m_lpBody))
			{
				int nLen = CheckJis2SjisLen( m_lpBody);
				if( m_lpszBody)delete [] m_lpszBody;
				m_lpszBody = new char[ nLen + 1];
				Jis2Sjis( m_lpszBody, nLen + 1, m_lpBody);
			}
		}
		return TRUE;
	}
	catch( int nEcode)
	{
		if( pszWork)
		{
			delete [] pszWork;
			pszWork = NULL;
		}

		return FALSE;
	}
}
*/
