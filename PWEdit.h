#if !defined(AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_)
#define AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PWEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPWEdit �_�C�A���O

#define	_MINLEN_PW	4	// �p�X���[�h�̍Œᒷ

class CPWEdit : public CDialog
{
// �R���X�g���N�V����
public:
	CPWEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPWEdit)
	enum { IDD = IDD_PASSWORD };
	CEdit	m_cEdtPassWord1st;
	CEdit	m_cEdtPassWord2nd;
	CString	m_cStrPassWord1st;
	CString	m_cStrPassWord2nd;
	BOOL	m_blPassWordEnable;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPWEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPWEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnPasswordenable();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PWEDIT_H__5B9B7387_043F_11D3_92F4_004005469C82__INCLUDED_)
