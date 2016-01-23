
// 描画拡張
inline int CSOboeApp::GetDxlMaxEntry( void) const
{
	return _DRAWEXLIB_MAX;
}

inline int CSOboeApp::GetDxlCount( void) const
{
	int	nResult = 0;

	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		if( m_acDrawExlib[ nIndex].IsFunctionAssign())nResult++;
	}

	return nResult;
}

// 通信拡張
inline int CSOboeApp::GetNxlMaxEntry( void) const
{
	return _NETEXLIB_MAX;
}

inline int CSOboeApp::GetNxlCount( void) const
{
	int	nResult = 0;

	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())nResult++;
	}

	return nResult;
}

inline int CSOboeApp::FindDxlID( UINT unDxlID) const
{
	int	nResult = -1;

	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		if( unDxlID == m_acDrawExlib[ nIndex].GetDxlID())
		{
			nResult = nIndex;
			break;
		}
	}

	return nResult;
}

inline int CSOboeApp::FindNxlID( UINT unNxlID) const
{
	int	nResult = -1;

	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( unNxlID == m_acNetExlib[ nIndex].GetNxlID())
		{
			nResult = nIndex;
			break;
		}
	}

	return nResult;
}

// 小片拡張
inline int CSOboeApp::GetCelMaxCount( void)
{
	return _CHIPEXLIB_MAX;;
}

inline CChipExLib* CSOboeApp::GetChipExLib( int nIndex)
{
	if( 0 <= nIndex && _CHIPEXLIB_MAX > nIndex)
	{
		if( !m_acChipExlib[ nIndex].GetMenu().IsEmpty())return &m_acChipExlib[ nIndex];
	}
	return NULL;
}

inline void CSOboeApp::GetDefaultFont( LOGFONT* pstLogFont) const
{
	CopyMemory( pstLogFont, &m_stLOGFONT, sizeof( LOGFONT));
}

inline COLORREF CSOboeApp::GetDefaultForeColor( void) const
{
	return m_clrForeColor;
}

inline COLORREF CSOboeApp::GetDefaultBackColor( void) const
{
	return m_clrBackColor;
}

inline int CSOboeApp::GetDefaultIconIndex( void) const
{
	return m_nIconIndex;
}

inline COLORREF CSOboeApp::GetBeforeForeColor( void) const
{
	return m_clrBeforeFore;
}

inline COLORREF CSOboeApp::GetBeforeBackColor( void) const
{
	return m_clrBeforeBack;
}

inline COLORREF CSOboeApp::GetAfterForeColor( void) const
{
	return m_clrAfterFore;
}

inline const CString& CSOboeApp::GetSignature( void) const
{
	return m_cStrSignature;
}

inline const CString& CSOboeApp::GetEntryName( void) const
{
	return m_cStrEntryName;
}

inline COLORREF CSOboeApp::GetAfterBackColor( void) const
{
	return m_clrAfterBack;
}

// 保存時のファイル
inline const CString& CSOboeApp::GetSaveFilePath( void) const
{
	return m_cStrSaveFilePath;
}

inline const CString& CSOboeApp::GetDelLogFilePath( void) const
{
	return m_cStrDelLogFilePath;
}

inline const CString& CSOboeApp::GetSendLogFilePath( void) const
{
	return m_cStrSendLogFilePath;
}

inline const CString& CSOboeApp::GetReceiveLogFilePath( void) const
{
	return m_cStrReceiveLogFilePath;
}

// グローバルパスワード
inline const CString& CSOboeApp::GetGlobalPassWord( void) const
{
	return m_cStrGlobalPassWord;
}

inline HSZ CSOboeApp::GetAppName( void) const
{
	return m_hszAppName;
}

inline HSZ CSOboeApp::GetAppTopic( void) const
{
	return m_hszTopic;
}

inline UINT CSOboeApp::GetOboeMemoFormat( void) const
{
	return m_unOboeFmt;
}

inline int CSOboeApp::GetLDblClkFunc( void) const
{
	return m_nLDblClkFunc;
}

inline int CSOboeApp::GetMDblClkFunc( void) const
{
	return m_nMDblClkFunc;
}

inline BOOL CSOboeApp::IsAutoResize( void) const
{
	return m_blAutoResize;
}

inline BOOL CSOboeApp::IsAutoTriming( void) const
{
	return m_blAutoTriming;
}

inline BOOL CSOboeApp::IsAutoIMEOpen( void) const
{
	return m_blAutoIMEOpen;
}

inline BOOL CSOboeApp::IsAutoScroll( void) const
{
	return m_blAutoScroll;
}

inline void CSOboeApp::AutoScroll( BOOL blAutoScroll)
{
	m_blAutoScroll = blAutoScroll;
}

inline BOOL CSOboeApp::PopupReceive( BOOL blPopupReceive)
{
	BOOL	blResult = m_blPopupReceive;
	m_blPopupReceive = blPopupReceive;
	return blResult;
}

inline BOOL CSOboeApp::IsPopupReceive( void) const
{
	return m_blPopupReceive;
}

