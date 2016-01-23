// ResultListBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SOboe.h"
#include "ResultListBox.h"
#include "NetEx_IF.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResultListBox

CResultListBox::CResultListBox()
{
}

CResultListBox::~CResultListBox()
{
}


BEGIN_MESSAGE_MAP(CResultListBox, CListBox)
	//{{AFX_MSG_MAP(CResultListBox)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResultListBox メッセージ ハンドラ

void CResultListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	COLORREF	clrBkColor;
	COLORREF	clrTxColor;
	if( lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		clrBkColor = GetSysColor( COLOR_HIGHLIGHT);
		clrTxColor = GetSysColor( COLOR_HIGHLIGHTTEXT);
		CBrush brush( GetSysColor( COLOR_HIGHLIGHT));
		pDC->FillRect( rcItem, &brush);
	}
	else
	{
		clrBkColor = GetSysColor( COLOR_WINDOW);
		clrTxColor = GetSysColor( COLOR_WINDOWTEXT);
		CBrush brush( GetSysColor( COLOR_WINDOW));
		pDC->FillRect( rcItem, &brush);
	}
	if( 0 <= lpDrawItemStruct->itemID)
	{
		CString	cStr;
		CRect rcItem(lpDrawItemStruct->rcItem);
		int nSaveID = pDC->SaveDC();

		if( _SEND_ABSENCE ==  int( lpDrawItemStruct->itemData))
		{
			clrTxColor = RGB( 0x00, 0x00, 0xFF);
		}
		else
		{
			if( _SEND_SUCCESS != int( lpDrawItemStruct->itemData))
			{
				clrTxColor = RGB( 0xFF, 0x00, 0x00);
			}
		}
		pDC->SetTextColor( clrTxColor);
		pDC->SetBkColor( clrBkColor);
		pDC->SetBkMode( TRANSPARENT);
		GetText( lpDrawItemStruct->itemID, cStr);
		pDC->DrawText( cStr, cStr.GetLength(), rcItem, DT_VCENTER | DT_SINGLELINE);
		pDC->RestoreDC( nSaveID);
	}

	// フォーカスの矩形
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CResultListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: 指定されたアイテムのサイズを調べるためのコードを追加してください
	
}
