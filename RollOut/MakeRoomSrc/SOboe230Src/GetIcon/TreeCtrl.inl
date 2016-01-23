
inline HWND CTreeCtrl::GetSafeHwnd( void) const
{
	return m_hWnd;
}

inline BOOL CTreeCtrl::DeleteAllItems( void)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
}

inline BOOL CTreeCtrl::DeleteItem(HTREEITEM hItem)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_DELETEITEM, 0, (LPARAM)hItem);
}

inline HIMAGELIST CTreeCtrl::GetImageList(UINT nImageList) const
{
	return ( HIMAGELIST)::SendMessage( GetSafeHwnd(), TVM_GETIMAGELIST, (UINT)nImageList, 0);
}

inline HIMAGELIST CTreeCtrl::SetImageList( HIMAGELIST hImageList, int nImageListType)
{
	return ( HIMAGELIST)::SendMessage( GetSafeHwnd(), TVM_SETIMAGELIST, (UINT)nImageListType, (LPARAM)hImageList);
}

inline UINT CTreeCtrl::GetCount() const
{
	return ( UINT)::SendMessage( GetSafeHwnd(), TVM_GETCOUNT, 0, 0);
}

inline SHORT CTreeCtrl::SetItemHeight( SHORT cyHeight )
{
	return ( SHORT)::SendMessage( GetSafeHwnd(), TVM_SETITEMHEIGHT, cyHeight, 0L);
}

inline HTREEITEM CTreeCtrl::HitTest( POINT& stPoint, UINT* pFlags)
{
	TVHITTESTINFO stHitTestInfo;
	stHitTestInfo.pt = stPoint;
	HTREEITEM hResult = ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_HITTEST, 0, (LPARAM)&stHitTestInfo);
	if (pFlags != NULL)*pFlags = stHitTestInfo.flags;

	return hResult;
}

inline HIMAGELIST CTreeCtrl::CreateDragImage( HTREEITEM hItem)
{
	return ( HIMAGELIST)::SendMessage( GetSafeHwnd(), TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem);
}

inline BOOL CTreeCtrl::SelectDropTarget( HTREEITEM hItem)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)hItem);
}

inline BOOL CTreeCtrl::GetItem( TVITEM* pItem)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_GETITEM, 0, (LPARAM)pItem);
}

inline BOOL CTreeCtrl::SetItem( TVITEM* pItem)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_SETITEM, 0, (LPARAM)pItem);
}

inline HTREEITEM CTreeCtrl::InsertItem( LPTVINSERTSTRUCT lpstInsertStruct)
{
	return ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_INSERTITEM, 0, (LPARAM)lpstInsertStruct);
}

inline HTREEITEM CTreeCtrl::GetChildItem( HTREEITEM hItem) const
{
	return ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
}

inline HTREEITEM CTreeCtrl::GetParentItem( HTREEITEM hItem)
{
	return ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_GETNEXTITEM, TVGN_PARENT, (LPARAM)hItem);
}

inline HTREEITEM CTreeCtrl::GetNextSiblingItem( HTREEITEM hItem) const
{
	return ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
}

inline HTREEITEM CTreeCtrl::GetPrevSiblingItem( HTREEITEM hItem) const
{
	return ( HTREEITEM)::SendMessage( GetSafeHwnd(), TVM_GETNEXTITEM, TVGN_PREVIOUS, (LPARAM)hItem);
}

inline BOOL CTreeCtrl::SortChildrenCB( LPTVSORTCB pSort)
{
	return ( BOOL)::SendMessage( GetSafeHwnd(), TVM_SORTCHILDRENCB, 0, (LPARAM)pSort);
}
