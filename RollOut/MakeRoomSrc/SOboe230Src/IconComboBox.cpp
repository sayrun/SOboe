// IconComboBox.cpp : �C���v�������e�[�V���� �t�@�C��
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
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconComboBox ���b�Z�[�W �n���h��

void CIconComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if( GetSafeHwnd() != lpDrawItemStruct->hwndItem)return;
	if( NULL == lpDrawItemStruct->hDC)return;

	CDC* pDC = CDC::FromHandle( lpDrawItemStruct->hDC);
	if( NULL == pDC)return;
	// TODO: �w�肳�ꂽ�A�C�e����`�悷�邽�߂̃R�[�h��ǉ����Ă�������

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

	// �t�H�[�J�X�̋�`
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: �w�肳�ꂽ�A�C�e���̃T�C�Y�𒲂ׂ邽�߂̃R�[�h��ǉ����Ă�������
	lpMeasureItemStruct->itemWidth = _ICON_WIDTH + ( GetSystemMetrics( SM_CXBORDER) * 2);	
	lpMeasureItemStruct->itemHeight = _ICON_HEIGHT + ( GetSystemMetrics( SM_CYBORDER) * 2);	
}
