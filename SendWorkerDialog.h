#if !defined(AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_)
#define AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SendWorkerDialog.h : �w�b�_�[ �t�@�C��
//

#include "NetEx_IF.h"
#include "ResultListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSendWorkerDialog �_�C�A���O

class CSendWorkerDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CSendWorkerDialog( CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CSendWorkerDialog( void);

	BOOL SetSendData( const INNERSENDDATA& stInnerSendData);
	BOOL AddSendEntry( const CEntryData* pcEntry);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSendWorkerDialog)
	enum { IDD = IDD_SENDWORKER };
	CButton	m_cBtnOK;
	CButton	m_cBtnCancel;
	CResultListBox	m_cLstSendResult;
	int		m_nLstSendResult;
	BOOL	m_blAutoClose;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CSendWorkerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	BOOL DoSend( void);
// �C���v�������e�[�V����
protected:
	BOOL			m_blCanceled;

	HANDLE			m_hCancelEvent;

	CCriticalSection	m_cNotifyCS;

	static UINT		sm_unNotifyEnd;

	INNERSENDDATA	m_stInnerSendData;
	CList	< CEntryData*, CEntryData*&>	m_cLstSendNodes;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SENDWORKERDIALOG_H__22F0235C_4913_11D2_90C1_00804C15184E__INCLUDED_)
