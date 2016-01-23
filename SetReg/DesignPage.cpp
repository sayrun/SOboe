// DesignPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "DesignPage.h"

#include "IconImgList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDesignPage プロパティ ページ

IMPLEMENT_DYNCREATE(CDesignPage, CPropertyPage)

CDesignPage::CDesignPage() : CPropertyPage(CDesignPage::IDD)
{
	//{{AFX_DATA_INIT(CDesignPage)
	m_nDesignType = 0;
	m_blAutoIMEOpen = FALSE;
	m_cStrSaveFilePath = _T("");
	m_nGenerationCount = 0;
	//}}AFX_DATA_INIT
	m_cStrSample.LoadString( IDS_EXSAMPLE);
	m_pcFntEdit = NULL;

	m_pcIcon = NULL;
}

CDesignPage::~CDesignPage()
{
	if( m_pcFntEdit)
	{
		delete m_pcFntEdit;
		m_pcFntEdit = NULL;
	}
}

void CDesignPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDesignPage)
	DDX_Control(pDX, IDC_ICONSEL, m_cBtnIconSel);
	DDX_Control(pDX, IDC_GENERATIONCOUNT, m_cEdtGenerationCount);
	DDX_Control(pDX, IDC_GENERATIONCOUNTSPIN, m_cSpnGenerationCount);
	DDX_Control(pDX, IDC_DATAFILESEL, m_cBtnSaveFileSel);
	DDX_Control(pDX, IDC_DATAFILE, m_cEdtSaveFilePath);
	DDX_Control(pDX, IDC_SAMPLE, m_cEdtSample);
	DDX_Control(pDX, IDC_DESIGNTYPE, m_cCmbDesignList);
	DDX_Text(pDX, IDC_SAMPLE, m_cStrSample);
	DDX_CBIndex(pDX, IDC_DESIGNTYPE, m_nDesignType);
	DDX_Check(pDX, IDC_IMEAUTO, m_blAutoIMEOpen);
	DDX_Text(pDX, IDC_DATAFILE, m_cStrSaveFilePath);
	DDX_Text(pDX, IDC_GENERATIONCOUNT, m_nGenerationCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDesignPage, CPropertyPage)
	//{{AFX_MSG_MAP(CDesignPage)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_DESIGNTYPE, OnSelchangeDesigntype)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_BN_CLICKED(IDC_COLOR, OnColor)
	ON_BN_CLICKED(IDC_DATAFILESEL, OnDatafilesel)
	ON_BN_CLICKED(IDC_ICONSEL, OnIconsel)
	//}}AFX_MSG_MAP
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDesignPage メッセージ ハンドラ

BOOL CDesignPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CopyMemory( &m_stLogFont, &m_pcProfileDatas->m_stLogFont, sizeof( LOGFONT));
	m_clrNormalFore = m_pcProfileDatas->m_clrForeColor;
	m_clrNormalBack = m_pcProfileDatas->m_clrBackColor;
	m_nIconIndex	= m_pcProfileDatas->m_nIconIndex;
	m_clrFore		= m_clrNormalFore;
	m_clrBack		= m_clrNormalBack;

	m_clrBeforeFore	= m_pcProfileDatas->m_clrBeforeFore;
	m_clrBeforeBack	= m_pcProfileDatas->m_clrBeforeBack;
	m_clrAfterFore	= m_pcProfileDatas->m_clrAfterFore;
	m_clrAfterBack	= m_pcProfileDatas->m_clrAfterBack;

	m_nGenerationCount = m_pcProfileDatas->m_nGenerationCount;
	
	m_blAutoIMEOpen	= m_pcProfileDatas->m_blAutoIMEOpen;

	m_cStrSaveFilePath = m_pcProfileDatas->m_cStrSaveFilePath;

	CDC*	pcDC;
	pcDC = GetDC();
	if( pcDC)
	{
		m_cBrhBack.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
		ReleaseDC( pcDC);
	}
	if( m_pcFntEdit)
	{
		delete m_pcFntEdit;
		m_pcFntEdit = NULL;
	}
	m_pcFntEdit = new CFont;
	m_pcFntEdit->CreateFontIndirect( &m_stLogFont);
	m_cEdtSample.SetFont( m_pcFntEdit);
	m_cEdtSample.SetMargins( 0, 0);

	m_nDesignTypeOld = m_nDesignType;

	m_cEdtSaveFilePath.EnableWindow( m_blSpecSet);
	m_cBtnSaveFileSel.EnableWindow( m_blSpecSet);

	m_cSpnGenerationCount.SetRange( 0, 10);

	m_cEdtGenerationCount.EnableWindow( m_blSpecSet);
	m_cSpnGenerationCount.EnableWindow( m_blSpecSet);

	CSetRegApp*	pcSetRegApp = ( CSetRegApp*)AfxGetApp();
	ASSERT( pcSetRegApp);
	const CIconImgList* pcIconImgs = pcSetRegApp->GetIconList();
	pcDC = m_cBtnIconSel.GetDC();
	if( pcDC)
	{
		CDC	cMemDC;
		if( cMemDC.CreateCompatibleDC( pcDC))
		{
			m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
			cMemDC.SelectObject( &m_cBmpIcon);
			pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

			m_cBtnIconSel.SetBitmap( m_cBmpIcon);

			cMemDC.DeleteDC();
		}
		m_cBtnIconSel.ReleaseDC( pcDC);
	}

	UpdateData( FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

HBRUSH CDesignPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	// TODO: この位置で DC のアトリビュートを変更してください
	HBRUSH hbr;
	if( ( CTLCOLOR_EDIT == nCtlColor || CTLCOLOR_MSGBOX == nCtlColor) && m_cEdtSample == *pWnd)
	{
		
		hbr = m_cBrhBack;
		pDC->SetBkColor( m_clrBack);
		pDC->SetTextColor( m_clrFore);
	}
	else
	{
		hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CDesignPage::OnSelchangeDesigntype() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_nDesignTypeOld == m_nDesignType)return;

	switch( m_nDesignTypeOld)
	{
	case 0:
		m_clrNormalFore = m_clrFore;
		m_clrNormalBack = m_clrBack;
		break;
	case 1:
		m_clrBeforeFore = m_clrFore;
		m_clrBeforeBack = m_clrBack;
		break;
	case 2:
		m_clrAfterFore = m_clrFore;
		m_clrAfterBack = m_clrBack;
		break;
	}
	switch( m_nDesignType)
	{
	case 0:
		m_clrFore = m_clrNormalFore;
		m_clrBack = m_clrNormalBack;
		break;
	case 1:
		m_clrFore = m_clrBeforeFore;
		m_clrBack = m_clrBeforeBack;
		break;
	case 2:
		m_clrFore = m_clrAfterFore;
		m_clrBack = m_clrAfterBack;
		break;
	}
	m_cBtnIconSel.EnableWindow( ( 0 == m_nDesignType) ? TRUE : FALSE);
	m_nDesignTypeOld = m_nDesignType;

	m_cBrhBack.DeleteObject();
	CDC*	pcDC;
	pcDC = GetDC();
	m_cBrhBack.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
	ReleaseDC( pcDC);

	m_cEdtSample.InvalidateRect( NULL);
}

