// SendWorkerDialog.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include <process.h>
#include "soboe.h"
#include "SendWorkerDialog.h"
#include "NetExLib.h"
#include "ResultListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendWorkerDialog ダイアログ

typedef struct tagSENDPARAM
{
	INNERSENDDATA							stInnerSendData;
	CList < CEntryData*, CEntryData*&>*		pcLstSendNodes;
	HWND									hWnd;
	UINT									unNotifyMessage;
	HANDLE									hCancelEvent;
}SENDPARAM;

void DataSendThread( void* lpvData)
{
	SENDPARAM*	pSendParam = ( SENDPARAM*)lpvData;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CEntryData*		pcEntryMaster;
	CEntryData*		pcEntrySlave;
	CNetExLib*		pcNetExLib;
	int				nResult;
	int nCount = pSendParam->pcLstSendNodes->GetCount();
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pcEntryMaster = pSendParam->pcLstSendNodes->GetAt( pSendParam->pcLstSendNodes->FindIndex( nIndex));
		ASSERT( pcEntryMaster);
		pcNetExLib = pcSOboe->GetNetExLib( pcSOboe->FindNxlID( pcEntryMaster->GetNxlID()));
		if( pcNetExLib->IsSupportBulkSend())
		{
			CList < CEntryData*, CEntryData*&>	cWorkList;
			cWorkList.RemoveAll();
			for( int nIDCheckIndex = nIndex; nIDCheckIndex < nCount; nIDCheckIndex++)
			{
				pcEntrySlave = pSendParam->pcLstSendNodes->GetAt( pSendParam->pcLstSendNodes->FindIndex( nIDCheckIndex));
				ASSERT( pcEntrySlave);
				if( pcEntryMaster->GetNxlID() == pcEntrySlave->GetNxlID())
				{
					cWorkList.AddTail( pcEntrySlave);
				}
				else
				{
					break;
				}
			}
			int		nEntryCount = cWorkList.GetCount();

			int*	pnResult;
			pnResult = new int [ nEntryCount];
			ASSERT( pnResult);
			ZeroMemory( pnResult, sizeof( int) * nEntryCount);
			nResult = pcNetExLib->SendMemoDataInBulk( cWorkList, pnResult, nEntryCount, &pSendParam->stInnerSendData, pSendParam->hCancelEvent);
			for( int nResultCheckIndex = 0; nResultCheckIndex < nEntryCount; nResultCheckIndex++)
			{
				PostMessage( pSendParam->hWnd, pSendParam->unNotifyMessage, pnResult[ nResultCheckIndex], ( LPARAM)nIndex + nResultCheckIndex);
			}
			delete [] pnResult;
			pnResult = NULL;

			nIndex += ( nEntryCount - 1);	// 取り出した分だけインクリメントする
		}
		else
		{
			nResult = pcNetExLib->SendMemoData( pcEntryMaster, &pSendParam->stInnerSendData, pSendParam->hCancelEvent);
			PostMessage( pSendParam->hWnd, pSendParam->unNotifyMessage, nResult, ( LPARAM)nIndex);
		}
		if( WAIT_OBJECT_0 == WaitForSingleObject( pSendParam->hCancelEvent, 0))break;
	}

	delete pSendParam;

	_endthread();
}

UINT CSendWorkerDialog::sm_unNotifyEnd = RegisterWindowMessage( "SOBOE#MSG!FROM-SENDTHREAD");

CSendWorkerDialog::CSendWorkerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSendWorkerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendWorkerDialog)
	m_nLstSendResult = -1;
	m_blAutoClose = FALSE;
	//}}AFX_DATA_INIT
	m_blCanceled = FALSE;
	m_cLstSendNodes.RemoveAll();
	m_hCancelEvent = NULL;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	m_blAutoClose = pcSOboe->IsSendDlgAutoClose();
}

CSendWorkerDialog::~CSendWorkerDialog( void)
{
	if( !m_cLstSendNodes.IsEmpty())
	{
		CEntryData*	pcEntry;
		while( m_cLstSendNodes.GetCount())
		{
			pcEntry = m_cLstSendNodes.RemoveHead();
			delete pcEntry;
		}
	}
	if( m_hCancelEvent)
	{
		CloseHandle( m_hCancelEvent);
	}
}

void CSendWorkerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendWorkerDialog)
	DDX_Control(pDX, IDOK, m_cBtnOK);
	DDX_Control(pDX, IDCANCEL, m_cBtnCancel);
	DDX_Control(pDX, IDC_SENDRESULTLIST, m_cLstSendResult);
	DDX_LBIndex(pDX, IDC_SENDRESULTLIST, m_nLstSendResult);
	DDX_Check(pDX, IDC_AUTOCLOSE, m_blAutoClose);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSendWorkerDialog, CDialog)
	//{{AFX_MSG_MAP(CSendWorkerDialog)
	ON_LBN_DBLCLK(IDC_SENDRESULTLIST, OnDblclkSendresultlist)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_AUTOCLOSE, OnAutoclose)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( sm_unNotifyEnd, OnNotifyEnd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendWorkerDialog メッセージ ハンドラ

BOOL CSendWorkerDialog::SetSendData( const INNERSENDDATA& stInnerSendData)
{
	m_stInnerSendData = stInnerSendData;

	return TRUE;
}

BOOL CSendWorkerDialog::AddSendEntry( const CEntryData* pcEntry)
{
	if( NULL != pcEntry)
	{
		CEntryData*	pcEntryNode;
		pcEntryNode = new CEntryData;
		if( pcEntryNode)
		{
			*pcEntryNode = *pcEntry;

			CEntryData*	pcEntryData;
			int nCount = m_cLstSendNodes.GetCount();
			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pcEntryData = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
				if( pcEntryData)
				{
					if( pcEntryNode->GetNxlID() == pcEntryData->GetNxlID())
					{
						for( ; nIndex < nCount; nIndex++)
						{
							pcEntryData = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
							if( pcEntryNode->GetNxlID() != pcEntryData->GetNxlID())
							{
								m_cLstSendNodes.InsertBefore( m_cLstSendNodes.FindIndex( nIndex), pcEntryNode);
								return TRUE;
							}
						}
					}
				}
			}
			m_cLstSendNodes.AddTail( pcEntryNode);
			return TRUE;
		}
	}
	return FALSE;
}

LONG CSendWorkerDialog::OnNotifyEnd( UINT wParam, LPARAM lParam)
{
	CSingleLock	cLickObj( &m_cNotifyCS);
	if( cLickObj.Lock())
	{
		int	nNotifyIndex = ( int)lParam;
		POSITION psEntry = m_cLstSendNodes.FindIndex( nNotifyIndex);
		if( psEntry)
		{
			CEntryData*	pcEntryNode;
			pcEntryNode = m_cLstSendNodes.GetAt( psEntry);
			if( pcEntryNode)
			{
				int		nEndCode = wParam;
				CString	cStr;
				int		nPos;
				cStr = ( _SEND_SUCCESS == nEndCode) ? "OK-" : ( ( _SEND_ABSENCE == nEndCode) ? "NH-" : "NG-");
				cStr += pcEntryNode->GetEntryName();
				nPos = m_cLstSendResult.InsertString( nNotifyIndex, cStr);
				m_cLstSendResult.DeleteString( nNotifyIndex + 1);
				// エラーコード保存
				m_cLstSendResult.SetItemData( nPos, nEndCode);

				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				if( pcSOboe->IsLoggingSendChip())
				{
					CString	cPathName;
					cPathName = pcSOboe->GetSendLogFilePath();
					CFile	cLogFile;
					if( cLogFile.Open( cPathName, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
					{
						if( ( 1024 * 1024)/*MAX_FILESIZE*/ <= cLogFile.GetLength())
						{
							CString	cStr;
							cStr.LoadString( IDS_DELLOGTOOBIG);
							if( IDYES == MessageBox( cStr, NULL, MB_YESNO | MB_ICONQUESTION))
							{
								cLogFile.SetLength( 0);
							}
						}
						cLogFile.Seek( 0, CFile::end);
						CString	cStrLog;
						CString	cStrSendTime;
						SYSTEMTIME	stSysTime;
						GetLocalTime( &stSysTime);
						COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
						cStrSendTime = cTime.Format( "%y/%m/%d - %H:%M:%S");
						CString	cStrTitle;
						m_stInnerSendData.m_cMemoData.GetTitle( cStrTitle);
						cStrLog.Format( "%s - %s\r\nTo : %s\r\nData：%s", cStrSendTime, ( 0 == nEndCode) ? "Success" : "Failed!", pcEntryNode->GetEntryName(), cStrTitle);
						cLogFile.Write( cStrLog, cStrLog.GetLength());
						if( pcSOboe->IsFullLoggingSendChip())
						{
							cStrLog = "\r\nMemo\r\n";
							cLogFile.Write( cStrLog, cStrLog.GetLength());
							cLogFile.Write( m_stInnerSendData.m_cMemoData.GetMemo(), m_stInnerSendData.m_cMemoData.GetMemoLength());
						}
						cStrLog = "\r\n-----------------------------------\r\n";
						cLogFile.Write( cStrLog, cStrLog.GetLength());
						cLogFile.Close();
					}
				}
			}

			if( ( nNotifyIndex + 1) == m_cLstSendNodes.GetCount() || TRUE == m_blCanceled)
			{
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);
				if( pcSOboe->IsSendDlgAutoClose())
				{
					BOOL	blAllSuccess = TRUE;
					int nCount = m_cLstSendNodes.GetCount();
					for( int nIndex = 0; nIndex < nCount; nIndex++)
					{
						if( _SEND_SUCCESS != m_cLstSendResult.GetItemData( nIndex))
						{
							blAllSuccess = FALSE;
						}
					}
					if( blAllSuccess)
					{
						PostMessage( WM_COMMAND, IDOK);
						return 0;
					}
					/*
					else
					{
						SetForegroundWindow();
						BringWindowToTop();
					}
					*/
				}

				m_cBtnCancel.EnableWindow( FALSE);
				m_cBtnOK.EnableWindow( TRUE);

				CString	cStr;
				int	nPos;
				if( TRUE == m_blCanceled)
				{
					cStr.LoadString( IDS_SENDNOWCANCEL);
					nPos = m_cLstSendResult.FindStringExact( -1, cStr);
					if( LB_ERR != nPos)
					{
						m_cLstSendResult.DeleteString( nPos);
					}
					cStr.LoadString( IDS_SENDCANCELED);
				}
				else
				{
					cStr.LoadString( IDS_SENDEND);
				}
				nPos = m_cLstSendResult.AddString( cStr);
				m_cLstSendResult.SetItemData( nPos, 0);
			}
		}
		cLickObj.Unlock();
	}

	return 0;
}

BOOL CSendWorkerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	m_hCancelEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	ResetEvent( m_hCancelEvent);

	int	nCount = m_cLstSendNodes.GetCount();
	if( 0 < nCount)
	{
		m_cBtnCancel.EnableWindow( TRUE);
		m_cBtnOK.EnableWindow( FALSE);

		CEntryData*	pcEntryNode;
		int			nPos;
		CString		cStr;
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcEntryNode = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( nIndex));
			cStr.Format( "?\?-%s", pcEntryNode->GetEntryName());
			nPos = m_cLstSendResult.AddString( cStr);
			m_cLstSendResult.SetItemData( nPos, 0);
		}
		DoSend();
	}
	else
	{
		m_cBtnCancel.EnableWindow( FALSE);
		m_cBtnOK.EnableWindow( TRUE);
		m_cLstSendResult.EnableWindow( FALSE);
	}
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

