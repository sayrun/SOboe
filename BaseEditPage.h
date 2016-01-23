#if !defined(AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// BaseEditPage.h : ヘッダー ファイル
//

#include "SpEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseEditPage ダイアログ

class CBaseEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CBaseEditPage)

// コンストラクション
public:
	CBaseEditPage();
	~CBaseEditPage();

// ダイアログ データ
	BOOL		m_blForeColor;	// 文字色の有効性
	COLORREF	m_clrFore;
	BOOL		m_blBackColor;	// 背景色の有効性
	COLORREF	m_clrBack;
	BOOL		m_blLogFont;	// フォントの有効性
	LOGFONT		m_stLogFont;
	UINT		m_unDxlID;			// 描画拡張ID
	UINT		m_unDxlExtDataSize;	// DXLに関する特殊保存データサイズ
	BYTE*		m_pabyDxlExtData;	// DXLに関する特殊保存データ
	BOOL		m_blResizeLock;
	CString		m_cStrMemo4IF;
	int			m_nIconIndex;
	//{{AFX_DATA(CBaseEditPage)
	enum { IDD = IDD_BASEPAGE };
	CButton	m_cBtnIconSel;
	CButton	m_cBtnResize;
	CButton	m_cBtnEditHelper;
	CSpEdit	m_cEdtMemo;
	CString	m_cStrMemo;
	CString	m_cStrTitle;
	BOOL	m_blResize;
	BOOL	m_blDelOnQuit;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CBaseEditPage)
	public:
	virtual BOOL OnSetActive();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CBaseEditPage)
	afx_msg void OnFont();
	afx_msg void OnColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnDxledithelper();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnIconsel();
	//}}AFX_MSG
	afx_msg void OnTemplteMenu( UINT nID);
	afx_msg LRESULT OnSetFocusDelay( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// 作業用データ
protected:
	UINT	m_unOldDxlID;
	CBrush	m_cBrhEdit;
	CFont	m_cFntEdit;
	CBitmap m_cBmpIcon;
	CIconSelectMenu* m_pcIcon;
protected:
	BOOL DxlEditSupport( const CDrawExLib* pcDrawExLib);
	enum
	{
		_INSERT_TIME,
		_INSERT_DATE,
		_INSERT_DAYOFWEEK
	};
	void InsertTime( int nType);
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_BASEEDITPAGE_H__741DB808_2743_11D2_90A4_00804C15184E__INCLUDED_)
