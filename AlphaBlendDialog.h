#if !defined(AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_)
#define AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlphaBlendDialog.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CAlphaBlendDialog �_�C�A���O

class CAlphaBlendDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CAlphaBlendDialog(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

	void SetAlphaBlend( BYTE byAlphaBlend);
	BYTE GetAlphaBlend( void) const;

	// �g�p���H
	static BOOL IsUsed();
	static HWND GetUseWnd();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CAlphaBlendDialog)
	enum { IDD = IDD_ALPHABLEND };
	CSliderCtrl	m_cSldAlphaBlend;
	int		m_nAlphaBlend;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CAlphaBlendDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	static CSemaphore sm_cSmpDoABModal;
	static HWND sm_hWndParent;
	HWND m_hWndParent;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CAlphaBlendDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_ALPHABLENDDIALOG_H__DA494A02_C603_11D3_9517_004005469C82__INCLUDED_)
