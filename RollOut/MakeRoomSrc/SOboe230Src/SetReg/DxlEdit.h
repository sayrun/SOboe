#if !defined(AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_)
#define AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DxlEdit.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDxlEdit �_�C�A���O

class CDxlEdit : public CDialog
{
// �R���X�g���N�V����
public:
	UINT m_unDxlID;
	CDxlEdit(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDxlEdit)
	enum { IDD = IDD_DRAWEXDLL };
	CEdit	m_cEdtMenuText;
	CButton	m_cBtnDllAbt;
	CButton	m_cBtnDllCnfg;
	CButton	m_cBtnOK;
	CString	m_cStrMenuText;
	CString	m_cStrDxlPath;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDxlEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	BOOL GetExtendDll( CString& cStr);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDxlEdit)
	afx_msg void OnDxlsel();
	virtual BOOL OnInitDialog();
	afx_msg void OnChipexdllabout();
	afx_msg void OnChipexdllconfig();
	virtual void OnOK();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DXLEDIT_H__E929AC01_CFAF_11D1_82B0_004005469C82__INCLUDED_)
