// TempletPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "TempletPage.h"
#include "MemoListBox.h"
#include "MemoChip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTempletPage プロパティ ページ

IMPLEMENT_DYNCREATE(CTempletPage, CPropertyPage)

CTempletPage::CTempletPage() : CPropertyPage(CTempletPage::IDD)
{
	//{{AFX_DATA_INIT(CTempletPage)
	m_blEnableTemplet = FALSE;
	m_nLstTemplet = -1;
	//}}AFX_DATA_INIT
}

CTempletPage::~CTempletPage()
{
}

void CTempletPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTempletPage)
	DDX_Control(pDX, IDC_DOWNITEM, m_cBtnDownItem);
	DDX_Control(pDX, IDC_UPITEM, m_cBtnUpItem);
	DDX_Control(pDX, IDC_TEMPLETDEL, m_cBtnTemletDel);
	DDX_Control(pDX, IDC_TEMPLETLIST, m_cLstTemplet);
	DDX_Check(pDX, IDC_ENABLETEMPLET, m_blEnableTemplet);
	DDX_LBIndex(pDX, IDC_TEMPLETLIST, m_nLstTemplet);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTempletPage, CPropertyPage)
	//{{AFX_MSG_MAP(CTempletPage)
	ON_BN_CLICKED(IDC_ENABLETEMPLET, OnEnabletemplet)
	ON_BN_CLICKED(IDC_TEMPLETDEL, OnTempletdel)
	ON_LBN_SELCHANGE(IDC_TEMPLETLIST, OnSelchangeTempletlist)
	ON_BN_CLICKED(IDC_DOWNITEM, OnDownitem)
	ON_BN_CLICKED(IDC_UPITEM, OnUpitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTempletPage メッセージ ハンドラ

BOOL CTempletPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_blEnableTemplet = m_pcProfileDatas->m_blEnableTemplet;
	m_cStrTempletFile = m_pcProfileDatas->m_cStrTempletFile;

	m_cLstTemplet.EnableWindow( m_blEnableTemplet);
	m_cBtnTemletDel.EnableWindow( FALSE);
	m_cBtnDownItem.EnableWindow( FALSE);
	m_cBtnUpItem.EnableWindow( FALSE);

	UpdateData( FALSE);

	LoadTempletData();

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CTempletPage::LoadTempletData( void)
{
	m_cLstTemplet.ResetContent();

	try
	{
		CFile	cLoadFile;
		if( cLoadFile.Open( m_cStrTempletFile, CFile::modeRead | CFile::typeBinary))
		{
			CMemoChip*	pcMemoChip;
			const CMemoData*	pcMemoData;
			CArchive	cArchive( &cLoadFile, CArchive::load);
			for( int nIndex = 0; nIndex < 10; nIndex++)
			{
				pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
				if( NULL == pcMemoChip)break;
				pcMemoData = pcMemoChip->AccessMemoData();
				if( !pcMemoData->IsPassWordLock())
				{
					m_cLstTemplet.AddMemoChip( pcMemoChip);
				}
				delete pcMemoChip;
			}
		}
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
}


void CTempletPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	BOOL	blOldSetting = m_pcProfileDatas->m_blEnableTemplet;
	m_pcProfileDatas->m_blEnableTemplet = m_blEnableTemplet;
	m_pcProfileDatas->m_cStrTempletFile = m_cStrTempletFile;

	try
	{
		if( m_blEnableTemplet || blOldSetting)
		{
			CFile	cSaveFile;
			if( cSaveFile.Open( m_cStrTempletFile, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate))
			{
				const CMemoChip*	pcMemoChip;
				CArchive	cArchive( &cSaveFile, CArchive::store);
				int nCount = m_cLstTemplet.GetCount();
				for( int nIndex = 0; nIndex < nCount; nIndex++)
				{
					pcMemoChip = m_cLstTemplet.GetMemoChip( nIndex);
					if( pcMemoChip)
					{
						cArchive.WriteObject( pcMemoChip);
					}
				}
			}
		}
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		e->Delete();
	}
	
	CPropertyPage::OnOK();
}

BOOL CTempletPage::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( !m_blSpecSet)
	{
		m_cLstTemplet.StartDD();
	}
	
	return CPropertyPage::OnSetActive();
}

BOOL CTempletPage::OnKillActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( !m_blSpecSet)
	{
		m_cLstTemplet.EndDD();
	}
	
	return CPropertyPage::OnKillActive();
}

