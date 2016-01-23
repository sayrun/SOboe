// ExtDllPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "ExtDllPage.h"
#include "ChipExDllSettingDialog.h"
#include "DxlEdit.h"
#include "DrawExLib.h"
#include "ChipArg_IF.h"
#include ".\extdllpage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDllPage プロパティ ページ

IMPLEMENT_DYNCREATE(CExtDllPage, CPropertyPage)

CExtDllPage::CExtDllPage() : CPropertyPage(CExtDllPage::IDD)
	, m_unDefaultDxlID( 0)
{
	//{{AFX_DATA_INIT(CExtDllPage)
	m_nChipExDllList = -1;
	m_nDrawExDll = -1;
	m_nArgDll = 0;
	//}}AFX_DATA_INIT
}

CExtDllPage::~CExtDllPage()
{
}

void CExtDllPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtDllPage)
	DDX_Control(pDX, IDC_ARRANGEDLL_SEL, m_cBtnArgDllSel);
	DDX_Control(pDX, IDC_ARRANGEDLL_PATH, m_cEdtArgDllPath);
	DDX_Control(pDX, IDC_ARRANGEDLL_CONFIG, m_cBtnArgDllConfig);
	DDX_Control(pDX, IDC_ARRANGEDLL_ABOUT, m_cBtnArgDllAbout);
	DDX_Control(pDX, IDC_DRAWEXDELETE, m_cBtnDrawExDelete);
	DDX_Control(pDX, IDC_DRAWEXEDIT, m_cBtnDrawExEdit);
	DDX_Control(pDX, IDC_CHIPEXDLLDELETE, m_cBtnChipExDllDelete);
	DDX_Control(pDX, IDC_CHIPEXDLLEDIT, m_cBtnChipExDllEdit);
	DDX_Control(pDX, IDC_DRAWEXLIST, m_cLstDrawEx);
	DDX_Control(pDX, IDC_CHIPEXDLLLIST, m_cLstChipExDll);
	DDX_LBIndex(pDX, IDC_CHIPEXDLLLIST, m_nChipExDllList);
	DDX_LBIndex(pDX, IDC_DRAWEXLIST, m_nDrawExDll);
	DDX_CBIndex(pDX, IDC_ARRANGEDLL, m_nArgDll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtDllPage, CPropertyPage)
	//{{AFX_MSG_MAP(CExtDllPage)
	ON_LBN_SELCHANGE(IDC_CHIPEXDLLLIST, OnSelchangeChipexdlllist)
	ON_LBN_DBLCLK(IDC_CHIPEXDLLLIST, OnChipexdlledit)
	ON_BN_CLICKED(IDC_CHIPEXDLLDELETE, OnChipexdlldelete)
	ON_LBN_SELCHANGE(IDC_DRAWEXLIST, OnSelchangeDrawexlist)
	ON_BN_CLICKED(IDC_DRAWEXEDIT, OnDrawexedit)
	ON_BN_CLICKED(IDC_DRAWEXDELETE, OnDrawexdelete)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_UPDRAWEX, OnUpdrawex)
	ON_COMMAND(IDM_DOWNDRAWEX, OnDowndrawex)
	ON_CBN_SELCHANGE(IDC_ARRANGEDLL, OnSelchangeArrangedll)
	ON_BN_CLICKED(IDC_ARRANGEDLL_SEL, OnArrangedllSel)
	ON_BN_CLICKED(IDC_ARRANGEDLL_CONFIG, OnArrangedllConfig)
	ON_BN_CLICKED(IDC_ARRANGEDLL_ABOUT, OnArrangedllAbout)
	ON_BN_CLICKED(IDC_CHIPEXDLLEDIT, OnChipexdlledit)
	ON_LBN_DBLCLK(IDC_DRAWEXLIST, OnDrawexedit)
	ON_EN_CHANGE(IDC_ARRANGEDLL_PATH, OnChangeArrangedllPath)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_SETDEFAULTDXL, OnSetdefaultdxl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtDllPage メッセージ ハンドラ

