#ifndef	__MEMODATA_H__
#define	__MEMODATA_H__

#include "DDData.h"

#define	_MAX_TITLE_LEN	63
#define	_CODE_BASE		16

#define	_SCHEMA_ID_TYPE1	0x094A	//1.29以前
#define	_SCHEMA_ID_TYPE2	0x094B	//1.40
#define	_SCHEMA_ID_TYPE3	0x094C	//1.60
#define	_SCHEMA_ID			0x2000	//2.00

class CMemoData : public CObject
{
public:	// メソッド
	CMemoData();
	CMemoData( const CMemoData& cMemoData);
	~CMemoData();

	// 表示データ
	void SetTitle( const CString& cStrTitle);
	BOOL GetTitle( CString& cStrTitle) const;
	void SetMemo( const CString& cStrMemo);
	int GetMemoLength( void) const;
	const CString& GetMemo( void) const;

	// 表示スタイル
	void SetFont( const LOGFONT* pstLogFont = NULL);
	BOOL GetFont( LOGFONT* const stLogFont) const;
	void SetForeColor( const COLORREF& clrFore = CLR_INVALID);
	BOOL GetForeColor( COLORREF& clrFore) const;
	void SetBackColor( const COLORREF& clrBack = CLR_INVALID);
	BOOL GetBackColor( COLORREF& clrBack) const;
	void SetIconIndex( const int nIconIndex);
	int GetIconIndex( void) const;

	// 関連付け
	BOOL Link( const BOOL blLinkEnable = FALSE);
	BOOL IsLink( void) const;
	// タイマでの関連付け
	BOOL TimerExec( const BOOL blTimerExec = FALSE);
	BOOL IsTimerExec( void) const;
	// リンク情報！
	BOOL SetLinkData( const CString& cStrFile, const CString& cStrParameters);
	BOOL GetLinkData( CString& cStrFile, CString& cStrParameters) const;

	// パスワードロック
	BOOL PassWordLock( const BOOL blPassWordLock = FALSE);
	BOOL IsPassWordLock( void) const;
	BOOL SetPassWord( const CString& cStrPassWord);
	const CString& GetPassWord( void) const;

	// タイマ
	BOOL Timer( BOOL blTimer = FALSE);
	BOOL IsTimer( void) const;

	// タイマ種別
	enum
	{
		TIMERTYPE_EVERYDAY,		// 毎日通知
		TIMERTYPE_WEEK,			// 曜日指定
		TIMERTYPE_DAY			// 日付指定
	};
	int SetTimerType( int nType = TIMERTYPE_EVERYDAY);
	int GetTimerType( void) const;

	// 通知日の移動
	enum
	{
		NOTIFYSHIFT_NOMOVE,				// 移動無し
		NOTIFYSHIFT_TOSAT,				// 土曜日へ
		NOTIFYSHIFT_TOFRI,				// 金曜日へ
		NOTIFYSHIFT_TOMON_FROMSUN,		// 月曜日へ 日曜日
		NOTIFYSHIFT_TOMON_FROMSATSUN	// 月曜日へ 土曜日／日曜日
	};
	int SetNotifyShift( int nShift = NOTIFYSHIFT_NOMOVE);
	int GetNotifyShift( void) const;

	// 通知月
	int SetTimerMonth( int nMonth);
	int GetTimerMonth( void) const;
	int GetNotifyMonth( const CTime& cTimeNow) const;	// 通知日移動あるいは毎月による移動

	// 通知日
	int SetTimerDay( int nDay);
	int GetTimerDay( void) const;
	int GetNotifyDay( const CTime& cTimeNow) const;		// 通知日移動による移動

	// 曜日の通知
	enum
	{
		WEEK_DISABLE = 0,
		WEEK_SUN = 0x00000001,
		WEEK_MON = 0x00000002,
		WEEK_TUE = 0x00000004,
		WEEK_WED = 0x00000008,
		WEEK_THU = 0x00000010,
		WEEK_FRI = 0x00000010,
		WEEK_SAT = 0x00000020,
		WEEK_ALL = ( WEEK_SUN | WEEK_MON | WEEK_TUE | WEEK_WED | WEEK_THU | WEEK_FRI | WEEK_SAT)
	};
	int SetWeek( int nWeek);
	int GetWeek( void) const;

	// 通知時
	int SetTimerHour( int nHour);
	int GetTimerHour( void) const;
	// 通知分
	int SetTimerMin( int nMin);
	int GetTimerMin( void) const;

	int SetBeforeNotify( int nBefore = 0);
	int GetBeforeNotify( void) const;

	BOOL AfterNotify( BOOL blNotify);
	BOOL IsAfterNotify( void) const;

	BOOL SetNotifySound( const CString& cStrNotifySound);
	BOOL GetNotifySound( CString& cStrNotifySound) const;

	// 描画拡張
	UINT SetDxlID( const UINT unDxlID);
	UINT GetDxlID( void) const;
	BOOL SetDxlExtData( const BYTE* pabyDxlExtData, const UINT unSize);
	UINT GetDxlExtData( BYTE* pabyDxlExtData = NULL, UINT unSize = 0) const;

