#if !defined(AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_)
#define AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PassWordCheckDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CPassWordCheckDialog �_�C�A���O

class CPassWordCheckDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CPassWordCheckDialog( const CString& cMemoTitle, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPassWordCheckDialog)
	enum { IDD = IDD_PWCHECK };
	CString	m_cStrMemoTitle;
	CString	m_cStrPassWord;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CPassWordCheckDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPassWordCheckDialog)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PASSWORDCHECKDIALOG_H__471CB20A_2D92_11D2_90A8_00804C15184E__INCLUDED_)