BOOL CSendWorkerDialog::DoSend( void)
{
	ASSERT( m_cLstSendNodes.GetCount());

	SENDPARAM*	pstSendParam;

	pstSendParam = new SENDPARAM;
	if( pstSendParam)
	{
		pstSendParam->stInnerSendData	= m_stInnerSendData;
		pstSendParam->pcLstSendNodes	= &m_cLstSendNodes;
		pstSendParam->hWnd				= GetSafeHwnd();
		pstSendParam->unNotifyMessage	= sm_unNotifyEnd;
		pstSendParam->hCancelEvent		= m_hCancelEvent;

		if( -1 != _beginthread( DataSendThread, 0, pstSendParam))
		{
			return TRUE;
		}
		delete pstSendParam;
	}

	return FALSE;
}

void CSendWorkerDialog::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	delete this;
}

void CSendWorkerDialog::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	if( m_hCancelEvent)
	{
		CString	cStr;
		cStr.LoadString( IDS_SENDNOWCANCEL);
		int	nPos = m_cLstSendResult.AddString( cStr);
		m_cLstSendResult.SetItemData( nPos, -1);

		m_blCanceled = TRUE;
		m_cBtnCancel.EnableWindow( FALSE);
		SetEvent( m_hCancelEvent);
	}
	//CDialog::OnCancel();
}

void CSendWorkerDialog::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	DestroyWindow();
	
//	CDialog::OnOK();
}

void CSendWorkerDialog::OnDblclkSendresultlist() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	if( 0 <= m_nLstSendResult)
	{
		if( 0 <= m_nLstSendResult && m_cLstSendNodes.GetCount() > m_nLstSendResult)
		{
			int nResultCode = int(  m_cLstSendResult.GetItemData( m_nLstSendResult));
			if( 0 < nResultCode)
			{
				CSOboeApp*	pcSOboe;
				pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);

				CEntryData*	pcEntryNode;
				pcEntryNode = m_cLstSendNodes.GetAt( m_cLstSendNodes.FindIndex( m_nLstSendResult));
				if( pcEntryNode)
				{
					UINT unNxlID = pcEntryNode->GetNxlID();

					CNetExLib*		pcNetExLib;
					pcNetExLib = pcSOboe->GetNetExLib( pcSOboe->FindNxlID( unNxlID));
					CString	cStrErrorMsg;
					pcNetExLib->GetErrorMsg( nResultCode, cStrErrorMsg);

					MessageBox( cStrErrorMsg);
				}
			}
		}
	}
}

BOOL CSendWorkerDialog::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_SENDRESULTLIST:
	case IDC_AUTOCLOSE:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CDialog::OnHelpInfo(pHelpInfo);
}

void CSendWorkerDialog::OnAutoclose() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	UpdateData();
	
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pcSOboe->SendDlgAutoClose( m_blAutoClose);
}
