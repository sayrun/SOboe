#include "stdafx.h"
#include "TreeCtrl.h"

CTreeCtrl::CTreeCtrl( void)
{
	m_hWnd = NULL;
}

HTREEITEM CTreeCtrl::InsertItem( LPCTSTR lpszItem, HTREEITEM hParent, HTREEITEM hInsertAfter, LPARAM lParam)
{
	TVINSERTSTRUCT stTVINS;

	stTVINS.hParent = hParent;
	stTVINS.hInsertAfter = hInsertAfter;
	stTVINS.item.mask = TVIF_TEXT | TVIF_PARAM;
	stTVINS.item.pszText = ( LPTSTR)lpszItem;
	stTVINS.item.iImage = 0;
	stTVINS.item.iSelectedImage = 0;
	stTVINS.item.state = 0;
	stTVINS.item.stateMask = 0;
	stTVINS.item.lParam = lParam;

	return InsertItem( &stTVINS);
}

HTREEITEM CTreeCtrl::InsertItem( LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter, LPARAM lParam)
{
	TVINSERTSTRUCT stTVINS;

	stTVINS.hParent = hParent;
	stTVINS.hInsertAfter = hInsertAfter;
	stTVINS.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	stTVINS.item.pszText = ( LPTSTR)lpszItem;
	stTVINS.item.iImage = nImage;
	stTVINS.item.iSelectedImage = nSelectedImage;
	stTVINS.item.state = 0;
	stTVINS.item.stateMask = 0;
	stTVINS.item.lParam = lParam;

	return InsertItem( &stTVINS);
}

BOOL CTreeCtrl::TransferItem( HTREEITEM hItemDst, HTREEITEM hItemSrc, BOOL blToPrev)
{
	HTREEITEM hParent;
	HTREEITEM hNewItem;

	TCHAR szBuffer[ 512];
	TVINSERTSTRUCT stTVINS;

	stTVINS.item.hItem = hItemDst;
	stTVINS.item.mask = TVIF_HANDLE | TVIF_PARAM;
	GetItem( &stTVINS.item);  // �h���b�O���ꂽ�v�f�̏����擾���܂��B
	BOOL blToChild = ( 0 != stTVINS.item.lParam) ? TRUE : FALSE;

	hParent = GetParentItem( hItemDst);

	stTVINS.item.hItem = hItemSrc;
	stTVINS.item.cchTextMax = 512;
	stTVINS.item.pszText = szBuffer;
	stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
	stTVINS.item.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
	GetItem( &stTVINS.item);  // �h���b�O���ꂽ�v�f�̏����擾���܂��B

	if( blToPrev || blToChild)
	{
		// �A�C�R���Ȃ�A�O�Ɉړ�
		stTVINS.hParent = hParent;
		HTREEITEM hItemPrev = GetPrevSiblingItem( hItemDst);
		if( NULL == hItemPrev)
		{
			hItemPrev = TVI_FIRST;
		}
		stTVINS.hInsertAfter = hItemPrev;//hItemDst;
		stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
		stTVINS.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		hNewItem = InsertItem( &stTVINS);
	}
	else
	{
		// �O���[�v�Ȃ�O���[�v���Ɉړ�
		stTVINS.hParent = hItemDst;
		stTVINS.hInsertAfter = TVI_LAST;
		stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
		stTVINS.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		hNewItem = InsertItem( &stTVINS);

		/*
		// �`�F�b�N�{�b�N�X�̔r��
		stTVINS.item.mask = TVIF_HANDLE | TVIF_STATE;
		stTVINS.item.hItem = hNewItem;
		stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
		stTVINS.item.state = 0;
		SetItem( &stTVINS.item);
		*/
	}

	TransferChildItem( hNewItem, hItemSrc);
	DeleteItem( hItemSrc);

	TVSORTCB stTVSort;
	stTVSort.hParent = GetParentItem( hItemDst);
	if( NULL == stTVSort.hParent)stTVSort.hParent = TVI_ROOT;
	stTVSort.lParam = 0;
	stTVSort.lpfnCompare = CompareFunc;
	SortChildrenCB( &stTVSort);

	return TRUE;
}

