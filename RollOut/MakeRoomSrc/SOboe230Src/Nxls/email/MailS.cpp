#include "StdeMail.h"
#include "MailS.h"

CMailS::CMailS( void)
{
	m_nToAddressCount = 0;
	m_papszToAddress = NULL;
	m_papszToAddressRaw = NULL;
	m_pszFromAddress = NULL;

	m_pszSubject = NULL;
	m_pszBody = NULL;

	m_lpvRawData = NULL;
	m_nRawDataSize = 0;
}

CMailS::~CMailS( void)
{
	if( NULL != m_papszToAddress)
	{
		for( int nIndex = 0; nIndex < m_nToAddressCount; nIndex++)
		{
			delete [] m_papszToAddress[ nIndex];
		}
		delete [] m_papszToAddress;
		m_papszToAddress = NULL;
	}
	if( NULL != m_papszToAddress)
	{
		for( int nIndex = 0; nIndex < m_nToAddressCount; nIndex++)
		{
			delete [] m_papszToAddressRaw[ nIndex];
		}
		delete [] m_papszToAddressRaw;
		m_papszToAddressRaw = NULL;
	}
	if( NULL != m_pszFromAddress)
	{
		delete [] m_pszFromAddress;
		m_pszFromAddress = NULL;
	}
	if( NULL != m_pszSubject)
	{
		delete [] m_pszSubject;
		m_pszSubject = NULL;
	}

	if( NULL != m_lpvRawData)
	{
		free( m_lpvRawData);
		m_lpvRawData = NULL;
	}
}

BOOL CMailS::SetToAddress( const ENTRYDATA** pstEntry, int nCount)
{
	m_nToAddressCount = nCount;

	m_papszToAddress = new LPSTR [ m_nToAddressCount];
	if( NULL == m_papszToAddress)goto errordetect;

	m_papszToAddressRaw = new LPSTR [ m_nToAddressCount];
	if( NULL == m_papszToAddressRaw)goto errordetect;

	ZeroMemory( m_papszToAddress, sizeof( LPSTR) * m_nToAddressCount);
	ZeroMemory( m_papszToAddressRaw, sizeof( LPCSTR) * m_nToAddressCount);

	LPCSTR lpcszToAddr;
	int	nIndex;
	for( nIndex = 0; nIndex < m_nToAddressCount; nIndex++)
	{
		lpcszToAddr = ( LPSTR)&( ( LPBYTE)( pstEntry[ nIndex]))[ sizeof( ENTRYDATA)];
		if( NULL == lpcszToAddr)goto errordetect;
		if( 0 >= lstrlen( lpcszToAddr))goto errordetect;

		// ���M�Ɏg�p�ł���A�h���X�̎擾�i���X��₱�������c�c�j
		int		nMailAddress = 0;
		char	szMailAddress[ 256];
		{
			ZeroMemory( szMailAddress, 256);

			BOOL blFindA = FALSE;
			int nIndex = 0;
			int nLength = lstrlen( lpcszToAddr);
			for( nIndex = 0; nIndex < nLength + 1; nIndex++)
			{
				// �R�����g����p�łȂ�@��T���܂��B
				if( '@' == lpcszToAddr[ nIndex])
				{
					// �A�h���X�Ƒz���ł���̂ŁA�I�[�������莟�攲����I
					blFindA = TRUE;
				}
				else
				{
					// �ꕶ�����p
					if( '\\' == lpcszToAddr[ nIndex])
					{
						nIndex++;
						szMailAddress[ nMailAddress] = lpcszToAddr[ nIndex];
						nMailAddress++;
						if( '\\' == lpcszToAddr[ nIndex])nIndex++;
					}
					else
					{
						// ���p������J�n
						if( '\"' == lpcszToAddr[ nIndex])
						{
							nIndex++;
							for( ; nIndex < nLength; nIndex++)
							{
								szMailAddress[ nMailAddress] = lpcszToAddr[ nIndex];
								if( '\"' == lpcszToAddr[ nIndex])break;
								// �ꕶ�����p
								if( '\\' == lpcszToAddr[ nIndex])
								{
									nMailAddress++;
									nIndex++;
									szMailAddress[ nMailAddress] = lpcszToAddr[ nIndex];
								}
								nMailAddress++;
							}
						}
						else
						{
							// �R�����g�J�n
							if( '(' == lpcszToAddr[ nIndex])
							{
								int nComment = 1;
								nIndex++;
								for( ; nIndex < nLength; nIndex++)
								{
									// ����q�ɂȂ�Ƃ�
									if( '(' == lpcszToAddr[ nIndex])nComment++;
									if( ')' == lpcszToAddr[ nIndex])
									{
										nComment--;
										if( 0 == nComment)break;
									}
									// �ꕶ�����p
									if( '\\' == lpcszToAddr[ nIndex])
									{
										nIndex++;
									}
								}
								continue;
							}
							else
							{
								if( ' ' == lpcszToAddr[ nIndex] || '\t' == lpcszToAddr[ nIndex] || '<' == lpcszToAddr[ nIndex])
								{
									if( blFindA)
									{
										szMailAddress[ nMailAddress] = 0;
										break;
									}
									nMailAddress = 0;
									continue;
								}
								else
								{
									if( '>' == lpcszToAddr[ nIndex])
									{
										szMailAddress[ nMailAddress] = 0;
										break;
									}
								}
							}
						}
					}
				}
				szMailAddress[ nMailAddress] = lpcszToAddr[ nIndex];
				nMailAddress++;
			}
		}

		int nAddressLen = lstrlen( szMailAddress);
		if( 0 >= nAddressLen)goto errordetect;
		m_papszToAddress[ nIndex] = new char[ nAddressLen + 1];
		if( NULL == m_papszToAddress[ nIndex])goto errordetect;
		lstrcpy( m_papszToAddress[ nIndex], szMailAddress);
		m_papszToAddressRaw[ nIndex] = new char[ lstrlen( lpcszToAddr) + 1];
		if( NULL == m_papszToAddressRaw[ nIndex])goto errordetect;
		lstrcpy( m_papszToAddressRaw[ nIndex], lpcszToAddr);
	}

	return TRUE;

errordetect:
	return FALSE;
}

