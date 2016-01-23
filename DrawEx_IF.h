#ifndef	__OBOE_DXL_IF_H__
#define	__OBOE_DXL_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// 構造体のバージョン
#define	_DXL_IF_DATA_VER_100	0x00000100
#define	_DXL_IF_DATA_VER_200	0x00000200

#if ( _DXLVER >= 0x200)
	#define	_DXL_IF_DATA_VER	_DXL_IF_DATA_VER_200
#else
	#define	_DXL_IF_DATA_VER	_DXL_IF_DATA_VER_100
#endif


#if ( _DXLVER >= 0x200)
#define	_GET_MEMO			1
#define	_GET_TITLE			2
#define	_GET_FONT			3
#define	_GET_FORECOLOR		4
#define	_GET_BACKCOLOR		5
#define	_GET_ICON			6
#define	_GET_ZORDER			7
#define	_GET_WINDOWTYPE		8
#define	_GET_RECT_NORMAL	9
#define	_GET_POINT_TITLE	10
#define	_GET_POINT_ICON		11
#define _GET_RECT_TITLE		12
#define _GET_RECT_ICON		13

#define	_GET_TIMERENABLE	20
#define	_GET_TIMER_TYPE		21
#define	_GET_TIMER_MONTH	22
#define	_GET_TIMER_DAY		23
#define	_GET_TIMER_HOUR		24
#define	_GET_TIMER_MIN		25
#define	_GET_TIMER_WEEK		26
#define	_GET_TIMER_SOUND	27

#define	_GET_LINKENABLE		40
#define	_GET_LINK_FILE		41
#define	_GET_LINK_PARAM		42

#define	_GET_CREATE_YEAR	60
#define	_GET_CREATE_MONTH	61
#define	_GET_CREATE_DAY		62

#define	_GET_DEF_FONT		80
#define	_GET_DEF_FORECLR	81
#define	_GET_DEF_BACKCLR	82

#define	_GET_VERSION		100

typedef int (CALLBACK* GETMEMODATA)( LPVOID lp, int nGetType, LPVOID lpData, int nDataSize);
#endif

#pragma pack(8)
typedef struct tagDXLSAVEDATA
{
	int		m_nDataSize;
	BYTE	m_abyData[1];
}DXLSAVEDATA;

typedef struct tagDXLIFDATA100
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	LPCSTR			m_lpcszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル
	BOOL			m_blZoom;				// 小片／アイコン
	DXLSAVEDATA*	m_pstSaveData;			// 
}DXLIFDATA100;

#if ( _DXLVER >= 0x200)
typedef struct tagDXLIFDATA200
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	LPCSTR			m_lpcszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル
	BOOL			m_blZoom;				// 小片／アイコン
	DXLSAVEDATA*	m_pstSaveData;			// 
	GETMEMODATA		m_lpfnGetMemoData;		// callback
	LPVOID			m_lpGetMemoID;			// for callback:ここに触ると死ぬぞ！だんがぁ……ｱﾊﾊ( ^<
}DXLIFDATA200;
#endif

#if ( _DXLVER >= 0x200)
	typedef DXLIFDATA200 DXLIFDATA;
#else
	typedef DXLIFDATA100 DXLIFDATA;
#endif

typedef struct tagEDITDATA
{
	int		m_nSelStart;
	int		m_nSelEnd;
	HGLOBAL	m_hEdit;
	int		m_nSize;
	HGLOBAL m_hSaveData;
}EDITDATA;
#pragma pack()

// このＤＸＬの機能
#define	_DXL_NOUSERRESEIZE			0x0000000000000001		// リサイズは禁止
#define	_DXL_MOUSECLICK				0x0000000000000002		// マウスクリックを処理する
#define	_DXL_USEMENU				0x0000000000000004		// メニューを使用する
#define	_DXL_EDITHEPLER				0x0000000000000008		// 入力補助を行う
#define	_DXL_NATIVEDATA				0x0000000000000010		// 固有データ保存データ
#define	_DXL_NOEXSEND				0x0000000000000020		// 送信時は署名／拡張などのはサポートしない
#define	_DXL_TIMER					0x0000000000000040		// タイマ機能を使用する
#define	_DXL_NEEDSTRING				0x0000000000000080		// 描画処理時に文字列を必要とする
#define	_DXL_NOALPHABLEND			0x0000000000000100		// AlphaBlendを禁止

#define	_DT_EXCHANGE				0x0000000000000001		// 文字変換のみ：描画は覚え書
#define	_DT_OWNERDRAW				0x0000000000000002		// 描画処理有り

