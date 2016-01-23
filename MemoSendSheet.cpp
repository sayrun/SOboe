#include "stdafx.h"
#include "soboe.h"
#include "MemoSendSheet.h"

IMPLEMENT_DYNAMIC( CMemoSendSheet, CMemoEditSheet)

CMemoSendSheet::CMemoSendSheet( UINT nIDCaption, CWnd* pParentWnd, BOOL blSendDirect, UINT iSelectPage)
	: CMemoEditSheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_blSendDirect = blSendDirect;
	m_cSendEditPage.SendDirect( blSendDirect);
	AddPage( &m_cSendEditPage);

	SetActivePage( &m_cSendEditPage);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	m_cBasePage.m_blDelOnQuit = pcSOboe->IsCheckDelOnQuit();
	ResizeLock( TRUE);
}

CMemoSendSheet::CMemoSendSheet( LPCTSTR pszCaption, CWnd* pParentWnd, BOOL blSendDirect, UINT iSelectPage)
	: CMemoEditSheet(pszCaption, pParentWnd, iSelectPage)
{
	m_blSendDirect = blSendDirect;
	m_cSendEditPage.SendDirect( blSendDirect);
	AddPage( &m_cSendEditPage);

	SetActivePage( &m_cSendEditPage);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	m_cBasePage.m_blDelOnQuit = pcSOboe->IsCheckDelOnQuit();
	ResizeLock( TRUE);
}