BOOL CMailS::SetFromAddress( LPCSTR lpcszAddress, LPCSTR lpcszRealName)
{
	if( NULL == lpcszAddress || 0 >= lstrlen( lpcszAddress))goto errordetect;

	if( NULL != lpcszRealName && 0 < lstrlen( lpcszRealName))
	{
		int nFromAddressLen = lstrlen( lpcszAddress);
		nFromAddressLen += lstrlen( lpcszRealName);
		nFromAddressLen += 5;	// 5 == lstrlen( "\"\" <>")
		m_pszFromAddress = new char [ nFromAddressLen + 1];
		if( NULL == m_pszFromAddress)goto errordetect;
		wsprintf( m_pszFromAddress, "\"%s\" <%s>", lpcszRealName, lpcszAddress);
	}
	else
	{
		m_pszFromAddress = new char [ lstrlen( lpcszAddress) + 1];
		if( NULL == m_pszFromAddress)goto errordetect;
		lstrcpy( m_pszFromAddress, lpcszAddress);
	}
	return TRUE;

errordetect:
	return FALSE;
}

LPCSTR	CMailS::m_lpcszCheck = "\r\n-------\r\n���̃��C���Ɋւ���񓚂����񂹉������B\r\n�P�D%s\r\n�Q�D%s";
LPCSTR	CMailS::m_lpcszOtherA = "\r\n�R�D%s";
LPCSTR	CMailS::m_lpcszOtherD = "\r\n�R�D���̑��i�C�ӂ̉�)";

