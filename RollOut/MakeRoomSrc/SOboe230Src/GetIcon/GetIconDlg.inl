
inline LRESULT CDialog::SendDlgItemMessage( int nIDDlgItem, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return ::SendDlgItemMessage( GetSafeHwnd(), nIDDlgItem, Msg, wParam, lParam);
}

inline int CDialog::MessageBox( LPCTSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	return ::MessageBox( GetSafeHwnd(), lpText, lpCaption, uType);
}

inline HWND CDialog::GetDlgItem( int nIDDlgItem)
{
	return ::GetDlgItem( GetSafeHwnd(), nIDDlgItem);
}

inline void CDialog::OnCancel( void)
{
	EndDialog( GetSafeHwnd(), IDCANCEL);
}

inline void CDialog::OnOK( void)
{
	EndDialog( GetSafeHwnd(), IDOK);
}

inline HWND CDialog::GetSafeHwnd( void) const
{
#ifdef	_DEBUG
	if( NULL == m_hWnd)
	{
		DebugBreak();
	}
#endif
	return m_hWnd;
}

inline void CDialog::ScreenToClient( LPPOINT lpPoint)
{
	::ScreenToClient( GetSafeHwnd(), lpPoint);
}

inline HWND CDialog::SetCapture( void)
{
	return ::SetCapture( GetSafeHwnd());
}

inline void CDialog::CenterWindow( HWND hWndAlternateOwner)
{
	if( NULL == hWndAlternateOwner)
	{
		hWndAlternateOwner = ( NULL == m_hWndParent) ? GetDesktopWindow() : m_hWndParent;
	}

	RECT stRect;
	::GetWindowRect( GetSafeHwnd(), &stRect);
	RECT stRectOwner;
	::GetWindowRect( hWndAlternateOwner, &stRectOwner);

	::MoveWindow( GetSafeHwnd(), ( ( stRectOwner.right - stRectOwner.left) - ( stRect.right - stRect.left)) / 2,
									( ( stRectOwner.bottom - stRectOwner.top) - ( stRect.bottom - stRect.top)) / 2,
									stRect.right - stRect.left,
									stRect.bottom - stRect.top, TRUE);
}