BOOL CExtDllPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	CString	cStr;
	for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
	{
		m_cStrCelPath[ nIndex] = m_pcProfileDatas->m_cStrCelPath[ nIndex];
		m_cStrCelMenuString[ nIndex] = m_pcProfileDatas->m_cStrCelMenuString[ nIndex];

		cStr = m_cStrCelMenuString[ nIndex];
		if( cStr.IsEmpty())cStr.LoadString( IDS_INVALID);
		m_cLstChipExDll.AddString( cStr);
	}
	m_cStrMenuSeparator = m_pcProfileDatas->m_cStrMenuSeparator;

	CDrawExLib	cDrawExLib;
	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		m_cStrDxlPath[ nIndex] = m_pcProfileDatas->m_cStrDxlPath[ nIndex];
		m_cStrDxlMenuString[ nIndex] = m_pcProfileDatas->m_cStrDxlMenuString[ nIndex];
		m_unDxlID[ nIndex] = 0;

		cDrawExLib.SetPath( m_cStrDxlPath[ nIndex]);
		if( !cDrawExLib.LoadDxl( m_hWnd))
		{
			m_unDxlID[ nIndex] = cDrawExLib.GetDxlID();
		}

		cStr = m_cStrDxlMenuString[ nIndex];
		if( cStr.IsEmpty())cStr.LoadString( IDS_INVALID);
		m_cLstDrawEx.AddString( cStr);
	}
	m_unDefaultDxlID = m_pcProfileDatas->m_unDefaultDxlID;

	m_cBtnChipExDllEdit.EnableWindow( FALSE);
	m_cBtnChipExDllDelete.EnableWindow( FALSE);

	m_cBtnDrawExEdit.EnableWindow( FALSE);
	m_cBtnDrawExDelete.EnableWindow( FALSE);

	m_cStrArrangeDll[ 0] = m_pcProfileDatas->m_cStrArrangeNormalDLL;
	m_cStrArrangeDll[ 1] = m_pcProfileDatas->m_cStrArrangeTitleDLL;
	m_cStrArrangeDll[ 2] = m_pcProfileDatas->m_cStrArrangeIconDLL;
	m_cStrArrangeDll[ 3] = m_pcProfileDatas->m_cStrArrangeAllDLL;
	if( 0 <= m_nArgDll && ( _ARGDLL_TYPE_COUNT - 1) >= m_nArgDll)
	{
		m_cEdtArgDllPath.SetWindowText( m_cStrArrangeDll[ m_nArgDll]);
		m_cBtnArgDllSel.EnableWindow( TRUE);
		m_cEdtArgDllPath.EnableWindow( TRUE);
		m_cBtnArgDllConfig.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
		m_cBtnArgDllAbout.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
	}
	m_nArgDllOld = m_nArgDll;

	UpdateData( FALSE);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CExtDllPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CString	cStr;
	for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
	{
		m_pcProfileDatas->m_cStrCelPath[ nIndex] = m_cStrCelPath[ nIndex];
		m_pcProfileDatas->m_cStrCelMenuString[ nIndex] = m_cStrCelMenuString[ nIndex];
	}
	m_pcProfileDatas->m_cStrMenuSeparator = m_cStrMenuSeparator;

	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		m_pcProfileDatas->m_cStrDxlPath[ nIndex] = m_cStrDxlPath[ nIndex];
		m_pcProfileDatas->m_cStrDxlMenuString[ nIndex] = m_cStrDxlMenuString[ nIndex];
	}
	m_pcProfileDatas->m_unDefaultDxlID = m_unDefaultDxlID;

	m_pcProfileDatas->m_cStrArrangeNormalDLL = m_cStrArrangeDll[ 0];
	m_pcProfileDatas->m_cStrArrangeTitleDLL = m_cStrArrangeDll[ 1];
	m_pcProfileDatas->m_cStrArrangeIconDLL = m_cStrArrangeDll[ 2];
	m_pcProfileDatas->m_cStrArrangeAllDLL = m_cStrArrangeDll[ 3];

	CPropertyPage::OnOK();
}

