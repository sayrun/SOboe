#if !defined(AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_)
#define AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlphaBlendDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAlphaBlendDialog ダイアログ

class CAlphaBlendDialog : public CDialog
{
// コンストラクション
public:
	CAlphaBlendDialog(CWnd* pParent = NULL);   // 標準のコンストラクタ

	void SetAlphaBlend( BYTE byAlphaBlend);
	BYTE GetAlphaBlend( void) const;

	// 使用中？
	static BOOL IsUsed();
	static HWND GetUseWnd();

// ダイアログ データ
	//{{AFX_DATA(CAlphaBlendDialog)
	enum { IDD = IDD_ALPHABLEND };
	CSliderCtrl	m_cSldAlphaBlend;
	int		m_nAlphaBlend;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CAlphaBlendDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	static CSemaphore sm_cSmpDoABModal;
	static HWND sm_hWndParent;
	HWND m_hWndParent;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAlphaBlendDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_)
