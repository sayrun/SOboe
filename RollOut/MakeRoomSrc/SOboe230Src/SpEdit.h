#if !defined(AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_)
#define AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CSpEdit ウィンドウ

class CSpEdit : public CEdit
{
// コンストラクション
public:
	CSpEdit();

// アトリビュート
public:

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSpEdit)
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CSpEdit();

	// 生成されたメッセージ マップ関数
protected:
	int	m_nSelStart;
	int	m_nSelEnd;
	//{{AFX_MSG(CSpEdit)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SPEDIT_H__EA817CF3_09DE_11D3_92FF_004005469C82__INCLUDED_)
