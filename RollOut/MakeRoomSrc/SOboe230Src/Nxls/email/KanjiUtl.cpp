#include "StdeMail.h"
#include "KanjiUtl.h"
#include "Base64.h"

static char*	pszJisStart = "=?ISO-2022-JP?B?";	//16
static int		nJisStart = lstrlen( pszJisStart);
static char*	pszJisEnd = "?=";					//2
static int		nJisEnd = lstrlen( pszJisEnd);		//3
static char*	pszStartKnj = "\x1b$B";
static int		nStartKnj = lstrlen( pszStartKnj);	//3
static char*	pszEndKnj = "\x1b(J";
static int		nEndKnj = lstrlen( pszEndKnj);

int MimeEncode( LPSTR szEncoded, LPCSTR lpcsz, int nIndex, int nMaxLen)
{
	int		nSrcIndex = nIndex;
	char	szWork[ 128];
	int		nWork = 0;
	ZeroMemory( szWork, 128);

	int nLen = ( ( ( nMaxLen - ( nJisStart + nJisEnd)) * 3 ) / 4) - ( nStartKnj + nEndKnj);

	while( lpcsz[ nSrcIndex])
	{
		// �������ȁH
		if( _ismbblead( lpcsz[ nSrcIndex]))
		{
			// �����A�ꉞ�����݂�����
			if( _ismbbtrail( lpcsz[ nSrcIndex + 1]))
			{
				szWork[ nWork] = lpcsz[ nSrcIndex];
				nWork++;
				nSrcIndex++;
				szWork[ nWork] = lpcsz[ nSrcIndex];
				nWork++;
				nSrcIndex++;
				if( nLen <= nWork)break;
				continue;
			}
		}
		if( ' ' == lpcsz[ nSrcIndex])
		{
			BOOL	blCont = FALSE;
			for( int nWorkIndex = ( nSrcIndex + 1); lpcsz[ nWorkIndex]; nWorkIndex++)
			{
				if( _ismbblead( lpcsz[ nWorkIndex]))
				{
					blCont = TRUE;
					break;
				}
				if( ' ' == lpcsz[ nWorkIndex])
				{
					continue;
				}
			}
			if( TRUE == blCont)
			{
				for( int nWorkIndex = ( nSrcIndex + 1); lpcsz[ nWorkIndex]; nWorkIndex++)
				{
					szWork[ nWork] = lpcsz[ nSrcIndex];
					nWork++;
					nSrcIndex++;
					if( _ismbblead( lpcsz[ nWorkIndex]))break;
				}
				continue;
			}
		}
		break;
	}

	if( 0 < lstrlen( szWork))
	{
		char	szWork2[ 128];
		Sjis2Jis( szWork2, 128, szWork);
		Base64Encode( szWork, ( BYTE*)szWork2, lstrlen( szWork2));
		lstrcpy( szEncoded, pszJisStart);
		lstrcat( szEncoded, szWork);
		lstrcat( szEncoded, pszJisEnd);
	}
	else
	{
		szEncoded[ 0] = 0;
	}

	return ( nSrcIndex - nIndex);
}

int MimeDecode( LPSTR lpsz, int nSize, LPCSTR lpcszSource)
{
	int	nResult = 0;

	ZeroMemory( lpsz, nSize);

	int nSourceLen = lstrlen( lpcszSource);
	for( int nIndex = 0; nIndex < nSourceLen; nIndex++)
	{
		if( '=' == lpcszSource[ nIndex])
		{
			if( CSTR_EQUAL == CompareString( MAKELCID( LANG_JAPANESE, SORT_JAPANESE_XJIS), NORM_IGNORECASE, &lpcszSource[ nIndex], nJisStart, pszJisStart, nJisStart))
			{
				for( int nMimeIndex = 0; nMimeIndex < nSourceLen; nMimeIndex++)
				{
					if( '?' == lpcszSource[ nIndex + nJisStart + nMimeIndex])
					{
						if( '=' == lpcszSource[ nIndex + nJisStart + nMimeIndex + 1])
						{
							char*	pszBase64;
							pszBase64 = new char [ nMimeIndex + 1];
							lstrcpyn( pszBase64, &lpcszSource[ nIndex + nJisStart], nMimeIndex);

							int nSize = Base64Decode( NULL, 0, pszBase64);
							char*	pData;
							pData = new char [ nSize + 1];
							ZeroMemory( pData, nSize + 1);
							Base64Decode( ( BYTE*)pData, nSize, pszBase64);
							pData[ nSize] = 0;

							int		nSjis = CheckJis2SjisLen( pData);
							char*	pSjis = new char [ nSjis + 1];
							Jis2Sjis( pSjis, nSjis + 1, pData);

							int nLen = lstrlen( lpsz);
							int nLenS = lstrlen( pSjis);
							if( ( nLen + nLenS) >= nSize)
							{
								for( int nIndex = nLen; nIndex < ( nSize - 1); nIndex++)
								{
									lpsz[ nIndex] = pSjis[ nLen - nIndex];
								}
							}
							else
							{
								lstrcat( lpsz, pSjis);
							}

							delete [] pSjis;
							delete [] pData;

							delete [] pszBase64;

							nIndex += ( nMimeIndex + nJisStart + nJisEnd - 1);
							break;
						}
					}
				}
				continue;
			}
		}
		int nLen = lstrlen( lpsz);
		if( nLen >= ( nSize - 1))
		{
			lpsz[ ( nSize - 1)] = 0;
			break;
		}
		lpsz[ nLen] = lpcszSource[ nIndex];
	}

#ifdef	_DEBUG
	OutputDebugString( lpsz);
	OutputDebugString( "\r\n");
#endif

	return nResult;
}

