inline BOOL CDxlControl::IsDataExchange() const
{
	return ( 0 != ( m_unDrawType & _DT_EXCHANGE));
}

inline BOOL CDxlControl::IsOwnerDraw() const
{
	return ( 0 != ( m_unDrawType & _DT_OWNERDRAW));
}

inline BOOL CDxlControl::IsSupportResize() const
{
	return ( 0 == ( m_unStyle & _DXL_NOUSERRESEIZE));
}

inline BOOL CDxlControl::IsSupportClick() const
{
	return ( 0 != ( m_unStyle & _DXL_MOUSECLICK));
}

inline BOOL CDxlControl::IsSupportMenu() const
{
	return ( 0 != ( m_unStyle & _DXL_USEMENU));
}

inline BOOL CDxlControl::IsSupportNativeData() const
{
	return ( 0 != ( m_unStyle & _DXL_NATIVEDATA));
}

inline BOOL CDxlControl::IsSupportTimer() const
{
	return ( 0 != ( m_unStyle & _DXL_TIMER));
}

inline BOOL CDxlControl::IsNeedString() const
{
	return ( 0 != ( m_unStyle & _DXL_NEEDSTRING));
}

inline int CDxlControl::GetTimerSpan( void) const
{
	if( NULL == m_lpfnGetTimerSpan)return -1;
	return m_lpfnGetTimerSpan( m_dwDxlWord);
}

inline DWORD CDxlControl::DoTimer( void) const
{
	if( NULL == m_lpfnDoTimer)return 0;
	return m_lpfnDoTimer( m_dwDxlWord);
}

// メニュー使用中？
inline BOOL CDxlControl::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoMenu);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

inline BOOL CDxlControl::IsNoAlphaBlend() const
{
	return ( 0 != ( m_unStyle & _DXL_NOALPHABLEND));
}
