// IconComboBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SOboe.h"
#include "IconComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox

CIconComboBox::CIconComboBox() : CComboBox()
{
}

CIconComboBox::~CIconComboBox()
{
}


BEGIN_MESSAGE_MAP(CIconComboBox, CComboBox)
	//{{AFX_MSG_MAP(CIconComboBox)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox メッセージ ハンドラ

void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( GetSafeHwnd() != lpDrawItemStruct->hwndItem)return;
	if( NULL == lpDrawItemStruct->hDC)return;

	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC);
	if( NULL == pDC)return;
	// TODO: 指定されたアイテムを描画するためのコードを追加してください

	CRect rcItem(lpDrawItemStruct->rcItem);
	COLORREF	clrBkColor;
	COLORREF	clrTextColor;

	if( lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		clrBkColor = GetSysColor( COLOR_HIGHLIGHT);
		clrTextColor = GetSysColor( COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		if( lpDrawItemStruct->itemState & ODS_DISABLED)
		{
			clrBkColor = pDC->GetBkColor();
			clrTextColor = pDC->GetTextColor();
		}
		else
		{
			clrBkColor = GetSysColor( COLOR_WINDOW);
			clrTextColor = GetSysColor( COLOR_WINDOWTEXT);
		}
	}

	CBrush brush( clrBkColor);
	pDC->FillRect( rcItem, &brush);
	if( 0 <= lpDrawItemStruct->itemID)
	{
		int nSaveID = pDC->SaveDC();
		CDC	cMemDC;
		if( cMemDC.CreateCompatibleDC( pDC))
		{
			CBitmap		cBitmap;
			if( cBitmap.LoadBitmap( IDB_ICONIMG))
			{
				CBitmap*	pcOldBitmap;
				pcOldBitmap = cMemDC.SelectObject( &cBitmap);
				int nHeight = ( rcItem.bottom - rcItem.top) - 1;
				pDC->SetBkColor( clrBkColor);
				pDC->SetTextColor( clrTextColor);

				int nXPos = rcItem.left + ( rcItem.Width() - _ICON_WIDTH) / 2;
				int nYPos = rcItem.top + ( rcItem.Height() - _ICON_HEIGHT) / 2;
				pDC->BitBlt( nXPos, nYPos, _ICON_WIDTH, min( _ICON_HEIGHT, nHeight), &cMemDC, _ICON_WIDTH * lpDrawItemStruct->itemID, 0, SRCCOPY);
				cMemDC.SelectObject( pcOldBitmap);
				cBitmap.DeleteObject();
			}
			cMemDC.DeleteDC();
		}
		pDC->RestoreDC( nSaveID);
	}

	// フォーカスの矩形
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: 指定されたアイテムのサイズを調べるためのコードを追加してください
	lpMeasureItemStruct->itemWidth = _ICON_WIDTH + ( GetSystemMetrics( SM_CXBORDER) * 2);	
	lpMeasureItemStruct->itemHeight = _ICON_HEIGHT + ( GetSystemMetrics( SM_CYBORDER) * 2);	
}
