#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Base64.h"

static char* pszBase64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static int panBase64[] = { -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -1, -2, -2, -2, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2, -2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};


int Base64Decode( unsigned char* pabyData, int nDataSize, const char* pszDecodeData)
{
	int nSourceLen = lstrlen( pszDecodeData);
	if( 0 >= nSourceLen)return 0;

	int nSize = ( nSourceLen * 6) / 8;
	if( NULL == pabyData || nSize > nDataSize)return nSize;
/*
	char	achDecode[ 256];
	for( int nIndex = 0; nIndex < 256; nIndex++)
	{
		achDecode[ nIndex] = -2;	// 無効データ域
	}
	for( nIndex = 0; nIndex < 64; nIndex++)
	{
		achDecode[ pszBase64[ nIndex]] = nIndex;
	}
	achDecode[ '='] = -1;			// 無効データ*/

	int nDecodeSize = 0;
	char chData;
	for( int nIndex = 0; nIndex < nSourceLen; nIndex++)
	{
		chData = panBase64[ pszDecodeData[ nIndex]];
		if( -2 == chData)return -1;
		if( -1 == chData)break;
		switch( nIndex % 4)
		{
		case 0:
			pabyData[ nDecodeSize] = ( chData << 2) & 0xFC;
			break;
		case 1:
			pabyData[ nDecodeSize] |= ( chData >> 4) & 0x03;
			nDecodeSize++;
			pabyData[ nDecodeSize] = ( chData << 4) & 0xF0;
			break;
		case 2:
			pabyData[ nDecodeSize] |= ( chData >> 2) & 0x0F;
			nDecodeSize++;
			pabyData[ nDecodeSize] = ( chData << 6) & 0xC0;
			break;
		case 3:
			pabyData[ nDecodeSize] |= chData & 0x3F;
			nDecodeSize++;
			break;
		}
	}
	return nDecodeSize;
}

int Base64Encode( char* pszEncodeData, const unsigned char* pcachData, int nDataSize)
{
	int nSize;
	int nBase64Pos;
	int nEncodePos = 0;
	nSize = ( nDataSize * 8) / 6;
	if( nSize % 4)nSize += ( 4 - ( nSize % 4));

	if( NULL == pszEncodeData)return nSize;

	ZeroMemory( pszEncodeData, nSize + 1);
	FillMemory( pszEncodeData, nSize, '=');

	char* pszWork;
	pszWork = pszEncodeData;

	BYTE	byData1;
	BYTE	byData2;
	BYTE	byData3;
	int		nIndex = 0;

	while( TRUE)
	{
		byData1 = pcachData[ nIndex];
		nBase64Pos = ( byData1 >> 2) & 0x3F;
		*pszWork = pszBase64[ nBase64Pos];
		pszWork++;
		if( nDataSize <= nIndex)break;

		nIndex++;
		byData2 = ( nDataSize > nIndex) ? pcachData[ nIndex] : 0;
		nBase64Pos = ( byData1 << 4) & 0x30;
		nBase64Pos |= ( byData2 >> 4) & 0x0F;
		*pszWork = pszBase64[ nBase64Pos];
		pszWork++;
		if( nDataSize <= nIndex)break;

		nIndex++;
		byData3 = ( nDataSize > nIndex) ? pcachData[ nIndex] : 0;
		nBase64Pos = ( byData2 << 2) & 0x3C;
		nBase64Pos |= ( byData3 >> 6) & 0x03;
		*pszWork = pszBase64[ nBase64Pos];
		pszWork++;

		if( nDataSize <= nIndex)break;
		nBase64Pos = byData3 & 0x3F;
		*pszWork = pszBase64[ nBase64Pos];
		pszWork++;

		nIndex++;
		if( nDataSize <= nIndex)break;
	}
	return nSize;
}