#define	_TM_REDRAW					0x0000000000000001		// 再描画
#define	_TM_CALCRECT				0x0000000000000002		// 大きさ再計算
#define	_TM_NOTIFYBLINK				0x0000000000000004		// ブリンク
#if ( _DXLVER >= 0x200)
#define	_TM_TONORMAL				0x0000000000000010		// 通常状態
#define	_TM_TOICON					0x0000000000000020		// アイコン化
#define	_TM_TOTITLE					0x0000000000000040		// タイトル化
#endif

typedef struct tagDXLSPECIFICATION
{
	UINT	unLowerStructVersion;	// 処理可能な最古バージョン
	UINT	unUpperStructVersion;	// 処理可能な最新バージョン
	char	szMenuString[ 50];		// デフォルトの処理名称
	UINT	unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	UINT	unDrawType;				// dxlの処理内容
	UINT	unStyle;				// Dxlのスタイル
}DXLSPECIFICATION;

#define	_DXL_MENUTOP			10000
#define	_DXL_MENU0				10000
#define	_DXL_MENU1				10001
#define	_DXL_MENU2				10002
#define	_DXL_MENU3				10003
#define	_DXL_MENU4				10004
#define	_DXL_MENU5				10005
#define	_DXL_MENU6				10006
#define	_DXL_MENU7				10007
#define	_DXL_MENU8				10008
#define	_DXL_MENU9				10009
#define	_DXL_MENU10				10010
#define	_DXL_MENU11				10011
#define	_DXL_MENU12				10012
#define	_DXL_MENU13				10013
#define	_DXL_MENU14				10014
#define	_DXL_MENU15				10015
#define	_DXL_MENU16				10016
#define	_DXL_MENU17				10017
#define	_DXL_MENU18				10018
#define	_DXL_MENU19				10019
#define	_DXL_MENUBOTTOM			10019

#ifdef	__cplusplus
extern "C"
{
#endif


// 機能確認
__declspec( dllexport ) BOOL DrawExSpecification( DXLSPECIFICATION* pDxlSpecification);
// 設定変更用
__declspec( dllexport ) BOOL DrawExConfigure( HWND hWnd);
// Copyright表示用
__declspec( dllexport ) BOOL DrawExAbout( HWND hWnd);
// 初期化処理
__declspec( dllexport ) BOOL InitializeChip( DWORD* pdwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// 終了処理
__declspec( dllexport ) BOOL UnInitializeChip( DWORD dwDxlWord, HWND hWnd);
// 大きさ再計算：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) BOOL CalcDefaultRect( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize, LPCSTR lpcszMemoData);
// 表示：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) BOOL DrawChip( DWORD dwDxlWord, HWND hWnd, HDC hDC, LPCSTR lpcszMemoData);
// マウスクリック
__declspec( dllexport ) BOOL MouseClik( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// サイズ変更：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) BOOL Resize( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// 小片化／アイコン化に呼ばれる：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) void Zoom( DWORD dwDxlWord, HWND hWnd, BOOL blZoom);
// データ更新：編集により発生
__declspec( dllexport ) BOOL Update( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// 送信する文字列
__declspec( dllexport ) int ExchangeSendString( HGLOBAL* phgMem);
// 描画する文字列：_DT_EXCHANGE指定時のみ処理
__declspec( dllexport ) int GetMemoString( DWORD dwDxlWord, LPSTR lpszData, int nSize, LPCSTR lpcszMemoData);
// ポップアップメニュー
__declspec( dllexport ) HMENU GetDrawExMenu( DWORD dwDxlWord);
// ポップアップメニュー
__declspec( dllexport ) DWORD DoDrawExMenu( DWORD dwDxlWord, HWND hWnd, UINT unMenuID);
// 特殊編集：入力ヘルパー
__declspec( dllexport ) int EditHelper( HWND hWnd, EDITDATA* pstEditData);
// データ保存時の編集
__declspec( dllexport ) BOOL GetSaveData( DWORD dwDxlWord, HWND hWnd, HGLOBAL* phGMem);
// タイマ周期取得
__declspec( dllexport ) int GetTimerSpan( DWORD dwDxlWord);
// タイマ処理
__declspec( dllexport ) DWORD DoTimer( DWORD dwDxlWord);


#if ( _DXLVER >= 0x200)
// 以降のインターフェイスは、シン覚え書で利用
// マウスクリック：拡張
// このインターフェイスが無い場合は、旧MouseClikが呼び出されます。
__declspec( dllexport ) DWORD MouseClikEx( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// 送信する文字列：拡張
// このインターフェイスが無い場合は、旧ExchangeSendStringが呼び出されます。
__declspec( dllexport ) int ExchangeSendStringEx( EDITDATA* pstEditData);
// 小片のアクティブ：新規
__declspec( dllexport ) void Active( DWORD dwDxlWord, HWND hWnd, BOOL blActive);
#endif

#ifdef	__cplusplus
}
#endif

#endif	//__OBOE_DXL_IF_H__