void CDesignPage::OnFont() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( 0 == m_nDesignType)
	{
		CFontDialog	cFontDlg( &m_stLogFont, CF_EFFECTS | CF_SCREENFONTS, NULL, this);
		cFontDlg.m_cf.rgbColors = m_clrFore;

		if( IDOK == cFontDlg.DoModal())
		{
			m_clrFore = cFontDlg.GetColor();
			if( m_pcFntEdit)
			{
				delete m_pcFntEdit;
				m_pcFntEdit = NULL;
			}
			m_pcFntEdit = new CFont;
			m_pcFntEdit->CreateFontIndirect( &m_stLogFont);
			m_cEdtSample.SetFont( m_pcFntEdit);
		}
	}
	else
	{
		CColorDialog	cColorDlg( m_clrFore, 0, this);
		
		if( IDOK == cColorDlg.DoModal())
		{
			m_clrFore = cColorDlg.GetColor();
			CDC*	pcDC;
			pcDC = GetDC();
			m_clrFore = pcDC->GetNearestColor( m_clrFore);
			ReleaseDC( pcDC);
			m_cEdtSample.InvalidateRect( NULL);
		}
	}
}

void CDesignPage::OnColor() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CColorDialog	cColorDlg( m_clrBack, 0, this);
	
	if( m_pcProfileDatas)
	{
		CopyMemory( cColorDlg.m_cc.lpCustColors, m_pcProfileDatas->m_clrCustomColor, sizeof( COLORREF) * _USERCOLOR_MAX);
	}
	if( IDOK == cColorDlg.DoModal())
	{
		m_clrBack = cColorDlg.GetColor();
		m_cBrhBack.DeleteObject();
		CDC*	pcDC;
		pcDC = GetDC();
		m_cBrhBack.CreateSolidBrush( pcDC->GetNearestColor( m_clrBack));
		ReleaseDC( pcDC);
		m_cEdtSample.InvalidateRect( NULL);
	}
}

void CDesignPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	switch( m_nDesignType)
	{
	case 0:
		m_clrNormalFore = m_clrFore;
		m_clrNormalBack = m_clrBack;
		break;
	case 1:
		m_clrBeforeFore = m_clrFore;
		m_clrBeforeBack = m_clrBack;
		break;
	case 2:
		m_clrAfterFore = m_clrFore;
		m_clrAfterBack = m_clrBack;
		break;
	}

	m_pcProfileDatas->m_clrForeColor = m_clrNormalFore;
	m_pcProfileDatas->m_clrBackColor = m_clrNormalBack;
	m_pcProfileDatas->m_nIconIndex = m_nIconIndex;
	CopyMemory( &m_pcProfileDatas->m_stLogFont, &m_stLogFont, sizeof( LOGFONT));

	m_pcProfileDatas->m_clrBeforeFore = m_clrBeforeFore;
	m_pcProfileDatas->m_clrBeforeBack = m_clrBeforeBack;
	m_pcProfileDatas->m_clrAfterFore = m_clrAfterFore;
	m_pcProfileDatas->m_clrAfterBack = m_clrAfterBack;

	m_pcProfileDatas->m_nGenerationCount = max( 0, min( 9, m_nGenerationCount));

	m_pcProfileDatas->m_blAutoIMEOpen	= m_blAutoIMEOpen;
	
	m_pcProfileDatas->m_cStrSaveFilePath = m_cStrSaveFilePath;

	CPropertyPage::OnOK();
}

