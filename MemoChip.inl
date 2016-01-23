
inline void CMemoChip::SetMemoData( const CMemoData& cMemoData)
{
	m_cMemoData = cMemoData;
}

inline const CMemoData* CMemoChip::AccessMemoData( void) const
{
	return &m_cMemoData;
}

inline UINT CMemoChip::GetID( void) const
{
	return m_unID;
}

inline BOOL CMemoChip::IsDelOnQuit( void) const
{
	return m_blDelOnQuit;
}

inline BOOL CMemoChip::DrawResizeFrame( BOOL blResize)
{
	BOOL blResult = ( false != m_blFrameDraw);

	m_blFrameDraw = ( FALSE != blResize);

	return blResult;
}

inline void CMemoChip::SmartDelete( void)
{
	m_blSmartClose = true;

	SendMessage( WM_CLOSE, 0, 0);

	return;
}

// 内部での管理色
inline const COLORREF CMemoChip::GetInternalForeColor( void) const
{
	// 内部処理用の文字色
	return m_clrInternalForeColor;
}

inline const COLORREF CMemoChip::GetInternalBackColor( void) const
{
	// 内部処理用の背景色
	return m_clrInternalBackColor;
}

inline BOOL CMemoChip::IsPack( void) const
{
	return m_blPack;
}

inline void CMemoChip::SetContextID( LPCSTR lpcszID)
{
	// もともと表示側で記憶していた揮発性データであるが、
	// 不揮発性データとして扱うことと変更。
	// そのため、設定されたデータをバイパスする実装へ変更
	m_cMemoData.SetContextID( lpcszID);
}

inline BOOL CMemoChip::IsEqualContextID( LPCSTR lpcszID)
{
	// もともと表示側で記憶していた揮発性データであるが、
	// 不揮発性データとして扱うことと変更。
	// そのため、設定されたデータをバイパスする実装へ変更
	return m_cMemoData.IsEqualContextID( lpcszID);
}

inline BOOL CMemoChip::NoCheckReceive( void) const
{
	return m_blReceive;
}

inline void CMemoChip::OneTimeTop( void)
{
	m_blOnceTop = true;
}
