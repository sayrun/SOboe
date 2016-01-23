// AlphaBlendDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "memodata.h"
#include "AlphaBlendDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlphaBlendDialog �_�C�A���O


CAlphaBlendDialog::CAlphaBlendDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAlphaBlendDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlphaBlendDialog)
	m_nAlphaBlend = 0;
	//}}AFX_DATA_INIT
	if( NULL != pParent)
	{
		m_hWndParent = pParent->GetSafeHwnd();
	}
}


void CAlphaBlendDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlphaBlendDialog)
	DDX_Control(pDX, IDC_SLDALPHABLEND, m_cSldAlphaBlend);
	DDX_Slider(pDX, IDC_SLDALPHABLEND, m_nAlphaBlend);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlphaBlendDialog, CDialog)
	//{{AFX_MSG_MAP(CAlphaBlendDialog)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlphaBlendDialog ���b�Z�[�W �n���h��

BOOL CAlphaBlendDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	m_cSldAlphaBlend.SetRange( 26, 255, TRUE);
	CToolTipCtrl* pcToolTip = m_cSldAlphaBlend.GetToolTips();
	if( pcToolTip)
	{
		pcToolTip->AddTool( this, LPSTR_TEXTCALLBACK);
	}

	CString cStr;
	cStr.Format( IDS_ALPHABLENDCAPTION, 100 - ( ( ( m_nAlphaBlend) * 100) / 255));
	SetWindowText( cStr);

	m_cSldAlphaBlend.SetPos( m_nAlphaBlend);
	
	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

BOOL CAlphaBlendDialog::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
			TCHAR szWork[ 16];
			wsprintf( szWork, TEXT( "[%d%%]"), 100 - ( ( ( m_cSldAlphaBlend.GetPos()) * 100) / 255));
            pTTT->lpszText = szWork;
            return(TRUE);
        }
    }
    return(FALSE);
}

void CAlphaBlendDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	if( SB_THUMBTRACK != nSBCode)
	{
		if( pScrollBar->GetSafeHwnd() == m_cSldAlphaBlend.GetSafeHwnd())
		{
			CString cStr;
			cStr.Format( IDS_ALPHABLENDCAPTION, 100 - ( ( ( m_cSldAlphaBlend.GetPos()) * 100) / 255));
			SetWindowText( cStr);
		}
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CAlphaBlendDialog::SetAlphaBlend( BYTE byAlphaBlend)
{
	m_nAlphaBlend = byAlphaBlend;
}

BYTE CAlphaBlendDialog::GetAlphaBlend( void) const
{
	// ����Ӑ}�Ɠ����p�[�Z���g�ɂȂ�悤�ɒ���
	int nPercent = 100 - ( ( m_nAlphaBlend) * 100) / 255;
	switch( nPercent)
	{
	case 75:
		return CMemoData::ALPHABLEND_075;
	case 50:
		return CMemoData::ALPHABLEND_050;
	case 25:
		return CMemoData::ALPHABLEND_025;
	default:
		return LOBYTE( LOWORD( max( 26, min( 255, m_nAlphaBlend))));
	}
}

HWND CAlphaBlendDialog::sm_hWndParent = NULL;
CSemaphore CAlphaBlendDialog::sm_cSmpDoABModal( 1, 1, "SOBOE|ALPHABLEND#SEMAPHORE");
int CAlphaBlendDialog::DoModal() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	int nResult = IDCANCEL;
	CSingleLock	cSlk( &sm_cSmpDoABModal);
	if( cSlk.Lock( 0))
	{
		sm_hWndParent = m_hWndParent;
		nResult = CDialog::DoModal();
		sm_hWndParent = NULL;
	}
	return nResult;
}

// �g�p���H
BOOL CAlphaBlendDialog::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoABModal);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

HWND CAlphaBlendDialog::GetUseWnd()
{
	return sm_hWndParent;
}
