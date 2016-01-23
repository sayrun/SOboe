#ifndef __TREECTRL_H__
#define	__TREECTRL_H__

// MFCのCTreeCtrlのまねっこクラス
// でもMFCはでかいから今回は利用しない。
// とはいっても、常駐するものでなし、問題ないのだが・・・・・
// ちこっとの凝りってヤツですわ。
class CTreeCtrl
{
public:
	CTreeCtrl( void);

	BOOL Attach( HWND hWndNew);
	HWND Detach( void);

	HWND GetSafeHwnd( void) const;

	UINT GetCount() const;

	HTREEITEM InsertItem( LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST, LPARAM lParam = 0);
	HTREEITEM InsertItem( LPCTSTR lpszItem, int nImage, int nSelectedImage, HTREEITEM hParent, HTREEITEM hInsertAfter, LPARAM lPram = 0);
	HTREEITEM InsertItem( LPTVINSERTSTRUCT lpInsertStruct);
	BOOL GetItem( TVITEM* pItem);
	BOOL SetItem( TVITEM* pItem);
	BOOL SetCheck( HTREEITEM hItem, BOOL blCheck = TRUE);
	BOOL GetCheck( HTREEITEM hItem ) const;
	HTREEITEM GetNextSiblingItem( HTREEITEM hItem) const;
	HTREEITEM GetPrevSiblingItem( HTREEITEM hItem) const;


	HTREEITEM GetChildItem( HTREEITEM hItem) const;
	HTREEITEM GetParentItem( HTREEITEM hItem);
	BOOL DeleteItem(HTREEITEM hItem);
	BOOL DeleteAllItems( void);

	HIMAGELIST GetImageList(UINT nImageList) const;
	HIMAGELIST SetImageList( HIMAGELIST hImageList, int nImageListType);
	HIMAGELIST CreateDragImage( HTREEITEM hItem);

	SHORT SetItemHeight( SHORT cyHeight );
	HTREEITEM HitTest( POINT& stPoint, UINT* pFlags);
	BOOL SelectDropTarget( HTREEITEM hItem);

	BOOL TransferItem( HTREEITEM hItemDst, HTREEITEM hItemSrc, BOOL blToPrev);
	BOOL TransferChildItem( HTREEITEM hParentDst, HTREEITEM hParentSrc);

	BOOL SortChildrenCB( LPTVSORTCB pSort);

protected:
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort); 

	HWND m_hWnd;
};

#include "TreeCtrl.inl"

#endif	//__TREECTRL_H__
