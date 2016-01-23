
inline const CString& CMemoListNode::GetTitle( void) const
{
	return m_cStrTitle;
}

inline const CString& CMemoListNode::GetMemo( void) const
{
	return m_cStrMemo;
}

inline const CString& CMemoListNode::GetDate( void) const
{
	return m_cStrDate;
}

inline const int CMemoListNode::GetIconIndex( void) const
{
	return m_nIconIndex;
}

inline COLORREF CMemoListNode::GetForeColor( void) const
{
	return m_clrFore;
}

inline COLORREF CMemoListNode::GetBackColor( void) const
{
	return m_clrBack;
}

inline const HWND CMemoListNode::GetWndHandle( void) const
{
	return m_hWnd;
}

inline BOOL CMemoListNode::IsHide() const
{
	return !m_blShow;
}

inline BOOL CMemoListNode::IsLink() const
{
	return m_blLink;
}

inline BOOL CMemoListNode::IsDelOnQuit() const
{
	return m_blDelOnQuit;
}

inline BOOL CMemoListNode::IsSeeThrough() const
{
	return m_blSeeThrough;
}

inline BOOL CMemoListNode::IsPassWordLock() const
{
	return m_blPassWordLock;
}