BOOL CExtDllPage::GetExtendDll( CString & cStrFile)
{
	DWORD dwLen = GetCurrentDirectory( 0, NULL);
	char*	pszFolder;
	dwLen++;
	pszFolder = new char[ dwLen];
	GetCurrentDirectory( dwLen, pszFolder);
	CString	cStrFolder = pszFolder;
	delete pszFolder;
	pszFolder = NULL;

	CString	cStrData;
	cStrData = cStrFile;
	CFileDialog	cFileDlg( TRUE, "dll", cStrData, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, "整列拡張ファイル(*.dll;*.arg)|*.dll;*.arg||", this);

	int nResult = cFileDlg.DoModal();
	if( IDOK == nResult)
	{
		cStrFile = cFileDlg.GetPathName();
	}
	SetCurrentDirectory( cStrFolder);

	return ( IDOK == nResult);
}

void CExtDllPage::OnSelchangeChipexdlllist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	m_cBtnChipExDllEdit.EnableWindow( ( 0 > m_nChipExDllList) ? FALSE : TRUE);
	CString	cStr;
	cStr.Empty();
	if( 0 <= m_nChipExDllList)
	{
		cStr = m_cStrCelMenuString[ m_nChipExDllList];
	}
	m_cBtnChipExDllDelete.EnableWindow( cStr.IsEmpty() ? FALSE : TRUE);
}

void CExtDllPage::OnChipexdlledit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( 0 > m_nChipExDllList)return;

	CChipExDllSettingDialog	cChipExDlg;
	cChipExDlg.m_cStrChipExMenuText = m_cStrCelMenuString[ m_nChipExDllList];
	cChipExDlg.m_cStrChipExDllName  = m_cStrCelPath[ m_nChipExDllList];
	cChipExDlg.m_blSetSepa = ( '0' != m_cStrMenuSeparator[ m_nChipExDllList]) ? TRUE : FALSE;
	cChipExDlg.m_nMenuIndex = m_nChipExDllList;
	if( IDOK == cChipExDlg.DoModal())
	{
		m_cStrCelMenuString[ m_nChipExDllList] = cChipExDlg.m_cStrChipExMenuText;
		m_cStrCelPath[ m_nChipExDllList] = cChipExDlg.m_cStrChipExDllName;
		m_cStrMenuSeparator.SetAt( m_nChipExDllList, cChipExDlg.m_blSetSepa ? '1' : '0');
		m_cBtnChipExDllDelete.EnableWindow( cChipExDlg.m_cStrChipExMenuText.IsEmpty() ? FALSE : TRUE);

		m_cLstChipExDll.SetRedraw( FALSE);

		CString cStr = cChipExDlg.m_cStrChipExMenuText;
		if( cStr.IsEmpty())cStr.LoadString( IDS_INVALID);
		m_cLstChipExDll.InsertString( m_nChipExDllList, cStr);
		m_cLstChipExDll.DeleteString( m_nChipExDllList + 1);
		m_cLstChipExDll.SetCurSel( m_nChipExDllList);

		m_cLstChipExDll.SetRedraw( TRUE);
	}
}

void CExtDllPage::OnChipexdlldelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( 0 > m_nChipExDllList)return;

	CString	cStr;
	cStr.Empty();
	m_cStrCelMenuString[ m_nChipExDllList].Empty();
	m_cStrCelPath[ m_nChipExDllList].Empty();
	m_cStrMenuSeparator.SetAt( m_nChipExDllList, '0');

	m_cLstChipExDll.SetRedraw( FALSE);

	cStr.LoadString( IDS_INVALID);
	m_cLstChipExDll.InsertString( m_nChipExDllList, cStr);
	m_cLstChipExDll.DeleteString( m_nChipExDllList + 1);
	m_cLstChipExDll.SetCurSel( m_nChipExDllList);

	m_cLstChipExDll.SetRedraw( TRUE);

	m_cBtnChipExDllDelete.EnableWindow( FALSE);
}

