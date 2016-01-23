#include "stdafx.h"
#include "MemoData.h"
#include "MemoChip.h"
#include "BrwIF.h"
#include "BrwOldStruct.h"
#include "resource.h"

/*
CList < CMemoChip*, CMemoChip*&>	m_cLstMemoChip;
*/

BOOL ParamTest( BRWIFDATA* pBrwData)
{
	if( NULL == pBrwData)return -1;
	if( _BRW_IF_DATA_OLD_VER != pBrwData->m_nVersion &&
		_BRW_IF_DATA_OLD_VER != pBrwData->m_nVersion &&
		_BRW_IF_DATA_CUR_VER != pBrwData->m_nVersion)return -1;

	if( _BRW_IF_DATA_CUR_VER == pBrwData->m_nVersion)
	{
		if( sizeof( BRWIFDATA) != pBrwData->m_nSize)return -1;
	}
	else
	{
		if( _BRW_IF_DATA_300_VER == pBrwData->m_nVersion)
		{
			if( sizeof( BRWIFDATA300) != pBrwData->m_nSize)return -1;
		}
		else
		{
			if( _BRW_IF_DATA_OLD_VER == pBrwData->m_nVersion)
			{
				if( sizeof( BRWIFDATA200) != pBrwData->m_nSize)return -1;
			}
		}
	}
	return 0;
}


CObArray	g_cChipListArray;

DWORD WINAPI GetBrwVersion( int* pnOldStruct, int* pnNewStruct)
{
	DWORD	dwVersion;

	// Brw.DLLのバージョン
	dwVersion = MAKELONG( 0/* マイナー*/, 3/*メジャー*/);

	// サポートする最古の構造体バージョン
	if( pnOldStruct)
	{
		*pnOldStruct = int( _BRW_IF_DATA_200_VER);
	}
	// サポートする最新の構造体バージョン
	if( pnNewStruct)
	{
		*pnNewStruct = int( _BRW_IF_DATA_CUR_VER);
	}

	return dwVersion;
}

void WINAPI InitializeBrw( void)
{
	g_cChipListArray.RemoveAll();
}

void WINAPI UnInitializeBrw( void)
{
	int nSize = g_cChipListArray.GetSize();
	for( int nIndex = 0; nIndex < nSize; nIndex++)
	{
		ReleaseChipGroup( nIndex);
	}
}

int WINAPI CreateChipGroup( void)
{
	int	nArrayNum;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;
	pcLstMemoChip = new CList < CMemoChip*, CMemoChip*&>;
	pcLstMemoChip->RemoveAll();

	nArrayNum = g_cChipListArray.Add( pcLstMemoChip);

	return nArrayNum;
}

int WINAPI ReleaseChipGroup( int nChipGroupID)
{
	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	CMemoChip*	pcMemoChip;
	int nMemoCount = pcLstMemoChip->GetCount();
	while( pcLstMemoChip->GetCount())
	{
		pcMemoChip = pcLstMemoChip->GetHead();
		pcLstMemoChip->RemoveHead();
		if( pcMemoChip)
		{
			delete pcMemoChip;
			pcMemoChip = NULL;
		}
	}
	delete pcLstMemoChip;
	pcLstMemoChip = NULL;

	g_cChipListArray.SetAt( nChipGroupID, NULL);

	return nMemoCount;
}

int WINAPI ReadChipGroup( int nChipGroupID, LPCSTR lpcszFileName)
{
	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	int	ReadCount = 0;
	CFile	cLoadFile;
	CMemoChip*	pcMemoChip;
	CMemoData	cMemoData;
	while( pcLstMemoChip->GetCount())
	{
		pcMemoChip = pcLstMemoChip->GetHead();
		pcLstMemoChip->RemoveHead();
		if( pcMemoChip)
		{
			delete pcMemoChip;
			pcMemoChip = NULL;
		}
	}

	if( cLoadFile.Open( lpcszFileName, CFile::modeRead))
	{
		CArchive	cArchive( &cLoadFile, CArchive::load);

		try
		{
			while( TRUE)
			{
				pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
				pcLstMemoChip->AddTail( pcMemoChip);
				ReadCount++;
			}
		}
		catch( CArchiveException* e)
		{
			e->Delete();
		}

		cArchive.Close();
		cLoadFile.Close();
	}
	return ReadCount;
}

