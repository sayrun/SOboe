#if !defined(AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_)
#define AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintChipDialog.h : �w�b�_�[ �t�@�C��
//

class CMemoData;
/////////////////////////////////////////////////////////////////////////////
// CPrintChipDialog �_�C�A���O

class CPrintChipDialog : public CDialog
{
// �R���X�g���N�V����
public:
	CPrintChipDialog( HDC& hPrinterDC, RECT& stRctMargin, CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	~CPrintChipDialog( void);

	BOOL AddPrintChip( const CMemoData* pcMemoData);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPrintChipDialog)
	enum { IDD = IDD_PRINTCHIPDLG };
	CStatic	m_cStcTitle;
	CButton	m_cBtnCancel;
	CProgressCtrl	m_cPrgrsPrintChip;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPrintChipDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	CList <CMemoData*, CMemoData*&>	m_cLstMemoData;

	HANDLE		m_hCancelEvent;
	HDC			m_hPrinterDC;
	RECT		m_stRctMargin;

	static UINT	sm_unNotifyEnd;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPrintChipDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LONG OnNotifyEnd( UINT wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	enum
	{
		RESULT_SUCCESS,
		RESULT_ERROR,
		RESULT_ABORT
	};

	friend void PrintChipThread( void* lpvData);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

void PrintChipThread( void* lpvData);

#endif // !defined(AFX_PRINTCHIPDIALOG_H__84BBBA33_15CB_11D3_9322_004005469C82__INCLUDED_)