void CExtDllPage::OnSelchangeDrawexlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_blSpecSet)
	{
		CString	cStr = m_cStrDxlPath[ m_nDrawExDll];
		m_cBtnDrawExEdit.EnableWindow( TRUE);
		m_cBtnDrawExDelete.EnableWindow( !cStr.IsEmpty());
	}
}

void CExtDllPage::OnDrawexedit() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( !m_blSpecSet)return;
	if( 0 > m_nDrawExDll || _DRAWEXLIB_MAX <= m_nDrawExDll)return;

	CDxlEdit	cDxlEdit( this);

	cDxlEdit.m_cStrMenuText = m_cStrDxlMenuString[ m_nDrawExDll];
	cDxlEdit.m_cStrDxlPath = m_cStrDxlPath[ m_nDrawExDll];

	if( IDOK == cDxlEdit.DoModal())
	{
		UINT	unDxlID;
		CDrawExLib	cDrawExLib;
		for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
		{
			if( nIndex == m_nDrawExDll)continue;
			cDrawExLib.SetPath( m_cStrDxlPath[ nIndex]);
			cDrawExLib.LoadDxl( m_hWnd);
			unDxlID = cDrawExLib.GetDxlID();
			cDrawExLib.FreeDxl();
			if( unDxlID == cDxlEdit.m_unDxlID)
			{
				MessageBox( "ＤＸＬはＣＥＬとは異なり、多重登録することはできません。");
				return;
			}
		}
		m_cStrDxlMenuString[ m_nDrawExDll] = cDxlEdit.m_cStrMenuText;
		m_cStrDxlPath[ m_nDrawExDll] = cDxlEdit.m_cStrDxlPath;
		m_unDxlID[ m_nDrawExDll] = cDrawExLib.GetDxlID();

		m_cLstDrawEx.SetRedraw( FALSE);

		CString cStr = cDxlEdit.m_cStrMenuText;
		if( cStr.IsEmpty())cStr.LoadString( IDS_INVALID);
		m_cLstDrawEx.InsertString( m_nDrawExDll, cStr);
		m_cLstDrawEx.DeleteString( m_nDrawExDll + 1);
		m_cLstDrawEx.SetCurSel( m_nDrawExDll);

		m_cLstDrawEx.SetRedraw( TRUE);
	}
}

void CExtDllPage::OnDrawexdelete() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( 0 > m_nDrawExDll || _DRAWEXLIB_MAX < m_nDrawExDll)return;
	if( IDYES == MessageBox( "このＤＸＬを解放した場合、\nＤＸＬ使用中の小片が正しく表示されなくなります。\n本当に解放しても宜しいですか？", "確認", MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2))
	{
		m_cStrDxlMenuString[ m_nDrawExDll].Empty();
		m_cStrDxlPath[ m_nDrawExDll].Empty();

		m_cLstDrawEx.SetRedraw( FALSE);

		CString cStr;
		cStr.LoadString( IDS_INVALID);
		m_cLstDrawEx.InsertString( m_nDrawExDll, cStr);
		m_cLstDrawEx.DeleteString( m_nDrawExDll + 1);
		m_cLstDrawEx.SetCurSel( m_nDrawExDll);

		m_cLstDrawEx.SetRedraw( TRUE);
	}
}

