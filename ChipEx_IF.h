#ifndef	__OBOE_EX_IF_H__
#define	__OBOE_EX_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// 構造体のバージョン
#define	_IF_DATA_VER			0x00000100

// 変更通知フラグ
#define	_MODIFY_MEMO				0x00000001	// 表示内容／タイトルの変更
#define	_MODIFY_COLOR				0x00000002	// 色変更
#define	_MODIFY_FONT				0x00000004	// フォント変更
#define	_MODIFY_SHELL				0x00000008	// 関連付け変更
#define	_MODIFY_TIMER				0x00000010	// タイマ変更
#define	_MODIFY_ICON				0x00000020	// アイコン変更
#define	_MODIFY_PASSWORD			0x00000040	// パスワード変更
#define	_MODIFY_ZODER				0x00000080	// Ｚオーダー変更
#define	_MODIFY_RECTCHIP			0x00000100	// 小片サイズ変更
#define	_MODIFY_POINTICON			0x00000200	// アイコン位置変更
#define	_MODIFY_AUTORESIZE			0x00000400	// 小片リサイズ要求

// Ｚオーダー
#define	_ZODER_NORMAL				0			// 通常表示
#define	_ZODER_TOPMOST				1			// 常にトップ
#define	_ZODER_BOTTOMMOST			2			// 常にボトム

// 通知日のシフト
#define	_NOTIFY_NOSHIFT				0		// 移動無し
#define	_NOTIFY_SAT_FROM_SUN		1		// 土曜日へ移動：日曜日
#define	_NOTIFY_FRI_FROM_SAT_SUN	2		// 金曜日へ移動：土／日曜日
#define	_NOTIFY_MON_FROM_SUN		3		// 月曜日へ移動：日曜日
#define	_NOTIFY_MON_FROM_SAT_SUN	4		// 月曜日へ移動：土／日曜日

// 曜日種別タイマの曜日指定
#define	_WEEK_SUNDAY				0x00000001	// 日曜日
#define	_WEEK_MONDAY				0x00000002	// 月曜日
#define	_WEEK_TUESDAY				0x00000004	// 火曜日
#define	_WEEK_WEDNESDAY				0x00000008	// 水曜日
#define	_WEEK_THURSDAY				0x00000010	// 木曜日
#define	_WEEK_FRIDAY				0x00000020	// 金曜日
#define	_WEEK_SATURDAY				0x00000040	// 土曜日

// タイマタイプ
#define	_TIMERTYPE_DAILY			0			// 毎日
#define	_TIMERTYPE_WEEKLY			1			// 曜日
#define	_TIMERTYPE_MONTHLY			2			// 月日

#define	_ICON_COUNT					14			//アイコン種別

typedef struct tagIFDATA
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	UINT			m_unModify;				// DLL側での変更フラグ

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
	int				m_nTimerType;			// タイマタイプ
	int				m_nMonth;				// タイマの月
	int				m_nDay;					// タイマの日
	int				m_nWeekDay;				// 曜日種別タイマの曜日指定
	int				m_nNotifyShift;			// 通知日のシフト
	int				m_nBeforeNotify;		// ｎ日前からの通知
	BOOL			m_blExpireTimer;		// 経過後のタイマー
	HGLOBAL			m_hachszSoundFile;		// サウンドファイル

	int				m_nIconIndex;			// アイコン種別

	RECT			m_stRctChip;			// 小片サイズ
	POINT			m_stPntIcon;			// アイコン位置
}IFDATA;

typedef struct tagSPECIFICATION
{
	UINT	unLowerStructVersion;	// 処理可能な最古バージョン
	UINT	unUpperStructVersion;	// 処理可能な最新バージョン
	char	szMenuString[ 50];		// デフォルトのメニュー名称
}SPECIFICATION;

#ifdef	__cplusplus
extern "C" 
{
#endif
// この関数は必ず実装する必要が有ります。
__declspec( dllexport ) BOOL ChipExSpecification( SPECIFICATION* pSpecification);
// 項目設定用
__declspec( dllexport ) BOOL ChipExConfigure( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
// Copyright変更用
__declspec( dllexport ) BOOL ChipExAbout( UINT unStructVersion, HWND hWnd, UINT unMenuNum);
// 実行関数
__declspec( dllexport ) BOOL ChipExProc( HWND hWnd, UINT unMenuNum, IFDATA* pstIFData);
#ifdef	__cplusplus
}
#endif
#endif	//__OBOE_EX_IF_H__
