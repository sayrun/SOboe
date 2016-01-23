#if !defined(AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_)
#define AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AboutDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog �_�C�A���O

typedef	UINT ( WINAPI *GetFreeSystemResource)( UINT);

class CAboutDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CAboutDialog( int nWinType = TRUE/*TRUE=98*/, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	enum
	{
		WINTYPE_98,
		WINTYPE_NT
	};
	enum
	{
		IDD98 = IDD_ABOUTBOX98,
		IDDNT = IDD_ABOUTBOXNT
	};
	//{{AFX_DATA(CAboutDialog)
	CProgressCtrl	m_cPgbUSER;
	CProgressCtrl	m_cPgbSYSTEM;
	CProgressCtrl	m_cPgbGDI;
	CString	m_cStrGDI;
	CString	m_cStrSYSTEM;
	CString	m_cStrUSER;
	CProgressCtrl	m_cPgbPhysical;
	CProgressCtrl	m_cPgbPagefile;
	CString	m_cStrPhysical;
	CString	m_cStrPagefile;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CAboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	int	m_nWinType;
	GetFreeSystemResource		m_lpGetFreeSystemResource;
	HINSTANCE m_hInst;
	BOOL CheckResources( void);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CAboutDialog)
	afx_msg void OnGosuport();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ABOUTDIALOG_H__6135E898_36FA_11D2_90B3_00804C15184E__INCLUDED_)