void CExtDllPage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	CRect	cRect;
	m_cLstDrawEx.GetWindowRect( cRect);
	if( cRect.PtInRect(point))
	{
		UpdateData();

		if( 0 <= m_nDrawExDll)
		{
			CMenu menu;
			VERIFY( menu.LoadMenu( IDR_DXLLISTMENU));

			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT( pPopup != NULL);

			int nSel = m_cLstDrawEx.GetCurSel();

			pPopup->EnableMenuItem( IDC_DRAWEXEDIT, ( !m_blSpecSet || LB_ERR == m_nDrawExDll) ? MF_GRAYED : MF_ENABLED);
			pPopup->EnableMenuItem( IDC_DRAWEXDELETE, ( !m_blSpecSet || LB_ERR == m_nDrawExDll) ? MF_GRAYED : MF_ENABLED);
			pPopup->EnableMenuItem( IDM_SETDEFAULTDXL, m_cStrDxlPath[ nSel].IsEmpty() ? MF_GRAYED : MF_ENABLED);
			pPopup->EnableMenuItem( IDM_UPDRAWEX, ( !m_blSpecSet || LB_ERR == m_nDrawExDll || 0 == m_nDrawExDll) ? MF_GRAYED : MF_ENABLED);
			pPopup->EnableMenuItem( IDM_DOWNDRAWEX, ( !m_blSpecSet || LB_ERR == m_nDrawExDll || ( _DRAWEXLIB_MAX - 1) <= m_nDrawExDll) ? MF_GRAYED : MF_ENABLED);
			if( m_unDxlID[ m_nDrawExDll] == m_unDefaultDxlID)
			{
				pPopup->CheckMenuItem( IDM_SETDEFAULTDXL, MF_CHECKED);
			}

			pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		}
	}
	else
	{
		m_cLstChipExDll.GetWindowRect( cRect);
		if( cRect.PtInRect(point))
		{
			int	nSel = m_cLstChipExDll.GetCurSel();
			if( LB_ERR != nSel)
			{
				CMenu menu;
				VERIFY( menu.LoadMenu( IDR_DXLLISTMENU));

				CMenu* pPopup = menu.GetSubMenu(1);
				ASSERT( pPopup != NULL);
				if( LB_ERR == nSel)
				{
					pPopup->EnableMenuItem( IDC_CHIPEXDLLEDIT, MF_GRAYED);
					pPopup->EnableMenuItem( IDC_CHIPEXDLLDELETE, MF_GRAYED);
				}
				pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}
	}
}

void CExtDllPage::OnUpdrawex() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	UpdateData();
	if( LB_ERR != m_nDrawExDll && 0 < m_nDrawExDll)
	{
		CString	cStrDxlMenuStringSrc = m_cStrDxlMenuString[ m_nDrawExDll];
		CString	cStrDxlPathSrc = m_cStrDxlPath[ m_nDrawExDll];
		CString	cStrDxlMenuStringDst = m_cStrDxlMenuString[ m_nDrawExDll - 1];
		CString	cStrDxlPathDst = m_cStrDxlPath[ m_nDrawExDll - 1];

		m_cStrDxlMenuString[ m_nDrawExDll - 1] = cStrDxlMenuStringSrc;
		m_cStrDxlPath[ m_nDrawExDll - 1] = cStrDxlPathSrc;
		m_cStrDxlMenuString[ m_nDrawExDll] = cStrDxlMenuStringDst;
		m_cStrDxlPath[ m_nDrawExDll] = cStrDxlPathDst;

		m_cLstDrawEx.DeleteString( m_nDrawExDll);
		if( cStrDxlMenuStringSrc.IsEmpty())cStrDxlMenuStringSrc.LoadString( IDS_INVALID);
		m_nDrawExDll--;
		m_cLstDrawEx.InsertString( m_nDrawExDll, cStrDxlMenuStringSrc);
		UpdateData( FALSE);
	}
}

