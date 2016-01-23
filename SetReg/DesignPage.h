#if !defined(AFX_DESIGNPAGE_H__9E74C3E1_7FA7_11D2_91FE_004005469C82__INCLUDED_)
#define AFX_DESIGNPAGE_H__9E74C3E1_7FA7_11D2_91FE_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DesignPage.h : �w�b�_�[ �t�@�C��
//
class CProfileData;
class CIconSelectMenu;
/////////////////////////////////////////////////////////////////////////////
// CDesignPage �_�C�A���O

class CDesignPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CDesignPage)

// �R���X�g���N�V����
public:
	CDesignPage();
	~CDesignPage();

	BOOL m_blSpecSet;

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDesignPage)
	enum { IDD = IDD_DESIGN };
	CButton	m_cBtnIconSel;
	CEdit	m_cEdtGenerationCount;
	CSpinButtonCtrl	m_cSpnGenerationCount;
	CButton	m_cBtnSaveFileSel;
	CEdit	m_cEdtSaveFilePath;
	CEdit	m_cEdtSample;
	CComboBox	m_cCmbDesignList;
	CString	m_cStrSample;
	int		m_nDesignType;
	BOOL	m_blAutoIMEOpen;
	CString	m_cStrSaveFilePath;
	int		m_nGenerationCount;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDesignPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL
	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);

// �C���v�������e�[�V����
protected:
	LOGFONT		m_stLogFont;
	COLORREF	m_clrNormalFore;
	COLORREF	m_clrNormalBack;
	COLORREF	m_clrBeforeFore;
	COLORREF	m_clrBeforeBack;
	COLORREF	m_clrAfterFore;
	COLORREF	m_clrAfterBack;
	COLORREF	m_clrFore;
	COLORREF	m_clrBack;
	CBrush		m_cBrhBack;
	CFont*		m_pcFntEdit;

	int			m_nIconIndex;
	CBitmap		m_cBmpIcon;

	int			m_nDesignTypeOld;

	CIconSelectMenu* m_pcIcon;
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDesignPage)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchangeDesigntype();
	afx_msg void OnFont();
	afx_msg void OnColor();
	afx_msg void OnDatafilesel();
	afx_msg void OnIconsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CProfileData*	m_pcProfileDatas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DESIGNPAGE_H__9E74C3E1_7FA7_11D2_91FE_004005469C82__INCLUDED_)
