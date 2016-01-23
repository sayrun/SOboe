// SendEditPage.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "soboe.h"
#include "SendEditPage.h"
#include "NetExLib.h"
#include "ExtSettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendEditPage プロパティ ページ

IMPLEMENT_DYNCREATE(CSendEditPage, CPropertyPage)

CSendEditPage::CSendEditPage() : CPropertyPage(CSendEditPage::IDD)
{
	//{{AFX_DATA_INIT(CSendEditPage)
	m_blAppendSignature = TRUE;
	m_nAppendPos = 1;
	m_blAppendTime = FALSE;
	m_blSendCenter = TRUE;
	m_blSendResize = TRUE;
	m_blPack = FALSE;
	m_nProtcol = -1;
	m_blDeleteAfterSend = FALSE;
	//}}AFX_DATA_INIT
	m_unDxlID = 0;
	m_unNxlID = 0;
	m_pDefaultNxl = NULL;
	m_blSendDirect = FALSE;
	m_cLstSendNodes.RemoveAll();
	m_cLstEntrys.RemoveAll();
	m_cLstNxls.RemoveAll();

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	m_blCheckOffForce = FALSE;
	m_blAppendSignature = pcSOboe->IsAppendSignature();
	m_blAppendTime = pcSOboe->IsAppendTime();
	m_nAppendPos = pcSOboe->GetAppendPos();
	m_blPack = pcSOboe->IsPackSend();
	m_blDeleteAfterSend = pcSOboe->IsDeleteAfterSend();

	m_cStrResult1 = _T("Yes");
	m_cStrResult2 = _T("No");
	m_blNotifyOpen = pcSOboe->IsNotifyOpen();
	m_blNotifyResult = FALSE;
	m_blOtherResult = FALSE;
	m_blNeedPassWord = FALSE;

	m_pDefaultNxl = pcSOboe->GetDefaultNetExLib();
	if( m_pDefaultNxl)
	{
		if( !m_pDefaultNxl->IsInitialied() || !m_pDefaultNxl->IsSupportSend() || !m_pDefaultNxl->IsReadySend())
		{
			m_pDefaultNxl = NULL;
		}
	}

	CNetExLib*	pcNetExLib;
	int nMax = pcSOboe->GetNxlMaxEntry();
	for( int nIndex = 0; nIndex < nMax; nIndex++)
	{
		pcNetExLib = pcSOboe->GetNetExLib( nIndex);
		if( pcNetExLib)
		{
			if( pcNetExLib->IsInitialied())
			{
				if( pcNetExLib->IsSupportSend())
				{
					if( pcNetExLib->IsReadySend())
					{
						if( NULL == m_pDefaultNxl)m_pDefaultNxl = pcNetExLib;
						m_cLstNxls.AddTail( pcNetExLib);
					}
				}
			}
		}
	}

	m_pImageList = NULL;
	m_pcBitmap = NULL;
	m_hBitmap = NULL;
}