	// コピーだよん
	const CMemoData& operator=( const CMemoData& cMemoData);

	// 作成日付
	void SetCreateDate( void);
	void GetCreateDate( int& nYear, int& nMonth, int& nDay) const;

	// 表示形態
	enum
	{
		WINDOW_TYPE_NORMAL,
		WINDOW_TYPE_TITLE,
		WINDOW_TYPE_ICON
	};
	int SetWindowType( int nType = WINDOW_TYPE_NORMAL);
	int GetWindowType( void) const;

	BOOL Show( BOOL blShow);
	BOOL IsShow( void) const;

	// Ｚオーダ
	enum
	{
		ZORDER_NORMAL,
		ZORDER_TOPMOST,
		ZORDER_BOTTOMMOST
	};
	int SetZOrder( int nZOrder = ZORDER_NORMAL);
	int GetZOrder( void) const;

	// 位置サイズ固定
	BOOL PosLock( BOOL blLock);
	BOOL IsPosLock( void) const;
	// 文字のみ
	BOOL SeeThrough( BOOL SeeThrough);
	BOOL IsSeeThrough( void) const;

	CRect SetNormalRect( const CRect& cRctNormal);
	const CRect& GetNormalRect( void) const;
	CRect SetTitleRect( const CRect& cRctTitle);
	const CRect& GetTitleRect( void) const;
	CRect SetIconRect( const CRect& cRctIcon);
	const CRect& GetIconRect( void) const;

	// D&DとかClipBoard用のデータ生成
	int	CalcDDDataSize( void);
	int GetDDData( DDDATA* stDDData, int nSize);
	static CMemoData* CreateMemoFromBinData( BYTE* pabyData, int nSize);

	static const UINT GetClipboardFormat( void);

	// シリアライズ処理
	void Store( CArchive& ar);
	void Restore( CArchive& ar, UINT unVersion);

protected:	// メソッド
	// 保存時に、ちょとした「暗号化」モドキを施すために使用
	void Encode( CString& cStrDst, const CString& cStrSrc);
	void Decode( CString& cStrDst, const CString& cStrSrc);
	static CString sm_cStrData;

	BOOL CopyMember( const CMemoData& cMemoData);

	// 通知日の内部変換
	enum
	{
		GET_MONTH,
		GET_DAY
	};
	int ExchangeNotifyDay( const CTime& cTimeNow, const int nGetType) const;

	// 旧バージョンから読み出し
	void RestoreFromOldFormat( CArchive& ar, UINT unVersion);

protected:	// メンバー変数
	CRect		m_cRctNormal;	// 通常
	CRect		m_cRctTitle;	// タイトル
	CRect		m_cRctIcon;		// アイコン：位置
	int			m_nWindowType;	// 形状

	BOOL		m_blShow;		// 表示／非表示
	int			m_nZOrder;		// Ｚオーダ
	BOOL		m_blPosLock;	// 位置サイズ固定
	BOOL		m_blSeeThrough;	// シースルー


	CString		m_cStrTitle;	// タイトル
	CString		m_cStrMemo;		// 表示内容

	LOGFONT*	m_pstLogFont;	// 表示フォント
	COLORREF	m_clrForeColor;	// 文字色
	COLORREF	m_clrBackColor;	// 背景色

	int			m_nIconIndex; // アイコン種別

	// 関連付け
	BOOL		m_blLink;			// 関連付け有功無効
	CString		m_cStrFile;			// ファイル
	CString		m_cStrParameters;	// パラメータ
	BOOL		m_blTimerExec;		// タイマで起動

	BOOL		m_blPassWordLock;	// パスワードの有功無効
	CString		m_cStrPassWord;		// パスワード

	// 描画拡張
	UINT		m_unDxlID;			// 描画拡張ID
	UINT		m_unDxlExtDataSize;	// DXLに関する特殊保存データサイズ
	BYTE*		m_pabyDxlExtData;	// DXLに関する特殊保存データ

	// タイマ
	BOOL		m_blTimer;			// タイマ通知有功
	int			m_nTimerType;		// タイマ種別
	int			m_nNotifyShift;		// 通知日の移動処理
	int			m_nMonth;			// 通知月
	int			m_nDay;				// 通知日
	int			m_nHour;			// 通知時
	int			m_nMin;				// 通知分
	int			m_nWeek;			// 通知曜日
	int			m_nBeforeNotify;	// 事前通知
	BOOL		m_blAfterNotify;	// 経過通知
	CString		m_cStrNotifySound;	// 通知サウンドファイル

	union CREATEDATE
	{
		UINT		m_unCreateDate;	// 保存時のアクセス用
		struct			
		{
			UINT	m_btCreateDay:6;	// bit:0000000000000000-0000000000xxxxxx
			UINT	m_btCreateMonth:5;	// bit:0000000000000000-00000xxxxx000000
			UINT	m_btCreateYear:12;	// bit:000000000xxxxxxx-xxxxx00000000000
		};
	}	m_uniCreateDate;			// 作成日付

	static UINT	m_unOboeChip;
};

#include "MemoData.inl"

#endif	//__MEMODATA_H__