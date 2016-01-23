#if !defined(AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_)
#define AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LinkEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CLinkEdit �_�C�A���O

class CLinkEdit : public CDialog
{
// �R���X�g���N�V����
public:
	CLinkEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CLinkEdit)
	enum { IDD = IDD_LINK };
	CButton	m_cBtnTimerExec;
	CButton	m_cBtnParametersSel;
	CEdit	m_cEdtParameters;
	CButton	m_cBtnFileSel;
	CEdit	m_cEdtFile;
	CString	m_cStrFile;
	BOOL	m_blLink;
	CString	m_cStrParameters;
	BOOL	m_blTimerExec;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CLinkEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	int DoFileSel( CString& cStrFile, LPCSTR lpcszFilter);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CLinkEdit)
	afx_msg void OnFilesel();
	afx_msg void OnParameterssel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLink();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_LINKEDIT_H__5B9B7385_043F_11D3_92F4_004005469C82__INCLUDED_)
