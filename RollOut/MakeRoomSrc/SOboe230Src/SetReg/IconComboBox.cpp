// IconComboBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "SetReg.h"
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
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
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
		CBitmap		cBitmap;
		CBitmap*	cOldBitmap;
		cBitmap.LoadBitmap( IDB_POSTIT);
		CDC	cMemDC;
		cMemDC.CreateCompatibleDC( pDC);
		cOldBitmap = cMemDC.SelectObject( &cBitmap);
		int nHeight = ( rcItem.bottom - rcItem.top) - 1;
		pDC->SetBkColor( clrBkColor);
		pDC->SetTextColor( clrTextColor);
		pDC->BitBlt( rcItem.left + 1, rcItem.top + 1, 16, min( 16, nHeight), &cMemDC, 16 * lpDrawItemStruct->itemID, 0, SRCCOPY);
		cMemDC.SelectObject( cOldBitmap);
		cMemDC.DeleteDC();
		pDC->RestoreDC( nSaveID);
	}

	// �t�H�[�J�X�̋�`
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CIconComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: �w�肳�ꂽ�A�C�e���̃T�C�Y�𒲂ׂ邽�߂̃R�[�h��ǉ����Ă�������
	lpMeasureItemStruct->itemHeight = 16;
	
}