int WINAPI WriteChipGroup( int nChipGroupID, LPCSTR lpcszFileName)
{
	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	CFile		cSaveFile;
	int			nCount;
	CMemoChip*	pcMemoChip;

	cSaveFile.Open( lpcszFileName, CFile::modeWrite | CFile::modeCreate);
	CArchive	cArchive( &cSaveFile, CArchive::store);

	nCount = pcLstMemoChip->GetCount();
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nIndex));
		cArchive.WriteObject( pcMemoChip);
	}
	cArchive.Close();
	cSaveFile.Close();

	return nCount;
}

int WINAPI GetChipCount( int nChipGroupID)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	int nCount = pcLstMemoChip->GetCount();

	return nCount;
}

int WINAPI GetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord)
{
	if( ParamTest( pBrwData))return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->GetIFData( ( IFDATA*)pBrwData, lpcszPassWord);
	}

	return -3;
}

int WINAPI SetChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData, LPCSTR lpcszPassWord)
{
	if( ParamTest( pBrwData))return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;
	if( 0 > nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		pcMemoChip->SetIFData( ( IFDATA*)pBrwData, lpcszPassWord);

		return 0;
	}

	return -1;
}

int WINAPI AddChipData( int nChipGroupID, BRWIFDATA* pBrwData)
{
	if( ParamTest( pBrwData))return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = new CMemoChip;
	if( pcMemoChip)
	{
		pcMemoChip->SetIFData( ( IFDATA*)pBrwData, NULL);
		pcLstMemoChip->AddTail( pcMemoChip);

		return 0;
	}

	return -1;
}

int WINAPI InsertAfterChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData)
{
	if( ParamTest( pBrwData))return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;
	if( 0 > nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = new CMemoChip;
	if( pcMemoChip)
	{
		pcMemoChip->SetIFData( ( IFDATA*)pBrwData, NULL);
		pcLstMemoChip->InsertAfter( pcLstMemoChip->FindIndex( nDataIndex), pcMemoChip);

		return 0;
	}

	return -1;
}

int WINAPI InsertBeforeChipData( int nChipGroupID, int nDataIndex, BRWIFDATA* pBrwData)
{
	if( ParamTest( pBrwData))return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;
	if( 0 > nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = new CMemoChip;
	if( pcMemoChip)
	{
		pcMemoChip->SetIFData( ( IFDATA*)pBrwData, NULL);
		pcLstMemoChip->InsertBefore( pcLstMemoChip->FindIndex( nDataIndex), pcMemoChip);

		return 0;
	}

	return -1;
}

int WINAPI RemoveChipData( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		if( !pcMemoChip->IsPassWordLock( lpszPassWord))
		{
			pcLstMemoChip->RemoveAt( pcLstMemoChip->FindIndex( nDataIndex));
			delete pcMemoChip;
			pcMemoChip = NULL;

			return 0;
		}
	}

	return -1;
}

int WINAPI FreeChipData( BRWIFDATA* pBrwData)
{
	if( ParamTest( pBrwData))return -1;

	CMemoChip::ReleaseIFData( ( IFDATA*)pBrwData);

	return 0;
}

int WINAPI GetChipTitle( int nChipGroupID, int nDataIndex, LPSTR lpszTitle)
{
	if( NULL == lpszTitle)return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		pcMemoChip->GetMemoTitle( lpszTitle);
	}

	return -1;
}

