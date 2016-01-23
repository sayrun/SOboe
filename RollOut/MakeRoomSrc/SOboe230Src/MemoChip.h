#if !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemoChip.h : ヘッダー ファイル
//

#include "MemoData.h"
#include "GroupEntry.h"
#include "DxlControl.h"
#include "OleDrop.h"
#include "NetExLib.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoChip ウィンドウ

#define	MOVEAREAPOINT_MAX	4
#define	TOOLTIPSTR_MAX		512

#ifndef	WS_EX_LAYERED
/* Windows 2000でサポートされるAlphaBlendの実装
*/
#define	WS_EX_LAYERED	0x00080000L
#define	LWA_COLORKEY	1
#define	LWA_ALPHA		2
#endif

class CMemoChip : public CWnd
{
// コンストラクション
public:
	CMemoChip();

// アトリビュート
public:

// オペレーション
public:
	const CMemoChip& operator=( const CMemoChip& cMemoChip);
	BOOL ShowWindow( int nCmdShow);

	BOOL OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	DROPEFFECT OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CMemoChip)
	public:
	virtual BOOL Create( UINT unID, HWND hParentWnd, BOOL blHide, HICON hIcon, CToolTipCtrl* pcToolTip) ;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CMemoChip();

	// データへのアクセス
	void SetMemoData( const CMemoData& cMemoData);
	BOOL GetMemoData( CMemoData& cMemoData) const;
	const CMemoData* AccessMemoData( void) const;
	UINT GetID( void) const;

	// サイズ計算
	BOOL CalcDefaultRect( BOOL blCentering = FALSE);
	BOOL CalcTitleRect( CRect& cRectNew);

	void ActiveZOrder( void);

	BOOL DelOnQuit( BOOL blDelOnQuit);
	BOOL IsDelOnQuit( void) const;
	BOOL DrawResizeFrame( BOOL blResize);

	// 日付変更によるタイマ検査
	BOOL CheckTimerOnDayChange( const COleDateTime & cTime);
	void CheckTimer( const COleDateTime& cTime);
	void DxlCheckTimer( const COleDateTime& cTime);

	// OLEによるD&D
	BOOL OleDrop( LPCSTR lpcszData, const POINT& stPoint, BOOL blLink);

	// 小片の終了
	void SmartDelete();

	// 一時非表示
	void OnceHide( void);
	void RestoreOnceHide( void);

	// 内部での管理色
	const COLORREF GetInternalForeColor( void) const;
	const COLORREF GetInternalBackColor( void) const;

	BOOL SetEntryData( const CEntryData* pcEntryData);
	const CEntryData* GetEntryData( void) const;
	BOOL SetResults( const INNERRESULTS& stResults);

	BOOL IsPack( void) const;
	void SetResultID( const CString& cStrResultID);
	void Pack( BOOL blNeedPassWord);
	void CheckOpen( BOOL blCheckOpen);
	BOOL ReceiveNotify( void);
	void AddResult( const CString& cStrResult);

	void Serialize( CArchive& ar );

	BOOL ArrangeIcon( const POINT& pstPoint);
	BOOL ArrangeTitle( const RECT& stRect);
	BOOL ArrangeChip( const RECT& stRect);

	void SetContextID( LPCSTR lpcszID);
	BOOL IsEqualContextID( LPCSTR lpcszID);

	void SetReceiveTime( const COleDateTime& cReceiveTime);
	
	BOOL NoCheckReceive( void) const;

	void OneTimeTop( void);

	DECLARE_SERIAL( CMemoChip);
	// プライベートなメソッド
