#ifndef	__OBOE_DXL_IF_H__
#define	__OBOE_DXL_IF_H__

// 構造体のバージョン
#define	_DXL_IF_DATA_VER		0x00000100

typedef struct tagDXLIFDATA
{
	int				m_nSize;				// この構造体のサイズ
	int				m_nVersion;				// この構造体のバージョン：_IF_DATA_VERを指定の事

	char			m_achszMemoTitle[64];	// 小片のタイトル
	HGLOBAL			m_hachszMemoData;		// 小片の内容
	COLORREF		m_clrTextColor;			// テキスト色
	COLORREF		m_clrBackColor;			// 背景色
	LOGFONT			m_stLogFont;			// フォントスタイル
}DXLIFDATA;

// このＧＥＬがサポートしない機能
#define	_UNSUPPORT_RESEIZE		0x0000000000000001		// リサイズは禁止
#define	_UNSUPPORT_CLICK		0x0000000000000002		// クリックは処理しない
#define	_UNSUPPORT_MENU			0x0000000000000004		// メニューは使用しない
#define	_UNSUPPORT_EDITHEPLER	0x0000000000000008		// 入力補助はない

#define	_DT_EXCHANGE			0x0000000000000001		// 文字変換のみ：描画は覚え書
#define	_DT_OWNERDRAW			0x0000000000000002		// 描画処理有り

typedef struct tagDXLSPECIFICATION
{
	UINT	unLowerStructVersion;	// 処理可能な最古バージョン
	UINT	unUpperStructVersion;	// 処理可能な最新バージョン
	char	szMenuString[ 50];		// デフォルトの処理名称
	UINT	unDXLID;				// 全dxl間でユニークなＩＤ：T.Chibaの発行による
	UINT	unDrawType;				// dxlの処理内容
	UINT	unUnsupportMenu;		// サポートしない機能
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
#define	_DXL_MENUBOTTOM			10009

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
__declspec( dllexport ) BOOL CalcDefaultRect( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// 表示：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) BOOL DrawChip( DWORD dwDxlWord, HWND hWnd, HDC hDC);
// マウスクリック
__declspec( dllexport ) BOOL MouseClik( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// サイズ変更：_DT_OWNERDRAW指定時のみ処理
__declspec( dllexport ) BOOL Resize( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// データ更新：編集により発生
__declspec( dllexport ) BOOL Update( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// 送信する文字列
__declspec( dllexport ) int GetSendString( DWORD dwDxlWord, LPSTR lpszData, int nSize);
// 描画する文字列：_DT_EXCHANGE指定時のみ処理
__declspec( dllexport ) int GetMemoString( DWORD dwDxlWord, LPSTR lpszData, int nSize);
// ポップアップメニュー
__declspec( dllexport ) HMENU GetDrawExMenu( DWORD dwDxlWord);
// ポップアップメニュー
__declspec( dllexport ) BOOL DoDrawExMenu( DWORD dwDxlWord, UINT unMenuID);
// 特殊編集：入力ヘルパー
__declspec( dllexport ) int EditHelper( HWND hWnd, HGLOBAL* hgMem);

#ifdef	__cplusplus
}
#endif

#endif	//__OBOE_DXL_IF_H__
