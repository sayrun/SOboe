#ifndef	__CHIPARG_IF_H__
#define	__CHIPARG_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define	__ARRAGESTRUCT_VERSION100	0x0100
#define	__ARRAGESTRUCT_VERSION200	0x0200

// 整列構造体ね:__ARRAGESTRUCT_VERSION100
typedef	struct tagARRANGESTRUCT
{
	RECT		stRctChip;
	POINT		stPntIcon;
	COLORREF	clrText;
	COLORREF	clrBack;
	BOOL		blIcon;
	int			nIconIndex;
	BOOL		blHide;
	BOOL		blPosLock;
}ARRANGESTRUCT;

// 整列構造体ね:__ARRAGESTRUCT_VERSION200
#define	ARG_WINDOW_TYPE_NORMAL	0
#define	ARG_WINDOW_TYPE_TITLE	1
#define	ARG_WINDOW_TYPE_ICON	2

typedef	struct tagARRANGESTRUCT200
{
	// ここは本体に反映される
	RECT		stRctNormal;	// 全ての位置が反映される
	RECT		stRctTitle;		// タイトルはタイトルの左上だけ反映される
	POINT		stPntIcon;		// 全ての位置が反映される

	// 追加的データ
	COLORREF	clrText;		// テキストの色
	COLORREF	clrBack;		// 背景色
	int			nWindowType;	// Window形状
	int			nIconIndex;		// アイコン種別
	BOOL		blHide;			// 表示非表示
	BOOL		blPosLock;		// 位置固定

	// 使うかなぁ……
	BOOL		blDelOnQuit;	// 終了時に削除なので、整列させないときに便利か？！
	BOOL		blTimerEnabel;	// いらないかも……あるいは重ねてしまうようにするとか？！
	BOOL		blLinkEnabel;	// リンクされているものだけを集めるとか

	// Dxlに付随するもの…
	UINT		unDxlID;		// 0はDxlが設定されていない
	int			nDxlDataSize;	// 0 >= データなし。
	int			nDxlDataOffset;	// 配列全体の先頭からのオフセット
}ARRANGESTRUCT200;

typedef struct tagARRANGESPEC
{
	UINT	unLowerStructVersion; // 処理可能な最古バージョン
	UINT	unUpperStructVersion; // 処理可能な最新バージョン
	// ここにタッタ１byteでもデータを拾ってこれるように作っておけば
	// DLLとお話できたものを……しくしく(T-T
}ARRANGESPEC;

#ifdef	__cplusplus
extern "C" 
{
#endif

// 整列I/Fバージョンチェック：実装必須
__declspec( dllexport) BOOL GetArrangeSpecification( ARRANGESPEC* pstSpe);

// 以下のI/Fの実装は単位毎に実装のこと

// 通常小片整列I/F
__declspec( dllexport) BOOL ArrangeChipConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeChipAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeChip( UINT unVersion, int nArgDataCount, LPVOID pstArgData, int nIconCount);
// タイトル化小片整列I/F
__declspec( dllexport) BOOL ArrangeTitleConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeTitleAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeTitle( UINT unVersion, int nArgDataCount, LPVOID pstArgData, int nIconCount);
// アイコン化小片整列I/F
__declspec( dllexport) BOOL ArrangeIconConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeIconAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeIcon( UINT unVersion, int nArgDataCount, LPVOID pstArgData, LPSIZE pstIcon, int nIconCount);
// 状態非依存整列I/F
// これは全体に作用する。現在アイコン状態であっても通常サイズを設定することができる
__declspec( dllexport) BOOL ArrangeAllConfigure( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeAllAbout( UINT unVersion, HWND hParent);
__declspec( dllexport) BOOL ArrangeAll( UINT unVersion, int nArgDataCount, LPVOID pstArgData, LPSIZE pstIcon, int nIconCount);

#ifdef	__cplusplus
}
#endif

#endif	//__CHIPARG_IF_H__
