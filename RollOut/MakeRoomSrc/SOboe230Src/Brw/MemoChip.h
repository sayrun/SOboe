// MemoChip.h : ヘッダー ファイル
//
#ifndef	__MEMOCHIP_H__
#define	__MEMOCHIP_H__

/////////////////////////////////////////////////////////////////////////////
// CMemoChip ウィンドウ

#define	MOVEAREAPOINT_MAX	4
#define	WM_BYEBYE			( WM_USER + 100)
#define	MAX_FILESIZE		( 1024 * 1024)
#define	MAX_ICON_INDEX		14

#define	_ICON_WIDTH			16
#define	_ICON_HEIGHT		16

#define	_BRW_IF_DATA_OLD_VER		0x00000200
#define	_BRW_IF_DATA_300_VER		0x00000300
#define	_BRW_IF_DATA_CUR_VER		0x00000400

#pragma pack( push, 1)
typedef struct tagIFDATA
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
	int				m_nDxlSaveDataSize;		// dxlの固有データのサイズ:1.60
	HGLOBAL			m_hDxlSaveData;			// dxlの固有データ:1.60
}IFDATA;
#pragma pack( pop)


#include "MemoData.h"

class CMemoChip : public CWnd
{
// コンストラクション
public:
	const BOOL IsPassWordLock( LPCSTR lpcszPassWord = NULL);
	BOOL GetMemoData( CMemoData& cMemoData);
	void SetMemoData( CMemoData& cMemoData);
	~CMemoChip();
	CMemoChip();
	CMemoChip( CMemoChip& cMemoChip);
	// 変数

	void Serialize( CArchive& ar );

	int GetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord);
	BOOL SetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord);
	static BOOL ReleaseIFData( IFDATA* pstIFData);


	DECLARE_SERIAL( CMemoChip);
// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CMemoChip)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	BOOL IsLink( void) const;
	BOOL GetFont( LOGFONT* pLogFont);
	int GetIconIndex( void) const;
	COLORREF GetBackColor( void) const;
	COLORREF GetTextColor( void) const;
	int GetMemo( LPSTR lpszData, int nSize, LPCSTR lpcszPassWord);
	BOOL GetMemoTitle( LPSTR lpszTitle);

	// 生成されたメッセージ マップ関数
protected:
	/*
	BOOL GetIFData( IFDATA& stIFData);
	BOOL ReleaseIFData( IFDATA& stIFData);
	*/


	BOOL	m_blMove;
	COLORREF m_clrInternalTextColor;
	COLORREF m_clrInternalBackColor;
	CString m_cStrResult2;
	CString m_cStrResult1;
	BOOL m_blCheckOpen;			// 開封確認:1.40
	BOOL m_blCheckYesNo;		// はい／いいえ確認:1.40
	BOOL m_blOtherResult;		// その他の解:1.42
	CString m_cStrSignature;	// 送り主
	CString m_cStrComp;			// 送り元
	CString m_cStrUser;			// 送り元
	CString m_cStrCaption;		// 
	CString m_cStrPeerAddres;	// 送り元アドレス
	BOOL m_blCheckedReceive;

	BOOL m_blSysResize;
	BOOL m_blBeforeHide;
	BOOL m_blChipPosLock;
	BOOL m_blOndeHide;
	BOOL m_blFrameDraw;
	BOOL m_blSizeBackUp;
	CString m_cStrUndo;
	BOOL m_blUndo;
	BOOL m_blShow;
	CWnd* m_pcWndParent;
	CToolTipCtrl m_cTtcTipHelp;
//	CMemochipTip m_cTtcTipHelp;
	BOOL m_blFlash;
	int m_nTodayTimer;
	BOOL m_blClose;
	CPoint m_cPntStart;
	BOOL m_blMinSize;
	POINT m_astMoveRgnPoints[ MOVEAREAPOINT_MAX];
	BOOL m_blDataAppry;
	CMemoData m_cMemoData;

	//{{AFX_MSG(CMemoChip)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif	//__MEMOCHIP_H__