int WINAPI GetChipMemo( int nChipGroupID, int nDataIndex, LPSTR lpszData, int nSize, LPCSTR lpcszPassWord)
{
	int nArraySize = g_cChipListArray.GetUpperBound();
	if( nArraySize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->GetMemo( lpszData, nSize, lpcszPassWord);
	}

	return -1;
}

int WINAPI GetChipFontColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor)
{
	if( NULL == lpColor)return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		*lpColor = pcMemoChip->GetTextColor();
		return 0;
	}
	return -1;
}

int WINAPI GetChipBackColor( int nChipGroupID, int nDataIndex, COLORREF* lpColor)
{
	if( NULL == lpColor)return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		*lpColor = pcMemoChip->GetBackColor();
		return 0;
	}
	return -1;
}

int WINAPI GetChipIcon( int nChipGroupID, int nDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->GetIconIndex();
	}
	return -1;
}

int WINAPI GetChipFont( int nChipGroupID, int nDataIndex, LOGFONT* pstLogFont)
{
	if( NULL == pstLogFont)return -1;

	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		pcMemoChip->GetFont( pstLogFont);
		return 0;
	}
	return -1;
}

int WINAPI IsPwLockChip( int nChipGroupID, int nDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return 0;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return 0;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return 0;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->IsPassWordLock();
	}
	return 0;
}

int WINAPI TestChipPw( int nChipGroupID, int nDataIndex, LPCSTR lpszPassWord)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->IsPassWordLock( lpszPassWord);
	}
	return -1;
}

int WINAPI IsLink( int nChipGroupID, int nDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return 0;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return 0;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return 0;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		return pcMemoChip->IsLink();
	}
	return 0;
}

int WINAPI MoveChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nToChipGroupID)return -1;
	if( nSize < nFromChipGroupID)return -1;
	if( 0 > nToDataIndex)return -1;
	if( 0 > nFromDataIndex)return -1;
	if( nToChipGroupID == nFromChipGroupID && nToDataIndex == nFromDataIndex)return 0;

	if( nToChipGroupID != nFromChipGroupID)
	{
		// 複リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipTo;
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipFrom;

		pcLstMemoChipTo = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nToChipGroupID);
		if( NULL == pcLstMemoChipTo)return -1;
		int nMemoCountTo = pcLstMemoChipTo->GetCount();
		if( nMemoCountTo < nToDataIndex)return -1;

		pcLstMemoChipFrom = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nFromChipGroupID);
		if( NULL == pcLstMemoChipFrom)return -1;
		int nMemoCountFrom = pcLstMemoChipFrom->GetCount();
		if( nMemoCountFrom <= nFromDataIndex)return -1;

		CMemoChip*	pcMemoChip;
		pcMemoChip = pcLstMemoChipFrom->GetAt( pcLstMemoChipFrom->FindIndex( nFromDataIndex));
		if( pcMemoChip)
		{
			if( nMemoCountTo == nToDataIndex)
			{
				pcLstMemoChipTo->AddTail( pcMemoChip);
			}
			else
			{
				pcLstMemoChipTo->InsertBefore(  pcLstMemoChipTo->FindIndex( nToDataIndex), pcMemoChip);
			}
			pcLstMemoChipFrom->RemoveAt( pcLstMemoChipFrom->FindIndex( nFromDataIndex));
			return 0;
		}
		return -2;
	}
	else
	{
		// 単リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

		pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nToChipGroupID);
		if( NULL == pcLstMemoChip)return -1;
		int nMemoCount = pcLstMemoChip->GetCount();
		if( nMemoCount < nToDataIndex)return -1;
		if( nMemoCount <= nFromDataIndex)return -1;
		CMemoChip*	pcMemoChip;
		pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nFromDataIndex));
		if( pcMemoChip)
		{
			if( nMemoCount == nToDataIndex)
			{
				pcLstMemoChip->AddTail( pcMemoChip);
			}
			else
			{
				pcLstMemoChip->InsertBefore(  pcLstMemoChip->FindIndex( nToDataIndex), pcMemoChip);
			}
			if( nToDataIndex < nFromDataIndex)
			{
				nFromDataIndex++;
			}
			pcLstMemoChip->RemoveAt( pcLstMemoChip->FindIndex( nFromDataIndex));
			return 0;
		}
		return -2;
	}
}

