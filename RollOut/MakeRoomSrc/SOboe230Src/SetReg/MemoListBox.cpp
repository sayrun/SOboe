// MemoListBox.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "resource.h"
#include "MemoListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox
IMPLEMENT_DYNAMIC( CMemoListBox, CListBox )

CMemoListBox::CMemoListBox()
{
}

CMemoListBox::~CMemoListBox()
{
}


BEGIN_MESSAGE_MAP(CMemoListBox, CListBox)
	//{{AFX_MSG_MAP(CMemoListBox)
	ON_WM_DELETEITEM_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox メッセージ ハンドラ

void CMemoListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください
	if( m_hWnd != lpDrawItemStruct->hwndItem)return;
	if( NULL == lpDrawItemStruct->hDC)return;
	if( 0 > ( int)lpDrawItemStruct->itemID)return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	// 空白
	CRect rcItem(lpDrawItemStruct->rcItem);

	// 要求された色で色付きのアイテムを塗りつぶします
	int nSaveID = pDC->SaveDC();
	if( nSaveID)
	{
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
			clrTxColor = GetSysColor( ( lpDrawItemStruct->itemState & ODS_DISABLED) ? COLOR_GRAYTEXT : COLOR_WINDOWTEXT);
			CBrush brush( clrBkColor);
			pDC->FillRect( rcItem, &brush);
		}
		if( 0 <= lpDrawItemStruct->itemID)
		{
			const CMemoChip*	pcMemoChip;
			pcMemoChip = ( CMemoChip*)lpDrawItemStruct->itemData;
			const CMemoData*	pcMemoData;
			pcMemoData = pcMemoChip->AccessMemoData();

			// アイコンの描画
			CBitmap		cBitmap;
			CBitmap*	cOldBitmap;
			cBitmap.LoadBitmap( IDB_ICONIMG);
			CDC	cMemDC;
			cMemDC.CreateCompatibleDC( pDC);
			cOldBitmap = cMemDC.SelectObject( &cBitmap);
			COLORREF	clrWork;
			pcMemoData->GetBackColor( clrWork);
			pDC->SetBkColor( clrWork);
			pcMemoData->GetForeColor( clrWork);
			pDC->SetTextColor( clrWork);
			int nHeight = ( rcItem.bottom - rcItem.top) - 1;

			CSetRegApp*	pcSetRegApp = ( CSetRegApp*)AfxGetApp();
			ASSERT( pcSetRegApp);
			const CIconImgList* pcIconImgs = pcSetRegApp->GetIconList();
			pcIconImgs->Draw( pcMemoData->GetIconIndex(), pDC->GetSafeHdc(), CRect( rcItem.left + 1, rcItem.top + 1, rcItem.left + 1 + 16, rcItem.top + 1 + min( 16, nHeight)));

//			pDC->BitBlt( rcItem.left + 1, rcItem.top + 1, 16, min( 16, nHeight), &cMemDC, 16 * pcMemoData->GetIconIndex(), 0, SRCCOPY);
			cMemDC.SelectObject( cOldBitmap);
			cMemDC.DeleteDC();

			// 文字の描画
			pDC->SetBkColor( clrBkColor);
			pDC->SetTextColor( clrTxColor);
			rcItem.left += 19;
			CString	cStr;
			pcMemoData->GetTitle( cStr);
			pDC->DrawText( cStr, cStr.GetLength(), rcItem, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
		pDC->RestoreDC( nSaveID);
	}

	// フォーカスの矩形
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

int CMemoListBox::AddMemoChip( const CMemoChip* pcMemoChip)
{
	CMemoChip*	pcMemoNodeChip;

	pcMemoNodeChip = new CMemoChip;
	*pcMemoNodeChip = *pcMemoChip;

	int nResult = CListBox::AddString( ( LPCSTR)pcMemoNodeChip);

	if( LB_ERR == nResult || LB_ERRSPACE == nResult)
	{
		delete pcMemoNodeChip;
	}

	if( _MAX_TEMPLET <= CListBox::GetCount())
	{
		m_cOleDrop.DropEnable( FALSE);
	}

	return nResult;
}

int CMemoListBox::InsertMemoChip( int nIndex, const CMemoChip* pcMemoChip)
{
	CMemoChip*	pcMemoNodeChip;

	pcMemoNodeChip = new CMemoChip;
	*pcMemoNodeChip = *pcMemoChip;

	int nResult = CListBox::InsertString( nIndex, ( LPCSTR)pcMemoNodeChip);
	if( LB_ERR == nResult || LB_ERRSPACE == nResult)
	{
		delete pcMemoNodeChip;
	}

	return nResult;
}

const CMemoChip* CMemoListBox::GetMemoChip( int nIndex)
{
	if( 0 > nIndex || CListBox::GetCount() <= nIndex)return NULL;
	DWORD dwData = CListBox::GetItemData( nIndex);
	if( LB_ERR == dwData)return NULL;
	return ( const CMemoChip*)dwData;
}

int CMemoListBox::DeleteMemoChip( int nIndex)
{
	int nResult = CListBox::DeleteString( nIndex);
	if( _MAX_TEMPLET > CListBox::GetCount())
	{
		m_cOleDrop.DropEnable( TRUE);
	}
	return nResult;
}

void CMemoListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if( ODT_LISTBOX == lpDeleteItemStruct->CtlType)
	{
		CMemoChip*	pcMemoChip;
		pcMemoChip = ( CMemoChip*)lpDeleteItemStruct->itemData;
		delete pcMemoChip; 
	}
}

void CMemoListBox::StartDD( void)
{
	m_cOleDrop.Register( this);
}

void CMemoListBox::EndDD( void)
{
	m_cOleDrop.Revoke();
}

BOOL CMemoListBox::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	BOOL	blResult = 0;

	if( dropEffect & DROPEFFECT_COPY)
	{
		if( pDataObject->IsDataAvailable( CMemoData::GetClipboardFormat()))
		{
			HGLOBAL	hGMem;
			hGMem = pDataObject->GetGlobalData( CMemoData::GetClipboardFormat());
			if( hGMem)
			{
				DWORD dwSize = GlobalSize( hGMem);
				LPVOID	lpvData = GlobalLock( hGMem);
				if( lpvData)
				{
					CMemoData*	pcMemoData = CMemoData::CreateMemoFromBinData( ( BYTE*)lpvData, dwSize);
					if( pcMemoData)
					{
						if( !pcMemoData->IsPassWordLock())
						{
							CMemoChip	cMemoChip;
							cMemoChip.SetMemoData( *pcMemoData);
							AddMemoChip( &cMemoChip);
							blResult = 1;
						}
						delete pcMemoData;
					}
					GlobalUnlock( hGMem);
				}
			}
		}
	}
	return blResult;
}

DROPEFFECT CMemoListBox::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return OnDragOver( pDataObject, dwKeyState, point);
}

DROPEFFECT CMemoListBox::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
//	STGMEDIUM		stG;
	if( pDataObject->IsDataAvailable( CMemoData::GetClipboardFormat()))
	{
//		pDataObject->GetData( CMemoData::GetClipboardFormat(), &stG);
//		if( 1 == stG.tymed)return DROPEFFECT_COPY;
		return DROPEFFECT_COPY;
	}

	return DROPEFFECT_NONE;
}
