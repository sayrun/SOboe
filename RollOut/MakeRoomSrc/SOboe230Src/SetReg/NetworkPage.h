// NetworkPage.h : ヘッダー ファイル
//
#include "..\NetEx_IF.h"

#ifndef __NETWORKPAGE_H__
#define __NETWORKPAGE_H__

#define	_NETEXLIB_MAX	10

/////////////////////////////////////////////////////////////////////////////
// CNetworkPage ダイアログ
class CProfileData;

class CNetworkPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNetworkPage)

// 構築
public:
	CNetworkPage();
	~CNetworkPage();

// ダイアログ データ
	//{{AFX_DATA(CNetworkPage)
	enum { IDD = IDD_NETWORKPAGE };
	CSpinButtonCtrl	m_cSpnDelay;
	CEdit	m_cEdtSignature;
	CEdit	m_cEdtReadyDelay;
	CButton	m_cBtnConfig;
	CButton	m_cBtnAbout;
	CButton	m_cBtnInst;
	CButton	m_cBtnUninst;
	CListBox	m_cLstNxl;
	BOOL	m_blEnableNetwork;
	CString	m_cStrSignature;
	int		m_nResumeDelay;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CNetworkPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:
	CString		m_cStrNxlPath[ _NETEXLIB_MAX];
	CString		m_cStrNxlMenu[ _NETEXLIB_MAX];
	UINT		m_unNxlID[ _NETEXLIB_MAX];
	int			m_nDefaultNxl;

	// 機能確認
	typedef	BOOL ( *PNETEXSPECIFICATION)( NXLSPECIFICATION* pNxlSpecification);
	// 設定変更用
	typedef	BOOL ( *PNETEXCONFIGURE)( HWND hWnd);
	// Copyright表示用
	typedef	BOOL ( *PNETEXABOUT)( HWND hWnd);

	BOOL CheckNxl( const CString cStrPath, CString& cStrMenuString, UINT& unNxlID);

// Implementation
protected:
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CNetworkPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeNxllist();
	afx_msg void OnNxlabout();
	afx_msg void OnNxlconfig();
	afx_msg void OnDblclkNxllist();
	afx_msg void OnNxlinstall();
	afx_msg void OnNxluninstall();
	afx_msg void OnEnablenetwork();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CProfileData*	m_pcProfileDatas;
};



#endif // __NETWORKPAGE_H__