int WINAPI SwapChipData( int nDstChipGroupID, int nDstDataIndex, int nSrcChipGroupID, int nSrcDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nDstChipGroupID)return -1;
	if( nSize < nSrcChipGroupID)return -1;
	if( 0 > nDstDataIndex)return -1;
	if( 0 > nSrcDataIndex)return -1;
	if( nDstChipGroupID == nSrcChipGroupID && nDstDataIndex == nSrcDataIndex)return 0;

	if( nDstChipGroupID != nSrcChipGroupID)
	{
		// 複リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipDst;
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipSrc;

		pcLstMemoChipDst = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nDstChipGroupID);
		if( NULL == pcLstMemoChipDst)return -1;
		int nMemoCountTo = pcLstMemoChipDst->GetCount();
		if( nMemoCountTo <= nDstDataIndex)return -1;

		pcLstMemoChipSrc = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nSrcChipGroupID);
		if( NULL == pcLstMemoChipSrc)return -1;
		int nMemoCountFrom = pcLstMemoChipSrc->GetCount();
		if( nMemoCountFrom <= nSrcDataIndex)return -1;

		CMemoChip*	pcMemoChipFrom;
		pcMemoChipFrom = pcLstMemoChipSrc->GetAt( pcLstMemoChipSrc->FindIndex( nSrcDataIndex));
		if( pcMemoChipFrom)
		{
			CMemoChip*	pcMemoChipTo;
			pcMemoChipTo = pcLstMemoChipDst->GetAt( pcLstMemoChipDst->FindIndex( nDstDataIndex));

			if( pcMemoChipFrom)
			{
				pcLstMemoChipSrc->SetAt( pcLstMemoChipSrc->FindIndex( nSrcDataIndex), pcMemoChipTo);
				pcLstMemoChipDst->SetAt( pcLstMemoChipDst->FindIndex( nDstDataIndex), pcMemoChipFrom);
				return 0;
			}
		}
		return -2;
	}
	else
	{
		// 単リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

		pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nSrcChipGroupID);
		if( NULL == pcLstMemoChip)return -1;
		int nMemoCount = pcLstMemoChip->GetCount();
		if( nMemoCount <= nDstDataIndex)return -1;
		if( nMemoCount <= nSrcDataIndex)return -1;
		CMemoChip*	pcMemoChipDst;
		CMemoChip*	pcMemoChipSrc;
		pcMemoChipSrc = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nSrcDataIndex));
		if( pcMemoChipSrc)
		{
			pcMemoChipDst = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDstDataIndex));
			if( pcMemoChipDst)
			{
				pcLstMemoChip->SetAt( pcLstMemoChip->FindIndex( nSrcDataIndex), pcMemoChipDst);
				pcLstMemoChip->SetAt( pcLstMemoChip->FindIndex( nDstDataIndex), pcMemoChipSrc);
				return 0;
			}
		}
		return -2;
	}
	return -3;
}

int WINAPI CopyChipData( int nToChipGroupID, int nToDataIndex, int nFromChipGroupID, int nFromDataIndex)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nToChipGroupID)return -1;
	if( nSize < nFromChipGroupID)return -1;
	if( 0 > nToDataIndex)return -1;
	if( 0 > nFromDataIndex)return -1;