CSendEditPage::~CSendEditPage()
{
	if( !m_cLstEntrys.IsEmpty())
	{
		CEntryData*	pcEntryData;
		while( m_cLstEntrys.GetCount())
		{
			pcEntryData = m_cLstEntrys.RemoveHead();
			delete pcEntryData;
		}
	}
	if( !m_cLstSendNodes.IsEmpty())
	{
		CEntryData*	pcEntryData;
		while( m_cLstSendNodes.GetCount())
		{
			pcEntryData = m_cLstSendNodes.RemoveHead();
			delete pcEntryData;
		}
	}

	if( m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	if( m_pcBitmap)
	{
		VERIFY( m_pcBitmap->Detach());
		delete m_pcBitmap;
		m_pcBitmap = NULL;
	}
	if( NULL != m_hBitmap)
	{
		VERIFY( DeleteObject( m_hBitmap));
		m_hBitmap = NULL;
	}
}

void CSendEditPage::SendDirect( BOOL lbDirect)
{
	m_blSendDirect = lbDirect;
}

void CSendEditPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendEditPage)
	DDX_Control(pDX, IDC_DELETEAFTERSEND, m_cBtnDeleteAfterSend);
	DDX_Control(pDX, IDC_SENDRESIZE, m_cBtnSendResize);
	DDX_Control(pDX, IDC_SENDCENTER, m_cBtnSendCenter);
	DDX_Control(pDX, IDC_APPENDTIME, m_cBtnAppendTime);
	DDX_Control(pDX, IDC_APPENDSIGNATURE, m_cBtnAppendSignature);
	DDX_Control(pDX, IDC_PACKSEND, m_cBtnPack);
	DDX_Control(pDX, IDC_EXTSETTING, m_cBtnExtSetting);
	DDX_Control(pDX, IDC_DELNODE, m_cBtnDelNode);
	DDX_Control(pDX, IDC_SENDLIST, m_cLstSendList);
	DDX_Control(pDX, IDC_PROTCOL, m_cCmbProtcolList);
	DDX_Control(pDX, IDC_NODETREE, m_cTreeNode);
	DDX_Check(pDX, IDC_APPENDSIGNATURE, m_blAppendSignature);
	DDX_Radio(pDX, IDC_APPENDHEAD, m_nAppendPos);
	DDX_Check(pDX, IDC_APPENDTIME, m_blAppendTime);
	DDX_Check(pDX, IDC_SENDCENTER, m_blSendCenter);
	DDX_Check(pDX, IDC_SENDRESIZE, m_blSendResize);
	DDX_Check(pDX, IDC_PACKSEND, m_blPack);
	DDX_CBIndex(pDX, IDC_PROTCOL, m_nProtcol);
	DDX_Check(pDX, IDC_DELETEAFTERSEND, m_blDeleteAfterSend);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_APPENDHEAD, m_cBtnAppendHead);
	DDX_Control(pDX, IDC_APPENDTAIL, m_cBtnAppendTail);
}


BEGIN_MESSAGE_MAP(CSendEditPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSendEditPage)
	ON_NOTIFY(NM_DBLCLK, IDC_NODETREE, OnDblclkNodetree)
	ON_LBN_DBLCLK(IDC_SENDLIST, OnDblclkSendlist)
	ON_LBN_SELCHANGE(IDC_SENDLIST, OnSelchangeSendlist)
	ON_BN_CLICKED(IDC_DELNODE, OnDelnode)
	ON_BN_CLICKED(IDC_EXTSETTING, OnExtsetting)
	ON_CBN_SELCHANGE(IDC_PROTCOL, OnSelchangeProtcol)
	ON_BN_CLICKED(IDC_PACKSEND, OnPacksend)
	ON_BN_CLICKED(IDC_NODELISTUPDATE, OnNodelistupdate)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_APPENDSIGNATURE, OnAppend)
	ON_NOTIFY(TVN_KEYDOWN, IDC_NODETREE, OnKeydownNodetree)
	ON_BN_CLICKED(IDC_APPENDTIME, OnAppend)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendEditPage メッセージ ハンドラ