inline BOOL CSOboeApp::IsHideReceive( void) const
{
	return m_blHideReceive;
}

inline BOOL CSOboeApp::IsSubNotify( void) const
{
	return m_blSubNotify;
}

inline BOOL CSOboeApp::IsAppendSignature( void) const
{
	return m_blAppendSignature;
}

inline BOOL CSOboeApp::AppendSignature( BOOL blSignature)
{
	BOOL blResult;
	blResult = m_blAppendSignature;
	m_blAppendSignature = blSignature;
	return blResult;
}

inline BOOL CSOboeApp::IsAppendTime( void) const
{
	return m_blAppendTime;
}

inline BOOL CSOboeApp::AppendTime( BOOL blTime)
{
	BOOL blResult;
	blResult = m_blAppendTime;
	m_blAppendTime = blTime;
	return blResult;
}

inline int CSOboeApp::GetAppendPos( void) const
{
	return m_nAppendPos;
}

inline int CSOboeApp::SetAppendPos( int nPos)
{
	int nResult;
	nResult = m_nAppendPos;
	m_nAppendPos = nPos;
	return nResult;
}

inline BOOL CSOboeApp::IsPackSend( void) const
{
	return m_blPackSend;
}

inline BOOL CSOboeApp::IsNotifyOpen( void) const
{
	return m_blNotifyOpen;
}

inline BOOL CSOboeApp::IsDataListReverse( void) const
{
	return m_blDataListReverse;
}

inline BOOL CSOboeApp::DataListReverse( BOOL blReverse)
{
	BOOL blResult;
	blResult = m_blDataListReverse;
	m_blDataListReverse = blReverse;

	return blResult;
}

inline int CSOboeApp::GetDataListFilter( void) const
{
	return ( 0 <= m_nDataListFilter ) ? m_nDataListFilter : 0;
//	return ( 0 <= m_nDataListFilter && _MAX_ICONINDEX > m_nDataListFilter) ? m_nDataListFilter : _MAX_ICONINDEX;
//	return min( max( 0, m_nDataListFilter), _MAX_ICONINDEX);
}

inline int CSOboeApp::SetDataListFilter( int nFilter)
{
	int nResult;
	nResult = m_nDataListFilter;

	m_nDataListFilter = ( 0 <= nFilter ) ? nFilter : 0;
//	m_nDataListFilter = ( 0 <= nFilter && _MAX_ICONINDEX >= nFilter) ? nFilter : _MAX_ICONINDEX;
//	m_nDataListFilter = min( max( 0, nFilter), _MAX_ICONINDEX);

	return nResult;
}

inline BOOL CSOboeApp::IsDelListReverse() const
{
	return m_blDelListReverse;
}

inline BOOL CSOboeApp::DelListReverse( BOOL blReverse)
{
	int nResult;
	nResult = m_blDelListReverse;
	m_blDelListReverse = blReverse;

	return nResult;
}

inline UINT CSOboeApp::GetHKModifiers( void) const
{
	return ( UINT)HIWORD( m_dwHotKey);
}

inline UINT CSOboeApp::GetHKVirtKey( void) const
{
	return ( UINT)LOWORD( m_dwHotKey);
}

inline int CSOboeApp::GetResumeDelay( void) const
{
	return ( m_nResumeDelay * 2); // 最低分解能が500msecだかンね、倍にしないとさぁ！
}

inline const CString& CSOboeApp::ArrangeNormalDLL( void) const
{
	return m_cStrArrangeNormalDLL;
}

inline const CString& CSOboeApp::ArrangeTitleDLL( void) const
{
	return m_cStrArrangeTitleDLL;
}

inline const CString& CSOboeApp::ArrangeIconDLL( void) const
{
	return m_cStrArrangeIconDLL;
}

inline const CString& CSOboeApp::ArrangeAllDLL( void) const
{
	return m_cStrArrangeAllDLL;
}

inline const CCelControl* CSOboeApp::GetCelControl( void) const
{
	return &m_cCelControl;
}

inline int CSOboeApp::GetShiftStepX( void)
{
	return m_nShiftStepX;
}

inline int CSOboeApp::GetShiftStepY( void)
{
	return m_nShiftStepY;
}

inline BOOL CSOboeApp::IsEnableNetwork( void) const
{
	return m_blEnableNetwork;
}

inline BOOL CSOboeApp::IsStandbyNetwork( void) const
{
	return m_blStandbyNetwork;
}

inline BOOL CSOboeApp::IsFlatMenu( void) const
{
	return m_blFlatMenu;
}

inline BOOL CSOboeApp::IsHideStart( void) const
{
	return m_blHideStart;
}

inline BOOL CSOboeApp::IsLoggingDelChip( void) const
{
	return m_blLoggingDelChip;
}

inline BOOL CSOboeApp::IsHideback( void) const
{
	return m_blHideback;
}

inline BOOL CSOboeApp::IsNofityVersion( void) const
{
	return m_blNotifyVersion;
}

inline BOOL CSOboeApp::IsNoCheckDelete( void) const
{
	return m_blNoCheckDelete;
}