//	if( nToChipGroupID == nFromChipGroupID && nToDataIndex == nFromDataIndex)return 0;

	if( nToChipGroupID != nFromChipGroupID)
	{
		// 複リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipTo;
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChipFrom;

		pcLstMemoChipTo = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nToChipGroupID);
		if( NULL == pcLstMemoChipTo)return -1;
		int nMemoCountTo = pcLstMemoChipTo->GetCount();
		if( nMemoCountTo < nToDataIndex)return -1;

		pcLstMemoChipFrom = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nFromChipGroupID);
		if( NULL == pcLstMemoChipFrom)return -1;
		int nMemoCountFrom = pcLstMemoChipFrom->GetCount();
		if( nMemoCountFrom <= nFromDataIndex)return -1;

		CMemoChip*	pcMemoChip;
		pcMemoChip = pcLstMemoChipFrom->GetAt( pcLstMemoChipFrom->FindIndex( nFromDataIndex));
		if( pcMemoChip)
		{
			CMemoChip*	pcMemoChipNew;
			pcMemoChipNew = new CMemoChip( *pcMemoChip);
			if( nMemoCountTo == nToDataIndex)
			{
				pcLstMemoChipTo->AddTail( pcMemoChipNew);
			}
			else
			{
				pcLstMemoChipTo->InsertBefore(  pcLstMemoChipTo->FindIndex( nToDataIndex), pcMemoChipNew);
			}
			return 0;
		}
		return -2;
	}
	else
	{
		// 単リスト内での移動
		CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

		pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nToChipGroupID);
		if( NULL == pcLstMemoChip)return -1;
		int nMemoCount = pcLstMemoChip->GetCount();
		if( nMemoCount < nToDataIndex)return -1;
		if( nMemoCount <= nFromDataIndex)return -1;
		CMemoChip*	pcMemoChip;
		pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nFromDataIndex));
		if( pcMemoChip)
		{
			CMemoChip*	pcMemoChipNew;
			pcMemoChipNew = new CMemoChip( *pcMemoChip);
			if( nMemoCount == nToDataIndex)
			{
				pcLstMemoChip->AddTail( pcMemoChipNew);
			}
			else
			{
				pcLstMemoChip->InsertBefore(  pcLstMemoChip->FindIndex( nToDataIndex), pcMemoChipNew);
			}
			return 0;
		}
		return -2;
	}
}

int WINAPI DrawIconHelper( HDC hDC, int nLeft, int nTop, int nIconIndex, COLORREF clrFont, COLORREF clrBack)
{
	CDC*	pcDC = CDC::FromHandle( hDC);

	int nSaveID = pcDC->SaveDC();

	CBitmap		cBitmap;
	CBitmap*	cOldBitmap;
	cBitmap.LoadBitmap( IDB_POSTIT);
	CDC	cMemDC;
	cMemDC.CreateCompatibleDC( pcDC);
	cOldBitmap = cMemDC.SelectObject( &cBitmap);
	pcDC->SetBkColor( clrBack);
	pcDC->SetTextColor( clrFont);
	int nIndex = min( nIconIndex, ( MAX_ICON_INDEX + 1));
	pcDC->BitBlt( nLeft, nTop, 16, 16, &cMemDC, 16 * nIndex, 0, SRCERASE);
	pcDC->BitBlt( nLeft, nTop, 16, 16, &cMemDC, 16 * nIndex, 0, SRCPAINT);
	cMemDC.SelectObject( cOldBitmap);
	cMemDC.DeleteDC();

	pcDC->RestoreDC( nSaveID);

	return nIndex;
}

int WINAPI GetMaxIconIndex( void)
{
	return MAX_ICON_INDEX;
}

HBITMAP WINAPI GetIconImage( void)
{
	HBITMAP hBitmap = LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE( IDB_POSTIT));
	return hBitmap;
}

