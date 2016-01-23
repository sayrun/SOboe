#if !defined(AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_)
#define AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AboutDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog ダイアログ

typedef	UINT ( WINAPI *GetFreeSystemResource)( UINT);

class CAboutDialog : public CDialog
{
// コンストラクション
public:
	CAboutDialog( int nWinType = TRUE/*TRUE=98*/, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	enum
	{
		WINTYPE_98,
		WINTYPE_NT
	};
	enum
	{
		IDD98 = IDD_ABOUTBOX98,
		IDDNT = IDD_ABOUTBOXNT
	};
	//{{AFX_DATA(CAboutDialog)
	CProgressCtrl	m_cPgbUSER;
	CProgressCtrl	m_cPgbSYSTEM;
	CProgressCtrl	m_cPgbGDI;
	CString	m_cStrGDI;
	CString	m_cStrSYSTEM;
	CString	m_cStrUSER;
	CProgressCtrl	m_cPgbPhysical;
	CProgressCtrl	m_cPgbPagefile;
	CString	m_cStrPhysical;
	CString	m_cStrPagefile;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CAboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	int	m_nWinType;
	GetFreeSystemResource		m_lpGetFreeSystemResource;
	HINSTANCE m_hInst;
	BOOL CheckResources( void);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CAboutDialog)
	afx_msg void OnGosuport();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_)