void CDesignPage::OnDatafilesel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	DWORD dwLen = GetCurrentDirectory( 0, NULL);
	char*	pszFolder;
	dwLen++;
	pszFolder = new char[ dwLen];
	GetCurrentDirectory( dwLen, pszFolder);
	CString	cStrFolder = pszFolder;
	delete pszFolder;
	pszFolder = NULL;

	CString	cStrData;
	cStrData = m_cStrSaveFilePath;
	CFileDialog	cFileDlg( TRUE, "mrm", cStrData, OFN_HIDEREADONLY, "覚え書データファイル(*.mrm)|*.mrm||", this);

	int nResult = cFileDlg.DoModal();
	if( IDOK == nResult)
	{
		m_cStrSaveFilePath = cFileDlg.GetPathName();
		UpdateData( FALSE);
	}
	SetCurrentDirectory( cStrFolder);
}

void CDesignPage::OnIconsel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_cBtnIconSel.SetCheck( 1);
	CSetRegApp*	pcSetRegApp = ( CSetRegApp*)AfxGetApp();
	ASSERT( pcSetRegApp);
	const CIconImgList* pcIconImgs = pcSetRegApp->GetIconList();

	if( pcIconImgs)
	{
		CRect cRect;
		m_cBtnIconSel.GetWindowRect( cRect);

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

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nIconIndex = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CDC *pcDC = m_cBtnIconSel.GetDC();
			if( pcDC)
			{
				CDC	cMemDC;
				if( cMemDC.CreateCompatibleDC( pcDC))
				{
					if( NULL == m_cBmpIcon.GetSafeHandle())
					{
						m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					}
					cMemDC.SelectObject( &m_cBmpIcon);
					pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

					m_cBtnIconSel.SetBitmap( m_cBmpIcon);

					cMemDC.DeleteDC();
				}
				ReleaseDC( pcDC);
			}
		}
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, GetSafeHwnd(), 0, 0, PM_REMOVE));
		}
	}
	m_cBtnIconSel.SetCheck( 0);
	/*
	m_cBtnIconSel.SetCheck( 1);

	CSetRegApp*	pcSetRegApp = ( CSetRegApp*)AfxGetApp();
	ASSERT( pcSetRegApp);
	const CIconImgList* pcIconImgs = pcSetRegApp->GetIconList();

	CIconSelectmenu	cPopIcon;
	if( cPopIcon.CreatePopupMenu( pcIconImgs))
	{
		UINT	unMenuCMD;

		CRect	cRect;
		m_cBtnIconSel.GetWindowRect( cRect);

		unMenuCMD = cPopIcon.TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD, cRect.left, cRect.bottom, this, NULL);

		if( IDM_ICONIMG00 <= unMenuCMD)
		{
			m_nIconIndex = ( int)min( ( unMenuCMD - IDM_ICONIMG00), IDM_ICONIMG13);

			CDC *pcDC = m_cBtnIconSel.GetDC();
			if( pcDC)
			{
				CDC	cMemDC;
				if( cMemDC.CreateCompatibleDC( pcDC))
				{
					if( NULL == m_cBmpIcon.GetSafeHandle())
					{
						m_cBmpIcon.CreateCompatibleBitmap( &cMemDC, _ICON_WIDTH, _ICON_HEIGHT);
					}
					cMemDC.SelectObject( &m_cBmpIcon);
					pcIconImgs->Draw( m_nIconIndex, cMemDC.GetSafeHdc(), CRect( 0, 0, _ICON_WIDTH, _ICON_HEIGHT));

					m_cBtnIconSel.SetBitmap( m_cBmpIcon);

					cMemDC.DeleteDC();
				}
				ReleaseDC( pcDC);
			}
		}
		else
		{
			MSG stMsg;
			while( PeekMessage( &stMsg, GetSafeHwnd(), 0, 0, PM_REMOVE));
		}
	}
	m_cBtnIconSel.SetCheck( 0);
	*/
}

void CDesignPage::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	if( NULL != m_pcIcon)
	{
		m_pcIcon->DrawItem( lpDrawItemStruct);

		return;
	}

	CPropertyPage::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CDesignPage::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if( NULL != m_pcIcon)
	{
		m_pcIcon->MeasureItem( lpMeasureItemStruct);

		return;
	}

	CPropertyPage::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}