void CTempletPage::OnEnabletemplet() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	m_cLstTemplet.EnableWindow( m_blEnableTemplet);
	m_cBtnTemletDel.EnableWindow( m_blEnableTemplet);
	if( m_blEnableTemplet)
	{
		if( m_cStrTempletFile.IsEmpty())
		{
			char	szFile[ _MAX_PATH];
			GetModuleFileName( NULL, szFile, _MAX_PATH);
			char szDrive[ _MAX_DRIVE];
			char szPath[ _MAX_DIR];
			_splitpath( szFile, szDrive, szPath, NULL, NULL);
			_makepath( szFile, szDrive, szPath, "OboeGaki", "tpl");
			m_cStrTempletFile = szFile;
		}
	}
	else
	{
		m_nLstTemplet = -1;
		UpdateData( FALSE);
		m_cBtnDownItem.EnableWindow( FALSE);
		m_cBtnUpItem.EnableWindow( FALSE);
	}
}

void CTempletPage::OnTempletdel() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	if( m_blEnableTemplet)
	{
		if( 0 <= m_nLstTemplet)
		{
			m_cLstTemplet.DeleteMemoChip( m_nLstTemplet);
			int nCount = m_cLstTemplet.GetCount();
			if( m_nLstTemplet >= nCount)
			{
				m_nLstTemplet = nCount - 1;
			}
			if( 1 < nCount)
			{
				m_cBtnDownItem.EnableWindow( 0 <= m_nLstTemplet && m_nLstTemplet < ( nCount - 1));
				m_cBtnUpItem.EnableWindow( 0 < m_nLstTemplet && m_nLstTemplet <= ( nCount - 1));
			}
			UpdateData( FALSE);
		}
	}
}

void CTempletPage::OnSelchangeTempletlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();

	m_cBtnTemletDel.EnableWindow( ( 0 <= m_nLstTemplet && m_blEnableTemplet));
	if( m_blEnableTemplet)
	{
		int nCount = m_cLstTemplet.GetCount();
		if( 1 < nCount)
		{
			m_cBtnDownItem.EnableWindow( 0 <= m_nLstTemplet && m_nLstTemplet < ( nCount - 1));
			m_cBtnUpItem.EnableWindow( 0 < m_nLstTemplet && m_nLstTemplet <= ( nCount - 1));
		}
	}
}

void CTempletPage::OnDownitem() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( m_blEnableTemplet)
	{
		int nCount = m_cLstTemplet.GetCount();
		if( 1 < nCount)
		{
			if( 0 <= m_nLstTemplet && m_nLstTemplet < ( nCount - 1))
			{
				m_cLstTemplet.SetRedraw( FALSE);
				const CMemoChip*	pcMemoChip;
				pcMemoChip = m_cLstTemplet.GetMemoChip( m_nLstTemplet);
				if( pcMemoChip)
				{
					m_cLstTemplet.InsertMemoChip( m_nLstTemplet + 2, pcMemoChip);
					m_cLstTemplet.DeleteMemoChip( m_nLstTemplet);
					m_nLstTemplet++;
				}
				m_cLstTemplet.SetRedraw( TRUE);
				UpdateData( FALSE);
			}
			m_cBtnDownItem.EnableWindow( 0 <= m_nLstTemplet && m_nLstTemplet < ( nCount - 1));
			m_cBtnUpItem.EnableWindow( 0 < m_nLstTemplet && m_nLstTemplet <= ( nCount - 1));
		}
	}
}

void CTempletPage::OnUpitem() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( m_blEnableTemplet)
	{
		int nCount = m_cLstTemplet.GetCount();
		if( 1 < nCount)
		{
			if( 0 < m_nLstTemplet && m_nLstTemplet <= ( nCount - 1))
			{
				m_cLstTemplet.SetRedraw( FALSE);
				const CMemoChip*	pcMemoChip;
				pcMemoChip = m_cLstTemplet.GetMemoChip( m_nLstTemplet);
				if( pcMemoChip)
				{
					m_cLstTemplet.InsertMemoChip( m_nLstTemplet - 1, pcMemoChip);
					m_cLstTemplet.DeleteMemoChip( m_nLstTemplet + 1);
					m_nLstTemplet--;
				}
				m_cLstTemplet.SetRedraw( TRUE);
				UpdateData( FALSE);
			}
			m_cBtnDownItem.EnableWindow( 0 <= m_nLstTemplet && m_nLstTemplet < ( nCount - 1));
			m_cBtnUpItem.EnableWindow( 0 < m_nLstTemplet && m_nLstTemplet <= ( nCount - 1));
		}
	}
}