BOOL CSendEditPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_cBtnDelNode.EnableWindow( m_cLstSendList.GetSelCount());
	m_cBtnExtSetting.EnableWindow( m_blPack);

	int nSelIndex = 0;
	CNetExLib*	pcNetExLib;
	int	nListIndex;
	for( int nIndex = 0; nIndex < m_cLstNxls.GetCount(); nIndex++)
	{
		pcNetExLib = m_cLstNxls.GetAt( m_cLstNxls.FindIndex( nIndex));
		if( pcNetExLib)
		{
			nListIndex = m_cCmbProtcolList.AddString( pcNetExLib->GetLibName());
			if( m_pDefaultNxl)
			{
				if( pcNetExLib->GetNxlID() == m_pDefaultNxl->GetNxlID())
				{
					nSelIndex = nListIndex;
				}
			}
		}
	}
	m_cCmbProtcolList.SetCurSel( nSelIndex);
	SetNodeList();

	RestoreListBox();

	m_cBtnSendCenter.EnableWindow( !m_blSendDirect);
	m_cBtnSendResize.EnableWindow( !m_blSendDirect);
	m_cBtnDeleteAfterSend.EnableWindow( !m_blSendDirect);
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CSendEditPage::SetNodeList( void)
{
	if( NULL == m_pDefaultNxl)return FALSE;

	m_unNxlID = m_pDefaultNxl->GetNxlID();

	m_cTreeNode.SetRedraw( FALSE);
	m_cTreeNode.SelectItem( NULL);
	m_cTreeNode.DeleteAllItems();

	m_cTreeNode.SetImageList( NULL, TVSIL_STATE);
	if( m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
		if( m_pcBitmap)
		{
			VERIFY( m_pcBitmap->Detach());
			delete m_pcBitmap;
			m_pcBitmap = NULL;
		}
		if( NULL != m_hBitmap)
		{
			VERIFY( DeleteObject( m_hBitmap));
			m_hBitmap = NULL;
		}
	}

	m_pImageList = new CImageList;
	if( m_pDefaultNxl->IsSupportOriginalImage())
	{
		CSize		cSize;
		HBITMAP		hBitmap;
		COLORREF	clrMask;
		if( m_pDefaultNxl->GetEntryImage( cSize, hBitmap, clrMask))
		{
			if( NULL != hBitmap)
			{
				m_pcBitmap = new CBitmap;
				if( m_pcBitmap)
				{
					m_hBitmap = hBitmap;
					m_pcBitmap->Attach( m_hBitmap);
					m_pImageList->Create( cSize.cx, cSize.cy, ILC_MASK, 0, 5);
					m_pImageList->Add( m_pcBitmap, clrMask);
				}
			}
		}
	}
	else
	{
		m_pImageList->Create( IDB_TREEIMAGE, 12, 3, RGB( 0xFF, 0xFF, 0xFF));
	}

	m_cTreeNode.SetImageList( m_pImageList, TVSIL_STATE);


	///////////////////////////////////////////////////////
	//
	// 以前の情報の削除
	//
	if( !m_cLstEntrys.IsEmpty())
	{
		CEntryData*	pcEntryData;
		while( m_cLstEntrys.GetCount())
		{
			pcEntryData = m_cLstEntrys.RemoveHead();
			delete pcEntryData;
		}
	}
	//
	// 以前の情報の削除
	//
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//
	// リストロック
	//
	m_pDefaultNxl->LockEntryList();
	//
	// リストロック
	//
	///////////////////////////////////////////////////////

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	///////////////////////////////////////////////////////
	//
	// グループの登録処理
	//
	HTREEITEM*	phTreeItem = NULL;
	int			nGroupCount = m_pDefaultNxl->IsSupportGrouping() ? m_pDefaultNxl->GetGroupCount() : 0;
	if( m_pDefaultNxl->IsSupportGrouping() && pcSOboe->EnableNxlGroup())
	{
		if( 0 < nGroupCount)
		{
			phTreeItem = new HTREEITEM[ nGroupCount];
			ZeroMemory( phTreeItem, sizeof( HTREEITEM) * nGroupCount);

			UINT		unStatus;
			CGroupData	cGroupData;
			CString		cStrGroupName;
			for( int nIndex = 0; nIndex < nGroupCount; nIndex++)
			{
				if( m_pDefaultNxl->GetGroupData( nIndex, cGroupData))
				{
					cStrGroupName = cGroupData.GetGroupName();
					cStrGroupName.TrimLeft();
					cStrGroupName.TrimRight();
					if( 0 <= cGroupData.GetParentGroup() && nGroupCount > cGroupData.GetParentGroup())
					{
						phTreeItem[ nIndex] = m_cTreeNode.InsertItem( cStrGroupName, ( ( NULL != phTreeItem[ cGroupData.GetParentGroup()]) ? phTreeItem[ cGroupData.GetParentGroup()] : TVI_ROOT));
					}
					else
					{
						phTreeItem[ nIndex] = m_cTreeNode.InsertItem( cStrGroupName);
					}
					m_cTreeNode.SetItemData( phTreeItem[ nIndex], 0);
					unStatus = cGroupData.GetStatus();
					unStatus = INDEXTOSTATEIMAGEMASK( unStatus);
					m_cTreeNode.SetItemState( phTreeItem[ nIndex], unStatus, TVIS_STATEIMAGEMASK);
				}
			}
		}
	}
	//
	// グループの登録処理
	//
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//
	// グループのソート開始
	//
	if( m_pDefaultNxl->IsSupportGrouping() && 0 < nGroupCount && pcSOboe->EnableNxlGroup())
	{
		m_cTreeNode.SortChildren( NULL);
		for( int nIndex = 0; nIndex < nGroupCount; nIndex++)
		{
			m_cTreeNode.SortChildren( phTreeItem[ nIndex]);
		}
	}
	//
	// グループのソート終了
	//
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//
	// ノードの登録処理
	//
	int nEntryCount = m_pDefaultNxl->GetEntryCount();
	if( 0 < nEntryCount)
	{
		CEntryData*	pcEntryData;
		UINT		unState;
		CString		cStrEntryName;
		HTREEITEM	hItem;

		BOOL	blNodeAdd;
		for( int nIndex = 0; nIndex < nEntryCount; nIndex++)
		{
			pcEntryData = new CEntryData;
			blNodeAdd = FALSE;
			if( m_pDefaultNxl->GetEntryData( nIndex, *pcEntryData))
			{
				m_cLstEntrys.AddTail( pcEntryData);
				unState = pcEntryData->GetStatus();
				cStrEntryName = pcEntryData->GetEntryName();
				if( m_pDefaultNxl->IsSupportGrouping() && 0 < pcEntryData->GetParentCount() && pcSOboe->EnableNxlGroup())
				{
					int	nGroup;
					for( int nIndex = 0; nIndex < pcEntryData->GetParentCount(); nIndex++)
					{
						nGroup = pcEntryData->GetParent( nIndex);
						hItem = m_cTreeNode.InsertItem( cStrEntryName, ( ( nGroupCount > nGroup && nGroup >= 0) ? phTreeItem[ nGroup] : TVI_ROOT));
						m_cTreeNode.SetItemData( hItem, DWORD( pcEntryData));
						m_cTreeNode.SetItemState( hItem, INDEXTOSTATEIMAGEMASK( unState), TVIS_STATEIMAGEMASK);
						blNodeAdd = TRUE;
					}
				}

				// ノードがグループ化されていなくて、さらにローカルでも対象となっていない場合はルートに追加
				// ってゆーか、仲間外れ？とかしたらかわいそうじゃン。
				if( TRUE != blNodeAdd)
				{
					hItem = m_cTreeNode.InsertItem( cStrEntryName);
					m_cTreeNode.SetItemData( hItem, DWORD( pcEntryData));
					m_cTreeNode.SetItemState( hItem, INDEXTOSTATEIMAGEMASK( unState), TVIS_STATEIMAGEMASK);
				}
			}
			else
			{
				delete pcEntryData;
				pcEntryData = NULL;
			}
		}
	}
	//
	// ノードの登録処理
	//
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//
	// ノードのソート開始
	//
	TVSORTCB	stTvSort;
	stTvSort.hParent = TVI_ROOT;
	stTvSort.lpfnCompare = CompareFunc;
	stTvSort.lParam = NULL;
	m_cTreeNode.SortChildrenCB( &stTvSort);
	if( m_pDefaultNxl->IsSupportGrouping() && 0 < nGroupCount && pcSOboe->EnableNxlGroup())
	{
		for( int nIndex = 0; nIndex < nGroupCount; nIndex++)
		{
			stTvSort.hParent = phTreeItem[ nIndex];
			stTvSort.lpfnCompare = CompareFunc;
			stTvSort.lParam = NULL;
			m_cTreeNode.SortChildrenCB( &stTvSort);
		}
	}
	//
	// ノードのソート終了
	//
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//
	// ノードの展開
	//
	if( pcSOboe->IsExpandSendTree() && 0 < nGroupCount && m_pDefaultNxl->IsSupportGrouping() && pcSOboe->EnableNxlGroup())
	{
		for( int nIndex = 0; nIndex < nGroupCount; nIndex++)
		{
			m_cTreeNode.Expand( phTreeItem[ nIndex], TVE_EXPAND);
		}
	}
	//
	// ノードの展開
	//
	///////////////////////////////////////////////////////


	///////////////////////////////////////////////////////
	//
	// ノードハンドルの破棄開始
	//
	if( phTreeItem)
	{
		delete [] phTreeItem;
	}
	//
	// ノードハンドルの破棄終了
	//
	///////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	//
	// リストロック解除
	//
	m_pDefaultNxl->UnlockEntryList();
	//
	// リストロック解除
	//
	///////////////////////////////////////////////////////

	m_cTreeNode.SetRedraw();
	
	return TRUE;
}

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	DWORD dwEntry1 = lParam1;
	DWORD dwEntry2 = lParam2;

	if( dwEntry1 && dwEntry2)
	{
		CString		m_cStrNode1;
		CString		m_cStrNode2;
		CEntryData*	pEntry1 = ( CEntryData*)dwEntry1;
		CEntryData*	pEntry2 = ( CEntryData*)dwEntry2;

		m_cStrNode1 = pEntry1->GetEntryName();
		m_cStrNode2 = pEntry2->GetEntryName();

		return lstrcmp( m_cStrNode1, m_cStrNode2);
	}
	else
	{
		return ( int)( dwEntry1 - dwEntry2);
	}
	return 0;
}