int WINAPI ExportChipGroup( int nChipGroupID, LPCSTR lpcszFileName)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	CFile		cSaveFile;
	int			nCount;
	CMemoChip*	pcMemoChip;

	int	nFileCount = 0;
	if( !cSaveFile.Open( lpcszFileName, CFile::modeWrite | CFile::modeCreate))return -1;

	CString cStrVer( _T( "MRMDAT"));
	int	nVersion = 0x00000300;	//VER_141;

	cSaveFile.Write( cStrVer, cStrVer.GetLength());
	cSaveFile.Write( &nVersion, sizeof( int));

	CArchive	cArchive( &cSaveFile, CArchive::store);
	nCount = pcLstMemoChip->GetCount();
	cArchive << DWORD( nCount);
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nIndex));
		cArchive.WriteObject( pcMemoChip);
	}

	cArchive.Close();
	cSaveFile.Close();

	return 0;
}

int WINAPI ExportChipData( int nChipGroupID, int nDataIndex, LPCSTR lpcszFileName)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;
	int nMemoCount = pcLstMemoChip->GetCount();
	if( nMemoCount <= nDataIndex)return -1;

	CMemoChip*	pcMemoChip;
	pcMemoChip = pcLstMemoChip->GetAt( pcLstMemoChip->FindIndex( nDataIndex));
	if( pcMemoChip)
	{
		CFile		cSaveFile;

		int	nFileCount = 0;
		if( !cSaveFile.Open( lpcszFileName, CFile::modeWrite | CFile::modeCreate))return -1;

		CString cStrVer( _T( "MRMDAT"));
		int	nVersion = 0x00000600;	//VER_160;

		cSaveFile.Write( cStrVer, cStrVer.GetLength());
		cSaveFile.Write( &nVersion, sizeof( int));

		CArchive	cArchive( &cSaveFile, CArchive::store);
		cArchive << DWORD( 1);
		cArchive.WriteObject( pcMemoChip);

		cArchive.Close();
		cSaveFile.Close();
	}

	return 0;
}

int WINAPI ImportChipGroup( int nChipGroupID, LPCSTR lpcszFileName, BOOL blAppend)
{
	int nSize = g_cChipListArray.GetUpperBound();
	if( nSize < nChipGroupID)return -1;

	CList < CMemoChip*, CMemoChip*&>*	pcLstMemoChip;

	pcLstMemoChip = ( CList < CMemoChip*, CMemoChip*&>*)g_cChipListArray.GetAt( nChipGroupID);
	if( !pcLstMemoChip)return -1;

	int	nReadCount = 0;
	CFile	cLoadFile;
	CMemoChip*	pcMemoChip;
	CMemoData	cMemoData;
	if( !blAppend)
	{
		while( pcLstMemoChip->GetCount())
		{
			pcMemoChip = pcLstMemoChip->GetHead();
			pcLstMemoChip->RemoveHead();
			if( pcMemoChip)
			{
				delete pcMemoChip;
				pcMemoChip = NULL;
			}
		}
	}

	if( cLoadFile.Open( lpcszFileName, CFile::modeRead))
	{
		// Version check
		CString	cStrVer;
		CString	cStrGetVer;
		int		nVersion;
		char*	pszVer;

		cStrVer = _T( "MRMDAT");
		int	nLenVer = cStrVer.GetLength();
		pszVer = new char [ nLenVer + 1];
		cLoadFile.Read( pszVer, nLenVer);
		pszVer[ nLenVer] = NULL;
		cStrGetVer = pszVer;
		delete [] pszVer;
		cLoadFile.Read( &nVersion, sizeof( int));
		if( cStrGetVer != cStrVer)return -1;

		CArchive	cArchive( &cLoadFile, CArchive::load);
		try
		{
			DWORD	dwCount;
			cArchive >> dwCount;
			for( int nIndex = 0; nIndex < int( dwCount); nIndex++)
			{
				pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
				pcLstMemoChip->AddTail( pcMemoChip);
				nReadCount++;
			}
		}
		catch( CArchiveException* e)
		{
			e->Delete();
		}

		cArchive.Close();
		cLoadFile.Close();
	}
	return nReadCount;
}
