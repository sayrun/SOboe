// MemoEditSheet.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "MemoEditSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoEditSheet

IMPLEMENT_DYNAMIC(CMemoEditSheet, CPropertySheet)

CSemaphore CMemoEditSheet::sm_cSmpDoEdit( 1, 1, "SOBOE|EDIT#SEMAPHORE");
HWND CMemoEditSheet::sm_hWndParent = NULL;

// �g�p���H
BOOL CMemoEditSheet::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoEdit);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

HWND CMemoEditSheet::GetUseWnd()
{
	return sm_hWndParent;
}

CMemoEditSheet::CMemoEditSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	if( pParentWnd)m_hWndParent = pParentWnd->GetSafeHwnd();

	AddPage( &m_cBasePage);
	AddPage( &m_cExtPage);
	AddPage( &m_cTimerPage);
}

CMemoEditSheet::CMemoEditSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	if( pParentWnd)m_hWndParent = pParentWnd->GetSafeHwnd();

	AddPage( &m_cBasePage);
	AddPage( &m_cExtPage);
	AddPage( &m_cTimerPage);
}

CMemoEditSheet::~CMemoEditSheet()
{
}


BEGIN_MESSAGE_MAP(CMemoEditSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMemoEditSheet)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoEditSheet ���b�Z�[�W �n���h��

void CMemoEditSheet::SetMemoData( const CMemoData& cMemoData)
{
	// ��{�ݒ�(CBaseEditPage)���ڂ̎擾
	{
		// �\�����e
		m_cBasePage.m_cStrMemo = cMemoData.GetMemo();
		m_cBasePage.m_cStrMemo4IF = m_cBasePage.m_cStrMemo;
		// �^�C�g��
		if( FALSE == cMemoData.GetTitle( m_cBasePage.m_cStrTitle))
		{
			// �[���I�ȃ^�C�g����Ԃ��ꂽ�B����͈���Ȃ�
			m_cBasePage.m_cStrTitle.Empty();
		}
		// �t�H���g
		m_cBasePage.m_blLogFont = cMemoData.GetFont( &m_cBasePage.m_stLogFont);
		// �����F
		m_cBasePage.m_blForeColor = cMemoData.GetForeColor( m_cBasePage.m_clrFore);
		// �w�i�F
		m_cBasePage.m_blBackColor = cMemoData.GetBackColor( m_cBasePage.m_clrBack);
		// �A�C�R�����
		m_cBasePage.m_nIconIndex = cMemoData.GetIconIndex();

		m_cBasePage.m_unDxlID = cMemoData.GetDxlID();
		if( m_cBasePage.m_unDxlID)
		{
			m_cBasePage.m_unDxlExtDataSize = cMemoData.GetDxlExtData( NULL, 0);
			if( m_cBasePage.m_pabyDxlExtData)
			{
				delete [] m_cBasePage.m_pabyDxlExtData;
				m_cBasePage.m_pabyDxlExtData = NULL;
			}
			if( m_cBasePage.m_unDxlExtDataSize)
			{
				m_cBasePage.m_pabyDxlExtData = new BYTE [ m_cBasePage.m_unDxlExtDataSize];
				cMemoData.GetDxlExtData( m_cBasePage.m_pabyDxlExtData, m_cBasePage.m_unDxlExtDataSize);
			}
		}
	}
	// �g���ݒ�(CExtendEditPage)���ڂ̎擾
	{
		m_cExtPage.m_blLink = cMemoData.IsLink();
		m_cExtPage.m_blTimerExec = cMemoData.IsTimerExec();
		cMemoData.GetLinkData( m_cExtPage.m_cStrFile, m_cExtPage.m_cStrParameters);

		// �p�X���[�h
		m_cExtPage.m_blPassWordEnable = cMemoData.IsPassWordLock();
		m_cExtPage.m_cStrPassWord1st = cMemoData.GetPassWord();
		m_cExtPage.m_cStrPassWord2nd = m_cExtPage.m_cStrPassWord1st;

		// �`��g��
		m_cExtPage.m_unDxlID = cMemoData.GetDxlID();
		ForwardDxlID( m_cExtPage.m_unDxlID);

		// �y�I�[�_
		m_cExtPage.m_nZOrder = cMemoData.GetZOrder();
	}
	// �^�C�}�ݒ�(CTimerEditPage)���ڂ̎擾
	{
		m_cTimerPage.m_blTimerEnable = cMemoData.IsTimer();

		m_cTimerPage.m_nMonth	= cMemoData.GetTimerMonth();
		m_cTimerPage.m_nDay		= cMemoData.GetTimerDay();
		m_cTimerPage.m_nHour	= cMemoData.GetTimerHour();
		m_cTimerPage.m_nMin		= cMemoData.GetTimerMin();
		m_cTimerPage.m_nWeek	= cMemoData.GetWeek();

		m_cTimerPage.m_nTimerType	= cMemoData.GetTimerType();
		m_cTimerPage.m_nNotifyShift	= cMemoData.GetNotifyShift();

		m_cTimerPage.m_nBeforeNotify	= cMemoData.GetBeforeNotify();
		m_cTimerPage.m_blAfterNotify	= cMemoData.IsAfterNotify();

		cMemoData.GetNotifySound( m_cTimerPage.m_cStrNotifySound);
	}
}

BOOL CMemoEditSheet::GetMemoData( CMemoData& cMemoData)
{
	// ��{�ݒ�(CBaseEditPage)���ڂ̐ݒ�
	{
		// �\�����e
		cMemoData.SetMemo( m_cBasePage.m_cStrMemo4IF);
		// �^�C�g��
		cMemoData.SetTitle( m_cBasePage.m_cStrTitle);
		/*
		{
			// ���̕����̃f�b�g�R�[�h�́A�ݒ肳��Ȃ������ꍇ�ɉi���Ƀf�t�H���g��ʂ����ď����ɂ��Ă��������̏C��
			// ��a��������Ƃ̂��ƂŌ�����c�c�U���l��
				// �t�H���g�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
				cMemoData.SetFont( m_cBasePage.m_blLogFont ? &m_cBasePage.m_stLogFont : NULL);
				// �����F�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
				cMemoData.SetForeColor( m_cBasePage.m_blForeColor ? m_cBasePage.m_clrFore : CLR_INVALID);
				// �w�i�F�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
				cMemoData.SetBackColor( m_cBasePage.m_blBackColor ? m_cBasePage.m_clrBack : CLR_INVALID);
		}
		*/
		// �t�H���g�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
		cMemoData.SetFont( &m_cBasePage.m_stLogFont);
		// �����F�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
		cMemoData.SetForeColor( m_cBasePage.m_clrFore);
		// �w�i�F�F�ݒ肳��Ă��Ȃ��ꍇ�A�[���I�f�[�^��n��
		cMemoData.SetBackColor( m_cBasePage.m_clrBack);
		// �A�C�R�����
		cMemoData.SetIconIndex( m_cBasePage.m_nIconIndex);

		// �`��g��
		cMemoData.SetDxlExtData( m_cBasePage.m_pabyDxlExtData, m_cBasePage.m_unDxlExtDataSize);
		// �y�I�[�_
		cMemoData.SetZOrder( m_cExtPage.m_nZOrder);
	}
	// �g���ݒ�(CExtendEditPage)���ڂ̐ݒ�
	{
		cMemoData.Link( m_cExtPage.m_blLink);
		cMemoData.TimerExec( m_cExtPage.m_blTimerExec);
		cMemoData.SetLinkData( m_cExtPage.m_cStrFile, m_cExtPage.m_cStrParameters);

		// �p�X���[�h
		cMemoData.PassWordLock( m_cExtPage.m_blPassWordEnable);
		cMemoData.SetPassWord( m_cExtPage.m_cStrPassWord1st);

		// �`��g��
		cMemoData.SetDxlID( m_cExtPage.m_unDxlID);
	}
	// �^�C�}�ݒ�(CTimerEditPage)���ڂ̎擾
	{
		cMemoData.Timer( m_cTimerPage.m_blTimerEnable);

		cMemoData.SetTimerMonth( m_cTimerPage.m_nMonth);
		cMemoData.SetTimerDay( m_cTimerPage.m_nDay);
		cMemoData.SetTimerHour( m_cTimerPage.m_nHour);
		cMemoData.SetTimerMin( m_cTimerPage.m_nMin);
		cMemoData.SetWeek( m_cTimerPage.m_nWeek);

		cMemoData.SetTimerType( m_cTimerPage.m_nTimerType);
		cMemoData.SetNotifyShift( m_cTimerPage.m_nNotifyShift);

		cMemoData.SetBeforeNotify( m_cTimerPage.m_nBeforeNotify);
		cMemoData.AfterNotify( m_cTimerPage.m_blAfterNotify);

		cMemoData.SetNotifySound( m_cTimerPage.m_cStrNotifySound);
	}

	return TRUE;
}

int CMemoEditSheet::DoModal() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	int nResult = IDCANCEL;
	CSingleLock	cSlk( &sm_cSmpDoEdit);
	if( cSlk.Lock( 0))
	{
		sm_hWndParent = m_hWndParent;
		nResult = CPropertySheet::DoModal();
		sm_hWndParent = NULL;
	}
	return nResult;
}