void CSendEditPage::OnDblclkNodetree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	HTREEITEM hTreeItem = m_cTreeNode.GetSelectedItem();
	DWORD dwEntry = ( NULL == hTreeItem) ? 0 : m_cTreeNode.GetItemData( hTreeItem);
	if( dwEntry)
	{
		AddSendList( ( CEntryData*)dwEntry);
	}
	else
	{
		if( GetAsyncKeyState( VK_SHIFT) & ~1)
		{
			if( m_cTreeNode.ItemHasChildren( hTreeItem))
			{
				LoopSearch( hTreeItem);
			}
			*pResult = 1;
			return;
		}
	}
	
	*pResult = 0;
}

void CSendEditPage::LoopSearch( HTREEITEM hSearchTop)
{
	DWORD		dwEntry;
	HTREEITEM hChild = m_cTreeNode.GetChildItem( hSearchTop);
	while( hChild)
	{
		if( m_cTreeNode.ItemHasChildren( hChild))
		{
			LoopSearch( hChild);
		}
		else
		{
			dwEntry = m_cTreeNode.GetItemData( hChild);
			if( dwEntry)
			{
				AddSendList( ( CEntryData*)dwEntry);
			}
		}
		hChild = m_cTreeNode.GetNextItem( hChild, TVGN_NEXT);
	}
}