BOOL CMailS::SetMailBody( const SENDDATA* pstSendData, int nSendDataSize, LPCSTR lpcszSignaturePath)
{
	BOOL blResult = FALSE;

	LPCSTR	lpcszBody = NULL;
	int		nBodyLen = 0;
	LPCSTR	lpcszSignature = NULL;
	int		nSignatureLen = 0;
	int		nResultCheckLen = 0;
	LPSTR	pszResultCheck = NULL;
	HANDLE	hFileSignature = INVALID_HANDLE_VALUE;
	HANDLE	hFileMapping = INVALID_HANDLE_VALUE;
	BOOL	blAddSpace = FALSE;

	LPCSTR	lpcszTitle = NULL;

#ifdef	_POP3_TOO
	// ���Ԃ��ł̓t�H���g�Ȃǂ̏���MIME�����ő���
	if( pstSendData->m_blPack)
	{
		// �}�X�^�f�[�^��ۑ����܂�
		m_nRawDataSize = nSendDataSize;
		m_lpvRawData = malloc( m_nRawDataSize);
		if( NULL == m_lpvRawData)goto errordetect;
		CopyMemory( m_lpvRawData, pstSendData, m_nRawDataSize);
	}
#endif

	// �^�C�g���������o���܂��B
	if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszMemoTitle)
	{
		lpcszTitle = &( ( char*)pstSendData)[ pstSendData->m_nszMemoTitle];
	}
	if( NULL == lpcszTitle)goto errordetect;
	if( 0 >= lstrlen( lpcszTitle))goto errordetect;
	m_pszSubject = new char [ lstrlen( lpcszTitle) + 1];
	if( NULL == m_pszSubject)goto errordetect;
	lstrcpy( m_pszSubject, lpcszTitle);

	// �ŏ��ɑS�̃T�C�Y���v�Z���܂��傤�I

	// ���M�p�̕��͎擾
	if( ( 0 < pstSendData->m_unDxlID) && ( ( int)sizeof( SENDDATA) <= pstSendData->m_nszDxlDummy))
	{
		lpcszBody = &( ( char*)pstSendData)[ pstSendData->m_nszDxlDummy];
	}
	else
	{
		if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszMemoData)
		{
			lpcszBody = &( ( char*)pstSendData)[ pstSendData->m_nszMemoData];
		}
	}
	if( NULL == lpcszBody)goto errordetect;
	nBodyLen = lstrlen( lpcszBody);
	if( 0 >= nBodyLen)goto errordetect;

	// �����͕t���܂����H
	if( pstSendData->m_blSendMaster)
	{
		hFileSignature = CreateFile( lpcszSignaturePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if( INVALID_HANDLE_VALUE != hFileSignature)
		{
			nSignatureLen = GetFileSize( hFileSignature, NULL);
			nSignatureLen += 2; // �����Ɩ{���̊Ԃɂ͉��s������ł���H
			// ���ƌ�ׂ̈ɁA�ˁB
			hFileMapping = CreateFileMapping( hFileSignature, NULL, PAGE_READONLY, 0, 0, NULL);
			lpcszSignature = ( LPCSTR)MapViewOfFile( hFileMapping, FILE_MAP_READ, 0, 0, 0);
		}
		else
		{
			// ���M��񂩂珐�������o���Ă݂�
			if( ( int)sizeof( SENDDATA) <= pstSendData->m_nszSignature)
			{
				if( NULL != &( ( char*)pstSendData)[ pstSendData->m_nszSignature])
				{
					lpcszSignature = &( ( char*)pstSendData)[ pstSendData->m_nszSignature];
					nSignatureLen = lstrlen( lpcszSignature);
					nSignatureLen += 2; // �����Ɩ{���̊Ԃɂ͉��s������ł���H

					// ������"From:"�ł͂��܂�ƁA�T�[�o���ŕs�������������Ƃ��L�邻���Ȃ̂ŁA�擪��" "�}������
					char*	pszFrom = "From:";
					int nFrom = lstrlen( pszFrom);
					if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &( ( char*)pstSendData)[ pstSendData->m_nszSignature], nFrom, pszFrom, nFrom))
					{
						// �X�y�[�X����t�����܂��B
						blAddSpace = TRUE;
						nSignatureLen++;
					}
				}
			}
		}
	}

	if( pstSendData->m_blPack && pstSendData->m_blCheckYesNo && ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult1 && ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult2)
	{
		nResultCheckLen = lstrlen( m_lpcszCheck);
		nResultCheckLen += lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszResult1]);
		nResultCheckLen += lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszResult2]);

		if( pstSendData->m_blOtherResult)
		{
			if(  ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult3)
			{
				nResultCheckLen += lstrlen( m_lpcszOtherA);
				nResultCheckLen += lstrlen( &( ( char*)pstSendData)[ pstSendData->m_nszResult3]);
			}
			else
			{
				nResultCheckLen += lstrlen( m_lpcszOtherD);
			}
		}
	}

	// ���āA�K�v�ȗʂ̍�ƃG���A�����܂��傤�I
	m_pszBody = new char[ nBodyLen + nSignatureLen + nResultCheckLen + 1];
	ZeroMemory( m_pszBody, nBodyLen + nSignatureLen + nResultCheckLen + 1);

	// �l�b�g���[�N���c�֘A�̃f�[�^�����܂��傤
	if( 0 < nResultCheckLen)
	{
		pszResultCheck = ( char*)malloc( nResultCheckLen);
		if( pszResultCheck)
		{
			wsprintf( pszResultCheck, m_lpcszCheck, &( ( char*)pstSendData)[ pstSendData->m_nszResult1], &( ( char*)pstSendData)[ pstSendData->m_nszResult2]);

			if( pstSendData->m_blOtherResult)
			{
				if(  ( int)sizeof( SENDDATA) <= pstSendData->m_nszResult3)
				{
					char	szWork[ 128];
					wsprintf( szWork, m_lpcszOtherA, &( ( char*)pstSendData)[ pstSendData->m_nszResult3]);
					lstrcat( pszResultCheck, szWork);
				}
				else
				{
					lstrcat( pszResultCheck, m_lpcszOtherD);
				}
			}
		}
	}

	// �����͕t���܂����H
	if( lpcszSignature)
	{
		// �����͕��̏�H
		if( 0 == pstSendData->m_nAppendPos)
		{
			if( blAddSpace)lstrcpy( m_pszBody, " ");
			//lstrcat( m_pszBody, lpcszSignature);
			{
				int nLength = lstrlen( m_pszBody);
				memcpy( &m_pszBody[ nLength], lpcszSignature, ( nSignatureLen - 2));
				m_pszBody[ nLength + ( nSignatureLen - 2)] = 0;
			}
			lstrcat( m_pszBody, "\r\n");
			lstrcat( m_pszBody, lpcszBody);
			if( pszResultCheck)lstrcat( m_pszBody, pszResultCheck);
		}
		else
		{
			lstrcpy( m_pszBody, lpcszBody);
			if( pszResultCheck)lstrcat( m_pszBody, pszResultCheck);
			lstrcat( m_pszBody, "\r\n");
			if( blAddSpace)lstrcat( m_pszBody, " ");
			//lstrcat( m_pszBody, lpcszSignature);
			{
				int nLength = lstrlen( m_pszBody);
				memcpy( &m_pszBody[ nLength], lpcszSignature, ( nSignatureLen - 2));
				m_pszBody[ nLength + ( nSignatureLen - 2)] = 0;
			}
		}
	}
	else
	{
		lstrcat( m_pszBody, lpcszBody);
		if( pszResultCheck)lstrcat( m_pszBody, pszResultCheck);
	}

	blResult = TRUE;;

