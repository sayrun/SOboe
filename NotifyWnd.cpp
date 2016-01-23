// NotifyWnd.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "NotifyWnd.h"
#include "MemoChip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotifyWnd

UINT CNotifyWnd::m_sunKillMsg = RegisterWindowMessage( "KILL#NOTIFY");


CNotifyWnd::CNotifyWnd()
{
	m_nCount = 40;
	m_hParentWnd = NULL;
	m_pcParentWnd = NULL;
}

CNotifyWnd::~CNotifyWnd()
{
}


BEGIN_MESSAGE_MAP(CNotifyWnd, CWnd)
	//{{AFX_MSG_MAP(CNotifyWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( CNotifyWnd::m_sunKillMsg, OnKillMsg)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNotifyWnd ���b�Z�[�W �n���h��

BOOL CNotifyWnd::Create( LPCTSTR lpszWindowName, CMemoChip* pParentWnd) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_pcParentWnd = pParentWnd;
	m_hParentWnd = pParentWnd->m_hWnd;
	CString	cStrClass = AfxRegisterWndClass( CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, LoadCursor( NULL, IDC_ARROW));
	return CWnd::CreateEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, cStrClass, lpszWindowName, WS_BORDER | WS_POPUP, 0, 0, 0, 0, NULL, NULL);
}

int CNotifyWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	CRect	cRect( 0, 0, lpCreateStruct->cx, lpCreateStruct->cy);
	CString	cStrTitle;
	GetWindowText( cStrTitle);
	CDC*	pcDC = GetDC();
	pcDC->DrawText( cStrTitle, cRect, DT_LEFT | DT_CALCRECT | DT_SINGLELINE);
	ReleaseDC( pcDC);
	CalcWindowRect( cRect);
	MoveWindow( cRect);

	SetTimer( 5500, 500, NULL);
	
	return 0;
}

void CNotifyWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	m_nCount--;
	if( 0 >= m_nCount)
	{
		KillTimer( 5500);
		PostMessage( WM_CLOSE, 0, 0L);
	}
	else
	{
		CRect	cRect;
		GetClientRect( cRect);
		CDC*	pcDc = GetDC();
		pcDc->PatBlt( cRect.left, cRect.top, cRect.Width(), cRect.Height(), DSTINVERT);
		ReleaseDC( pcDc);
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CNotifyWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	KillTimer( 5500);

	if( ::IsWindow( m_hParentWnd))
	{
		m_pcParentWnd->RestoreOnceHide();
		m_pcParentWnd->BringWindowToTop();
		m_pcParentWnd->SetActiveWindow();
	}
	PostMessage( WM_CLOSE, 0, 0L);
	return;
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CNotifyWnd::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	delete this;
}

LONG CNotifyWnd::OnKillMsg( UINT, LONG )
{
	KillTimer( 5500);
	SendMessage( WM_CLOSE, 0, 0L);

	return 0;
}

void CNotifyWnd::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	int nSaveID = dc.SaveDC();
	if( nSaveID)
	{
		CRect	cRect;
		GetClientRect( cRect);
		CString	cStrTitle;
		GetWindowText( cStrTitle);
		CBrush cBrushBack( RGB( 255, 0, 0));
		dc.FillRect( cRect, &cBrushBack);
		dc.SetBkMode( TRANSPARENT);
		dc.SetTextColor( RGB( 0, 255, 255));
		dc.DrawText( cStrTitle, cRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_WORDBREAK);
		dc.RestoreDC( nSaveID);
	}
	
	// �`��p���b�Z�[�W�Ƃ��� CWnd::OnPaint() ���Ăяo���Ă͂����܂���
}

UINT CRenotifyWnd::sm_unMsgFromTray = RegisterWindowMessage( "SOBOE#MSG!FROM-TRAY");

CRenotifyWnd::CRenotifyWnd() : CNotifyWnd()
{
}

BOOL CRenotifyWnd::Create( LPCTSTR lpszNotifyMsg, CWnd* pcParent) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	m_hParentWnd = ( NULL != pcParent) ? pcParent->GetSafeHwnd() :  NULL;
	CString	cStrClass = AfxRegisterWndClass( CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW, LoadCursor( NULL, IDC_ARROW));
	return CWnd::CreateEx( WS_EX_TOOLWINDOW | WS_EX_TOPMOST, cStrClass, lpszNotifyMsg, WS_BORDER | WS_POPUP, 0, 0, 0, 0, NULL, NULL);
}

BEGIN_MESSAGE_MAP(CRenotifyWnd, CWnd)
	//{{AFX_MSG_MAP(CRenotifyWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRenotifyWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	KillTimer( 5500);

	if( ::IsWindow( m_hParentWnd))
	{
		::PostMessage( m_hParentWnd, sm_unMsgFromTray, 0, WM_LBUTTONDOWN);
	}
	PostMessage( WM_CLOSE, 0, 0L);
	return;
}