void CExtDllPage::OnDowndrawex() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	UpdateData();
	if( LB_ERR != m_nDrawExDll && ( _DRAWEXLIB_MAX - 1) > m_nDrawExDll)
	{
		CString	cStrDxlMenuStringSrc = m_cStrDxlMenuString[ m_nDrawExDll];
		CString	cStrDxlPathSrc = m_cStrDxlPath[ m_nDrawExDll];
		CString	cStrDxlMenuStringDst = m_cStrDxlMenuString[ m_nDrawExDll + 1];
		CString	cStrDxlPathDst = m_cStrDxlPath[ m_nDrawExDll + 1];

		m_cStrDxlMenuString[ m_nDrawExDll + 1] = cStrDxlMenuStringSrc;
		m_cStrDxlPath[ m_nDrawExDll + 1] = cStrDxlPathSrc;
		m_cStrDxlMenuString[ m_nDrawExDll] = cStrDxlMenuStringDst;
		m_cStrDxlPath[ m_nDrawExDll] = cStrDxlPathDst;

		m_cLstDrawEx.DeleteString( m_nDrawExDll);
		if( cStrDxlMenuStringSrc.IsEmpty())cStrDxlMenuStringSrc.LoadString( IDS_INVALID);
		m_nDrawExDll++;
		m_cLstDrawEx.InsertString( m_nDrawExDll, cStrDxlMenuStringSrc);
		UpdateData( FALSE);
	}
}

void CExtDllPage::OnSelchangeArrangedll() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_nArgDllOld != m_nArgDll)
	{
		m_cEdtArgDllPath.GetWindowText( m_cStrArrangeDll[ m_nArgDllOld]);
		m_nArgDllOld = m_nArgDll;
	}

	if( 0 <= m_nArgDll && ( _ARGDLL_TYPE_COUNT - 1) >= m_nArgDll)
	{
		m_cEdtArgDllPath.SetWindowText( m_cStrArrangeDll[ m_nArgDll]);
		m_cBtnArgDllSel.EnableWindow( TRUE);
		m_cEdtArgDllPath.EnableWindow( TRUE);
		m_cBtnArgDllConfig.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
		m_cBtnArgDllAbout.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
	}
	else
	{
		m_cEdtArgDllPath.SetWindowText( NULL);
		m_cBtnArgDllSel.EnableWindow( FALSE);
		m_cEdtArgDllPath.EnableWindow( FALSE);
		m_cBtnArgDllConfig.EnableWindow( FALSE);
		m_cBtnArgDllAbout.EnableWindow( FALSE);
	}
}

void CExtDllPage::OnArrangedllSel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( 0 > m_nArgDll || ( _ARGDLL_TYPE_COUNT - 1) < m_nArgDll)return;

	CString	cStrFile;
	cStrFile = m_cStrArrangeDll[ m_nArgDll];
	if( GetExtendDll( cStrFile))
	{
		m_cStrArrangeDll[ m_nArgDll] = cStrFile;
		m_cEdtArgDllPath.SetWindowText( m_cStrArrangeDll[ m_nArgDll]);
		m_cBtnArgDllConfig.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
		m_cBtnArgDllAbout.EnableWindow( !m_cStrArrangeDll[ m_nArgDll].IsEmpty());
	}
}

