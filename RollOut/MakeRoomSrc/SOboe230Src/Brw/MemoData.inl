
inline CMemoData::CMemoData( const CMemoData& cMemoData) : CObject()
{
	CopyMember( cMemoData);
}

inline void CMemoData::SetTitle( const CString& cStrTitle)
{
	// タイトルは最長63固定です
	m_cStrTitle = cStrTitle.Left( _MAX_TITLE_LEN);
}

inline void CMemoData::SetMemo( const CString& cStrMemo)
{
	m_cStrMemo = cStrMemo;
}

inline int CMemoData::GetMemoLength( void) const
{
	return m_cStrMemo.GetLength();
}

inline const CString& CMemoData::GetMemo( void) const
{
	return m_cStrMemo;
}

inline void CMemoData::SetForeColor( const COLORREF& clrFore)
{
	m_clrForeColor = clrFore;
}

inline void CMemoData::SetBackColor( const COLORREF& clrBack)
{
	m_clrBackColor = clrBack;
}

inline const CMemoData& CMemoData::operator=( const CMemoData& cMemoData)
{
	CopyMember( cMemoData);

	return *this;
}

inline void CMemoData::SetIconIndex( const int nIconIndex)
{
	m_nIconIndex = max( 0, min( _MAX_ICONINDEX, nIconIndex));
}

inline int CMemoData::GetIconIndex( void) const
{
	return m_nIconIndex;
}

inline BOOL CMemoData::Link( const BOOL blLinkEnable)
{
	BOOL	blResult;

	blResult = m_blLink;
	m_blLink = blLinkEnable;

	return blResult;
}

inline BOOL CMemoData::IsLink( void) const
{
	// 実行するのが無ければ、してもしょうがないじゃない？
	if( m_cStrFile.IsEmpty() && m_cStrParameters.IsEmpty())return FALSE;

	return m_blLink;
}

inline BOOL CMemoData::TimerExec( const BOOL blTimerExec)
{
	BOOL	blResult;

	blResult = m_blTimerExec;
	m_blTimerExec = blTimerExec;

	return blResult;
}

inline BOOL CMemoData::IsTimerExec( void) const
{
	// 関連付け名が無ければ、タイマ起動も無駄でしょ？
	if( IsLink())
	{
		return m_blTimerExec;
	}

	return FALSE;
}

// リンク情報！
inline BOOL CMemoData::SetLinkData( const CString& cStrFile, const CString& cStrParameters)
{
	m_cStrFile = cStrFile;
	m_cStrParameters = cStrParameters;

	return TRUE;
}

inline BOOL CMemoData::PassWordLock( const BOOL blPassWordLock)
{
	BOOL blResult;
	blResult = m_blPassWordLock;
	m_blPassWordLock = blPassWordLock;
	return blResult;
}

inline BOOL CMemoData::IsPassWordLock( void) const
{
	if( m_cStrPassWord.IsEmpty())
	{
		return FALSE;
	}
	return m_blPassWordLock;
}

inline BOOL CMemoData::SetPassWord( const CString& cStrPassWord)
{
	if( cStrPassWord.IsEmpty())
	{
		m_blPassWordLock = FALSE;
	}
	m_cStrPassWord = cStrPassWord;

	return TRUE;
}

inline const CString& CMemoData::GetPassWord( void) const
{
	return m_cStrPassWord;
}

// タイマ
inline BOOL CMemoData::Timer( BOOL blTimer)
{
	BOOL blResult;

	blResult = m_blTimer;
	m_blTimer = blTimer;

	return blResult;
}

inline BOOL CMemoData::IsTimer( void) const
{
	return m_blTimer;
}

// タイマ種別
inline int CMemoData::GetTimerType( void) const
{
	return m_nTimerType;
}

inline int CMemoData::GetNotifyShift( void) const
{
	return m_nNotifyShift;
}

// 通知月
inline int CMemoData::GetTimerMonth( void) const
{
	return m_nMonth;
}

// 通知日移動あるいは毎月による移動
inline int CMemoData::GetNotifyMonth( const CTime& cTimeNow) const
{
	return ExchangeNotifyDay( cTimeNow, GET_MONTH);
}

// 通知日
inline int CMemoData::GetTimerDay( void) const
{
	return m_nDay;
}

// 通知日移動による移動
inline int CMemoData::GetNotifyDay( const CTime& cTimeNow) const
{
	return ExchangeNotifyDay( cTimeNow, GET_DAY);
}

// 曜日の通知
inline int CMemoData::GetWeek( void) const
{
	return m_nWeek;
}

// 通知時
inline int CMemoData::SetTimerHour( int nHour)
{
	int nResult;
	nResult = m_nHour;
	m_nHour = nHour % 24;

	return nResult;
}

inline int CMemoData::GetTimerHour( void) const
{
	return m_nHour;
}

// 通知分
inline int CMemoData::SetTimerMin( int nMin)
{
	int nResult;
	nResult = m_nMin;
	m_nMin = nMin % 60;

	return nResult;
}

// 通知分
inline int CMemoData::GetTimerMin( void) const
{
	return m_nMin;
}

inline int CMemoData::SetBeforeNotify( int nBefore)
{
	int	nResult;
	nResult = m_nBeforeNotify;
	m_nBeforeNotify = nBefore;
	return nResult;
}

inline int CMemoData::GetBeforeNotify( void) const
{
	return m_nBeforeNotify;
}

inline BOOL CMemoData::AfterNotify( BOOL blNotify)
{
	BOOL blResult;
	blResult = m_blAfterNotify;
	m_blAfterNotify = blNotify;
	return blResult;
}

inline BOOL CMemoData::IsAfterNotify( void) const
{
	return m_blAfterNotify;
}

inline BOOL CMemoData::SetNotifySound( const CString& cStrNotifySound)
{
	m_cStrNotifySound = cStrNotifySound;

	return TRUE;
}

inline UINT CMemoData::SetDxlID( const UINT unDxlID)
{
	UINT	unResult;
	unResult = m_unDxlID;
	m_unDxlID = unDxlID;
	return unResult;
}

inline UINT CMemoData::GetDxlID( void) const
{
	return m_unDxlID;
}

inline BOOL CMemoData::PosLock( BOOL blLock)
{
	BOOL blResult = m_blPosLock;
	m_blPosLock = blLock;
	return blResult;
}

inline BOOL CMemoData::IsPosLock( void) const
{
	return m_blPosLock;
}

inline BOOL CMemoData::SeeThrough( BOOL SeeThrough)
{
	BOOL blResult = m_blSeeThrough;
	m_blSeeThrough = SeeThrough;
	return blResult;
}

inline BOOL CMemoData::IsSeeThrough( void) const
{
	return m_blSeeThrough;
}

inline int CMemoData::GetWindowType( void) const
{
	return m_nWindowType;
}

inline int CMemoData::GetZOrder( void) const
{
	return m_nZOrder;
}

inline BOOL CMemoData::Show( BOOL blShow)
{
	BOOL blResult = m_blShow;

	m_blShow = blShow;

	return blResult;
}

inline BOOL CMemoData::IsShow( void) const
{
	return m_blShow;
}

inline const CRect& CMemoData::GetNormalRect( void) const
{
	return m_cRctNormal;
}

inline const CRect& CMemoData::GetTitleRect( void) const
{
	return m_cRctTitle;
}

inline const CRect& CMemoData::GetIconRect( void) const
{
	return m_cRctIcon;
}

inline const UINT CMemoData::GetClipboardFormat( void)
{
	return m_unOboeChip;
}