int CheckSjis2JisLen( LPCSTR lpcszShiftJis)
{
	int nLen = lstrlen( lpcszShiftJis);
	BOOL blNowKanji = FALSE;

	int nResult = nLen;

	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		// �������ȁH
		if( _ismbblead( lpcszShiftJis[ nIndex]))
		{
			// �����A�ꉞ�����݂�����
			if( _ismbbtrail( lpcszShiftJis[ nIndex + 1]))
			{
				if( !blNowKanji)
				{
					nResult += nStartKnj;
					blNowKanji = TRUE;
				}
				nIndex++;
				continue;
			}
		}
		if( blNowKanji)
		{
			nResult += nEndKnj;
			blNowKanji = FALSE;
		}
	}
	if( blNowKanji)
	{
		nResult += nEndKnj;
		blNowKanji = FALSE;
	}

	return nResult + 1;
}

unsigned int FKan2EKan( unsigned int sjis)
{
	UINT	unChar = sjis;
	switch( unChar)
	{
	case 0x000fa54:
		unChar = 0x000081ca;
		break;
	case 0x000fa55:
		unChar = 0x0000eefa;
		break;
	case 0x000fa56:
		unChar = 0x0000eefb;
		break;
	case 0x000fa57:
		unChar = 0x0000eefc;
		break;
	case 0x000fa58:
		unChar = 0x0000878a;
		break;
	case 0x000fa59:
		unChar = 0x00008782;
		break;
	case 0x000fa5a:
		unChar = 0x00008784;
		break;
	case 0x000fa5b:
		unChar = 0x000081e6;
		break;
	default:
		if( unChar >= UINT( 0x000fa40) && unChar <= UINT( 0x000fa53))
		{
			unChar -= UINT( UINT( 0x000fa40) - UINT( 0x0000eeef));
		}
		else
		{
			if( unChar >= UINT( 0x0000fa5cL) && unChar <= UINT( 0x0000fa7eL))
			{
				// 0xFA5C(�\) - 0xED40(�\)
				// 0xFA5D(�]) - 0xED41(�])
				// 0xFA5E(�^) - 0xED42(�^)
				// 0xFA5F(�_) - 0xED43(�_)
				// 0xFA60(�`) - 0xED44(�`)
				// 0xFA61(�a) - 0xED45(�a)
				// 0xFA62(�b) - 0xED46(�b)
				// 0xFA63(�c) - 0xED47(�c)
				// 0xFA64(�d) - 0xED48(�d)
				// 0xFA65(�e) - 0xED49(�e)
				// 0xFA66(�f) - 0xED4A(�f)
				// 0xFA67(�g) - 0xED4B(�g)
				// 0xFA68(�h) - 0xED4C(�h)
				// 0xFA69(�i) - 0xED4D(�i)
				// 0xFA6A(�j) - 0xED4E(�j)
				// 0xFA6B(�k) - 0xED4F(�k)
				// 0xFA6C(�l) - 0xED50(�l)
				// 0xFA6D(�m) - 0xED51(�m)
				// 0xFA6E(�n) - 0xED52(�n)
				// 0xFA6F(�o) - 0xED53(�o)
				// 0xFA70(�p) - 0xED54(�p)
				// 0xFA71(�q) - 0xED55(�q)
				// 0xFA72(�r) - 0xED56(�r)
				// 0xFA73(�s) - 0xED57(�s)
				// 0xFA74(�t) - 0xED58(�t)
				// 0xFA75(�u) - 0xED59(�u)
				// 0xFA76(�v) - 0xED5A(�v)
				// 0xFA77(�w) - 0xED5B(�w)
				// 0xFA78(�x) - 0xED5C(�x)
				// 0xFA79(�y) - 0xED5D(�y)
				// 0xFA7A(�z) - 0xED5E(�z)
				// 0xFA7B(�{) - 0xED5F(�{)
				// 0xFA7C(�|) - 0xED60(�|)
				// 0xFA7D(�}) - 0xED61(�})
				// 0xFA7E(�~) - 0xED62(�~)
				unChar -= UINT( UINT( 0x0000fa5cL) - UINT( 0x0000ed40L));
			}
			else
			{
				if( unChar >= UINT( 0x0000fa80L) && unChar <= UINT( 0x0000fa9bL))
				{
					// 0xFA80(��) - 0xED63(��)
					// 0xFA81(��) - 0xED64(��)
					// 0xFA82(��) - 0xED65(��)
					// 0xFA83(��) - 0xED66(��)
					// 0xFA84(��) - 0xED67(��)
					// 0xFA85(��) - 0xED68(��)
					// 0xFA86(��) - 0xED69(��)
					// 0xFA87(��) - 0xED6A(��)
					// 0xFA88(��) - 0xED6B(��)
					// 0xFA89(��) - 0xED6C(��)
					// 0xFA8A(��) - 0xED6D(��)
					// 0xFA8B(��) - 0xED6E(��)
					// 0xFA8C(��) - 0xED6F(��)
					// 0xFA8D(��) - 0xED70(��)
					// 0xFA8E(��) - 0xED71(��)
					// 0xFA8F(��) - 0xED72(��)
					// 0xFA90(��) - 0xED73(��)
					// 0xFA91(��) - 0xED74(��)
					// 0xFA92(��) - 0xED75(��)
					// 0xFA93(��) - 0xED76(��)
					// 0xFA94(��) - 0xED77(��)
					// 0xFA95(��) - 0xED78(��)
					// 0xFA96(��) - 0xED79(��)
					// 0xFA97(��) - 0xED7A(��)
					// 0xFA98(��) - 0xED7B(��)
					// 0xFA99(��) - 0xED7C(��)
					// 0xFA9A(��) - 0xED7D(��)
					// 0xFA9B(��) - 0xED7E(��)
					unChar -= UINT( UINT( 0x0000fa80L) - UINT( 0x0000ed63L));
				}
				else
				{
					if( unChar >= UINT( 0x0000fa9cL) && unChar <= UINT( 0x00000fafcL))
					{
						// 0xFA9C(��) - 0xED80(��)
						// 0xFA9D(��) - 0xED81(��)
						// 0xFA9E(��) - 0xED82(��)
						// 0xFA9F(��) - 0xED83(��)
						// 0xFAA0(��) - 0xED84(��)
						// 0xFAA1(��) - 0xED85(��)
						// 0xFAA2(��) - 0xED86(��)
						// 0xFAA3(��) - 0xED87(��)
						// 0xFAA4(��) - 0xED88(��)
						// 0xFAA5(��) - 0xED89(��)
						// 0xFAA6(��) - 0xED8A(��)
						// 0xFAA7(��) - 0xED8B(��)
						// 0xFAA8(��) - 0xED8C(��)
						// 0xFAA9(��) - 0xED8D(��)
						// 0xFAAA(��) - 0xED8E(��)
						// 0xFAAB(��) - 0xED8F(��)
						// 0xFAAC(��) - 0xED90(��)
						// 0xFAAD(��) - 0xED91(��)
						// 0xFAAE(��) - 0xED92(��)
						// 0xFAAF(��) - 0xED93(��)
						// 0xFAB0(��) - 0xED94(��)
						// 0xFAB1(��) - 0xED95(��)
						// 0xFAB2(��) - 0xED96(��)
						// 0xFAB3(��) - 0xED97(��)
						// 0xFAB4(��) - 0xED98(��)
						// 0xFAB5(��) - 0xED99(��)
						// 0xFAB6(��) - 0xED9A(��)
						// 0xFAB7(��) - 0xED9B(��)
						// 0xFAB8(��) - 0xED9C(��)
						// 0xFAB9(��) - 0xED9D(��)
						// 0xFABA(��) - 0xED9E(��)
						// 0xFABB(��) - 0xED9F(��)
						// 0xFABC(��) - 0xEDA0(��)
						// 0xFABD(��) - 0xEDA1(��)
						// 0xFABE(��) - 0xEDA2(��)
						// 0xFABF(��) - 0xEDA3(��)
						// 0xFAC0(��) - 0xEDA4(��)
						// 0xFAC1(��) - 0xEDA5(��)
						// 0xFAC2(��) - 0xEDA6(��)
						// 0xFAC3(��) - 0xEDA7(��)
						// 0xFAC4(��) - 0xEDA8(��)
						// 0xFAC5(��) - 0xEDA9(��)
						// 0xFAC6(��) - 0xEDAA(��)
						// 0xFAC7(��) - 0xEDAB(��)
						// 0xFAC8(��) - 0xEDAC(��)
						// 0xFAC9(��) - 0xEDAD(��)
						// 0xFACA(��) - 0xEDAE(��)
						// 0xFACB(��) - 0xEDAF(��)
						// 0xFACC(��) - 0xEDB0(��)
						// 0xFACD(��) - 0xEDB1(��)
						// 0xFACE(��) - 0xEDB2(��)
						// 0xFACF(��) - 0xEDB3(��)
						// 0xFAD0(��) - 0xEDB4(��)
						// 0xFAD1(��) - 0xEDB5(��)
						// 0xFAD2(��) - 0xEDB6(��)
						// 0xFAD3(��) - 0xEDB7(��)
						// 0xFAD4(��) - 0xEDB8(��)
						// 0xFAD5(��) - 0xEDB9(��)
						// 0xFAD6(��) - 0xEDBA(��)
						// 0xFAD7(��) - 0xEDBB(��)
						// 0xFAD8(��) - 0xEDBC(��)
						// 0xFAD9(��) - 0xEDBD(��)
						// 0xFADA(��) - 0xEDBE(��)
						// 0xFADB(��) - 0xEDBF(��)
						// 0xFADC(��) - 0xEDC0(��)
						// 0xFADD(��) - 0xEDC1(��)
						// 0xFADE(��) - 0xEDC2(��)
						// 0xFADF(��) - 0xEDC3(��)
						// 0xFAE0(��) - 0xEDC4(��)
						// 0xFAE1(��) - 0xEDC5(��)
						// 0xFAE2(��) - 0xEDC6(��)
						// 0xFAE3(��) - 0xEDC7(��)
						// 0xFAE4(��) - 0xEDC8(��)
						// 0xFAE5(��) - 0xEDC9(��)
						// 0xFAE6(��) - 0xEDCA(��)
						// 0xFAE7(��) - 0xEDCB(��)
						// 0xFAE8(��) - 0xEDCC(��)
						// 0xFAE9(��) - 0xEDCD(��)
						// 0xFAEA(��) - 0xEDCE(��)
						// 0xFAEB(��) - 0xEDCF(��)
						// 0xFAEC(��) - 0xEDD0(��)
						// 0xFAED(��) - 0xEDD1(��)
						// 0xFAEE(��) - 0xEDD2(��)
						// 0xFAEF(��) - 0xEDD3(��)
						// 0xFAF0(��) - 0xEDD4(��)
						// 0xFAF1(��) - 0xEDD5(��)
						// 0xFAF2(��) - 0xEDD6(��)
						// 0xFAF3(��) - 0xEDD7(��)
						// 0xFAF4(��) - 0xEDD8(��)
						// 0xFAF5(��) - 0xEDD9(��)
						// 0xFAF6(��) - 0xEDDA(��)
						// 0xFAF7(��) - 0xEDDB(��)
						// 0xFAF8(��) - 0xEDDC(��)
						// 0xFAF9(��) - 0xEDDD(��)
						// 0xFAFA(��) - 0xEDDE(��)
						// 0xFAFB(��) - 0xEDDF(��)
						// 0xFAFC(��) - 0xEDE0(��)
						unChar -= UINT( UINT( 0x0000fa9cL) - UINT( 0x0000ed80L));
					}
					else
					{
						if( unChar >= UINT( 0x0000fb40L) && unChar <= UINT( 0x0000fb5bL))
						{
							// 0xFB40(�@) - 0xEDE1(�@)
							// 0xFB41(�A) - 0xEDE2(�A)
							// 0xFB42(�B) - 0xEDE3(�B)
							// 0xFB43(�C) - 0xEDE4(�C)
							// 0xFB44(�D) - 0xEDE5(�D)
							// 0xFB45(�E) - 0xEDE6(�E)
							// 0xFB46(�F) - 0xEDE7(�F)
							// 0xFB47(�G) - 0xEDE8(�G)
							// 0xFB48(�H) - 0xEDE9(�H)
							// 0xFB49(�I) - 0xEDEA(�I)
							// 0xFB4A(�J) - 0xEDEB(�J)
							// 0xFB4B(�K) - 0xEDEC(�K)
							// 0xFB4C(�L) - 0xEDED(�L)
							// 0xFB4D(�M) - 0xEDEE(�M)
							// 0xFB4E(�N) - 0xEDEF(�N)
							// 0xFB4F(�O) - 0xEDF0(�O)
							// 0xFB50(�P) - 0xEDF1(�P)
							// 0xFB51(�Q) - 0xEDF2(�Q)
							// 0xFB52(�R) - 0xEDF3(�R)
							// 0xFB53(�S) - 0xEDF4(�S)
							// 0xFB54(�T) - 0xEDF5(�T)
							// 0xFB55(�U) - 0xEDF6(�U)
							// 0xFB56(�V) - 0xEDF7(�V)
							// 0xFB57(�W) - 0xEDF8(�W)
							// 0xFB58(�X) - 0xEDF9(�X)
							// 0xFB59(�Y) - 0xEDFA(�Y)
							// 0xFB5A(�Z) - 0xEDFB(�Z)
							// 0xFB5B(�[) - 0xEDFC(�[)
							unChar -= UINT( UINT( 0x0000fb40L) - UINT( 0x0000ede1L));
						}
						else
						{
							if( unChar >= UINT( 0x0000fb5cL) && unChar <= UINT( 0x0000fb7eL))
							{
								// 0xFB5C(�\) - 0xEE40(�\)
								// 0xFB5D(�]) - 0xEE41(�])
								// 0xFB5E(�^) - 0xEE42(�^)
								// 0xFB5F(�_) - 0xEE43(�_)
								// 0xFB60(�`) - 0xEE44(�`)
								// 0xFB61(�a) - 0xEE45(�a)
								// 0xFB62(�b) - 0xEE46(�b)
								// 0xFB63(�c) - 0xEE47(�c)
								// 0xFB64(�d) - 0xEE48(�d)
								// 0xFB65(�e) - 0xEE49(�e)
								// 0xFB66(�f) - 0xEE4A(�f)
								// 0xFB67(�g) - 0xEE4B(�g)
								// 0xFB68(�h) - 0xEE4C(�h)
								// 0xFB69(�i) - 0xEE4D(�i)
								// 0xFB6A(�j) - 0xEE4E(�j)
								// 0xFB6B(�k) - 0xEE4F(�k)
								// 0xFB6C(�l) - 0xEE50(�l)
								// 0xFB6D(�m) - 0xEE51(�m)
								// 0xFB6E(�n) - 0xEE52(�n)
								// 0xFB6F(�o) - 0xEE53(�o)
								// 0xFB70(�p) - 0xEE54(�p)
								// 0xFB71(�q) - 0xEE55(�q)
								// 0xFB72(�r) - 0xEE56(�r)
								// 0xFB73(�s) - 0xEE57(�s)
								// 0xFB74(�t) - 0xEE58(�t)
								// 0xFB75(�u) - 0xEE59(�u)
								// 0xFB76(�v) - 0xEE5A(�v)
								// 0xFB77(�w) - 0xEE5B(�w)
								// 0xFB78(�x) - 0xEE5C(�x)
								// 0xFB79(�y) - 0xEE5D(�y)
								// 0xFB7A(�z) - 0xEE5E(�z)
								// 0xFB7B(�{) - 0xEE5F(�{)
								// 0xFB7C(�|) - 0xEE60(�|)
								// 0xFB7D(�}) - 0xEE61(�})
								// 0xFB7E(�~) - 0xEE62(�~)
								unChar -= UINT( UINT( 0x0000fb5cL) - UINT( 0x0000ee40L));
							}
							else
							{
								if( unChar >= UINT( 0x0000fb80L) && unChar <= UINT( 0x0000fb9bL))
								{
									// 0xFB80(��) - 0xEE63(��)
									// 0xFB81(��) - 0xEE64(��)
									// 0xFB82(��) - 0xEE65(��)
									// 0xFB83(��) - 0xEE66(��)
									// 0xFB84(��) - 0xEE67(��)
									// 0xFB85(��) - 0xEE68(��)
									// 0xFB86(��) - 0xEE69(��)
									// 0xFB87(��) - 0xEE6A(��)
									// 0xFB88(��) - 0xEE6B(��)
									// 0xFB89(��) - 0xEE6C(��)
									// 0xFB8A(��) - 0xEE6D(��)
									// 0xFB8B(��) - 0xEE6E(��)
									// 0xFB8C(��) - 0xEE6F(��)
									// 0xFB8D(��) - 0xEE70(��)
									// 0xFB8E(��) - 0xEE71(��)
									// 0xFB8F(��) - 0xEE72(��)
									// 0xFB90(��) - 0xEE73(��)
									// 0xFB91(��) - 0xEE74(��)
									// 0xFB92(��) - 0xEE75(��)
									// 0xFB93(��) - 0xEE76(��)
									// 0xFB94(��) - 0xEE77(��)
									// 0xFB95(��) - 0xEE78(��)
									// 0xFB96(��) - 0xEE79(��)
									// 0xFB97(��) - 0xEE7A(��)
									// 0xFB98(��) - 0xEE7B(��)
									// 0xFB99(��) - 0xEE7C(��)
									// 0xFB9A(��) - 0xEE7D(��)
									// 0xFB9B(��) - 0xEE7E(��)
									unChar -= UINT( UINT( 0x0000fb80L) - UINT( 0x0000ee63L));
								}
								else
								{
									if( unChar >= UINT( 0x0000fb9cL) && unChar <= UINT( 0x0000fbfcL))
									{
										// 0xFB9C(��) - 0xEE80(��)
										// 0xFB9D(��) - 0xEE81(��)
										// 0xFB9E(��) - 0xEE82(��)
										// 0xFB9F(��) - 0xEE83(��)
										// 0xFBA0(��) - 0xEE84(��)
										// 0xFBA1(��) - 0xEE85(��)
										// 0xFBA2(��) - 0xEE86(��)
										// 0xFBA3(��) - 0xEE87(��)
										// 0xFBA4(��) - 0xEE88(��)
										// 0xFBA5(��) - 0xEE89(��)
										// 0xFBA6(��) - 0xEE8A(��)
										// 0xFBA7(��) - 0xEE8B(��)
										// 0xFBA8(��) - 0xEE8C(��)
										// 0xFBA9(��) - 0xEE8D(��)
										// 0xFBAA(��) - 0xEE8E(��)
										// 0xFBAB(��) - 0xEE8F(��)
										// 0xFBAC(��) - 0xEE90(��)
										// 0xFBAD(��) - 0xEE91(��)
										// 0xFBAE(��) - 0xEE92(��)
										// 0xFBAF(��) - 0xEE93(��)
										// 0xFBB0(��) - 0xEE94(��)
										// 0xFBB1(��) - 0xEE95(��)
										// 0xFBB2(��) - 0xEE96(��)
										// 0xFBB3(��) - 0xEE97(��)
										// 0xFBB4(��) - 0xEE98(��)
										// 0xFBB5(��) - 0xEE99(��)
										// 0xFBB6(��) - 0xEE9A(��)
										// 0xFBB7(��) - 0xEE9B(��)
										// 0xFBB8(��) - 0xEE9C(��)
										// 0xFBB9(��) - 0xEE9D(��)
										// 0xFBBA(��) - 0xEE9E(��)
										// 0xFBBB(��) - 0xEE9F(��)
										// 0xFBBC(��) - 0xEEA0(��)
										// 0xFBBD(��) - 0xEEA1(��)
										// 0xFBBE(��) - 0xEEA2(��)
										// 0xFBBF(��) - 0xEEA3(��)
										// 0xFBC0(��) - 0xEEA4(��)
										// 0xFBC1(��) - 0xEEA5(��)
										// 0xFBC2(��) - 0xEEA6(��)
										// 0xFBC3(��) - 0xEEA7(��)
										// 0xFBC4(��) - 0xEEA8(��)
										// 0xFBC5(��) - 0xEEA9(��)
										// 0xFBC6(��) - 0xEEAA(��)
										// 0xFBC7(��) - 0xEEAB(��)
										// 0xFBC8(��) - 0xEEAC(��)
										// 0xFBC9(��) - 0xEEAD(��)
										// 0xFBCA(��) - 0xEEAE(��)
										// 0xFBCB(��) - 0xEEAF(��)
										// 0xFBCC(��) - 0xEEB0(��)
										// 0xFBCD(��) - 0xEEB1(��)
										// 0xFBCE(��) - 0xEEB2(��)
										// 0xFBCF(��) - 0xEEB3(��)
										// 0xFBD0(��) - 0xEEB4(��)
										// 0xFBD1(��) - 0xEEB5(��)
										// 0xFBD2(��) - 0xEEB6(��)
										// 0xFBD3(��) - 0xEEB7(��)
										// 0xFBD4(��) - 0xEEB8(��)
										// 0xFBD5(��) - 0xEEB9(��)
										// 0xFBD6(��) - 0xEEBA(��)
										// 0xFBD7(��) - 0xEEBB(��)
										// 0xFBD8(��) - 0xEEBC(��)
										// 0xFBD9(��) - 0xEEBD(��)
										// 0xFBDA(��) - 0xEEBE(��)
										// 0xFBDB(��) - 0xEEBF(��)
										// 0xFBDC(��) - 0xEEC0(��)
										// 0xFBDD(��) - 0xEEC1(��)
										// 0xFBDE(��) - 0xEEC2(��)
										// 0xFBDF(��) - 0xEEC3(��)
										// 0xFBE0(��) - 0xEEC4(��)
										// 0xFBE1(��) - 0xEEC5(��)
										// 0xFBE2(��) - 0xEEC6(��)
										// 0xFBE3(��) - 0xEEC7(��)
										// 0xFBE4(��) - 0xEEC8(��)
										// 0xFBE5(��) - 0xEEC9(��)
										// 0xFBE6(��) - 0xEECA(��)
										// 0xFBE7(��) - 0xEECB(��)
										// 0xFBE8(��) - 0xEECC(��)
										// 0xFBE9(��) - 0xEECD(��)
										// 0xFBEA(��) - 0xEECE(��)
										// 0xFBEB(��) - 0xEECF(��)
										// 0xFBEC(��) - 0xEED0(��)
										// 0xFBED(��) - 0xEED1(��)
										// 0xFBEE(��) - 0xEED2(��)
										// 0xFBEF(��) - 0xEED3(��)
										// 0xFBF0(��) - 0xEED4(��)
										// 0xFBF1(��) - 0xEED5(��)
										// 0xFBF2(��) - 0xEED6(��)
										// 0xFBF3(��) - 0xEED7(��)
										// 0xFBF4(��) - 0xEED8(��)
										// 0xFBF5(��) - 0xEED9(��)
										// 0xFBF6(��) - 0xEEDA(��)
										// 0xFBF7(��) - 0xEEDB(��)
										// 0xFBF8(��) - 0xEEDC(��)
										// 0xFBF9(��) - 0xEEDD(��)
										// 0xFBFA(��) - 0xEEDE(��)
										// 0xFBFB(��) - 0xEEDF(��)
										// 0xFBFC(��) - 0xEEE0(��)
										unChar -= UINT( UINT( 0x0000fb9cL) - UINT( 0x0000ee80L));
									}
									else
									{
										if( unChar >= UINT( 0x0000fc40L) && unChar <= UINT( 0x0000fc4bL))
										{
											// 0xFC40(�@) - 0xEEE1(�@)
											// 0xFC41(�A) - 0xEEE2(�A)
											// 0xFC42(�B) - 0xEEE3(�B)
											// 0xFC43(�C) - 0xEEE4(�C)
											// 0xFC44(�D) - 0xEEE5(�D)
											// 0xFC45(�E) - 0xEEE6(�E)
											// 0xFC46(�F) - 0xEEE7(�F)
											// 0xFC47(�G) - 0xEEE8(�G)
											// 0xFC48(�H) - 0xEEE9(�H)
											// 0xFC49(�I) - 0xEEEA(�I)
											// 0xFC4A(�J) - 0xEEEB(�J)
											// 0xFC4B(�K) - 0xEEEC(�K)
											unChar -= UINT( UINT( 0x0000fc40L) - UINT( 0x0000eee1L));
										}
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	}
	return unChar;
}

int Sjis2Jis( LPSTR lpszJis, int nSize, LPCSTR lpcszShiftJis)
{
	BOOL	blNowKanji = FALSE;
	UINT	unChar;
	int		nLen = lstrlen( lpcszShiftJis);
	UINT	unWork;
	int		nWritePos = 0;
	BOOL	bl2ndCode = FALSE;

	ZeroMemory( lpszJis, nSize);
	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		// �������ȁH
		if( _ismbblead( lpcszShiftJis[ nIndex]))
		{
			// �����A�ꉞ�����݂�����
			if( _ismbbtrail( lpcszShiftJis[ nIndex + 1]))
			{
				unWork = ( lpcszShiftJis[ nIndex] & 0x00FF);
				unChar = unWork;
				unChar <<= 8;
				unWork = ( lpcszShiftJis[ nIndex + 1] & 0x00FF);
				unChar |= unWork;

				// �U�i0x0000fa4b�j�ƇU�i0x00008755�j���Ⴄ�R�[�h���Ȃ�Ďv��Ȃ��W�����B
				// �A�i0x0000fa41�j���A�i0x0000eef0�j���Ⴄ�I
				// �K�i0x0000fc4b�j���K�i0x0000eeec�j���Ⴄ�I
				// ������0x0000fxxx�̃R�[�h����������_mbcjmstojis�ŕϊ��ł��Ȃ��Ȃ�Ă����B
				if( 0x0000f000 == ( unChar & 0x0000f000))
				{
					// ���������̍�Ōʂ̕ϊ��őΏ����邱�Ƃɂ����B
					unChar = FKan2EKan( unChar);
				}
				unChar = _mbcjmstojis( unChar);
				// ����ł��ϊ��ł��Ȃ������ꍇ�Ɂw�E�x������悤�ɂ����́B
				if( 0 == unChar)unChar = 0x00002126;	//_mbcjmstojis( int ( '�E'));

				if( !blNowKanji)
				{
					if( ( nWritePos + nStartKnj) >= nSize)goto memory_size_error;
					lstrcat( lpszJis, pszStartKnj);
					nWritePos += nStartKnj;
					blNowKanji = TRUE;
				}
				
				if( ( nWritePos + 2) >= nSize)goto memory_size_error;
				lpszJis[ nWritePos] = char( ( ( unChar >> 8) & 0x0000007F));
				nWritePos++;
				lpszJis[ nWritePos] = char( ( unChar & 0x0000007F));
				nWritePos++;
				nIndex++;
				continue;
			}
		}
		if( blNowKanji)
		{
			if( ( nWritePos + nEndKnj) >= nSize)goto memory_size_error;
			lstrcat( lpszJis, pszEndKnj);
			nWritePos += nEndKnj;
			blNowKanji = FALSE;
		}
		if( ( nWritePos + 1) >= nSize)goto memory_size_error;
		lpszJis[ nWritePos] = lpcszShiftJis[ nIndex];
		nWritePos++;
	}
	if( blNowKanji)
	{
		if( ( nWritePos + nEndKnj) >= nSize)goto memory_size_error;
		lstrcat( lpszJis, pszEndKnj);
		nWritePos += nEndKnj;
		blNowKanji = FALSE;
	}
	return lstrlen( lpszJis);
memory_size_error:
	ZeroMemory( lpszJis, nSize);
	return CheckSjis2JisLen( lpcszShiftJis);
}

int CheckJis2SjisLen( LPCSTR lpcszJis)
{
	BOOL	blNowKanji = FALSE;
	int		nLen = lstrlen( lpcszJis);

	int	nResult = 0;
	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		// �������ȁH
		if( blNowKanji)
		{
			if( '\x1b' == lpcszJis[ nIndex])
			{
				if( '(' == lpcszJis[ nIndex + 1])
				{
					if( 'J' == lpcszJis[ nIndex + 2])
					{
						blNowKanji = FALSE;
						nIndex += 2;
						continue;
					}
				}
			}
		}
		else
		{
			if( '\x1b' == lpcszJis[ nIndex])
			{
				if( '$' == lpcszJis[ nIndex + 1])
				{
					if( 'B' == lpcszJis[ nIndex + 2])
					{
						blNowKanji = TRUE;
						nIndex += 2;
						continue;
					}
				}
			}
		}
		nResult++;
	}
	return nResult;
}

int Jis2Sjis( LPSTR lpszShiftJis, int nSize, LPCSTR lpcszJis)
{
	BOOL	blNowKanji = FALSE;
	UINT	unChar;
	int		nLen = lstrlen( lpcszJis) + 1;
	UINT	unWork;
	int		nWritePos = 0;
	int		nStartKnj = lstrlen( "\x1b$B");
	int		nEndKnj = lstrlen( "\x1b(J");

	ZeroMemory( lpszShiftJis, nSize);
	for( int nIndex = 0; nIndex < nLen; nIndex++)
	{
		if( 0 == lpcszJis[ nIndex])
		{
			lpszShiftJis[ nWritePos] = lpcszJis[ nIndex];
			break;
		}

		// �������ȁH
		if( blNowKanji)
		{
			if( '\x1b' == lpcszJis[ nIndex])
			{
				if( '(' == lpcszJis[ nIndex + 1])
				{
					if( 'J' == lpcszJis[ nIndex + 2])
					{
						blNowKanji = FALSE;
						nIndex += 2;
						continue;
					}
				}
			}
			unWork = ( lpcszJis[ nIndex] & 0x00FF);
			unChar = unWork;
			unChar <<= 8;
			unWork = ( lpcszJis[ nIndex + 1] & 0x00FF);
			unChar |= unWork;

			unChar = _mbcjistojms( unChar);

			if( ( nWritePos + 2) > nSize)goto memory_size_error;
			lpszShiftJis[ nWritePos] = char( ( ( unChar >> 8) & 0x000000FF));
			nWritePos++;
			lpszShiftJis[ nWritePos] = char( ( unChar & 0x000000FF));
			nWritePos++;
			nIndex++;
			continue;
		}
		else
		{
			if( '\x1b' == lpcszJis[ nIndex])
			{
				if( '$' == lpcszJis[ nIndex + 1])
				{
					if( 'B' == lpcszJis[ nIndex + 2])
					{
						blNowKanji = TRUE;
						nIndex += 2;
						continue;
					}
				}
			}
			if( ( nWritePos + 1) > nSize)goto memory_size_error;
			lpszShiftJis[ nWritePos] = lpcszJis[ nIndex];
			nWritePos++;
		}
	}
	return lstrlen( lpszShiftJis);
memory_size_error:
	ZeroMemory( lpszShiftJis, nSize);
	return CheckJis2SjisLen( lpcszJis);
}