BOOL CSendEditPage::AddSendList( const CEntryData* pcEntryData, BOOL blListBox)
{
	BOOL	blAdd = TRUE;

	CEntryData*	pcEntryItem;
	int nCount = m_cLstSendNodes.GetCount();
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pcEntryItem = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
		if( *pcEntryItem == *pcEntryData)
		{
			blAdd = FALSE;
			break;
		}
	}

	if( blAdd)
	{
		CString	cStrNodeName;
		cStrNodeName = pcEntryData->GetEntryName();

		CEntryData*	pcEntryItem;
		pcEntryItem = new CEntryData;
		if( pcEntryItem)
		{
			*pcEntryItem = *pcEntryData;
			if( blListBox)
			{
				m_cLstSendList.AddString( cStrNodeName);
			}
			m_cLstSendNodes.AddTail( pcEntryItem);
			return TRUE;
		}
	}
	return FALSE;
}

void CSendEditPage::OnDblclkSendlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int nSelIndex = m_cLstSendList.GetCurSel();
	if( LB_ERR != nSelIndex)
	{
		m_cLstSendList.DeleteString( nSelIndex);

		CEntryData*	pcEntryItem;
		POSITION pos = m_cLstSendNodes.FindIndex( nSelIndex);
		if( NULL != pos)
		{
			pcEntryItem = m_cLstSendNodes.GetAt( pos);
			m_cLstSendNodes.RemoveAt( pos);
			delete pcEntryItem;
		}
	}
	m_cBtnDelNode.EnableWindow( m_cLstSendList.GetSelCount());
}

