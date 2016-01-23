inline void CResizableMemoEditDialog::ResizeLock( BOOL blResizeLock)
{
	m_blResizeLock = blResizeLock;
}

inline BOOL CResizableMemoEditDialog::IsMemoEmpty( void)
{
	return m_cMemoData.GetMemo().IsEmpty();
}

inline BOOL CResizableMemoEditDialog::IsResize( void)
{
	return m_blResize;
}

inline void CResizableMemoEditDialog::DelOnQuit( BOOL blDelOnQuit)
{
	m_blDelOnQuit = blDelOnQuit;
}

inline BOOL CResizableMemoEditDialog::IsDelOnQuit( void)
{
	return m_blDelOnQuit;
}

inline void CResizableMemoEditDialog::SetIcon( HICON hIcon)
{
	m_hIcon = hIcon;
}
