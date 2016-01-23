#if !defined(AFX_NETWORKEXTPAGE_H__BB6D2CB1_81C8_11D2_9204_004005469C82__INCLUDED_)
#define AFX_NETWORKEXTPAGE_H__BB6D2CB1_81C8_11D2_9204_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkExtPage.h : �w�b�_�[ �t�@�C��
//

class CProfileData;

/////////////////////////////////////////////////////////////////////////////
// CNetworkExtPage �_�C�A���O

class CNetworkExtPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CNetworkExtPage)

// �R���X�g���N�V����
public:
	CNetworkExtPage();
	~CNetworkExtPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CNetworkExtPage)
	enum { IDD = IDD_NETWORKEXT };
	CButton	m_cBtnNotifyOpen;
	CButton	m_cBtnFullLoggingReceiveChip;
	CButton	m_cBtnFullLoggingSendChip;
	BOOL	m_blPackSend;
	BOOL	m_blNotifyOpen;
	BOOL	m_blHideReceive;
	BOOL	m_blHideSubNotify;
	BOOL	m_blLoggingSendChip;
	BOOL	m_blFullLoggingSendChip;
	BOOL	m_blLoggingReceiveChip;
	BOOL	m_blFullLoggingReceiveChip;
	BOOL	m_blCheckDelOnQuit;
	BOOL	m_blReceiveAndSave;
	BOOL	m_blDeleteAfterSend;
	BOOL	m_blDelOnQuit2Rcv;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CNetworkExtPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CNetworkExtPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSendlog();
	afx_msg void OnReceivelog();
	afx_msg void OnSendpackdefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_NETWORKEXTPAGE_H__BB6D2CB1_81C8_11D2_9204_004005469C82__INCLUDED_)