void CExtDllPage::OnArrangedllConfig() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( 0 > m_nArgDll || ( _ARGDLL_TYPE_COUNT - 1) < m_nArgDll)return;

	m_cEdtArgDllPath.GetWindowText( m_cStrArrangeDll[ m_nArgDll]);
	if( m_cStrArrangeDll[ m_nArgDll].IsEmpty())
	{
		MessageBeep( MB_ICONHAND);
		return;
	}

	HINSTANCE	hInst = NULL;

	hInst = LoadLibrary( m_cStrArrangeDll[ m_nArgDll]);
	// リンクが正常になされた？
	if( NULL != hInst)
	{
		ARRANGESTRUCTVERSION	lpArrangeStructVersion;
		// ヴァージョンの確認
		lpArrangeStructVersion = ( ARRANGESTRUCTVERSION)GetProcAddress( hInst, "GetArrangeSpecification");
		if( lpArrangeStructVersion)
		{
			SPECIFICATION	stSpecification;
			lpArrangeStructVersion( &stSpecification);
			if( ( UINT)__ARRAGESTRUCT_VERSION200 >= stSpecification.unUpperStructVersion &&
				( UINT)__ARRAGESTRUCT_VERSION100 <= stSpecification.unLowerStructVersion)
			{
				ARRANGEDLG	lpArrageConfigureDialog = NULL;
				// リソースチェックのDLLから、目的の関数ポインタを取得します。
				if( 0 == m_nArgDll)lpArrageConfigureDialog = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeChipConfigure");
				if( 1 == m_nArgDll)lpArrageConfigureDialog = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeTitleConfigure");
				if( 2 == m_nArgDll)lpArrageConfigureDialog = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeIconConfigure");
				if( 3 == m_nArgDll)lpArrageConfigureDialog = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeMultiConfigure");
				if( lpArrageConfigureDialog)
				{
					lpArrageConfigureDialog( min( ( UINT)__ARRAGESTRUCT_VERSION200, stSpecification.unUpperStructVersion), m_hWnd);
				}
				else
				{
					CString	cStr;
					cStr.LoadString( IDS_MISSMATCHEXTENDED);
					MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
				}
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MISSMATCHEXTENEDEDVERSION);
				MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
			}
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_MISSMATCHEXTENDED);
			MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
		FreeLibrary( hInst);
	}
	else
	{
		CString	cStr;
		cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
		MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
	}
}

void CExtDllPage::OnArrangedllAbout() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( 0 > m_nArgDll || ( _ARGDLL_TYPE_COUNT - 1) < m_nArgDll)return;

	m_cEdtArgDllPath.GetWindowText( m_cStrArrangeDll[ m_nArgDll]);
	if( m_cStrArrangeDll[ m_nArgDll].IsEmpty())
	{
		MessageBeep( MB_ICONHAND);
		return;
	}

	HINSTANCE	hInst = NULL;

	hInst = LoadLibrary( m_cStrArrangeDll[ m_nArgDll]);
	// リンクが正常になされた？
	if( NULL != hInst)
	{
		ARRANGEDLG	lpArrageAbout;
		// リソースチェックのDLLから、目的の関数ポインタを取得します。
		if( 0 == m_nArgDll)lpArrageAbout = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeChipAbout");
		if( 1 == m_nArgDll)lpArrageAbout = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeTitleAbout");
		if( 2 == m_nArgDll)lpArrageAbout = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeIconAbout");
		if( 3 == m_nArgDll)lpArrageAbout = ( ARRANGEDLG)GetProcAddress( hInst, "ArrangeAllAbout");
		if( lpArrageAbout)
		{
			lpArrageAbout( ( UINT)0x0100, m_hWnd);
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_MISSMATCHEXTENDED);
			MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
		FreeLibrary( hInst);
	}
	else
	{
		CString	cStr;
		cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
		MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
	}
}

void CExtDllPage::OnChangeArrangedllPath() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで CRichEditCrtl().SetEventMask()
	// メッセージをコントロールへ送るために CPropertyPage::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( 0 > m_nArgDll || ( _ARGDLL_TYPE_COUNT - 1) < m_nArgDll)return;
	m_cBtnArgDllConfig.EnableWindow( m_cEdtArgDllPath.GetWindowTextLength());
	m_cBtnArgDllAbout.EnableWindow( m_cEdtArgDllPath.GetWindowTextLength());
}

void CExtDllPage::OnSetdefaultdxl()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	UpdateData();
	if( 0 > m_nDrawExDll || _DRAWEXLIB_MAX < m_nDrawExDll)return;

	m_unDefaultDxlID = m_unDxlID[ m_nDrawExDll];
}