int CALLBACK CTreeCtrl::CompareFunc( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	if( NULL == lParam1 && NULL != lParam2)
	{
		return -1;
	}
	else
	{
		if( NULL != lParam1 && NULL == lParam2)
		{
			return 1;
		}
	}

	return 0;
}

BOOL CTreeCtrl::TransferChildItem( HTREEITEM hParentDst, HTREEITEM hParentSrc)
{
	HTREEITEM hItemChild;
	HTREEITEM hNewItem;
	TCHAR szBuffer[ 128];
	TVINSERTSTRUCT stTVINS;

	while( ( hItemChild = GetChildItem( hParentSrc)) != NULL)
	{
		stTVINS.item.hItem = hItemChild;
		stTVINS.item.cchTextMax = 128;
		stTVINS.item.pszText = szBuffer;
		stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
		stTVINS.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		GetItem( &stTVINS.item);  // �h���b�O���ꂽ�v�f�̏����擾���܂��B
		stTVINS.hParent = hParentDst;
		stTVINS.hInsertAfter = TVI_FIRST;
		stTVINS.item.stateMask = TVIS_STATEIMAGEMASK;
		stTVINS.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
		hNewItem = InsertItem( &stTVINS);

		TransferChildItem( hNewItem, hItemChild);
		DeleteItem( hItemChild);
	}
	return TRUE;


	/*
	TVINSERTSTRUCT		stTVINS;
	TCHAR				sztBuffer[50];
	HTREEITEM			hNewItem, hFirstChild;

	// �����ċA��Ԃ�������܂��B
	stTVINS.item.hItem = hitemDrag;
	stTVINS.item.cchTextMax = 49;
	stTVINS.item.pszText = sztBuffer;
	stTVINS.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	GetItem(&stTVINS.item);  // �h���b�O���ꂽ�v�f�̏����擾���܂��B
	stTVINS.hParent = hitemDrop;
	stTVINS.hInsertAfter = TVI_SORT;
	stTVINS.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	hNewItem = InsertItem( &stTVINS);

	while( ( hFirstChild = GetChildItem( hitemDrag)) != NULL)
	{
		TransferItem( hFirstChild, hNewItem);	// ���ׂẴA�C�e�����ċA�I�ɓ]�����܂��B
		DeleteItem( hFirstChild);				// �ŏ��̎q�R���g���[���Ƃ��̂��ׂĂ̎q���폜���܂��B
	}
	return TRUE;
	*/
}

BOOL CTreeCtrl::Attach( HWND hWndNew)
{
	if( NULL == m_hWnd && NULL != hWndNew)
	{
		m_hWnd = hWndNew;

		return TRUE;
	}
	return FALSE;
}

HWND CTreeCtrl::Detach( void)
{
	HWND hResult = m_hWnd;
	m_hWnd = NULL;
	return hResult;
}

BOOL CTreeCtrl::SetCheck( HTREEITEM hItem, BOOL blCheck)
{
	TVITEM stTVItem;
	stTVItem.mask = TVIF_HANDLE | TVIF_STATE;
	stTVItem.hItem = hItem;
	stTVItem.stateMask = TVIS_STATEIMAGEMASK;
	stTVItem.state = INDEXTOSTATEIMAGEMASK( ( ( FALSE == blCheck) ? 1 : TVIS_SELECTED));
	// �Ȃ��A1��TVIS_SELECTED�Ȃ̂��A�悭�킩��Ȃ��B
	// MFC���Q�l�ɂ����B

	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_SETITEM, 0, ( LPARAM)&stTVItem);
}

BOOL CTreeCtrl::GetCheck( HTREEITEM hItem ) const
{
	TVITEM stTVItem;
	stTVItem.mask = TVIF_HANDLE | TVIF_STATE;
	stTVItem.hItem = hItem;
	stTVItem.stateMask = TVIS_STATEIMAGEMASK;
	stTVItem.state = 0;

	::SendMessage( GetSafeHwnd(), TVM_GETITEM, 0, ( LPARAM)&stTVItem);

	return ( 0 != ( INDEXTOSTATEIMAGEMASK( TVIS_SELECTED) & stTVItem.state));
}
