#pragma pack( push, 1)
typedef struct tagBRWIFDATA200
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	HGLOBAL			m_hachszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル

	int				m_nZOder;				// Ｚオーダー

	HGLOBAL			m_hachszPassWord;		// パスワード

	BOOL			m_blShellExec;			// 関連付け使用の有無
	BOOL			m_blTimerExec;			// 関連付けのタイマでの起動
	HGLOBAL			m_hachszFileName;		// 関連付けのファイル名称
	HGLOBAL			m_hachszProgName;		// 関連付けのプログラム名称

	BOOL			m_blTimerEnable;		// タイマ使用の有無
	int				m_nHour;				// タイマの時
	int				m_nMin;					// タイマの分
	int				m_nTimerType;			// タイマタイプ 0/1/2 = 毎日/曜日/月日
	int				m_nMonth;				// タイマの月
	int				m_nDay;					// タイマの日
	int				m_nWeekDay;				// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	int				m_nNotifyShift;			// 通知日が日曜日だった場合のシフト
	int				m_nBeforeNotify;		// ｎ日前からの通知
	BOOL			m_blExpireTimer;		// 経過後のタイマー
	HGLOBAL			m_hachszSoundFile;		// サウンドファイル

	BOOL			m_blMinSize;			// 小片の表示状態 ０通常／０以外アイコン
	BOOL			m_blShow;				// 表示状態
	int				m_nIconIndex;			// アイコン種別
	RECT			m_stRctChip;			// 小片サイズ
	POINT			m_stPntIcon;			// アイコン位置
	BOOL			m_blChipPosLock;		// 位置サイズ固定

	int				m_nDeleteYear;			// 削除の年
	int				m_nDeleteMonth;			// 削除の月
	int				m_nDeleteDay;			// 削除の日

	int				m_nReserved;			// 
}BRWIFDATA200;

typedef struct tagBRWIFDATA300
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	HGLOBAL			m_hachszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル

	int				m_nZOder;				// Ｚオーダー

	HGLOBAL			m_hachszPassWord;		// パスワード

	BOOL			m_blShellExec;			// 関連付け使用の有無
	BOOL			m_blTimerExec;			// 関連付けのタイマでの起動
	HGLOBAL			m_hachszFileName;		// 関連付けのファイル名称
	HGLOBAL			m_hachszProgName;		// 関連付けのプログラム名称

	BOOL			m_blTimerEnable;		// タイマ使用の有無
	int				m_nHour;				// タイマの時
	int				m_nMin;					// タイマの分
	int				m_nTimerType;			// タイマタイプ 0/1/2 = 毎日/曜日/月日
	int				m_nMonth;				// タイマの月
	int				m_nDay;					// タイマの日
	int				m_nWeekDay;				// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	int				m_nNotifyShift;			// 通知日が日曜日だった場合のシフト
	int				m_nBeforeNotify;		// ｎ日前からの通知
	BOOL			m_blExpireTimer;		// 経過後のタイマー
	HGLOBAL			m_hachszSoundFile;		// サウンドファイル

	BOOL			m_blMinSize;			// 小片の表示状態 ０通常／０以外アイコン
	BOOL			m_blShow;				// 表示状態
	int				m_nIconIndex;			// アイコン種別
	RECT			m_stRctChip;			// 小片サイズ
	POINT			m_stPntIcon;			// アイコン位置
	BOOL			m_blChipPosLock;		// 位置サイズ固定

	int				m_nDeleteYear;			// 削除の年
	int				m_nDeleteMonth;			// 削除の月
	int				m_nDeleteDay;			// 削除の日

	BOOL			m_blSeeTrough;			// シースルーフラグ:1.60
	UINT			m_unDXLID;				// DxlのユニークなID:1.60
}BRWIFDATA300;
#pragma pack( pop)