protected:
	// パスワードのチェック
	BOOL CheckPassWord( void);

	// リサイズエリアの描画
	void DrawResizeArea( CDC* pcDC, BOOL blActive);

	// リサイズ時のアニメーション
	void DoResizeFrameAnime( const CRect& cRectTo, const CRect& cRectFrom);

	// 最前列への移動とブリンク通知
	BOOL StartPopupNotify( void);
	BOOL EndPopupNotify( void);
	// ブリンク
	BOOL DoBlink( void);

	// 前後日の通知検査
	BOOL CheckExtensiveNotify( const COleDateTime& cTime);

	// 挿入貼り付け
	BOOL PasteMemo( const CString& cStrData);
	// 上書き貼り付け
	BOOL OverWriteMemo( const CString& cStrData, BOOL blNoTriming = FALSE);
	// 貼り付け継承作成
	BOOL CreateMemo( const CString& cStrData);
	// 関連付け貼り付け
	BOOL PasteLink( const CString& cStrData);

	// クリップボードからの文字取得
	BOOL GetClipboradString( CString& cStr);
	// クリップボードからのファイルリンク取得
	BOOL GetClipboradLink( CString& cStr);

	// OLE D&D
	DWORD DoDragDrop( BOOL blMoveEnable = FALSE);

	// シースルー用のリージョン作成
	CRgn* CreateSeeThroughRgn( CDC* pcDC, const CRect& cRctClient);

	// ツールチップ
	void CreateToolTip( void);
	void ModifyToolTip( LPCSTR lpcsz = NULL);
	void DestroyToolTip( void);

	// ダブルクリックの処理
	BOOL OnDblClk( UINT nFlags, int nFunc);

	// 裏返しを開く
	BOOL OpenPackChip( void);

	// Window領域を見えるところにするやつね
	const CRect ClipRectToMonitor( const CRect& cRctSrc);

	int EditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize);
	int ExEditChip( CMemoData& cMemoData, BOOL& blDelOnQuit, BOOL &blResize);

	// レイア処理の独自実装だね
	BOOL SetLayeredWindowAttributes( COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	BOOL IsSupportAlphaBlend( void);

	// メンバー変数
protected:
	static CString		m_cStrClassName;

	CMemoData	m_cMemoData;

	HWND		m_hParentWnd;	// 親のWindow

	POINT		m_astMoveRgnPoints[ MOVEAREAPOINT_MAX];		// リサイズエリアのポイント

	bool		m_blNowMove;	// 移動中フラッグ
	bool		m_blNowZMove;	// Ｚ移動フラッグ
	bool		m_blMouseClickStart;	// 移動のためのクリック開始
	CPoint		m_cPntStart;	// 移動開始のクリック位置

	CDxlControl*	m_pcDxlControl;

	bool		m_blBlink;	// ブリンク

	bool		m_blDelOnQuit;	// 終了時の削除指定

	COLORREF	m_clrInternalForeColor;	// 内部処理用の文字色
	COLORREF	m_clrInternalBackColor;	// 内部処理用の背景色

	bool		m_blBeforeVisible;	// 通知による表示の前状態
	int			m_nNotifyDay;		// 通知の日にち

	bool		m_blFrameDraw;		// リサイズ時のフレーム描画

	UINT		m_unID;				// 貼り付けられている間のみ有効な管理ID

	COleDrop	m_cOleDrop;			// OLE D&D用

	bool		m_blSmartClose;		// 削除判定

	BOOL		m_blShowBeforeOndeHide; // 一時表情の前状態

	CRgn*		m_pcRgnSeeThrough;

	bool		m_blOnceTop;

	CToolTipCtrl*	m_pcToolTip;

	CString		m_cStrUndo;

	static UINT	m_unURL;

	CEntryData* m_pcEntryData;

	BOOL		m_blPack;
	BOOL		m_blCheckOpen;
	BOOL		m_blNeedPassWord;
	CString*		m_pcStrResultID;
	INNERRESULTS*	m_pstResults;

	BOOL		m_blReceive;

	COleDateTime*	m_pcReceiveTime;

	BOOL		m_blSysResize;

	typedef BOOL (PASCAL FAR *LPSETLAYEREDWINDOWATTRIBUTES)( HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	static LPSETLAYEREDWINDOWATTRIBUTES	m_lpfnSetLayeredWindowAttributes;
	static LPSETLAYEREDWINDOWATTRIBUTES GetSetLayeredWindowAttributesProcAddress( void);

	DWORD m_dwTime;

	// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CMemoChip)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnToIcon();
	afx_msg void OnToNormal();
	afx_msg void OnToTitle();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnReedit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPasteAppend();
	afx_msg void OnPasteOverwrite();
	afx_msg void OnPasteCreate();
	afx_msg void OnPasteLink();
	afx_msg void OnDelete();
	afx_msg void OnNewmemo();
	afx_msg void OnCopy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLinkexec();
	afx_msg void OnDelonquit();
	afx_msg void OnMemoexport();
	afx_msg void OnTextsave();
	afx_msg void OnCopyMemodata();
	afx_msg void OnPoslock();
	afx_msg void OnSeetrough();
	afx_msg void OnHide();
	afx_msg void OnUndo();
	afx_msg void OnSend();
	afx_msg void OnSender();
	afx_msg void OnReply();
	//}}AFX_MSG
	afx_msg void OnCelMenu( UINT nID);
	afx_msg void OnDxlMenu( UINT nID);
	afx_msg void OnAlphaBlend( UINT nID);
	DECLARE_MESSAGE_MAP()

friend class COleDrag;
};

LPCTSTR AFXAPI RegisterMemoChipClass(UINT nClassStyle);
void ResultSendThread( void* lpvData);

#include "MemoChip.inl"
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_MEMOWND_H__386BA43D_26B0_11D2_90A3_00804C15184E__INCLUDED_)