void CSendEditPage::OnSelchangeSendlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_cBtnDelNode.EnableWindow( m_cLstSendList.GetSelCount());
}

void CSendEditPage::OnDelnode() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int		nCount = m_cLstSendList.GetSelCount();
	if( 0 < nCount)
	{
		int*	pSels;
		pSels = new int[ nCount];
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pSels[ nIndex] = -1;
		}
		m_cLstSendList.GetSelItems( nCount, pSels);
		for( int nIndex = nCount - 1; nIndex >= 0; nIndex--)
		{
			if( -1 != pSels[ nIndex])
			{
				m_cLstSendList.DeleteString( pSels[ nIndex]);

				CEntryData*	pcEntryItem;
				POSITION pos = m_cLstSendNodes.FindIndex( pSels[ nIndex]);
				if( NULL != pos)
				{
					pcEntryItem = m_cLstSendNodes.GetAt( pos);
					m_cLstSendNodes.RemoveAt( pos);
					delete pcEntryItem;
				}
			}
		}
		delete [] pSels;
	}
	m_cBtnDelNode.EnableWindow( m_cLstSendList.GetSelCount());
}

int CSendEditPage::GetEntryCount( void)
{
	return m_cLstSendNodes.GetCount();
}

const CEntryData* CSendEditPage::GetEntryData( int nIndex)
{
	if( m_cLstSendNodes.GetCount() <= nIndex)return NULL;

	return m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
}

void CSendEditPage::OnExtsetting() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BOOL blNormal = IsNormalDxl();

	CExtSettingDialog	cExtDlg( blNormal, this);

	cExtDlg.m_cStrResult1 = m_cStrResult1;
	cExtDlg.m_cStrResult2 = m_cStrResult2;
	cExtDlg.m_blNotifyOpen = blNormal ? m_blNotifyOpen : FALSE;
	cExtDlg.m_blNotifyResult = blNormal ? m_blNotifyResult : FALSE;
	cExtDlg.m_blOtherResult = blNormal ? m_blOtherResult : FALSE;
	cExtDlg.m_blNeedPassWord = m_blNeedPassWord;

	if( IDOK == cExtDlg.DoModal())
	{
		m_cStrResult1 = cExtDlg.m_cStrResult1;
		m_cStrResult2 = cExtDlg.m_cStrResult2;
		m_blNotifyOpen = blNormal ? cExtDlg.m_blNotifyOpen : FALSE;
		m_blNotifyResult = blNormal ? cExtDlg.m_blNotifyResult : FALSE;
		m_blOtherResult = blNormal ? cExtDlg.m_blOtherResult : FALSE;
		m_blNeedPassWord = cExtDlg.m_blNeedPassWord;
	}
}

void CSendEditPage::OnSelchangeProtcol() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		CNetExLib*	pcNetExLib;
		pcNetExLib = m_cLstNxls.GetAt( m_cLstNxls.FindIndex( m_nProtcol));
		if( pcNetExLib)
		{
			m_pDefaultNxl = pcNetExLib;
			SetNodeList();
		}
	}
}

