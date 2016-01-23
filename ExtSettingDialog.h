#if !defined(AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_)
#define AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ExtSettingDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CExtSettingDialog �_�C�A���O

class CExtSettingDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CExtSettingDialog( BOOL blEnableExt, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
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


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CExtSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	BOOL	m_blEnableExt;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CExtSettingDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSettingCheck();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_EXTSETTINGDIALOG_H__FFA6D402_4AC9_11D2_B9C8_004005469C82__INCLUDED_)
