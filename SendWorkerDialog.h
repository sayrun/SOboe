#if !defined(AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_)
#define AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SendWorkerDialog.h : ヘッダー ファイル
//

#include "NetEx_IF.h"
#include "ResultListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSendWorkerDialog ダイアログ

class CSendWorkerDialog : public CDialog
{
// コンストラクション
public:
	CSendWorkerDialog( CWnd* pParent = NULL);   // 標準のコンストラクタ
	~CSendWorkerDialog( void);

	BOOL SetSendData( const INNERSENDDATA& stInnerSendData);
	BOOL AddSendEntry( const CEntryData* pcEntry);

// ダイアログ データ
	//{{AFX_DATA(CSendWorkerDialog)
	enum { IDD = IDD_SENDWORKER };
	CButton	m_cBtnOK;
	CButton	m_cBtnCancel;
	CResultListBox	m_cLstSendResult;
	int		m_nLstSendResult;
	BOOL	m_blAutoClose;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CSendWorkerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	BOOL DoSend( void);
// インプリメンテーション
protected:
	BOOL			m_blCanceled;

	HANDLE			m_hCancelEvent;

	CCriticalSection	m_cNotifyCS;

	static UINT		sm_unNotifyEnd;

	INNERSENDDATA	m_stInnerSendData;
	CList	< CEntryData*, CEntryData*&>	m_cLstSendNodes;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSendWorkerDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkSendresultlist();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnAutoclose();
	//}}AFX_MSG
	afx_msg LONG OnNotifyEnd( UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_)
