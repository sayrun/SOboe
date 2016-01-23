
// リサイズ情報の取得
inline BOOL CMemoEditSheet::IsResize( void) const
{
	return m_cBasePage.m_blResize;
}

// 終了時の破棄フラグ
inline BOOL CMemoEditSheet::IsDelOnQuit( void) const
{
	return m_cBasePage.m_blDelOnQuit;
}

inline void CMemoEditSheet::DelOnQuit( BOOL blDelOnQuit)
{
	m_cBasePage.m_blDelOnQuit = blDelOnQuit;
}

// データ整合性テスト：内容
inline BOOL CMemoEditSheet::IsMemoEmpty( void) const
{
	return m_cBasePage.m_cStrMemo4IF.IsEmpty();
}

// Dxl-ID転送用のメソッド
inline BOOL CMemoEditSheet::ForwardDxlID( UINT unDxlID)
{
	return m_cBasePage.m_unDxlID = unDxlID;
}

// リサイズの禁止
inline void CMemoEditSheet::ResizeLock( BOOL blResizeLock)
{
	m_cBasePage.m_blResizeLock = blResizeLock;
}