BEGIN_MESSAGE_MAP(CMemoSendSheet, CMemoEditSheet)
	//{{AFX_MSG_MAP(CMemoSendSheet)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMemoSendSheet::GetSendData( INNERSENDDATA& stInnerSendData)
{
	if( IsMemoEmpty())return FALSE;

	stInnerSendData.m_cStrFrom.Empty();

	stInnerSendData.m_cStrDistribute.Empty();
	int nCount = m_cSendEditPage.GetEntryCount();
	const CEntryData*	pcEntry;
	for( int nIndex = 0; nIndex < nCount; nIndex++)
	{
		pcEntry = m_cSendEditPage.GetEntryData( nIndex);
		if( pcEntry)
		{
			if( !stInnerSendData.m_cStrDistribute.IsEmpty())
			{
				stInnerSendData.m_cStrDistribute += _T ( ",");
			}
			stInnerSendData.m_cStrDistribute += pcEntry->GetEntryName();
		}
	}

	stInnerSendData.m_blAppendSig = m_cSendEditPage.m_blAppendSignature;
	stInnerSendData.m_nAppendPos = m_cSendEditPage.m_nAppendPos;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	stInnerSendData.m_cStrSignature = pcSOboe->GetSignature();

	stInnerSendData.m_blAppendSendTime = m_cSendEditPage.m_blAppendTime;

	stInnerSendData.m_blSendResize = m_cSendEditPage.m_blSendResize;
	stInnerSendData.m_blSendCenter = m_cSendEditPage.m_blSendCenter;

	stInnerSendData.m_blPack = m_cSendEditPage.m_blPack;
		stInnerSendData.m_blPassWord = m_cSendEditPage.m_blNeedPassWord;
		stInnerSendData.m_blCheckOpen = m_cSendEditPage.m_blNotifyOpen;
	stInnerSendData.m_blChackResult = m_cSendEditPage.m_blNotifyResult;
		if( stInnerSendData.m_blCheckOpen || stInnerSendData.m_blChackResult)
		{
			SYSTEMTIME	stSysTime;
			GetLocalTime( &stSysTime);
			stInnerSendData.m_cStrResultID.Format( "%d%d%d%d%d%d:%d", stSysTime.wYear, stSysTime.wMonth, stSysTime.wDay, stSysTime.wHour, stSysTime.wMinute, stSysTime.wSecond, GetTickCount());
		}
		else
		{
			stInnerSendData.m_cStrResultID.Empty();
		}
		stInnerSendData.m_stResults.m_cStrResult1 = m_cSendEditPage.m_cStrResult1;
		stInnerSendData.m_stResults.m_cStrResult2 = m_cSendEditPage.m_cStrResult2;
		stInnerSendData.m_stResults.m_blOtherResult = m_cSendEditPage.m_blOtherResult;
			stInnerSendData.m_stResults.m_cStrResult3.Empty();

	stInnerSendData.m_blDelOnQuit = IsDelOnQuit();

	stInnerSendData.m_nCreateYear;
	stInnerSendData.m_nCreateMonth;
	stInnerSendData.m_nCreateDay;

	if( pcSOboe->IsNofityVersion())
	{
		stInnerSendData.m_unVersion = VERSION_CODE;
		stInnerSendData.m_cStrVersion.LoadString( IDS_VERSION);
	}
	else
	{
		stInnerSendData.m_unVersion = 0;
		stInnerSendData.m_cStrVersion.Empty();
	}

	GetMemoData( stInnerSendData.m_cMemoData);

	// タイトルが無ければ、署名をタイトルにする！
	if( m_blSendDirect)
	{
		CString	cStr;
		if( FALSE == stInnerSendData.m_cMemoData.GetTitle( cStr))
		{
			cStr = pcSOboe->GetSignature();
			cStr.TrimLeft();
			cStr.TrimRight();
			if( !cStr.IsEmpty())
			{
				stInnerSendData.m_cMemoData.SetTitle( cStr);
			}
		}
	}

	stInnerSendData.m_cStrDxlDummy.Empty();
	if( 0 != stInnerSendData.m_cMemoData.GetDxlID())
	{
		int nIndex = pcSOboe->FindDxlID( stInnerSendData.m_cMemoData.GetDxlID());
		if( 0 <= nIndex)
		{
			CDrawExLib* pcDrawExLib = pcSOboe->GetDrawExLib( nIndex);
			if( pcDrawExLib)
			{
				EDITDATA	stEditData;
				stEditData.m_nSelStart = 0;
				stEditData.m_nSelEnd = 0;
				stEditData.m_hEdit = NULL;
				stEditData.m_nSize = 0;
				stEditData.m_hSaveData = NULL;

				CString	cStrMemo;
				cStrMemo = stInnerSendData.m_cMemoData.GetMemo();
				if( !cStrMemo.IsEmpty())
				{
					stEditData.m_hEdit = GlobalAlloc( GHND, cStrMemo.GetLength() + 1);
					LPSTR lpsz = ( LPSTR)GlobalLock( stEditData.m_hEdit);
					lstrcpy( lpsz, cStrMemo);
					GlobalUnlock( stEditData.m_hEdit);
				}
				if( pcDrawExLib->IsSupportNativeData())
				{
					stEditData.m_nSize = stInnerSendData.m_cMemoData.GetDxlExtData();
					if( stEditData.m_nSize)
					{
						stEditData.m_hSaveData = GlobalAlloc( GHND, stEditData.m_nSize);
						LPBYTE lpbyData = ( LPBYTE)GlobalLock( stEditData.m_hSaveData);
						stInnerSendData.m_cMemoData.GetDxlExtData( lpbyData, stEditData.m_nSize);
						GlobalUnlock( stEditData.m_hSaveData);
					}
					else
					{
						stEditData.m_hSaveData = NULL;
					}
				}
				else
				{
					stEditData.m_nSize = 0;
					stEditData.m_hSaveData = NULL;
				}

				if( 0 < pcDrawExLib->ExchangeSendString( &stEditData))
				{
					LPSTR lpsz = ( LPSTR)GlobalLock( stEditData.m_hEdit);
					stInnerSendData.m_cStrDxlDummy = lpsz;
					GlobalUnlock( stEditData.m_hEdit);
				}
				else
				{
					stInnerSendData.m_cStrDxlDummy = stInnerSendData.m_cMemoData.GetMemo();
				}

				GlobalFree( stEditData.m_hEdit);
				GlobalFree( stEditData.m_hSaveData);
			}
		}
	}

	return TRUE;
}

BOOL CMemoSendSheet::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	switch( pHelpInfo->iCtrlId)
	{
	case IDC_PROTCOL:
		if( 0 != m_cSendEditPage.m_unNxlID)
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			int nIndex = pcSOboe->FindNxlID( m_cSendEditPage.m_unNxlID);
			if( 0 <= nIndex)
			{
				CNetExLib* pcNetExLib = pcSOboe->GetNetExLib( nIndex);
				if( pcNetExLib)
				{
					pcNetExLib->About( GetSafeHwnd());
				}
			}
			return TRUE;
		}
	//
	case IDC_NODETREE:
	case IDC_SENDLIST:
	case IDC_NODELISTUPDATE:
	case IDC_DELNODE:
	case IDC_SENDCENTER:
	case IDC_SENDRESIZE:
	case IDC_APPENDTIME:
	case IDC_PACKSEND:
	case IDC_APPENDSIGNATURE:
	case IDC_EXTSETTING:
	case IDC_APPENDHEAD:
	case IDC_APPENDTAIL:
	case IDC_DELETEAFTERSEND:
		WinHelp( pHelpInfo->iCtrlId | 0x60000, HELP_CONTEXTPOPUP);
		return TRUE;
	default:
		break;
	}
	
	return CMemoEditSheet::OnHelpInfo(pHelpInfo);
}
