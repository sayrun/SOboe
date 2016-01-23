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
		// 漢字かな？
		if( _ismbblead( lpcsz[ nSrcIndex]))
		{
			// おっ、一応漢字みたいだ
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
		// 漢字かな？
		if( _ismbblead( lpcszShiftJis[ nIndex]))
		{
			// おっ、一応漢字みたいだ
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
				// 0xFA5C(纊) - 0xED40(纊)
				// 0xFA5D(褜) - 0xED41(褜)
				// 0xFA5E(鍈) - 0xED42(鍈)
				// 0xFA5F(銈) - 0xED43(銈)
				// 0xFA60(蓜) - 0xED44(蓜)
				// 0xFA61(俉) - 0xED45(俉)
				// 0xFA62(炻) - 0xED46(炻)
				// 0xFA63(昱) - 0xED47(昱)
				// 0xFA64(棈) - 0xED48(棈)
				// 0xFA65(鋹) - 0xED49(鋹)
				// 0xFA66(曻) - 0xED4A(曻)
				// 0xFA67(彅) - 0xED4B(彅)
				// 0xFA68(丨) - 0xED4C(丨)
				// 0xFA69(仡) - 0xED4D(仡)
				// 0xFA6A(仼) - 0xED4E(仼)
				// 0xFA6B(伀) - 0xED4F(伀)
				// 0xFA6C(伃) - 0xED50(伃)
				// 0xFA6D(伹) - 0xED51(伹)
				// 0xFA6E(佖) - 0xED52(佖)
				// 0xFA6F(侒) - 0xED53(侒)
				// 0xFA70(侊) - 0xED54(侊)
				// 0xFA71(侚) - 0xED55(侚)
				// 0xFA72(侔) - 0xED56(侔)
				// 0xFA73(俍) - 0xED57(俍)
				// 0xFA74(偀) - 0xED58(偀)
				// 0xFA75(倢) - 0xED59(倢)
				// 0xFA76(俿) - 0xED5A(俿)
				// 0xFA77(倞) - 0xED5B(倞)
				// 0xFA78(偆) - 0xED5C(偆)
				// 0xFA79(偰) - 0xED5D(偰)
				// 0xFA7A(偂) - 0xED5E(偂)
				// 0xFA7B(傔) - 0xED5F(傔)
				// 0xFA7C(僴) - 0xED60(僴)
				// 0xFA7D(僘) - 0xED61(僘)
				// 0xFA7E(兊) - 0xED62(兊)
				unChar -= UINT( UINT( 0x0000fa5cL) - UINT( 0x0000ed40L));
			}
			else
			{
				if( unChar >= UINT( 0x0000fa80L) && unChar <= UINT( 0x0000fa9bL))
				{
					// 0xFA80(兤) - 0xED63(兤)
					// 0xFA81(冝) - 0xED64(冝)
					// 0xFA82(冾) - 0xED65(冾)
					// 0xFA83(凬) - 0xED66(凬)
					// 0xFA84(刕) - 0xED67(刕)
					// 0xFA85(劜) - 0xED68(劜)
					// 0xFA86(劦) - 0xED69(劦)
					// 0xFA87(勀) - 0xED6A(勀)
					// 0xFA88(勛) - 0xED6B(勛)
					// 0xFA89(匀) - 0xED6C(匀)
					// 0xFA8A(匇) - 0xED6D(匇)
					// 0xFA8B(匤) - 0xED6E(匤)
					// 0xFA8C(卲) - 0xED6F(卲)
					// 0xFA8D(厓) - 0xED70(厓)
					// 0xFA8E(厲) - 0xED71(厲)
					// 0xFA8F(叝) - 0xED72(叝)
					// 0xFA90(﨎) - 0xED73(﨎)
					// 0xFA91(咜) - 0xED74(咜)
					// 0xFA92(咊) - 0xED75(咊)
					// 0xFA93(咩) - 0xED76(咩)
					// 0xFA94(哿) - 0xED77(哿)
					// 0xFA95(喆) - 0xED78(喆)
					// 0xFA96(坙) - 0xED79(坙)
					// 0xFA97(坥) - 0xED7A(坥)
					// 0xFA98(垬) - 0xED7B(垬)
					// 0xFA99(埈) - 0xED7C(埈)
					// 0xFA9A(埇) - 0xED7D(埇)
					// 0xFA9B(﨏) - 0xED7E(﨏)
					unChar -= UINT( UINT( 0x0000fa80L) - UINT( 0x0000ed63L));
				}
				else
				{
					if( unChar >= UINT( 0x0000fa9cL) && unChar <= UINT( 0x00000fafcL))
					{
						// 0xFA9C(塚) - 0xED80(塚)
						// 0xFA9D(增) - 0xED81(增)
						// 0xFA9E(墲) - 0xED82(墲)
						// 0xFA9F(夋) - 0xED83(夋)
						// 0xFAA0(奓) - 0xED84(奓)
						// 0xFAA1(奛) - 0xED85(奛)
						// 0xFAA2(奝) - 0xED86(奝)
						// 0xFAA3(奣) - 0xED87(奣)
						// 0xFAA4(妤) - 0xED88(妤)
						// 0xFAA5(妺) - 0xED89(妺)
						// 0xFAA6(孖) - 0xED8A(孖)
						// 0xFAA7(寀) - 0xED8B(寀)
						// 0xFAA8(甯) - 0xED8C(甯)
						// 0xFAA9(寘) - 0xED8D(寘)
						// 0xFAAA(寬) - 0xED8E(寬)
						// 0xFAAB(尞) - 0xED8F(尞)
						// 0xFAAC(岦) - 0xED90(岦)
						// 0xFAAD(岺) - 0xED91(岺)
						// 0xFAAE(峵) - 0xED92(峵)
						// 0xFAAF(崧) - 0xED93(崧)
						// 0xFAB0(嵓) - 0xED94(嵓)
						// 0xFAB1(﨑) - 0xED95(﨑)
						// 0xFAB2(嵂) - 0xED96(嵂)
						// 0xFAB3(嵭) - 0xED97(嵭)
						// 0xFAB4(嶸) - 0xED98(嶸)
						// 0xFAB5(嶹) - 0xED99(嶹)
						// 0xFAB6(巐) - 0xED9A(巐)
						// 0xFAB7(弡) - 0xED9B(弡)
						// 0xFAB8(弴) - 0xED9C(弴)
						// 0xFAB9(彧) - 0xED9D(彧)
						// 0xFABA(德) - 0xED9E(德)
						// 0xFABB(忞) - 0xED9F(忞)
						// 0xFABC(恝) - 0xEDA0(恝)
						// 0xFABD(悅) - 0xEDA1(悅)
						// 0xFABE(悊) - 0xEDA2(悊)
						// 0xFABF(惞) - 0xEDA3(惞)
						// 0xFAC0(惕) - 0xEDA4(惕)
						// 0xFAC1(愠) - 0xEDA5(愠)
						// 0xFAC2(惲) - 0xEDA6(惲)
						// 0xFAC3(愑) - 0xEDA7(愑)
						// 0xFAC4(愷) - 0xEDA8(愷)
						// 0xFAC5(愰) - 0xEDA9(愰)
						// 0xFAC6(憘) - 0xEDAA(憘)
						// 0xFAC7(戓) - 0xEDAB(戓)
						// 0xFAC8(抦) - 0xEDAC(抦)
						// 0xFAC9(揵) - 0xEDAD(揵)
						// 0xFACA(摠) - 0xEDAE(摠)
						// 0xFACB(撝) - 0xEDAF(撝)
						// 0xFACC(擎) - 0xEDB0(擎)
						// 0xFACD(敎) - 0xEDB1(敎)
						// 0xFACE(昀) - 0xEDB2(昀)
						// 0xFACF(昕) - 0xEDB3(昕)
						// 0xFAD0(昻) - 0xEDB4(昻)
						// 0xFAD1(昉) - 0xEDB5(昉)
						// 0xFAD2(昮) - 0xEDB6(昮)
						// 0xFAD3(昞) - 0xEDB7(昞)
						// 0xFAD4(昤) - 0xEDB8(昤)
						// 0xFAD5(晥) - 0xEDB9(晥)
						// 0xFAD6(晗) - 0xEDBA(晗)
						// 0xFAD7(晙) - 0xEDBB(晙)
						// 0xFAD8(晴) - 0xEDBC(晴)
						// 0xFAD9(晳) - 0xEDBD(晳)
						// 0xFADA(暙) - 0xEDBE(暙)
						// 0xFADB(暠) - 0xEDBF(暠)
						// 0xFADC(暲) - 0xEDC0(暲)
						// 0xFADD(暿) - 0xEDC1(暿)
						// 0xFADE(曺) - 0xEDC2(曺)
						// 0xFADF(朎) - 0xEDC3(朎)
						// 0xFAE0(朗) - 0xEDC4(朗)
						// 0xFAE1(杦) - 0xEDC5(杦)
						// 0xFAE2(枻) - 0xEDC6(枻)
						// 0xFAE3(桒) - 0xEDC7(桒)
						// 0xFAE4(柀) - 0xEDC8(柀)
						// 0xFAE5(栁) - 0xEDC9(栁)
						// 0xFAE6(桄) - 0xEDCA(桄)
						// 0xFAE7(棏) - 0xEDCB(棏)
						// 0xFAE8(﨓) - 0xEDCC(﨓)
						// 0xFAE9(楨) - 0xEDCD(楨)
						// 0xFAEA(﨔) - 0xEDCE(﨔)
						// 0xFAEB(榘) - 0xEDCF(榘)
						// 0xFAEC(槢) - 0xEDD0(槢)
						// 0xFAED(樰) - 0xEDD1(樰)
						// 0xFAEE(橫) - 0xEDD2(橫)
						// 0xFAEF(橆) - 0xEDD3(橆)
						// 0xFAF0(橳) - 0xEDD4(橳)
						// 0xFAF1(橾) - 0xEDD5(橾)
						// 0xFAF2(櫢) - 0xEDD6(櫢)
						// 0xFAF3(櫤) - 0xEDD7(櫤)
						// 0xFAF4(毖) - 0xEDD8(毖)
						// 0xFAF5(氿) - 0xEDD9(氿)
						// 0xFAF6(汜) - 0xEDDA(汜)
						// 0xFAF7(沆) - 0xEDDB(沆)
						// 0xFAF8(汯) - 0xEDDC(汯)
						// 0xFAF9(泚) - 0xEDDD(泚)
						// 0xFAFA(洄) - 0xEDDE(洄)
						// 0xFAFB(涇) - 0xEDDF(涇)
						// 0xFAFC(浯) - 0xEDE0(浯)
						unChar -= UINT( UINT( 0x0000fa9cL) - UINT( 0x0000ed80L));
					}
					else
					{
						if( unChar >= UINT( 0x0000fb40L) && unChar <= UINT( 0x0000fb5bL))
						{
							// 0xFB40(涖) - 0xEDE1(涖)
							// 0xFB41(涬) - 0xEDE2(涬)
							// 0xFB42(淏) - 0xEDE3(淏)
							// 0xFB43(淸) - 0xEDE4(淸)
							// 0xFB44(淲) - 0xEDE5(淲)
							// 0xFB45(淼) - 0xEDE6(淼)
							// 0xFB46(渹) - 0xEDE7(渹)
							// 0xFB47(湜) - 0xEDE8(湜)
							// 0xFB48(渧) - 0xEDE9(渧)
							// 0xFB49(渼) - 0xEDEA(渼)
							// 0xFB4A(溿) - 0xEDEB(溿)
							// 0xFB4B(澈) - 0xEDEC(澈)
							// 0xFB4C(澵) - 0xEDED(澵)
							// 0xFB4D(濵) - 0xEDEE(濵)
							// 0xFB4E(瀅) - 0xEDEF(瀅)
							// 0xFB4F(瀇) - 0xEDF0(瀇)
							// 0xFB50(瀨) - 0xEDF1(瀨)
							// 0xFB51(炅) - 0xEDF2(炅)
							// 0xFB52(炫) - 0xEDF3(炫)
							// 0xFB53(焏) - 0xEDF4(焏)
							// 0xFB54(焄) - 0xEDF5(焄)
							// 0xFB55(煜) - 0xEDF6(煜)
							// 0xFB56(煆) - 0xEDF7(煆)
							// 0xFB57(煇) - 0xEDF8(煇)
							// 0xFB58(凞) - 0xEDF9(凞)
							// 0xFB59(燁) - 0xEDFA(燁)
							// 0xFB5A(燾) - 0xEDFB(燾)
							// 0xFB5B(犱) - 0xEDFC(犱)
							unChar -= UINT( UINT( 0x0000fb40L) - UINT( 0x0000ede1L));
						}
						else
						{
							if( unChar >= UINT( 0x0000fb5cL) && unChar <= UINT( 0x0000fb7eL))
							{
								// 0xFB5C(犾) - 0xEE40(犾)
								// 0xFB5D(猤) - 0xEE41(猤)
								// 0xFB5E(猪) - 0xEE42(猪)
								// 0xFB5F(獷) - 0xEE43(獷)
								// 0xFB60(玽) - 0xEE44(玽)
								// 0xFB61(珉) - 0xEE45(珉)
								// 0xFB62(珖) - 0xEE46(珖)
								// 0xFB63(珣) - 0xEE47(珣)
								// 0xFB64(珒) - 0xEE48(珒)
								// 0xFB65(琇) - 0xEE49(琇)
								// 0xFB66(珵) - 0xEE4A(珵)
								// 0xFB67(琦) - 0xEE4B(琦)
								// 0xFB68(琪) - 0xEE4C(琪)
								// 0xFB69(琩) - 0xEE4D(琩)
								// 0xFB6A(琮) - 0xEE4E(琮)
								// 0xFB6B(瑢) - 0xEE4F(瑢)
								// 0xFB6C(璉) - 0xEE50(璉)
								// 0xFB6D(璟) - 0xEE51(璟)
								// 0xFB6E(甁) - 0xEE52(甁)
								// 0xFB6F(畯) - 0xEE53(畯)
								// 0xFB70(皂) - 0xEE54(皂)
								// 0xFB71(皜) - 0xEE55(皜)
								// 0xFB72(皞) - 0xEE56(皞)
								// 0xFB73(皛) - 0xEE57(皛)
								// 0xFB74(皦) - 0xEE58(皦)
								// 0xFB75(益) - 0xEE59(益)
								// 0xFB76(睆) - 0xEE5A(睆)
								// 0xFB77(劯) - 0xEE5B(劯)
								// 0xFB78(砡) - 0xEE5C(砡)
								// 0xFB79(硎) - 0xEE5D(硎)
								// 0xFB7A(硤) - 0xEE5E(硤)
								// 0xFB7B(硺) - 0xEE5F(硺)
								// 0xFB7C(礰) - 0xEE60(礰)
								// 0xFB7D(礼) - 0xEE61(礼)
								// 0xFB7E(神) - 0xEE62(神)
								unChar -= UINT( UINT( 0x0000fb5cL) - UINT( 0x0000ee40L));
							}
							else
							{
								if( unChar >= UINT( 0x0000fb80L) && unChar <= UINT( 0x0000fb9bL))
								{
									// 0xFB80(祥) - 0xEE63(祥)
									// 0xFB81(禔) - 0xEE64(禔)
									// 0xFB82(福) - 0xEE65(福)
									// 0xFB83(禛) - 0xEE66(禛)
									// 0xFB84(竑) - 0xEE67(竑)
									// 0xFB85(竧) - 0xEE68(竧)
									// 0xFB86(靖) - 0xEE69(靖)
									// 0xFB87(竫) - 0xEE6A(竫)
									// 0xFB88(箞) - 0xEE6B(箞)
									// 0xFB89(精) - 0xEE6C(精)
									// 0xFB8A(絈) - 0xEE6D(絈)
									// 0xFB8B(絜) - 0xEE6E(絜)
									// 0xFB8C(綷) - 0xEE6F(綷)
									// 0xFB8D(綠) - 0xEE70(綠)
									// 0xFB8E(緖) - 0xEE71(緖)
									// 0xFB8F(繒) - 0xEE72(繒)
									// 0xFB90(罇) - 0xEE73(罇)
									// 0xFB91(羡) - 0xEE74(羡)
									// 0xFB92(羽) - 0xEE75(羽)
									// 0xFB93(茁) - 0xEE76(茁)
									// 0xFB94(荢) - 0xEE77(荢)
									// 0xFB95(荿) - 0xEE78(荿)
									// 0xFB96(菇) - 0xEE79(菇)
									// 0xFB97(菶) - 0xEE7A(菶)
									// 0xFB98(葈) - 0xEE7B(葈)
									// 0xFB99(蒴) - 0xEE7C(蒴)
									// 0xFB9A(蕓) - 0xEE7D(蕓)
									// 0xFB9B(蕙) - 0xEE7E(蕙)
									unChar -= UINT( UINT( 0x0000fb80L) - UINT( 0x0000ee63L));
								}
								else
								{
									if( unChar >= UINT( 0x0000fb9cL) && unChar <= UINT( 0x0000fbfcL))
									{
										// 0xFB9C(蕫) - 0xEE80(蕫)
										// 0xFB9D(﨟) - 0xEE81(﨟)
										// 0xFB9E(薰) - 0xEE82(薰)
										// 0xFB9F(蘒) - 0xEE83(蘒)
										// 0xFBA0(﨡) - 0xEE84(﨡)
										// 0xFBA1(蠇) - 0xEE85(蠇)
										// 0xFBA2(裵) - 0xEE86(裵)
										// 0xFBA3(訒) - 0xEE87(訒)
										// 0xFBA4(訷) - 0xEE88(訷)
										// 0xFBA5(詹) - 0xEE89(詹)
										// 0xFBA6(誧) - 0xEE8A(誧)
										// 0xFBA7(誾) - 0xEE8B(誾)
										// 0xFBA8(諟) - 0xEE8C(諟)
										// 0xFBA9(諸) - 0xEE8D(諸)
										// 0xFBAA(諶) - 0xEE8E(諶)
										// 0xFBAB(譓) - 0xEE8F(譓)
										// 0xFBAC(譿) - 0xEE90(譿)
										// 0xFBAD(賰) - 0xEE91(賰)
										// 0xFBAE(賴) - 0xEE92(賴)
										// 0xFBAF(贒) - 0xEE93(贒)
										// 0xFBB0(赶) - 0xEE94(赶)
										// 0xFBB1(﨣) - 0xEE95(﨣)
										// 0xFBB2(軏) - 0xEE96(軏)
										// 0xFBB3(﨤) - 0xEE97(﨤)
										// 0xFBB4(逸) - 0xEE98(逸)
										// 0xFBB5(遧) - 0xEE99(遧)
										// 0xFBB6(郞) - 0xEE9A(郞)
										// 0xFBB7(都) - 0xEE9B(都)
										// 0xFBB8(鄕) - 0xEE9C(鄕)
										// 0xFBB9(鄧) - 0xEE9D(鄧)
										// 0xFBBA(釚) - 0xEE9E(釚)
										// 0xFBBB(釗) - 0xEE9F(釗)
										// 0xFBBC(釞) - 0xEEA0(釞)
										// 0xFBBD(釭) - 0xEEA1(釭)
										// 0xFBBE(釮) - 0xEEA2(釮)
										// 0xFBBF(釤) - 0xEEA3(釤)
										// 0xFBC0(釥) - 0xEEA4(釥)
										// 0xFBC1(鈆) - 0xEEA5(鈆)
										// 0xFBC2(鈐) - 0xEEA6(鈐)
										// 0xFBC3(鈊) - 0xEEA7(鈊)
										// 0xFBC4(鈺) - 0xEEA8(鈺)
										// 0xFBC5(鉀) - 0xEEA9(鉀)
										// 0xFBC6(鈼) - 0xEEAA(鈼)
										// 0xFBC7(鉎) - 0xEEAB(鉎)
										// 0xFBC8(鉙) - 0xEEAC(鉙)
										// 0xFBC9(鉑) - 0xEEAD(鉑)
										// 0xFBCA(鈹) - 0xEEAE(鈹)
										// 0xFBCB(鉧) - 0xEEAF(鉧)
										// 0xFBCC(銧) - 0xEEB0(銧)
										// 0xFBCD(鉷) - 0xEEB1(鉷)
										// 0xFBCE(鉸) - 0xEEB2(鉸)
										// 0xFBCF(鋧) - 0xEEB3(鋧)
										// 0xFBD0(鋗) - 0xEEB4(鋗)
										// 0xFBD1(鋙) - 0xEEB5(鋙)
										// 0xFBD2(鋐) - 0xEEB6(鋐)
										// 0xFBD3(﨧) - 0xEEB7(﨧)
										// 0xFBD4(鋕) - 0xEEB8(鋕)
										// 0xFBD5(鋠) - 0xEEB9(鋠)
										// 0xFBD6(鋓) - 0xEEBA(鋓)
										// 0xFBD7(錥) - 0xEEBB(錥)
										// 0xFBD8(錡) - 0xEEBC(錡)
										// 0xFBD9(鋻) - 0xEEBD(鋻)
										// 0xFBDA(﨨) - 0xEEBE(﨨)
										// 0xFBDB(錞) - 0xEEBF(錞)
										// 0xFBDC(鋿) - 0xEEC0(鋿)
										// 0xFBDD(錝) - 0xEEC1(錝)
										// 0xFBDE(錂) - 0xEEC2(錂)
										// 0xFBDF(鍰) - 0xEEC3(鍰)
										// 0xFBE0(鍗) - 0xEEC4(鍗)
										// 0xFBE1(鎤) - 0xEEC5(鎤)
										// 0xFBE2(鏆) - 0xEEC6(鏆)
										// 0xFBE3(鏞) - 0xEEC7(鏞)
										// 0xFBE4(鏸) - 0xEEC8(鏸)
										// 0xFBE5(鐱) - 0xEEC9(鐱)
										// 0xFBE6(鑅) - 0xEECA(鑅)
										// 0xFBE7(鑈) - 0xEECB(鑈)
										// 0xFBE8(閒) - 0xEECC(閒)
										// 0xFBE9(隆) - 0xEECD(隆)
										// 0xFBEA(﨩) - 0xEECE(﨩)
										// 0xFBEB(隝) - 0xEECF(隝)
										// 0xFBEC(隯) - 0xEED0(隯)
										// 0xFBED(霳) - 0xEED1(霳)
										// 0xFBEE(霻) - 0xEED2(霻)
										// 0xFBEF(靃) - 0xEED3(靃)
										// 0xFBF0(靍) - 0xEED4(靍)
										// 0xFBF1(靏) - 0xEED5(靏)
										// 0xFBF2(靑) - 0xEED6(靑)
										// 0xFBF3(靕) - 0xEED7(靕)
										// 0xFBF4(顗) - 0xEED8(顗)
										// 0xFBF5(顥) - 0xEED9(顥)
										// 0xFBF6(飯) - 0xEEDA(飯)
										// 0xFBF7(飼) - 0xEEDB(飼)
										// 0xFBF8(餧) - 0xEEDC(餧)
										// 0xFBF9(館) - 0xEEDD(館)
										// 0xFBFA(馞) - 0xEEDE(馞)
										// 0xFBFB(驎) - 0xEEDF(驎)
										// 0xFBFC(髙) - 0xEEE0(髙)
										unChar -= UINT( UINT( 0x0000fb9cL) - UINT( 0x0000ee80L));
									}
									else
									{
										if( unChar >= UINT( 0x0000fc40L) && unChar <= UINT( 0x0000fc4bL))
										{
											// 0xFC40(髜) - 0xEEE1(髜)
											// 0xFC41(魵) - 0xEEE2(魵)
											// 0xFC42(魲) - 0xEEE3(魲)
											// 0xFC43(鮏) - 0xEEE4(鮏)
											// 0xFC44(鮱) - 0xEEE5(鮱)
											// 0xFC45(鮻) - 0xEEE6(鮻)
											// 0xFC46(鰀) - 0xEEE7(鰀)
											// 0xFC47(鵰) - 0xEEE8(鵰)
											// 0xFC48(鵫) - 0xEEE9(鵫)
											// 0xFC49(鶴) - 0xEEEA(鶴)
											// 0xFC4A(鸙) - 0xEEEB(鸙)
											// 0xFC4B(黑) - 0xEEEC(黑)
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
		// 漢字かな？
		if( _ismbblead( lpcszShiftJis[ nIndex]))
		{
			// おっ、一応漢字みたいだ
			if( _ismbbtrail( lpcszShiftJis[ nIndex + 1]))
			{
				unWork = ( lpcszShiftJis[ nIndex] & 0x00FF);
				unChar = unWork;
				unChar <<= 8;
				unWork = ( lpcszShiftJis[ nIndex + 1] & 0x00FF);
				unChar |= unWork;

				// Ⅱ（0x0000fa4b）とⅡ（0x00008755）が違うコードだなんて思わないジャン。
				// ⅱ（0x0000fa41）とⅱ（0x0000eef0）も違う！
				// 黑（0x0000fc4b）と黑（0x0000eeec）も違う！
				// 加えて0x0000fxxxのコードを持つ文字が_mbcjmstojisで変換できないなんてさぁ。
				if( 0x0000f000 == ( unChar & 0x0000f000))
				{
					// だから苦肉の策で個別の変換で対処することにした。
					unChar = FKan2EKan( unChar);
				}
				unChar = _mbcjmstojis( unChar);
				// それでも変換できなかった場合に『・』を入れるようにしたの。
				if( 0 == unChar)unChar = 0x00002126;	//_mbcjmstojis( int ( '・'));

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
		// 漢字かな？
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

		// 漢字かな？
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
