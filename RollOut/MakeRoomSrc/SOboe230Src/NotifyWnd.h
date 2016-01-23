#if !defined(AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_)
#define AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// NotifyWnd.h : ヘッダー ファイル
//

class CMemoChip;

/////////////////////////////////////////////////////////////////////////////
// CNotifyWnd ウィンドウ

class CNotifyWnd : public CWnd
{
// コンストラクション
public:
	CNotifyWnd();

// アトリビュート
public:
	static UINT m_sunKillMsg;

// オペレーション
public:
	BOOL Create( LPCTSTR lpszWindowName, CMemoChip* pParentWnd);

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CNotifyWnd)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CNotifyWnd();

	// 生成されたメッセージ マップ関数
protected:
	int			m_nCount;
	HWND		m_hParentWnd;
	CMemoChip*	m_pcParentWnd;

	//{{AFX_MSG(CNotifyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LONG OnKillMsg( UINT, LONG );
	DECLARE_MESSAGE_MAP()
};

class CRenotifyWnd : public CNotifyWnd
{
// コンストラクション
public:
	CRenotifyWnd();

// オペレーション
public:
	BOOL Create( LPCTSTR lpszNotifyMsg, CWnd* pcParent);

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CRenotifyWnd)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	static UINT sm_unMsgFromTray;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_NOTIFYWND_H__B636C504_5E6B_11D2_A0FF_00C04FCA10B7__INCLUDED_)
