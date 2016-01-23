#if !defined(AFX_TIMEREDITPAGE_H__741DB80A_2743_11D2_90A4_00804C15184E__INCLUDED_)
#define AFX_TIMEREDITPAGE_H__741DB80A_2743_11D2_90A4_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TimerEditPage.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CTimerEditPage �_�C�A���O

class CTimerEditPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTimerEditPage)

// �R���X�g���N�V����
public:
	CTimerEditPage();
	~CTimerEditPage();

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CTimerEditPage)
	enum { IDD = IDD_TIMERPAGE };
	CButton	m_cBtnNotifySoundPlay;
	CButton	m_cBtnNotifySoundSel;
	CEdit	m_cEdtNotifySound;
	CButton	m_cBtnAfterNotify;
	CSpinButtonCtrl	m_cSpnBeforeNotify;
	CEdit	m_cEdtBeforeNotify;
	CSpinButtonCtrl	m_cSpnMonth;
	CSpinButtonCtrl	m_cSpnMin;
	CSpinButtonCtrl	m_cSpnHour;
	CSpinButtonCtrl	m_cSpnDay;
	CComboBox	m_cCmbNotifyShift;
	CComboBox	m_cCmbTimerType;
	CEdit	m_cEdtDay;
	CEdit	m_cEdtMonth;
	CEdit	m_cEdtMin;
	CEdit	m_cEdtHour;
	CListBox	m_cLstDayOfWeek;
	int		m_nMin;
	int		m_nHour;
	int		m_nMonth;
	int		m_nDay;
	BOOL	m_blTimerEnable;
	int		m_nTimerType;
	int		m_nNotifyShift;
	BOOL	m_blAfterNotify;
	int		m_nBeforeNotify;
	CString	m_cStrNotifySound;
	//}}AFX_DATA
	int		m_nWeek;


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CTimerEditPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	void ControlEnable( void);
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CTimerEditPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimerenable();
	afx_msg void OnSelchangeTimertype();
	afx_msg void OnWavsel();
	afx_msg void OnWaveplay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_TIMEREDITPAGE_H__741DB80A_2743_11D2_90A4_00804C15184E__INCLUDED_)