void CSendEditPage::OnPacksend() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( UpdateData())
	{
		m_cBtnExtSetting.EnableWindow( m_blPack);
	}
}

BOOL CSendEditPage::AddEntryData( const CEntryData* pcEntryData)
{
	return AddSendList( pcEntryData, FALSE);
}

void CSendEditPage::RestoreListBox( void)
{
	if( !m_cLstSendNodes.IsEmpty())
	{
		int	nCount = m_cLstSendNodes.GetCount();

		CString	cStrNodeName;
		CEntryData*	pcEntry;
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcEntry = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
			if( pcEntry)
			{
				cStrNodeName = pcEntry->GetEntryName();
				m_cLstSendList.AddString( cStrNodeName);
			}
		}
	}
}

void CSendEditPage::OnOK() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->AppendSignature( m_blAppendSignature);
	pcSOboe->AppendTime( m_blAppendTime);
	pcSOboe->SetAppendPos( m_nAppendPos);
	
	CPropertyPage::OnOK();
}

BOOL CSendEditPage::OnSetActive() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	/*
	UpdateData();
	BOOL blExSend = TRUE;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( 0 != m_unDxlID)
	{
		int nIndex = pcSOboe->FindDxlID( m_unDxlID);
		if( 0 <= nIndex)
		{
			const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				blExSend = pcDrawExLib->IsSupportExSend();
			}
		}
	}

	if( blExSend)
	{
		if( m_blCheckOffForce)
		{
			m_blAppendSignature = pcSOboe->IsAppendSignature();
			m_blAppendTime = pcSOboe->IsAppendTime();
			m_blPack = pcSOboe->IsPackSend();
			UpdateData( FALSE);
		}
		m_blCheckOffForce = FALSE;
	}
	else
	{
		m_blCheckOffForce = TRUE;
		m_blAppendSignature = FALSE;
		m_blAppendTime = FALSE;
		m_blPack = FALSE;
		UpdateData( FALSE);
	}
	
//	m_cBtnPack.EnableWindow( blExSend);
	m_cBtnExtSetting.EnableWindow( m_blPack && blExSend);
	m_cBtnAppendSignature.EnableWindow( blExSend);
	m_cBtnAppendTime.EnableWindow( blExSend);
	m_cBtnAppendHead.EnableWindow( blExSend && ( m_blAppendSignature || m_blAppendTime));
	m_cBtnAppendTail.EnableWindow( blExSend && ( m_blAppendSignature || m_blAppendTime));
	*/

	UpdateData();

	BOOL blExSend = IsNormalDxl();
	if( blExSend)
	{
		if( m_blCheckOffForce)
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);

			m_blAppendSignature = pcSOboe->IsAppendSignature();
			m_blAppendTime = pcSOboe->IsAppendTime();
			m_blPack = pcSOboe->IsPackSend();
			UpdateData( FALSE);
		}
		m_blCheckOffForce = FALSE;
	}
	else
	{
		m_blCheckOffForce = TRUE;
		m_blAppendSignature = FALSE;
		m_blAppendTime = FALSE;
		m_blPack = FALSE;
		UpdateData( FALSE);
	}
	
	m_cBtnPack.EnableWindow( TRUE);
	m_cBtnExtSetting.EnableWindow( m_blPack);
	m_cBtnAppendSignature.EnableWindow( blExSend);
	m_cBtnAppendTime.EnableWindow( blExSend);
	m_cBtnAppendHead.EnableWindow( blExSend && ( m_blAppendSignature || m_blAppendTime));
	m_cBtnAppendTail.EnableWindow( blExSend && ( m_blAppendSignature || m_blAppendTime));

	UpdateData( FALSE);
	
	return CPropertyPage::OnSetActive();
}

