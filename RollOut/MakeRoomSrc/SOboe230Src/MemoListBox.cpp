// MemoListBox.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "MemoListBox.h"
#include "MemoListNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoListBox

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
// CMemoListBox ���b�Z�[�W �n���h��

void CMemoListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: �w�肳�ꂽ�A�C�e����`�悷�邽�߂̃R�[�h��ǉ����Ă�������
	if( GetSafeHwnd() != lpDrawItemStruct->hwndItem)return;
	if( NULL == lpDrawItemStruct->hDC)return;
	if( 0 > ( int)lpDrawItemStruct->itemID)return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	// ��
	CRect rcItem(lpDrawItemStruct->rcItem);

	// �v�����ꂽ�F�ŐF�t���̃A�C�e����h��Ԃ��܂�
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
			clrTxColor = GetSysColor( COLOR_WINDOWTEXT);
			CBrush brush( GetSysColor( COLOR_WINDOW));
			pDC->FillRect( rcItem, &brush);
		}
		if( 0 <= lpDrawItemStruct->itemID)
		{
			CMemoListNode*	pcMemoNode;
			pcMemoNode = ( CMemoListNode*)lpDrawItemStruct->itemData;

			// �A�C�R���̕`��
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
			if( pcIconImgs)
			{
				pDC->SetBkColor( pcMemoNode->GetBackColor());
				pDC->SetTextColor( pcMemoNode->GetForeColor());

				CRect cRect;
				cRect.left = rcItem.left + 1;
				cRect.right = cRect.left + _ICON_WIDTH;
				cRect.top = rcItem.top + 1;
				cRect.bottom = cRect.top + min( _ICON_HEIGHT, ( ( rcItem.bottom - rcItem.top) - 1));
				pcIconImgs->Draw( pcMemoNode->GetIconIndex(), *pDC, cRect);
			}
			else
			{
				CBitmap		cBitmap;
				CBitmap*	cOldBitmap;
				cBitmap.LoadBitmap( IDB_ICONIMG);
				CDC	cMemDC;
				cMemDC.CreateCompatibleDC( pDC);
				cOldBitmap = cMemDC.SelectObject( &cBitmap);
				pDC->SetBkColor( pcMemoNode->GetBackColor());
				pDC->SetTextColor( pcMemoNode->GetForeColor());
				int nHeight = ( rcItem.bottom - rcItem.top) - 1;
				pDC->BitBlt( rcItem.left + 1, rcItem.top + 1, 16, min( 16, nHeight), &cMemDC, 16 * pcMemoNode->GetIconIndex(), 0, SRCCOPY);
				cMemDC.SelectObject( cOldBitmap);
				cMemDC.DeleteDC();
			}

			// �����̕`��
			pDC->SetBkColor( clrBkColor);
			if( pcMemoNode->IsHide())
			{
				pDC->SetTextColor( GetSysColor( COLOR_GRAYTEXT));
			}
			else
			{
				if( pcMemoNode->IsDelOnQuit())
				{
					pDC->SetTextColor( RGB( 0xFF, 0x00, 0x00));
				}
				else
				{
					pDC->SetTextColor( clrTxColor);
				}
			}
			rcItem.left += 19;
			CString	cStr = pcMemoNode->GetTitle();
			pDC->DrawText( cStr, cStr.GetLength(), rcItem, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		}
		pDC->RestoreDC( nSaveID);
	}

	// �t�H�[�J�X�̋�`
	if( lpDrawItemStruct->itemState & ODS_FOCUS)pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

int CMemoListBox::AddMemoChip( const CMemoChip& cMemoChip, BOOL blDelData)
{
	CMemoListNode*	pcMemoNode;

	pcMemoNode = new CMemoListNode( &cMemoChip, blDelData ? CMemoListNode::DATA_DELETE : CMemoListNode::DATA_DISPLAY);

	int nResult = CListBox::AddString( ( LPCSTR)pcMemoNode);

	if( LB_ERR == nResult || LB_ERRSPACE == nResult)
	{
		delete pcMemoNode;
	}

	return nResult;
}

int CMemoListBox::InsertMemoChip( int nIndex, const CMemoChip& cMemoChip, BOOL blDelData)
{
	CMemoListNode*	pcMemoNode;

	pcMemoNode = new CMemoListNode( &cMemoChip, blDelData ? CMemoListNode::DATA_DELETE : CMemoListNode::DATA_DISPLAY);
	int nResult = CListBox::InsertString( nIndex, ( LPCSTR)pcMemoNode);
	if( LB_ERR == nResult || LB_ERRSPACE == nResult)
	{
		delete pcMemoNode;
	}

	return nResult;
}

int CMemoListBox::GetPreviewData( int nIndex, CString& cStrMemo, COLORREF& clrFore, COLORREF& clrBack)
{
	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			if( pcMemoNode->IsPassWordLock())
			{
				cStrMemo.LoadString( IDS_DONOTVIEW);
			}
			else
			{
				cStrMemo = pcMemoNode->GetMemo();
				cStrMemo += pcMemoNode->GetDate();
			}
			clrFore = pcMemoNode->GetForeColor();
			clrBack = pcMemoNode->GetBackColor();

			return TRUE;
		}
	}

	return FALSE;
}

int CMemoListBox::GetPrintData( int nIndex, CMemoData& cMemoData)
{
	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			pcMemoNode->GetData( cMemoData);

			return TRUE;
		}
	}

	return FALSE;
}

HWND CMemoListBox::GetWndHandle( int nIndex)
{
	HWND	hWnd = NULL;

	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			hWnd = pcMemoNode->GetWndHandle();
		}
	}

	return hWnd;
}

BOOL CMemoListBox::IsLink( int nIndex)
{
	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			return pcMemoNode->IsLink();
		}
	}
	return FALSE;
}

BOOL CMemoListBox::IsSeeThrough( int nIndex)
{
	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			return pcMemoNode->IsSeeThrough();
		}
	}
	return FALSE;
}

BOOL CMemoListBox::IsDelOnQuit( int nIndex)
{
	DWORD dwItemData = CListBox::GetItemData( nIndex);
	if( LB_ERR != dwItemData)
	{
		CMemoListNode*	pcMemoNode = ( CMemoListNode*)dwItemData;
		if( pcMemoNode)
		{
			return pcMemoNode->IsDelOnQuit();
		}
	}
	return FALSE;
}

void CMemoListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if( ODT_LISTBOX == lpDeleteItemStruct->CtlType)
	{
		CMemoListNode*	pcMemoNode;
		pcMemoNode = ( CMemoListNode*)lpDeleteItemStruct->itemData;
		delete pcMemoNode; 
	}
}