BOOL CMemoEditSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	// TODO: ���̈ʒu�ɌŗL�̃R�[�h��ǉ����Ă�������
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe->IsAutoIMEOpen())
	{
		HIMC	hImc;
		hImc = ::ImmGetContext( GetSafeHwnd());
		if( hImc)
		{
			if( !::ImmGetOpenStatus( hImc))::ImmSetOpenStatus( hImc, TRUE);
			::ImmReleaseContext( GetSafeHwnd(), hImc);
		}
	}

	// �^�X�N�o�[�ւ̕\��
	m_hIcon = reinterpret_cast<HICON>( ::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0));
	SetIcon( m_hIcon, FALSE);

//	ShowWindow( SW_HIDE);
	ModifyStyleEx( 0, WS_EX_APPWINDOW | WS_EX_CONTEXTHELP, 0);
//	ShowWindow( SW_SHOW);


	// �ҏW��ʂ������I�ɑO�ɏo���B
	{
		int nTargetID, nActiveId;

		nTargetID = ::GetWindowThreadProcessId( GetParent()->GetSafeHwnd(), NULL );
		nActiveId = ::GetWindowThreadProcessId( ::GetForegroundWindow(), NULL );

		::AttachThreadInput( nTargetID, nActiveId, TRUE);
		::SetForegroundWindow( GetParent()->GetSafeHwnd());
		::AttachThreadInput( nTargetID, nActiveId, FALSE);

		::SetWindowPos( GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	
	return bResult;
}

BOOL CMemoEditSheet::DestroyWindow() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe->IsAutoIMEOpen())
	{
		HIMC	hImc;
		hImc = ::ImmGetContext( GetSafeHwnd());
		if( hImc)
		{
			COMPOSITIONFORM	stComp;
			if( ::ImmGetCompositionWindow( hImc, &stComp))
			{
				if( ::ImmGetOpenStatus( hImc))::ImmSetOpenStatus( hImc, FALSE);
				::ImmReleaseContext( GetSafeHwnd(), hImc);
			}
		}
	}

	if( NULL != m_hIcon)
	{
		DestroyIcon( m_hIcon);
	}
	
	return CPropertySheet::DestroyWindow();
}

BOOL CMemoEditSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_DXLSEL:
		if( 0 != m_cExtPage.m_unDxlID)
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			int nIndex = pcSOboe->FindDxlID( m_cExtPage.m_unDxlID);
			if( 0 <= nIndex)
			{
				CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
				if( pcDrawExLib)
				{
					pcDrawExLib->About( GetSafeHwnd());
				}
			}
			return TRUE;
		}
	// CBaseEditPage
	case IDC_TITLE:
	case IDC_ICONLIST:
	case IDC_MEMOEDIT:
	case IDC_FONT:
	case IDC_COLOR:
	case IDC_DXLEDITHELPER:
	case IDC_DELONQUIT:
	case IDC_CALCRESIZE:
	// CExtendEditPage
	case IDC_PASSWORDENABLE:
	case IDC_PASSWORD1ST:
	case IDC_PASSWORD2ND:
	case IDC_ZODER:
	case IDC_LINK:
	case IDC_TIMEREXEC:
	case IDC_FILE:
	case IDC_PARAMETERS:
	case IDC_FILESEL:
	case IDC_PARAMETERSSEL:
	//
	case IDC_TIMERENABLE:
	case IDC_HOUR:
	case IDC_SPNHOUR:
	case IDC_MIN:
	case IDC_SPNMIN:
	case IDC_MONTH:
	case IDC_SPNMONTH:
	case IDC_DAY:
	case IDC_SPNDAY:
	case IDC_WEEKLIST:
	case IDC_TIMERTYPE:
	case IDC_BEFORENOTIFY:
	case IDC_BEFORSPIN:
	case IDC_AFTERNOTIFY:
	case IDC_NOTIFYSHIFT:
	case IDC_WAVEFILE:
	case IDC_WAVSEL:
	case IDC_WAVEPLAY:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CPropertySheet::OnHelpInfo(pHelpInfo);
}
