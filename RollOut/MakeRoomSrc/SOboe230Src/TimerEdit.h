#if !defined(AFX_TIMEREDIT_H__5B9B7386_043F_11D3_92F4_004005469C82__INCLUDED_)
#define AFX_TIMEREDIT_H__5B9B7386_043F_11D3_92F4_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimerEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CTimerEdit ダイアログ

class CTimerEdit : public CDialog
{
// コンストラクション
public:
	CTimerEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CTimerEdit)
	enum { IDD = IDD_TIMER };
	CButton	m_cBtnNotifySoundPlay;
	CButton	m_cBtnNotifySoundSel;
	CEdit	m_cEdtNotifySound;
	CButton	m_cBtnAfterNotify;
	CSpinButtonCtrl	m_cSpnBeforeNotify;
	CEdit	m_cEdtBeforeNotify;
	CSpinButtonCtrl	m_cSpnMonth;
	CSpinButtonCtrl	m_cSpnMin;
	CSpinButtonCtrl	m_cSpnHour;
	CSpinButtonCtrl	m_cSpnDay;
	CComboBox	m_cCmbNotifyShift;
	CComboBox	m_cCmbTimerType;
	CEdit	m_cEdtDay;
	CEdit	m_cEdtMonth;
	CEdit	m_cEdtMin;
	CEdit	m_cEdtHour;
	CListBox	m_cLstDayOfWeek;
	int		m_nMin;
	int		m_nHour;
	int		m_nMonth;
	int		m_nDay;
	BOOL	m_blTimerEnable;
	int		m_nTimerType;
	int		m_nNotifyShift;
	BOOL	m_blAfterNotify;
	int		m_nBeforeNotify;
	CString	m_cStrNotifySound;
	//}}AFX_DATA
	int		m_nWeek;


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTimerEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void ControlEnable( void);

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CTimerEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimerenable();
	afx_msg void OnSelchangeTimertype();
	afx_msg void OnWavsel();
	afx_msg void OnWaveplay();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_TIMEREDIT_H__5B9B7386_043F_11D3_92F4_004005469C82__INCLUDED_)