inline BOOL CSOboeApp::IsLoggingSendChip( void) const
{
	return m_blLoggingSendChip;
}

inline BOOL CSOboeApp::IsFullLoggingSendChip( void) const
{
	return m_blFullLoggingSendChip;
}

inline BOOL CSOboeApp::IsLoggingReceiveChip( void) const
{
	return m_blLoggingReceiveChip;
}

inline BOOL CSOboeApp::IsFullLoggingReceiveChip( void) const
{
	return m_blFullLoggingReceiveChip;
}

inline BOOL CSOboeApp::IsCheckDelOnQuit( void) const
{
	return m_blCheckDelOnQuit;
}

inline BOOL CSOboeApp::IsDelOnQuit2Rcv( void) const
{
	return m_blDelOnQuit2Rcv;
}

inline BOOL CSOboeApp::IsReceptionLink( void) const
{
	return m_blReceptionLink;
}

inline BOOL CSOboeApp::IsReceptionTimer( void) const
{
	return m_blReceptionTimer;
}

inline BOOL CSOboeApp::IsReceptionDxl( void) const
{
	return m_blReceptionDxl;
}

inline BOOL CSOboeApp::IsEnableTemplet( void) const
{
	return m_blEnableTemplet;
}

inline int CSOboeApp::GetTempletCount( void) const
{
	return m_cLstSeeds.GetCount();
}

inline BOOL CSOboeApp::GetUserColor( COLORREF* paclrUserColors)
{
	CopyMemory( paclrUserColors, m_clrCustomColor, sizeof( COLORREF) * _USERCOLOR_MAX);

	return TRUE;
}

inline BOOL CSOboeApp::SetUserColor( const COLORREF* paclrUserColors)
{
	CopyMemory( m_clrCustomColor, paclrUserColors, sizeof( COLORREF) * ( _USERCOLOR_MAX - 2));

	return TRUE;
}

inline BOOL CSOboeApp::IsCompatibleOU( void) const
{
	return m_blCompatibleOU;
}

inline BOOL CSOboeApp::GetExportFolder( CString& cStrFolder)
{
	cStrFolder = m_cStrExportFolder;
	return TRUE;
}

inline BOOL CSOboeApp::SetExportFolder( LPCSTR lpcszFolder)
{
	m_cStrExportFolder = lpcszFolder;
	return TRUE;
}

inline BOOL CSOboeApp::IsMakeBackupFile( void) const
{
	return m_blMakeBackupFile;
}

inline int CSOboeApp::GetGenerationCount( void) const
{
	return min( 10, m_nGenerationCount);
}

inline BOOL CSOboeApp::EnableNxlGroup( void) const
{
	return m_blEnableNxlGroup;
}

inline void CSOboeApp::SuspendReceive( BOOL blSuspend)
{
	m_blSuspendReceive = TRUE;
}

inline BOOL CSOboeApp::IsEnableSpMenu() const
{
	return m_blEnableSpMenu;
}

inline BOOL CSOboeApp::IsExEdit( void) const
{
	return m_blExEdit;
}

inline void CSOboeApp::ExEdit( BOOL blExEdit)
{
	m_blExEdit = blExEdit;
}

inline BOOL CSOboeApp::IsBigButton( void) const
{
	return m_blBigButton;
}

inline void CSOboeApp::BigButton( BOOL blBigButton)
{
	m_blBigButton = blBigButton;
}

inline const CRect& CSOboeApp::GetEditWindowRect( void) const
{
	return m_cRctEditWindow;
}

inline void CSOboeApp::SetEditWindowRect( const CRect& cRect)
{
	m_cRctEditWindow = cRect;
}

inline DWORD CSOboeApp::GetDelayTime( void) const
{
	return m_dwInitalDeray;
}

inline BOOL CSOboeApp::IsSendDlgAutoClose( void)
{
	return m_blSendDlgAutoClose;
}

inline void CSOboeApp::SendDlgAutoClose( BOOL blSendDlgAutoClose)
{
	m_blSendDlgAutoClose = blSendDlgAutoClose;
}

inline BOOL CSOboeApp::IsColorPrint( void) const
{
	return m_blColorPrint;
}

inline void CSOboeApp::ColorPrint( BOOL blColor)
{
	m_blColorPrint = blColor;
}

// カタログ表示
inline BOOL CSOboeApp::IsEnableCatalogView( void) const
{
	return m_blCatalog;
}

// 受信と保存
inline BOOL CSOboeApp::IsReceiveAndSave( void) const
{
	return m_blReceiveAndSave;
}

// 編集と保存
inline BOOL CSOboeApp::IsModifyAndSave( void) const
{
	return m_blModifyAndSave;
}

// 送信リストの展開
inline BOOL CSOboeApp::IsExpandSendTree( void) const
{
	return m_blExpandSendTree;
}

// アイコン情報
inline const CIconImgList* CSOboeApp::GetIconList( void) const
{
	return m_pcIconImgs;
}

// 送信後の削除
inline BOOL CSOboeApp::IsDeleteAfterSend( void) const
{
	return m_blDeleteAfterSend;
}