#if !defined(AFX_SENDERINFODIALOG_H__B17DF383_72CF_11D2_91DD_004005469C82__INCLUDED_)
#define AFX_SENDERINFODIALOG_H__B17DF383_72CF_11D2_91DD_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SenderInfoDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSenderInfoDialog �_�C�A���O

class CSenderInfoDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CSenderInfoDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSenderInfoDialog)
	enum { IDD = IDD_SENDERINFO };
	CString	m_cStrEntryName;
	CString	m_cStrReceiveTime;
	CString	m_cStrNxlName;
	CString	m_cStrExInfo;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSenderInfoDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSenderInfoDialog)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SENDERINFODIALOG_H__B17DF383_72CF_11D2_91DD_004005469C82__INCLUDED_)
