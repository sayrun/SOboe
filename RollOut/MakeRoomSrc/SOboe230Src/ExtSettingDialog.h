#if !defined(AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_)
#define AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExtSettingDialog.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CExtSettingDialog ダイアログ

class CExtSettingDialog : public CDialog
{
// コンストラクション
public:
	CExtSettingDialog( BOOL blEnableExt, CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CExtSettingDialog)
	enum { IDD = IDD_EXTSETTINGDLG };
	CButton	m_cBtnOtherResult;
	CButton	m_cBtnNeedPassWord;
	CButton	m_cBtnNotifyResult;
	CButton	m_cBtnNotifyOpen;
	CEdit	m_cEdtResult2;
	CEdit	m_cEdtResult1;
	CString	m_cStrResult1;
	CString	m_cStrResult2;
	BOOL	m_blNotifyOpen;
	BOOL	m_blNotifyResult;
	BOOL	m_blOtherResult;
	BOOL	m_blNeedPassWord;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CExtSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL	m_blEnableExt;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CExtSettingDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingCheck();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_)