errordetect:
	if( pszResultCheck)
	{
		free(  pszResultCheck);
	}
	// ��n���ł�
	if( INVALID_HANDLE_VALUE != hFileMapping)
	{
		UnmapViewOfFile( lpcszSignature);
		CloseHandle( hFileMapping);
		hFileMapping = NULL;
	}
	if( INVALID_HANDLE_VALUE != hFileSignature)
	{
		CloseHandle( hFileSignature);
		hFileSignature = NULL;
	}
	return blResult;
}

BOOL CMailS::SetMailBody( const RESULTDATA* pResultData, int nResultDataSize, LPCSTR lpcszSignaturePath)
{
	BOOL blResult = FALSE;

	LPCSTR	lpcszBody = NULL;
	int		nBodyLen = 0;
	LPCSTR	lpcszSignature = NULL;
	int		nSignatureLen = 0;
	BOOL	blAddSpace = FALSE;

	LPCSTR	lpcszTitle = NULL;

	// �}�X�^�f�[�^��ۑ����܂�
	m_nRawDataSize = nResultDataSize;
	m_lpvRawData = malloc( m_nRawDataSize);
	if( NULL == m_lpvRawData)goto errordetect;
	CopyMemory( m_lpvRawData, pResultData, m_nRawDataSize);

	// �^�C�g���������o���܂��B
	if( ( int)sizeof( RESULTDATA) <= pResultData->m_nszCHIPID)
	{
		lpcszTitle = &( ( char*)pResultData)[ pResultData->m_nszCHIPID];
	}
	if( NULL == lpcszTitle)goto errordetect;
	if( 0 >= lstrlen( lpcszTitle))goto errordetect;
	m_pszSubject = new char [ lstrlen( lpcszTitle) + 1];
	if( NULL == m_pszSubject)goto errordetect;
	lstrcpy( m_pszSubject, lpcszTitle);

	// �ŏ��ɑS�̃T�C�Y���v�Z���܂��傤�I

	// ���M�p�̕��͎擾
	if( ( int)sizeof( RESULTDATA) <= pResultData->m_nszResult)
	{
		lpcszBody = &( ( char*)pResultData)[ pResultData->m_nszResult];
	}
	if( NULL == lpcszBody)goto errordetect;
	nBodyLen = lstrlen( lpcszBody);
	if( 0 >= nBodyLen)goto errordetect;

	// ���M��񂩂珐�������o���Ă݂�
	if( ( int)sizeof( RESULTDATA) <= pResultData->m_nszSignature)
	{
		if( NULL != &( ( char*)pResultData)[ pResultData->m_nszSignature])
		{
			lpcszSignature = &( ( char*)pResultData)[ pResultData->m_nszSignature];
			nSignatureLen = lstrlen( lpcszSignature);
			nSignatureLen += 2; // �����Ɩ{���̊Ԃɂ͉��s������ł���H

			// ������"From:"�ł͂��܂�ƁA�T�[�o���ŕs�������������Ƃ��L�邻���Ȃ̂ŁA�擪��" "�}������
			char*	pszFrom = "From:";
			int nFrom = lstrlen( pszFrom);
			if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &( ( char*)pResultData)[ pResultData->m_nszSignature], nFrom, pszFrom, nFrom))
			{
				// �X�y�[�X����t�����܂��B
				blAddSpace = TRUE;
				nSignatureLen++;
			}
		}
	}

	// ���āA�K�v�ȗʂ̍�ƃG���A�����܂��傤�I
	m_pszBody = new char[ nBodyLen + nSignatureLen + 1];
	ZeroMemory( m_pszBody, nBodyLen + nSignatureLen + 1);

	lstrcpy( m_pszBody, lpcszBody);
	// �����͕t���܂����H
	if( lpcszSignature)
	{
		lstrcat( m_pszBody, "\r\n");
		if( blAddSpace)lstrcat( m_pszBody, " ");
		lstrcat( m_pszBody, lpcszSignature);
	}

	blResult = TRUE;

errordetect:
	return blResult;
}

BOOL CMailS::SetSubject( LPCSTR lpcszSubject)
{
	if( NULL != lpcszSubject)
	{
		LPSTR	lpszWork = new char [ lstrlen( lpcszSubject) + 1];
		if( lpszWork)
		{
			if( NULL != m_pszSubject)
			{
				delete [] m_pszSubject;
				m_pszSubject = NULL;
			}
			m_pszSubject = lpszWork;
			lstrcpy( m_pszSubject, lpcszSubject);
			return TRUE;
		}
	}
	return FALSE;
}
