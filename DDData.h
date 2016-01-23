#ifndef	_DDDATA_H__
#define	_DDDATA_H__

typedef	struct tagDDDATA
{
	int			m_nSize;

	char		m_szDataFmt[ 20];	// "OboeGakiChipData100\0"

	RECT		m_stRctNormal;
	RECT		m_stRctTitle;
	RECT		m_stRctIcon;
	int			m_nWindowType;

	BOOL		m_blShow;
	int			m_nZOrder;
	BOOL		m_blPosLock;	// 位置サイズ固定
	BOOL		m_blSeeThrough;	// シースルー

	int			m_nszTitle;
	int			m_nszMemo;

	int			m_nstLogFont;
	COLORREF	m_clrForeColor;
	COLORREF	m_clrBackColor;

	int			m_nIconIndex;


	BOOL		m_blLink;
	int			m_nszFile;
	int			m_nszParam;
	BOOL		m_blTimerExec;

	BOOL		m_blPassWordLock;
	int			m_nszPassWord;

	UINT		m_unDxlID;
	UINT		m_unDxlExtDataSize;
	int			m_nabyDxlExtData;

	BOOL		m_blTimer;			// タイマ通知有功
	int			m_nTimerType;			// タイマ種別
	int			m_nNotifyShift;		// 通知日の移動処理
	int			m_nMonth;				// 通知月
	int			m_nDay;				// 通知日
	int			m_nHour;				// 通知時
	int			m_nMin;				// 通知分
	int			m_nWeek;				// 通知曜日
	int			m_nBeforeNotify;		// 事前通知
	BOOL		m_blAfterNotify;		// 経過通知
	int			m_nszNotifySound;	// 通知サウンドファイル

	int			m_nCreateYear;
	int			m_nCreateMonth;
	int			m_nCreateDay;

	// この後ろに拡張データが続く
}DDDATA;
#endif	// __DDDATA_H__
