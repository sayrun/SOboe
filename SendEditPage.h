#if !defined(AFX_SENDEDITPAGE_H__D4B3B904_4392_11D2_90BD_00804C15184E__INCLUDED_)
#define AFX_SENDEDITPAGE_H__D4B3B904_4392_11D2_90BD_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SendEditPage.h : ヘッダー ファイル
//
class CNetExLib;
#include "GroupEntry.h"

/////////////////////////////////////////////////////////////////////////////
// CSendEditPage ダイアログ

class CSendEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSendEditPage)

// コンストラクション
public:
	CSendEditPage();
	~CSendEditPage();

	void SendDirect( BOOL lbDirect);

	int GetEntryCount( void);
	const CEntryData* GetEntryData( int nIndex);
	BOOL AddEntryData( const CEntryData* pcEntryData);

	UINT	m_unDxlID;
	UINT	m_unNxlID;
// ダイアログ データ
	CString		m_cStrResult1;
	CString		m_cStrResult2;
	BOOL		m_blNotifyOpen;
	BOOL		m_blNotifyResult;
	BOOL		m_blOtherResult;
	BOOL		m_blNeedPassWord;
	//{{AFX_DATA(CSendEditPage)
	enum { IDD = IDD_SENDPAGE };
	CButton	m_cBtnDeleteAfterSend;
	CButton	m_cBtnSendResize;
	CButton	m_cBtnSendCenter;
	CButton	m_cBtnAppendTime;
	CButton	m_cBtnAppendSignature;
	CButton	m_cBtnPack;
	CButton	m_cBtnExtSetting;
	CButton	m_cBtnDelNode;
	CListBox	m_cLstSendList;
	CComboBox	m_cCmbProtcolList;
	CTreeCtrl	m_cTreeNode;
	BOOL	m_blAppendSignature;
	int		m_nAppendPos;
	BOOL	m_blAppendTime;
	BOOL	m_blSendCenter;
	BOOL	m_blSendResize;
	BOOL	m_blPack;
	int		m_nProtcol;
	BOOL	m_blDeleteAfterSend;
	//}}AFX_DATA
	CButton	m_cBtnAppendHead;
	CButton	m_cBtnAppendTail;

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CSendEditPage)
	public:
	virtual void OnOK();
	virtual BOOL OnSetActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	BOOL AddSendList( const CEntryData* pcEntryData, BOOL blListBox = TRUE);
	BOOL SetNodeList( void);
	void LoopSearch( HTREEITEM hSearchTop);
	BOOL IsNormalDxl( void);

	void RestoreListBox( void);

	CNetExLib*	m_pDefaultNxl;
	CList	< CNetExLib*, CNetExLib*&>	m_cLstNxls;

	CImageList*	m_pImageList;
	CBitmap*	m_pcBitmap;
	HBITMAP		m_hBitmap;

	BOOL		m_blSendDirect;
	BOOL		m_blCheckOffForce;

	CList	< CEntryData*, CEntryData*&>	m_cLstEntrys;
	CList	< CEntryData*, CEntryData*&>	m_cLstSendNodes;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSendEditPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkNodetree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSendlist();
	afx_msg void OnSelchangeSendlist();
	afx_msg void OnDelnode();
	afx_msg void OnExtsetting();
	afx_msg void OnSelchangeProtcol();
	afx_msg void OnPacksend();
	afx_msg void OnNodelistupdate();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAppend();
	afx_msg void OnKeydownNodetree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SENDEDITPAGE_H__D4B3B904_4392_11D2_90BD_00804C15184E__INCLUDED_)
