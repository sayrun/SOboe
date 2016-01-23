// SenderInfoDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "SenderInfoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSenderInfoDialog �_�C�A���O


CSenderInfoDialog::CSenderInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSenderInfoDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSenderInfoDialog)
	m_cStrEntryName = _T("");
	m_cStrReceiveTime = _T("");
	m_cStrNxlName = _T("");
	m_cStrExInfo = _T("");
	//}}AFX_DATA_INIT
}


void CSenderInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSenderInfoDialog)
	DDX_Text(pDX, IDC_ENTRYNAME, m_cStrEntryName);
	DDX_Text(pDX, IDC_RECEIVETIME, m_cStrReceiveTime);
	DDX_Text(pDX, IDC_USEDNXL, m_cStrNxlName);
	DDX_Text(pDX, IDC_ENTRYINFO, m_cStrExInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSenderInfoDialog, CDialog)
	//{{AFX_MSG_MAP(CSenderInfoDialog)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSenderInfoDialog ���b�Z�[�W �n���h��
