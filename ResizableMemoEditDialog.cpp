// ResizableMemoEditDialog.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "ResizableMemoEditDialog.h"
#include "LinkEdit.h"
#include "TimerEdit.h"
#include "PWEdit.h"
#include ".\resizablememoeditdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResizableMemoEditDialog �_�C�A���O
TBBUTTON CResizableMemoEditDialog::m_sastTButton[ EDITDLG_TOOLBAR] = 
#ifdef _WIN32
{
	{ 0, IDOK, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_OKBUTTON, 0},
	{ 1, IDCANCEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_CANCELBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 2, IDM_TEMPLET, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_TEMPLETBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 17, IDM_ICONSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_ICONSELBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 4, IDM_FONTSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_FONTBUTTON, 0},
	{ 5, IDM_FORECOLOR, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_FONTCOLORBUTTON, 0},
	{ 6, IDM_BACKCOLOR, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_COLORBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 7, IDM_DELONQUIT, TBSTATE_ENABLED, TBSTYLE_CHECK, { 0, 0}, IDS_DELONQUITBUTTON, 0},
	{ 8, IDM_CALCRESIZE, TBSTATE_ENABLED, TBSTYLE_CHECK, { 0, 0}, IDS_CALCRESIZE, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 9, IDM_LINK, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_LINKBUTTON, 0},
	{ 10, IDM_TIMER, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_TIMERBUTTON, 0},
	{ 11, IDM_ZORDER, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_ZORDERBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 12, IDM_DXLSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_DXLSELBUTTON, 0},
	{ 13, IDM_DXLEDITHELPER, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_DXLEDITHELPERBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 14, IDM_PWSET, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_PWBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, { 0, 0}, 0, 0},
	{ 15, IDM_INSERTDATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_INSERTDATEBUTTON, 0},
	{ 16, IDM_INSERTTIME, TBSTATE_ENABLED, TBSTYLE_BUTTON, { 0, 0}, IDS_INSERTTIMEBUTTON, 0},
};
#else
{
	{ 0, IDOK, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_OKBUTTON, 0},
	{ 1, IDCANCEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_CANCELBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 2, IDM_TEMPLET, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_TEMPLETBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 17, IDM_ICONSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_ICONSELBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 4, IDM_FONTSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_FONTBUTTON, 0},
	{ 5, IDM_FORECOLOR, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_FONTCOLORBUTTON, 0},
	{ 6, IDM_BACKCOLOR, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_COLORBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 7, IDM_DELONQUIT, TBSTATE_ENABLED, TBSTYLE_CHECK, IDS_DELONQUITBUTTON, 0},
	{ 8, IDM_CALCRESIZE, TBSTATE_ENABLED, TBSTYLE_CHECK, IDS_CALCRESIZE, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 8, IDM_LINK, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_LINKBUTTON, 0},
	{ 10, IDM_TIMER, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_TIMERBUTTON, 0},
	{ 11, IDM_ZORDER, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_ZODERBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 12, IDM_DXLSEL, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_DXLSELBUTTON, 0},
	{ 13, IDM_DXLEDITHELPER, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_DXLEDITHELPERBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 14, IDM_PWSET, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_PWBUTTON, 0},
		{ 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0},
	{ 15, IDM_INSERTDATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_INSERTDATEBUTTON, 0},
	{ 16, IDM_INSERTTIME, TBSTATE_ENABLED, TBSTYLE_BUTTON, IDS_INSERTTIMEBUTTON, 0},
};
#endif


CResizableMemoEditDialog::CResizableMemoEditDialog( const CString cStrCaption, CWnd* pParent, const CMemoData* pcMemoData) : CDialog(CResizableMemoEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResizableMemoEditDialog)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	m_blResize = pcSOboe->IsAutoResize();
	m_blDelOnQuit = FALSE;

	m_cStrCaption = cStrCaption;

	m_hIcon = NULL;

	if( pParent)m_hWndParent = pParent->GetSafeHwnd();

	if( pcMemoData)
	{
		m_cMemoData = ( *pcMemoData);
	}
	else
	{
		m_cMemoData.SetDefaultStyle();
	}
	m_pcIcon = NULL;
}

CResizableMemoEditDialog::~CResizableMemoEditDialog( void)
{
	m_cExtIcons.DeleteObject();
}

CSemaphore CResizableMemoEditDialog::sm_cSmpDoEdit( 1, 1, "SOBOE|EDIT#SEMAPHORE");
HWND CResizableMemoEditDialog::sm_hWndParent = NULL;

// �g�p���H
BOOL CResizableMemoEditDialog::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoEdit);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

HWND CResizableMemoEditDialog::GetUseWnd()
{
	return sm_hWndParent;
}

void CResizableMemoEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResizableMemoEditDialog)
	DDX_Control(pDX, IDC_TITLETITLE, m_cTitleTitle);
	DDX_Control(pDX, IDC_TITLE, m_cTitleEdit);
	DDX_Control(pDX, IDC_MEMOEDIT, m_cMainEdit);
	//}}AFX_DATA_MAP
}

void CResizableMemoEditDialog::SetMemoData( const CMemoData& cMemoData)
{
	m_cMemoData = cMemoData;
}

BOOL CResizableMemoEditDialog::GetMemoData( CMemoData& cMemoData)
{
	cMemoData = m_cMemoData;
	return TRUE;
}

BEGIN_MESSAGE_MAP(CResizableMemoEditDialog, CDialog)
	//{{AFX_MSG_MAP(CResizableMemoEditDialog)
	ON_COMMAND(IDOK, OnOk)
	ON_WM_NCDESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_SETFOCUS()
	ON_COMMAND(IDM_FONTSEL, OnFont)
	ON_COMMAND(IDM_FORECOLOR, OnFontColor)
	ON_COMMAND(IDM_BACKCOLOR, OnColor)
	ON_COMMAND(IDM_TEMPLET, OnTemplet)
	ON_COMMAND(IDM_ZORDER, OnZOrder)
	ON_COMMAND(IDM_DXLSEL, OnDxlsel)
	ON_COMMAND(IDM_DXLEDITHELPER, OnDxledithelper)
	ON_COMMAND(IDM_CALCRESIZE, OnCalcresize)
	ON_COMMAND(IDM_DELONQUIT, OnDelonquit)
	ON_COMMAND(IDM_LINK, OnLink)
	ON_COMMAND(IDM_TIMER, OnTimer)
	ON_COMMAND(IDM_PWSET, OnPwset)
	ON_COMMAND(IDM_INSERTDATE, OnInsertdate)
	ON_COMMAND(IDM_INSERTTIME, OnInserttime)
	ON_WM_HELPINFO()
	ON_COMMAND(IDM_ICONSEL, OnIconsel)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify)
	ON_MESSAGE( WM_USER + 100, OnSetFocusDelay)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_SETDEFAULT, OnSetdefault)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResizableMemoEditDialog ���b�Z�[�W �n���h��

// WM_INITDIALOG�ł��܂��t�H�[�J�X��ݒ�ł��Ȃ������̂ŁA
// �f���C�ŏ������邱�ƂɁc�c
// �{����RegisterWindowMessage���g�������Ƃ��l�������ǁA
// �Ȃ񂩁A���ʂ��ۂ��̂Ń��������B
LRESULT CResizableMemoEditDialog::OnSetFocusDelay( WPARAM wParam, LPARAM lParam)
{
	if( ( HWND)wParam == m_cMainEdit.GetSafeHwnd())
	{
		// ime on
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		if( pcSOboe->IsAutoIMEOpen())
		{
			HIMC hImc = ::ImmGetContext( GetSafeHwnd());
			if( hImc)
			{
				if( !::ImmGetOpenStatus( hImc))::ImmSetOpenStatus( hImc, TRUE);
				::ImmReleaseContext( GetSafeHwnd(), hImc);
			}
		}

		m_cMainEdit.SetFocus();
		return 0;
	}
	else
	{
		return DefWindowProc( WM_USER + 100, wParam, lParam);
	}
}

BOOL CResizableMemoEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( NULL != m_hIcon)
	{
		CDialog::SetIcon( m_hIcon, FALSE);
	}

	// TODO: ���̈ʒu�ɏ������̕⑫������ǉ����Ă�������
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	// �ҏW�G���A��
	{
		if( !pcSOboe->IsAutoScroll())
		{
			DWORD dwStyle = GetWindowLong( m_cMainEdit.GetSafeHwnd(), GWL_STYLE);
			DWORD dwStyleEx = GetWindowLong( m_cMainEdit.GetSafeHwnd(), GWL_EXSTYLE);
			CRect	cRect;
			m_cMainEdit.GetWindowRect( cRect);
			char szClass[ 512];
			GetClassName( m_cMainEdit.GetSafeHwnd(), szClass, 512);
			ScreenToClient( cRect);
			m_cMainEdit.DestroyWindow();
			dwStyle ^= ( ES_AUTOHSCROLL | WS_HSCROLL);
			m_cMainEdit.CreateEx( dwStyleEx, szClass, NULL, dwStyle, cRect.left, cRect.top, cRect.Width(), cRect.Height(), GetSafeHwnd(), ( HMENU)IDC_MEMOEDIT, NULL);
			m_cMainEdit.SetFocus();
		}

		m_cMainEdit.SetWindowText( m_cMemoData.GetMemo());

		// �F�֌W�̏���
		m_cMemoData.GetBackColor( m_clrBack);
		m_cMemoData.GetForeColor( m_clrFore);
		CDC*	pcDc;
		pcDc = GetDC();
		m_cBrush.CreateSolidBrush( pcDc->GetNearestColor( m_clrBack));
		ReleaseDC( pcDc);

		// �t�H���g���g�p���鎞�̐ݒ�
		LOGFONT stLogFont;
		m_cMemoData.GetFont( &stLogFont);
		m_cFntEdit.DeleteObject();
		m_cFntEdit.CreateFontIndirect( &stLogFont);
		m_cMainEdit.SetFont( &m_cFntEdit);
		m_cMainEdit.SetMargins( 0, 0);
	}

	// �c�[���o�[�̏����惓
	{
		if( !m_cToolBar.Create( WS_CHILD | WS_VISIBLE  | TBSTYLE_TOOLTIPS | TBSTYLE_WRAPABLE, CRect( 0, 0, 0, 0), this, IDC_EXEDITTOOLBAR))
		{
			return -1;
		}
		m_cToolBar.SetBitmapSize( CSize( 16, 16));
		m_cToolBar.AddBitmap( 32, IDB_TOOLBAR);

		// �g���A�C�R���̒ǉ�
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
		if( pcIconImgs)
		{
			if( pcIconImgs->GetEnableCount())
			{
				int nCount = pcIconImgs->GetExtendCount();
				int nExtStart = pcIconImgs->GetExtendStartIndex();

				m_cExtIcons.DeleteObject();
				m_cExtIcons.CreateBitmap( _ICON_WIDTH * nCount, _ICON_HEIGHT, 1, 1, NULL);
				CDC cMemDC;
				cMemDC.CreateCompatibleDC( NULL);
				CBitmap* pOld = cMemDC.SelectObject( &m_cExtIcons);
				int nIconIndex = 0;
				for( int nIndex = 0; nIndex < nCount; nIndex++)
				{
					if( pcIconImgs->IsEnable( nIndex + nExtStart))
					{
						pcIconImgs->Draw( nIndex + nExtStart, cMemDC, CRect( _ICON_WIDTH * nIndex, 0, _ICON_WIDTH * ( nIndex + 1), _ICON_HEIGHT));
					}
				}
				cMemDC.SelectObject( pOld);
				m_nExtIconTop = m_cToolBar.AddBitmap( nCount, &m_cExtIcons);
				TRACE( TEXT( "m_nExtIconTop:%d\n"), m_nExtIconTop);
			}
		}


		if( pcSOboe->IsBigButton())
		{
			CString	cStrWork;
			for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
			{
				if( TBSTYLE_SEP != m_sastTButton[ nIndex].fsStyle)
				{
					cStrWork.LoadString( m_sastTButton[ nIndex].dwData);
					m_sastTButton[ nIndex].iString = ::SendMessage( m_cToolBar.GetSafeHwnd(), TB_ADDSTRING, 0, ( LPARAM)( LPCSTR)cStrWork);
				}
				if( IDM_ICONSEL == m_sastTButton[ nIndex].idCommand)
				{
					m_sastTButton[ nIndex].iBitmap = 17 + m_cMemoData.GetIconIndex();
				}
			}
		}
		else
		{
			for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
			{
				if( IDM_ICONSEL == m_sastTButton[ nIndex].idCommand)
				{
					int nIconIndex = 17 + m_cMemoData.GetIconIndex();

					const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
					if( pcIconImgs)
					{
						if( m_cMemoData.GetIconIndex() >= pcIconImgs->GetDefaultCount())
						{
							if( pcIconImgs->IsEnable( m_cMemoData.GetIconIndex()))
							{
								nIconIndex = m_nExtIconTop + ( m_cMemoData.GetIconIndex() - pcIconImgs->GetExtendStartIndex());
							}
							else
							{
								nIconIndex = 17 + pcIconImgs->GetDefaultCount();
							}
						}
					}

					m_sastTButton[ nIndex].iBitmap = nIconIndex;
					break;
				}
			}
		}

		m_cToolBar.AddButtons( EDITDLG_TOOLBAR, m_sastTButton);

		m_cToolBar.CheckButton( IDM_CALCRESIZE, m_blResize);
		m_cToolBar.EnableButton( IDM_CALCRESIZE, !m_blResizeLock);

		m_cToolBar.CheckButton( IDM_DELONQUIT, m_blDelOnQuit);

		m_cToolBar.EnableButton( IDM_TEMPLET, ( 0 < pcSOboe->GetTempletCount()));
		m_cToolBar.EnableButton( IDM_DXLSEL, ( 0 < pcSOboe->GetDxlCount()));


		BOOL	blEnable = FALSE;
		
		if( 0 != m_cMemoData.GetDxlID())
		{
			int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
			if( 0 <= nIndex)
			{
				const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
				if( pcDrawExLib)
				{
					if( pcDrawExLib->IsSupportEditHelper())
					{
						blEnable = TRUE;
					}
				}
			}
		}
		m_cToolBar.EnableButton( IDM_DXLEDITHELPER, blEnable);
		// �c�[���o�[�̃T�C�Y�v�Z�I
		m_cToolBar.AutoSize();
	}

	{
		CString	cStrTitle;
		if( FALSE != m_cMemoData.GetTitle( cStrTitle))
		{
			m_cTitleEdit.SetWindowText( cStrTitle);
		}
		SetWindowText( m_cStrCaption);
	}

	{
		MoveWindow( pcSOboe->GetEditWindowRect());

		// �E�B���h�E�ɍ��킹�ăR���g���[�����`
		Formating();
	}

	// �J�[�\�����˕ҏW�G���A�ɓ����񂾂�
	m_cMainEdit.SetSel( m_cMainEdit.GetWindowTextLength(), m_cMainEdit.GetWindowTextLength());
	m_cMainEdit.LineScroll( m_cMainEdit.GetLineCount());

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

	// �t�H�[�J�X�̐ݒ肾��
	PostMessage( WM_USER + 100, ( WPARAM)m_cMainEdit.GetSafeHwnd(), 0L);
	if( !pcSOboe->IsAutoScroll())
	{
		return FALSE;
	}

	return TRUE;  // �R���g���[���Ƀt�H�[�J�X��ݒ肵�Ȃ��Ƃ��A�߂�l�� TRUE �ƂȂ�܂�
	              // ��O: OCX �v���p�e�B �y�[�W�̖߂�l�� FALSE �ƂȂ�܂�
}

void CResizableMemoEditDialog::Formating( void)
{
	CRect	cRect;
	CRect	cRectTitleTitle;
	CRect	cRectTitle;
	CRect	cRectClient;
	GetClientRect( cRectClient);

	m_cToolBar.AutoSize();
	m_cToolBar.GetWindowRect( cRect);
	m_cTitleEdit.GetWindowRect( cRectTitle);
	m_cTitleTitle.GetWindowRect( cRectTitleTitle);
	ScreenToClient( cRectTitle);
	ScreenToClient( cRectTitleTitle);
	m_cTitleTitle.MoveWindow( 0, cRect.Height() + ( ( cRectTitle.Height() - cRectTitleTitle.Height()) / 2), cRectTitleTitle.Width(), cRectTitleTitle.Height());
	m_cTitleEdit.MoveWindow( cRectTitleTitle.right, cRect.Height(), cRectClient.Width() - cRectTitleTitle.right, cRectTitle.Height());
	m_cMainEdit.MoveWindow( 0, cRect.Height() + cRectTitle.Height(), cRectClient.Width(), cRectClient.Height() - ( cRect.Height() + cRectTitle.Height()));
}

void CResizableMemoEditDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	if( IsWindow( m_cToolBar.GetSafeHwnd()) && IsWindow( m_cMainEdit.GetSafeHwnd()))
	{
		// �E�B���h�E�ɍ��킹�ăR���g���[�����`
		Formating();
	}
}

void CResizableMemoEditDialog::ByeBye( void)
{
	CRect	cRect;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	GetWindowRect( cRect);
	pcSOboe->SetEditWindowRect( cRect);

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
}

void CResizableMemoEditDialog::OnCancel() 
{
	// TODO: ���̈ʒu�ɓ��ʂȌ㏈����ǉ����Ă��������B
	ByeBye();

	CDialog::OnCancel();
}

void CResizableMemoEditDialog::OnOk() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CString	cStr;
	m_cMainEdit.GetWindowText( cStr);
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	BOOL blTrim = pcSOboe->IsAutoTriming();
	if( 0 != ( GetKeyState( VK_CONTROL) & ~1))blTrim = !blTrim;
	if( blTrim)
	{
		cStr.TrimLeft();
		cStr.TrimRight();
	}
	m_cMemoData.SetMemo( cStr);

	m_cTitleEdit.GetWindowText( cStr);
	m_cMemoData.SetTitle( cStr);

	m_blResize = m_cToolBar.IsButtonChecked( IDM_CALCRESIZE);
	m_blDelOnQuit = m_cToolBar.IsButtonChecked( IDM_DELONQUIT);

	ByeBye();

	CDialog::OnOK();
}

void CResizableMemoEditDialog::OnNcDestroy() 
{
	CDialog::OnNcDestroy();
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	m_cBrush.DeleteObject();
}

BOOL CResizableMemoEditDialog::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
    if( ! ( pTTT->uFlags & TTF_IDISHWND))
    {
		switch( nID)
		{
		case IDOK:
		case IDCANCEL:
		case IDM_TEMPLET:
		case IDM_ICONSEL:
		case IDM_FONTSEL:
		case IDM_FORECOLOR:
		case IDM_BACKCOLOR:
		case IDM_DELONQUIT:
		case IDM_CALCRESIZE:
		case IDM_LINK:
		case IDM_TIMER:
		case IDM_DXLEDITHELPER:
		case IDM_PWSET:
		case IDM_INSERTDATE:
		case IDM_INSERTTIME:
			if( !pcSOboe->IsBigButton())
			{
				for( int nIndex = 0; nIndex < 321; nIndex++)
				{
					if( ( UINT)m_sastTButton[ nIndex].idCommand == nID)
					{
						pTTT->lpszText = MAKEINTRESOURCE( m_sastTButton[ nIndex].dwData);
						pTTT->hinst = AfxGetResourceHandle();
				        return TRUE;
					}
				}
			}
			break;

		case IDM_ZORDER:
			{
				CString	cStr;

				UINT unID;
				switch( m_cMemoData.GetZOrder())
				{
				case CMemoData::ZORDER_NORMAL:
					unID = IDS_NORMAL_ZORDER;
					break;
				case CMemoData::ZORDER_TOPMOST:
					unID = IDS_TOP_ZORDER;
					break;
				case CMemoData::ZORDER_BOTTOMMOST:
					unID = IDS_BOTTOM_ZORDER;
					break;
				default:
					return TRUE;
				}

				cStr.LoadString( unID);
				if( !pcSOboe->IsBigButton())
				{
					CString	cStrMenu;
					cStrMenu.LoadString( IDS_ZORDERBUTTON);
					lstrcpy( pTTT->szText, cStrMenu);
					lstrcat( pTTT->szText, TEXT( " - "));
					lstrcat( pTTT->szText, cStr);
				}
				else
				{
					lstrcpy( pTTT->szText, cStr);
				}
				pTTT->hinst = NULL;

			}
			return TRUE;

		case IDM_DXLSEL:
			{
				CString	cStr;

				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);
				int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
				if( 0 <= nIndex)
				{
					const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
					if( NULL != pcDrawExLib)
					{
						cStr = pcDrawExLib->GetMenuString();
					}
				}
				if( cStr.IsEmpty())
				{
					cStr = _T( "���I��");
				}

				if( !pcSOboe->IsBigButton())
				{
					CString	cStrMenu;
					cStrMenu.LoadString( IDS_DXLSELBUTTON);

					lstrcpy( pTTT->szText, cStrMenu);
					lstrcat( pTTT->szText, TEXT( " - "));
					lstrcat( pTTT->szText, cStr);
				}
				else
				{
					lstrcpy( pTTT->szText, cStr);
				}
				pTTT->hinst = NULL;
			}
			return TRUE;

		default:
			return FALSE;
		}
    }
	return FALSE;
}

HBRUSH CResizableMemoEditDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
	HBRUSH hbr;
	if( ( CTLCOLOR_EDIT == nCtlColor || CTLCOLOR_MSGBOX == nCtlColor) && ( pWnd->GetSafeHwnd() == m_cMainEdit.GetSafeHwnd()))
	{
		hbr = m_cBrush;
		pDC->SetBkColor( m_clrBack);
		pDC->SetTextColor( m_clrFore);
	}
	else
	{
		// TODO: ���̈ʒu�� DC �̃A�g���r���[�g��ύX���Ă�������
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	// TODO: �f�t�H���g�̃u���V���]�݂̂��̂łȂ��ꍇ�ɂ́A�Ⴄ�u���V��Ԃ��Ă�������
	return hbr;
}

void CResizableMemoEditDialog::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	m_cMainEdit.SetFocus();
}

void CResizableMemoEditDialog::OnFont()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	LOGFONT stLogFont;
	m_cMemoData.GetFont( &stLogFont);
	CFontDialog	cFontDlg( &stLogFont, CF_SCREENFONTS, NULL, this);

	if( IDOK == cFontDlg.DoModal())
	{
		// �ҏW�̈�փt�H���g��ݒ�
		m_cFntEdit.DeleteObject();
		m_cFntEdit.CreateFontIndirect( &stLogFont);
		m_cMainEdit.SetFont( &m_cFntEdit);
		m_cMainEdit.SetMargins( 0, 0);

		m_cMemoData.SetFont( &stLogFont);
	}
}

void CResizableMemoEditDialog::OnFontColor()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog	cColorDlg( m_clrFore, 0, this);
	
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->GetUserColor( cColorDlg.m_cc.lpCustColors);
	if( IDOK == cColorDlg.DoModal())
	{
		m_clrFore = cColorDlg.GetColor();

		m_cMemoData.SetForeColor( m_clrFore);

		pcSOboe->SetUserColor( cColorDlg.GetSavedCustomColors());
		m_cMainEdit.InvalidateRect( NULL);
	}
}

void CResizableMemoEditDialog::OnTemplet()
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	CMenu cTempletMemu;
	if( cTempletMemu.CreatePopupMenu())
	{
		CString		cStr;
		CString		cStrMenu;

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		
		int nCount = pcSOboe->GetTempletCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			if( pcSOboe->GetTempletName( nIndex, cStr))
			{
				cStrMenu.Format( "(&%d) %s", nIndex, cStr);
				cTempletMemu.AppendMenu( MF_BYCOMMAND, IDM_TEMPLET0MENU + nIndex, cStrMenu);
			}
		}
		for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
		{
			if( IDM_TEMPLET == m_sastTButton[ nIndex].idCommand)
			{
				CRect	cRect;
				m_cToolBar.GetItemRect( nIndex, cRect);
				ClientToScreen( cRect);

				UINT	unMenuID = cTempletMemu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);
				if( IDM_TEMPLET0MENU <= unMenuID && IDM_TEMPLET9MENU >= unMenuID)
				{
					UseTemplet( ( int)( unMenuID - IDM_TEMPLET0MENU));
				}
				return;
			}
		}
	}
}

BOOL CResizableMemoEditDialog::UseTemplet( int nTempletIndex)
{
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( 0 > nTempletIndex && pcSOboe->GetTempletCount() <= nTempletIndex)return FALSE;

	CString	cStrTempletMsg;
	cStrTempletMsg.LoadString( IDS_TEMPLETMSG);
	if( IDOK != MessageBox( cStrTempletMsg, NULL, MB_OKCANCEL | MB_ICONQUESTION))return FALSE;

	const CMemoData* pcMemoData = pcSOboe->GetTemplet( nTempletIndex);
	ASSERT( pcMemoData);
	if( pcMemoData)
	{
		SetMemoData( *pcMemoData);
		{
			m_cMainEdit.SetWindowText( m_cMemoData.GetMemo());
			m_cMemoData.GetBackColor( m_clrBack);
			m_cMemoData.GetForeColor( m_clrFore);
			// �ҏW�G���A�p�̃u���V�쐬
			m_cBrush.DeleteObject();
			CDC*	pcDc;
			pcDc = GetDC();
			m_cBrush.CreateSolidBrush( pcDc->GetNearestColor( m_clrBack));
			ReleaseDC( pcDc);
			
			// �f�t�H���g�̃t�H���g�̐ݒ�
			LOGFONT stLogFont;
			m_cMemoData.GetFont( &stLogFont);
			m_cFntEdit.DeleteObject();
			m_cFntEdit.CreateFontIndirect( &stLogFont);
			m_cMainEdit.SetFont( &m_cFntEdit);

			m_cToolBar.EnableButton( IDM_CALCRESIZE, !m_blResizeLock);

			BOOL blEnable = TRUE;
			if( 0 != m_cMemoData.GetDxlID())
			{
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
				if( 0 <= nIndex)
				{
					const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
					if( pcDrawExLib)
					{
						if( pcDrawExLib->IsSupportEditHelper())
						{
							blEnable = TRUE;
						}
					}
				}
			}
			m_cToolBar.EnableButton( IDM_DXLEDITHELPER, blEnable);

			// �A�C�R���̍X�V
			TBBUTTONINFO	stTbBInfo;
			stTbBInfo.cbSize = sizeof( TBBUTTONINFO);
			stTbBInfo.dwMask = TBIF_IMAGE;
			stTbBInfo.iImage = 17 + m_cMemoData.GetIconIndex();

			m_cToolBar.SetButtonInfo( IDM_ICONSEL, &stTbBInfo);
		}
		return TRUE;
	}
	return FALSE;
}

void CResizableMemoEditDialog::OnZOrder()
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����Ă�������
	CMenu	cEditMemu;
	cEditMemu.LoadMenu( IDR_EDITWINDOW);
	CMenu*	pcZOrder = cEditMemu.GetSubMenu( 1);
	if( pcZOrder)
	{
		UINT	uMenuCMD = 0;
		switch( m_cMemoData.GetZOrder())
		{
		case CMemoData::ZORDER_NORMAL:
			uMenuCMD = IDM_NORMAL_ZORDER;
			break;
		case CMemoData::ZORDER_TOPMOST:
			uMenuCMD = IDM_TOP_ZORDER;
			break;
		case CMemoData::ZORDER_BOTTOMMOST:
			uMenuCMD = IDM_BOTTOM_ZORDER;
			break;
		}
		pcZOrder->CheckMenuItem( uMenuCMD, MF_BYCOMMAND | MF_CHECKED);

		uMenuCMD = 0;
		for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
		{
			if( IDM_ZORDER == m_sastTButton[ nIndex].idCommand)
			{
				CRect	cRect;
				m_cToolBar.GetItemRect( nIndex, cRect);
				ClientToScreen( cRect);
				uMenuCMD = pcZOrder->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);
			}
		}
		if( uMenuCMD)
		{
			switch( uMenuCMD)
			{
			case IDM_NORMAL_ZORDER:
				m_cMemoData.SetZOrder();
				break;
			case IDM_TOP_ZORDER:
				m_cMemoData.SetZOrder( CMemoData::ZORDER_TOPMOST);
				break;
			case IDM_BOTTOM_ZORDER:
				m_cMemoData.SetZOrder( CMemoData::ZORDER_BOTTOMMOST);
				break;
			}
		}
	}
}

void CResizableMemoEditDialog::OnColor()
{
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	CColorDialog	cColorDlg( m_clrBack, 0, this);
	
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->GetUserColor( cColorDlg.m_cc.lpCustColors);
	if( IDOK == cColorDlg.DoModal())
	{
		m_clrBack = cColorDlg.GetColor();

		m_cMemoData.SetBackColor( m_clrBack);

		pcSOboe->SetUserColor( cColorDlg.GetSavedCustomColors());

		// �ҏW�̈�̃u���V�쐬
		m_cBrush.DeleteObject();
		CDC*	pcDC;
		pcDC = GetDC();
		m_cBrush.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
		ReleaseDC( pcDC);
		m_cMainEdit.InvalidateRect( NULL);
	}
}

BOOL CResizableMemoEditDialog::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	if( WM_KEYDOWN == pMsg->message)
	{
		if( m_cMainEdit == *GetFocus())
		{
			switch( pMsg->wParam)
			{
			case 'T':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_TIME);
					return TRUE;
				}
				break;
			case 'D':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_DATE);
					return TRUE;
				}
				break;
			case 'Q':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					PostMessage( WM_COMMAND, IDM_DXLEDITHELPER, 0L);
				}
				break;
			case 'W':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					InsertTime( _INSERT_DAYOFWEEK);
					return TRUE;
				}
				break;
			case 'S':
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					DWORD dwStyle = GetWindowLong( m_cMainEdit.GetSafeHwnd(), GWL_STYLE);
					DWORD dwStyleEx = GetWindowLong( m_cMainEdit.GetSafeHwnd(), GWL_EXSTYLE);
					CRect	cRect;
					CString	cStr;
					int	nStartSel, nEndSel;
					m_cMainEdit.GetSel( nStartSel, nEndSel);
					m_cMainEdit.GetWindowText(cStr);
					m_cMainEdit.GetWindowRect( cRect);
					char	szClass[ 512];
					GetClassName( m_cMainEdit.GetSafeHwnd(), szClass, 512);
					ScreenToClient( cRect);
					m_cMainEdit.DestroyWindow();

					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);
					if( ES_AUTOHSCROLL & dwStyle)
					{
						dwStyle ^= ( ES_AUTOHSCROLL | WS_HSCROLL);
						pcSOboe->AutoScroll( FALSE);
					}
					else
					{
						dwStyle |= ( ES_AUTOHSCROLL | WS_HSCROLL);
						pcSOboe->AutoScroll( TRUE);
					}
					m_cMainEdit.CreateEx( dwStyleEx, szClass, cStr, dwStyle, cRect.left, cRect.top, cRect.Width(), cRect.Height(), GetSafeHwnd(), ( HMENU)2526, NULL);
					m_cMainEdit.SetFont( &m_cFntEdit);
					m_cMainEdit.SetFocus();
					m_cMainEdit.SetSel( nStartSel, nEndSel);
					return TRUE;
				}
				break;
			case VK_RETURN:
				if( 0 == ( GetAsyncKeyState( VK_SHIFT) & ~1))break;
				PostMessage( WM_COMMAND, IDOK, 0);
				return TRUE;
			case VK_ESCAPE:
				if( 0 != ( GetAsyncKeyState( VK_SHIFT) & ~1))
				{
					PostMessage( WM_COMMAND, IDCANCEL, 0);
				}
				return TRUE;
			}
		}
		else
		{
			if( m_cTitleEdit == *GetFocus())
			{
				switch( pMsg->wParam)
				{
				case VK_RETURN:
					if( 0 != ( GetAsyncKeyState( VK_SHIFT) & ~1))
					{
						PostMessage( WM_COMMAND, IDOK, 0);
					}
					return TRUE;
				case VK_ESCAPE:
					if( 0 != ( GetAsyncKeyState( VK_SHIFT) & ~1))
					{
						PostMessage( WM_COMMAND, IDCANCEL, 0);
					}
					return TRUE;
				}
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CResizableMemoEditDialog::InsertTime( int nType)
{
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	CString	cStrSendTime;
	switch( nType)
	{
	case _INSERT_TIME:
		cStrSendTime = cTime.Format( "%H:%M:%S");
		break;
	case _INSERT_DATE:
		cStrSendTime = cTime.Format( "%y/%m/%d");
		break;
	case _INSERT_DAYOFWEEK:
		cStrSendTime = cTime.Format( "(%a)");
		break;
	default:
		return;
	}

	m_cMainEdit.ReplaceSel( cStrSendTime, TRUE);
}

void CResizableMemoEditDialog::OnDxlsel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CMenu	cDxlSelMemu;
	if( cDxlSelMemu.CreatePopupMenu())
	{
		UINT unFlag;
		CString	cStrMenu;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		
		unFlag = ( 0 == m_cMemoData.GetDxlID()) ? MF_CHECKED | MF_BYCOMMAND | MF_STRING : MF_BYCOMMAND | MF_STRING;
		cDxlSelMemu.AppendMenu( unFlag, IDM_DXLITEMXX, "���g�p");
		cDxlSelMemu.AppendMenu( MF_BYCOMMAND | MF_SEPARATOR);

		int nMaxEntry = pcSOboe->GetDxlMaxEntry();
		for( int nIndex = 0; nIndex < nMaxEntry; nIndex++)
		{
			const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				if( pcDrawExLib->IsFunctionAssign())
				{
					// ���j���[��&��\������Ƃ���&&����Ȃ��ƑʖڂȂ�
					if( -1 != pcDrawExLib->GetMenuString().Find( '&'))
					{
						cStrMenu.Empty();
						for( int nSIndex = 0; nSIndex < pcDrawExLib->GetMenuString().GetLength(); nSIndex++)
						{
							if( '&' == pcDrawExLib->GetMenuString()[ nSIndex])cStrMenu += '&';
							cStrMenu += pcDrawExLib->GetMenuString()[ nSIndex];
						}
					}
					else
					{
						cStrMenu = pcDrawExLib->GetMenuString();
					}

					unFlag = ( pcDrawExLib->GetDxlID() == m_cMemoData.GetDxlID()) ? MF_CHECKED | MF_BYCOMMAND | MF_STRING : MF_BYCOMMAND | MF_STRING;
					cDxlSelMemu.AppendMenu( unFlag, IDM_DXLITEM00 + nIndex, cStrMenu);
				}
			}
		}

		GetAsyncKeyState( VK_RBUTTON);
		UINT	unMenuCMD = 0;
		for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
		{
			if( IDM_DXLSEL == m_sastTButton[ nIndex].idCommand)
			{
				CRect	cRect;
				m_cToolBar.GetItemRect( nIndex, cRect);
				ClientToScreen( cRect);
				unMenuCMD = cDxlSelMemu.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);
			}
		}
		if( unMenuCMD)
		{
			BOOL	blEnable = FALSE;
			UINT	unDxlID = 0;

			TRACE( "menu sel %x\n", unMenuCMD);
			
			if( 0 < ( unMenuCMD - IDM_DXLITEMXX))
			{
				const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( ( unMenuCMD - IDM_DXLITEM00));
				if( pcDrawExLib)
				{
					if( GetAsyncKeyState( VK_RBUTTON))
					{
						pcDrawExLib->About( GetSafeHwnd());
						return;
					}
					else
					{
						blEnable = pcDrawExLib->IsSupportEditHelper();
						unDxlID = pcDrawExLib->GetDxlID();
					}
				}
			}
			else
			{
				if( IDM_DXLITEMXX == unMenuCMD)
				{
					blEnable = FALSE;
					unDxlID = 0;
				}
				else
				{
					MessageBeep( MB_ICONINFORMATION);
					return;
				}
			}

			if( unDxlID != m_cMemoData.GetDxlID())
			{
				m_cMemoData.SetDxlID( unDxlID);
				m_cMemoData.SetDxlExtData( NULL, 0);
			}
			m_cToolBar.EnableButton( IDM_DXLEDITHELPER, blEnable);
		}
		else
		{
			TRACE( "menu cancel\n");
		}
	}
}

void CResizableMemoEditDialog::OnDxledithelper() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	const CDrawExLib* pcDrawExLib = NULL;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
	if( 0 <= nIndex)
	{
		pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
		if( NULL != pcDrawExLib)
		{
			if( !pcDrawExLib->IsSupportEditHelper())pcDrawExLib = NULL;
		}
	}
	if( NULL == pcDrawExLib)
	{
		m_cToolBar.EnableButton( IDM_DXLEDITHELPER, FALSE);
		return;
	}


	CString	cStrMemo;
	m_cMainEdit.GetWindowText( cStrMemo);

	EDITDATA	stEdit;
	stEdit.m_nSelStart = 0;
	stEdit.m_nSelEnd = 0;
	stEdit.m_hEdit = NULL;
	if( !cStrMemo.IsEmpty())
	{
		m_cMainEdit.GetSel( stEdit.m_nSelStart, stEdit.m_nSelEnd);
		stEdit.m_hEdit = GlobalAlloc( GHND, cStrMemo.GetLength() + 1);
		LPSTR lpsz = ( LPSTR)GlobalLock( stEdit.m_hEdit);
		lstrcpy( lpsz, cStrMemo);
		GlobalUnlock( stEdit.m_hEdit);
	}

	if( pcDrawExLib->IsSupportNativeData())
	{
		stEdit.m_nSize = m_cMemoData.GetDxlExtData();
		if( stEdit.m_nSize)
		{
			stEdit.m_hSaveData = GlobalAlloc( GHND, stEdit.m_nSize);
			LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEdit.m_hSaveData);
			m_cMemoData.GetDxlExtData( lpbyData, stEdit.m_nSize);
			GlobalUnlock( stEdit.m_hSaveData);
		}
		else
		{
			stEdit.m_hSaveData = NULL;
		}
	}
	else
	{
		stEdit.m_nSize = 0;
		stEdit.m_hSaveData = NULL;
	}
	if( IDOK == pcDrawExLib->DoEditHelper( GetSafeHwnd(), &stEdit))
	{
		LPSTR lpsz = ( LPSTR)GlobalLock( stEdit.m_hEdit);
		cStrMemo = lpsz;
		GlobalUnlock( stEdit.m_hEdit);
		m_cMainEdit.SetWindowText( cStrMemo);
		m_cMainEdit.SetSel( stEdit.m_nSelStart, stEdit.m_nSelEnd);

		if( pcDrawExLib->IsSupportNativeData())
		{
			if( stEdit.m_nSize)
			{
				LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEdit.m_hSaveData);
				m_cMemoData.SetDxlExtData( lpbyData, stEdit.m_nSize);
				GlobalUnlock( stEdit.m_hSaveData);
			}
		}
	}
	if( stEdit.m_hEdit)GlobalFree( stEdit.m_hEdit);
	if( stEdit.m_hSaveData)GlobalFree( stEdit.m_hSaveData);
}

void CResizableMemoEditDialog::OnCalcresize() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	m_blResize = m_cToolBar.IsButtonChecked( IDM_CALCRESIZE);
}

void CResizableMemoEditDialog::OnDelonquit() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	m_blDelOnQuit = m_cToolBar.IsButtonChecked( IDM_DELONQUIT);
}

void CResizableMemoEditDialog::OnLink() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CLinkEdit	cLinkEdit( this);

	cLinkEdit.m_blLink = m_cMemoData.IsLink();
	cLinkEdit.m_blTimerExec = m_cMemoData.IsTimerExec();
	m_cMemoData.GetLinkData( cLinkEdit.m_cStrFile, cLinkEdit.m_cStrParameters);

	if( IDOK == cLinkEdit.DoModal())
	{
		m_cMemoData.Link( cLinkEdit.m_blLink);
		m_cMemoData.TimerExec( cLinkEdit.m_blTimerExec);
		m_cMemoData.SetLinkData( cLinkEdit.m_cStrFile, cLinkEdit.m_cStrParameters);
	}
}

void CResizableMemoEditDialog::OnTimer() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CTimerEdit	cTimerEdit( this);

	cTimerEdit.m_blTimerEnable = m_cMemoData.IsTimer();

	cTimerEdit.m_nMonth	= m_cMemoData.GetTimerMonth();
	cTimerEdit.m_nDay		= m_cMemoData.GetTimerDay();
	cTimerEdit.m_nHour	= m_cMemoData.GetTimerHour();
	cTimerEdit.m_nMin		= m_cMemoData.GetTimerMin();
	cTimerEdit.m_nWeek	= m_cMemoData.GetWeek();

	cTimerEdit.m_nTimerType	= m_cMemoData.GetTimerType();
	cTimerEdit.m_nNotifyShift	= m_cMemoData.GetNotifyShift();

	cTimerEdit.m_nBeforeNotify	= m_cMemoData.GetBeforeNotify();
	cTimerEdit.m_blAfterNotify	= m_cMemoData.IsAfterNotify();

	m_cMemoData.GetNotifySound( cTimerEdit.m_cStrNotifySound);

	if( IDOK == cTimerEdit.DoModal())
	{
		m_cMemoData.Timer( cTimerEdit.m_blTimerEnable);

		m_cMemoData.SetTimerMonth( cTimerEdit.m_nMonth);
		m_cMemoData.SetTimerDay( cTimerEdit.m_nDay);
		m_cMemoData.SetTimerHour( cTimerEdit.m_nHour);
		m_cMemoData.SetTimerMin( cTimerEdit.m_nMin);
		m_cMemoData.SetWeek( cTimerEdit.m_nWeek);

		m_cMemoData.SetTimerType( cTimerEdit.m_nTimerType);
		m_cMemoData.SetNotifyShift( cTimerEdit.m_nNotifyShift);

		m_cMemoData.SetBeforeNotify( cTimerEdit.m_nBeforeNotify);
		m_cMemoData.AfterNotify( cTimerEdit.m_blAfterNotify);

		m_cMemoData.SetNotifySound( cTimerEdit.m_cStrNotifySound);
	}
}

void CResizableMemoEditDialog::OnPwset() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CPWEdit	cPasswordEdit( this);

	cPasswordEdit.m_blPassWordEnable = m_cMemoData.IsPassWordLock();
	cPasswordEdit.m_cStrPassWord1st = m_cMemoData.GetPassWord();
	cPasswordEdit.m_cStrPassWord2nd = cPasswordEdit.m_cStrPassWord1st;

	if( IDOK == cPasswordEdit.DoModal())
	{
		m_cMemoData.PassWordLock( cPasswordEdit.m_blPassWordEnable);
		m_cMemoData.SetPassWord( cPasswordEdit.m_cStrPassWord1st);
	}
}

void CResizableMemoEditDialog::OnInsertdate() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	InsertTime( _INSERT_DATE);
}

void CResizableMemoEditDialog::OnInserttime() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	InsertTime( _INSERT_TIME);
}

BOOL CResizableMemoEditDialog::OnTBHelpInfo(HELPINFO* pHelpInfo) 
{
	CRect	cRect;
	for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
	{				
		m_cToolBar.GetItemRect( nIndex, cRect);
		ClientToScreen( cRect);
		if( cRect.PtInRect( pHelpInfo->MousePos))
		{
			switch( m_sastTButton[ nIndex].idCommand)
			{
			case IDM_TEMPLET:
			case IDM_ICONSEL:
			case IDM_FONTSEL:
			case IDM_FORECOLOR:
			case IDM_BACKCOLOR:
			case IDM_DELONQUIT:
			case IDM_CALCRESIZE:
			case IDM_LINK:
			case IDM_TIMER:
			case IDM_ZORDER:
			case IDM_DXLEDITHELPER:
			case IDM_PWSET:
			case IDM_INSERTDATE:
			case IDM_INSERTTIME:
				WinHelp( m_sastTButton[ nIndex].idCommand | 0x10000, HELP_CONTEXTPOPUP);
				return TRUE;
			case IDM_DXLSEL:
				if( 0 != m_cMemoData.GetDxlID())
				{
					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);
					int nIndex = pcSOboe->FindDxlID( m_cMemoData.GetDxlID());
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
			default:
				return FALSE;
			}
		}
	}
	return FALSE;
}

BOOL CResizableMemoEditDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: ���̈ʒu�Ƀ��b�Z�[�W �n���h���p�̃R�[�h��ǉ����邩�܂��̓f�t�H���g�̏������Ăяo���Ă�������
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_MEMOEDIT:
	case IDC_TITLE:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	case IDC_EXEDITTOOLBAR:
		return OnTBHelpInfo( pHelpInfo);
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

int CResizableMemoEditDialog::DoModal() 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	int nResult = IDCANCEL;
	CSingleLock	cSlk( &sm_cSmpDoEdit);
	if( cSlk.Lock( 0))
	{
		sm_hWndParent = m_hWndParent;
		nResult = CDialog::DoModal();
		sm_hWndParent = NULL;
	}
	return nResult;
}

void CResizableMemoEditDialog::OnIconsel() 
{
	// TODO: ���̈ʒu�ɃR�}���h �n���h���p�̃R�[�h��ǉ����Ă�������
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	const CIconImgList* pcIconImgs = pcSOboe->GetIconList();

	if( pcIconImgs)
	{
		CRect cRect;
		for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
		{
			if( IDM_ICONSEL == m_sastTButton[ nIndex].idCommand)
			{
				m_cToolBar.GetItemRect( nIndex, cRect);
				ClientToScreen( cRect);
			}
		}

		m_pcIcon = pcIconImgs->CreateIconSelectMenu( this, FALSE);

		UINT unMenuCMD = m_pcIcon->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this);
		if( NULL != m_pcIcon)
		{
			delete m_pcIcon;
			m_pcIcon = NULL;
		}
		/*
		UINT unMenuCMD = pcIconImgs->TrackPopupMenu( cRect.left, cRect.bottom, this);
		*/

		if( IDM_ICONIMG00 <= unMenuCMD && ( IDM_ICONIMG13 + 1) >= unMenuCMD)
		{
			m_cMemoData.SetIconIndex( ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13));
			TBBUTTONINFO	stTbBInfo;
			stTbBInfo.cbSize = sizeof( TBBUTTONINFO);
			stTbBInfo.dwMask = TBIF_IMAGE;
			stTbBInfo.iImage = 17 + ( int)( unMenuCMD - IDM_ICONIMG00);

			m_cToolBar.SetButtonInfo( IDM_ICONSEL, &stTbBInfo);
		}
		else
		{
			if( IDM_ICONIMG00 <= unMenuCMD)
			{
				m_cMemoData.SetIconIndex( ( int)( unMenuCMD - IDM_ICONIMG00));

				TBBUTTONINFO	stTbBInfo;
				stTbBInfo.cbSize = sizeof( TBBUTTONINFO);
				stTbBInfo.dwMask = TBIF_IMAGE;
				stTbBInfo.iImage = m_nExtIconTop + ( int)( unMenuCMD - ( IDM_ICONIMG13 + 2));
				m_cToolBar.SetButtonInfo( IDM_ICONSEL, &stTbBInfo);
			}
		}
	}
	/*
	CIconSelectmenu	cPopIcon;
	if( cPopIcon.CreatePopupMenu())
	{
		UINT	unMenuCMD;
		for( int nIndex = 0; nIndex < EDITDLG_TOOLBAR; nIndex++)
		{
			if( IDM_ICONSEL == m_sastTButton[ nIndex].idCommand)
			{
				CRect	cRect;
				m_cToolBar.GetItemRect( nIndex, cRect);
				ClientToScreen( cRect);
				unMenuCMD = cPopIcon.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);
			}
		}
		if( IDM_ICONIMG00 <= unMenuCMD && ( IDM_ICONIMG13 + 1) >= unMenuCMD)
		{
			m_cMemoData.SetIconIndex( ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13));
			TBBUTTONINFO	stTbBInfo;
			stTbBInfo.cbSize = sizeof( TBBUTTONINFO);
			stTbBInfo.dwMask = TBIF_IMAGE;
			stTbBInfo.iImage = 17 + ( int)( unMenuCMD - IDM_ICONIMG00);

			m_cToolBar.SetButtonInfo( IDM_ICONSEL, &stTbBInfo);
		}
		else
		{
			if( IDM_ICONIMG00 <= unMenuCMD)
			{
				m_cMemoData.SetIconIndex( ( int)( unMenuCMD - IDM_ICONIMG00));

				TBBUTTONINFO	stTbBInfo;
				stTbBInfo.cbSize = sizeof( TBBUTTONINFO);
				stTbBInfo.dwMask = TBIF_IMAGE;
				stTbBInfo.iImage = m_nExtIconTop + ( int)( unMenuCMD - ( IDM_ICONIMG13 + 2));
				m_cToolBar.SetButtonInfo( IDM_ICONSEL, &stTbBInfo);
			}
		}
	}
	*/
}

void CResizableMemoEditDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	if( NULL != m_pcIcon)
	{
		m_pcIcon->DrawItem( lpDrawItemStruct);

		return;
	}

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CResizableMemoEditDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	if( NULL != m_pcIcon)
	{
		m_pcIcon->MeasureItem( lpMeasureItemStruct);

		return;
	}

	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CResizableMemoEditDialog::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
	if( m_cToolBar.GetSafeHwnd() == pWnd->GetSafeHwnd())
	{
		TBBUTTON stButton;
		CRect cRect;
		for( int nIndex = 0; nIndex < m_cToolBar.GetButtonCount(); nIndex++)
		{
			ZeroMemory( &stButton, sizeof( TBBUTTON));
			m_cToolBar.GetButton( nIndex, &stButton);

			if( IDM_DXLSEL == stButton.idCommand)
			{
				m_cToolBar.GetItemRect( nIndex, cRect);
				m_cToolBar.ClientToScreen( cRect);
				if( cRect.PtInRect( point))
				{
					CMenu cMenu;
					if( cMenu.LoadMenu( IDR_DLGMENU))
					{
						CMenu* pcPopup = cMenu.GetSubMenu( 2);

						CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
						if( pcSOboe->GetDefaultDxlID() == m_cMemoData.GetDxlID())
						{
							pcPopup->CheckMenuItem( IDM_SETDEFAULT, MF_CHECKED | MF_BYCOMMAND);
						}
						pcPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this);
					}
				}
			}
		}
	}
}

void CResizableMemoEditDialog::OnSetdefault()
{
	// TODO : �����ɃR�}���h �n���h�� �R�[�h��ǉ����܂��B
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	pcSOboe->SetDefaultDxlID( m_cMemoData.GetDxlID());
}