void CSendEditPage::OnNodelistupdate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	BeginWaitCursor();
	EnableWindow( FALSE);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe)
	{
		pcSOboe->m_pMainWnd->PostMessage( WM_COMMAND, IDM_CLIENTRESERCH, 0L);
		m_cTreeNode.SetRedraw( FALSE);
		m_cTreeNode.SelectItem( NULL);
		m_cTreeNode.DeleteAllItems();
		CString	cStr;
		cStr.LoadString( IDS_RESEARCH_MSG1);
		m_cTreeNode.InsertItem( cStr, TVI_ROOT);
		cStr.LoadString( IDS_RESEARCH_MSG2);
		m_cTreeNode.InsertItem( cStr, TVI_ROOT);
		cStr.LoadString( IDS_RESEARCH_MSG3);
		m_cTreeNode.InsertItem( cStr, TVI_ROOT);
		if( 0 != ( GetAsyncKeyState( VK_SHIFT) & ~1))
		{
			UINT unSRand = GetTickCount();
			srand( unSRand);
			int nOmiku = rand() % 13;
			CString	cStr;
			cStr.LoadString( IDS_FORTUNE);
			CString	cStrResult;
			switch( nOmiku)
			{
			case 0:
				cStrResult.LoadString( IDS_FORTUNE_RESULT1);
				break;
			case 1:
			case 2:
				cStrResult.LoadString( IDS_FORTUNE_RESULT2);
				break;
			case 3:
			case 4:
			case 5:
				cStrResult.LoadString( IDS_FORTUNE_RESULT3);
				break;
			case 6:
			case 7:
			case 8:
				cStrResult.LoadString( IDS_FORTUNE_RESULT4);
				break;
			case 9:
			case 10:
			case 11:
				cStrResult.LoadString( IDS_FORTUNE_RESULT5);
				break;
			case 12:
			default:
				cStrResult.LoadString( IDS_FORTUNE_RESULT6);
				break;
			}
			cStr += cStrResult;
			m_cTreeNode.InsertItem( cStr, TVI_ROOT);
		}
		m_cTreeNode.SetRedraw();
	}

	SetTimer( 4040, 4000, NULL);
}

void CSendEditPage::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	KillTimer( 4040);
	SetNodeList();
	
	EnableWindow( TRUE);
	EndWaitCursor();
	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CSendEditPage::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( WM_KEYDOWN == pMsg->message)
	{
		switch( pMsg->wParam)
		{
		case 'A':
			if( m_cLstSendList == *GetFocus())
			{
				if( 0 != ( GetKeyState( VK_CONTROL) & ~1))
				{
					ASSERT( m_pDefaultNxl);
					CEntryData	cEntryData;
					if( m_pDefaultNxl->DoInputEntry( GetSafeHwnd(), cEntryData))
					{
						AddSendList( &cEntryData);
					}
					return TRUE;
				}
			}
			break;
		case VK_DELETE:
			if( m_cLstSendList == *GetFocus())
			{
				OnDelnode();
			}
			break;
		}
	}
	
	return CPropertyPage::PreTranslateMessage(pMsg);
}

void CSendEditPage::OnAppend() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	m_cBtnAppendHead.EnableWindow( m_blAppendSignature || m_blAppendTime);
	m_cBtnAppendTail.EnableWindow( m_blAppendSignature || m_blAppendTime);
}

void CSendEditPage::OnKeydownNodetree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if( VK_SPACE == pTVKeyDown->wVKey || VK_PROCESSKEY == pTVKeyDown->wVKey)
	{
		OnDblclkNodetree( NULL, pResult);
		*pResult = 1;
		return;
	}
	
	*pResult = 0;
}

void CSendEditPage::OnDestroy() 
{
	CPropertyPage::OnDestroy();
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	KillTimer( 4040);
}

BOOL CSendEditPage::IsNormalDxl( void)
{
	BOOL blExSend = TRUE;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( 0 != m_unDxlID)
	{
		int nIndex = pcSOboe->FindDxlID( m_unDxlID);
		if( 0 <= nIndex)
		{
			const CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				blExSend = pcDrawExLib->IsSupportExSend();
			}
		}
	}

	return blExSend;
}
