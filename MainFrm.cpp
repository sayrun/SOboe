// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "SOboe.h"
#include "MemoData.h"
#include "MemoChip.h"
#include "ChipArg_IF.h"
#include "MemoEditSheet.h"
#include "MemoSendSheet.h"
#include "MemoChipList.h"
#include "DataListDialog.h"
#include "AboutDialog.h"
#include "DelListDialog.h"
#include "SendWorkerDialog.h"
#include "NotifyWnd.h"
#include "ResizableMemoEditDialog.h"
#include "PrintChipDialog.h"
#include "CatalogList.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame
UINT CMainFrame::sm_unMsgFromTray = RegisterWindowMessage( "SOBOE#MSG!FROM-TRAY");
UINT CMainFrame::sm_unMsgFromMemo = RegisterWindowMessage( "SOBOE#MSG!FROM-MEMO");
// explorerの蘇生対応
UINT CMainFrame::sm_unMsgFromExplorer = ::RegisterWindowMessage( "TaskbarCreated");


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_MESSAGE( WM_POWERBROADCAST, OnPowerBroadcast)
	ON_MESSAGE( WM_HOTKEY, OnHotKey)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(IDM_NEWMEMO, OnNewmemo)
	ON_COMMAND(IDM_DATALIST, OnDatalist)
	ON_COMMAND(IDM_PASTE, OnPaste)
	ON_UPDATE_COMMAND_UI(IDM_PASTE, OnUpdatePaste)
	ON_UPDATE_COMMAND_UI(IDM_NEWMEMO, OnUpdateNewmemo)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_UPDATE_COMMAND_UI(ID_APP_ABOUT, OnUpdateAppAbout)
	ON_COMMAND(IDM_ALL_ICON, OnAllIcon)
	ON_UPDATE_COMMAND_UI(IDM_ALL_ICON, OnUpdatePopups)
	ON_COMMAND(IDM_ALL_NORMAL, OnAllNormal)
	ON_COMMAND(IDM_ALL_ONCEHIDE, OnAllOncehide)
	ON_COMMAND(IDM_ALL_TOP, OnAllTop)
	ON_COMMAND(IDM_ALL_TITLE, OnAllTitle)
	ON_COMMAND(IDM_MEMOIMPORT, OnMemoimport)
	ON_COMMAND(IDM_DELLIST, OnDellist)
	ON_UPDATE_COMMAND_UI(IDM_DELLIST, OnUpdateDellist)
	ON_COMMAND(IDM_SEND_DIRECT, OnSendDirect)
	ON_UPDATE_COMMAND_UI(IDM_SEND_DIRECT, OnUpdateSendDirect)
	ON_COMMAND(IDM_ARRANGE_ICON, OnArrangeIcon)
	ON_UPDATE_COMMAND_UI(IDM_ARRANGE_ICON, OnUpdateArrangeIcon)
	ON_COMMAND(IDM_ARRANGE_NORMAL, OnArrangeNormal)
	ON_UPDATE_COMMAND_UI(IDM_ARRANGE_NORMAL, OnUpdateArrangeNormal)
	ON_COMMAND(IDM_CLIENTRESERCH, OnClientreserch)
	ON_UPDATE_COMMAND_UI(IDM_CLIENTRESERCH, OnUpdateClientreserch)
	ON_WM_COPYDATA()
	ON_COMMAND(IDM_NONPOPUP, OnNonpopup)
	ON_UPDATE_COMMAND_UI(IDM_NONPOPUP, OnUpdateNonpopup)
	ON_COMMAND(IDM_CUSTOM, OnCustom)
	ON_UPDATE_COMMAND_UI(IDM_CUSTOM, OnUpdateCustom)
	ON_WM_QUERYENDSESSION()
	ON_WM_ENDSESSION()
	ON_COMMAND(IDM_ABSENCE, OnAbsence)
	ON_UPDATE_COMMAND_UI(IDM_ABSENCE, OnUpdateAbsence)
	ON_COMMAND(IDM_HELP, OnHelp)
	ON_COMMAND(IDM_SAVENOW, OnSavenow)
	ON_COMMAND(IDM_STOPNXL, OnStopnxl)
	ON_COMMAND(IDM_STARTNXL, OnStartnxl)
	ON_UPDATE_COMMAND_UI(IDM_STOPNXL, OnUpdateStopnxl)
	ON_UPDATE_COMMAND_UI(IDM_STARTNXL, OnUpdateStartnxl)
	ON_COMMAND(IDM_EXEDIT, OnExedit)
	ON_UPDATE_COMMAND_UI(IDM_EXEDIT, OnUpdateExedit)
	ON_COMMAND(IDM_BIGBUTTON, OnBigbutton)
	ON_UPDATE_COMMAND_UI(IDM_BIGBUTTON, OnUpdateBigbutton)
	ON_COMMAND(IDM_COLORPRINT, OnColorprint)
	ON_UPDATE_COMMAND_UI(IDM_COLORPRINT, OnUpdateColorprint)
	ON_COMMAND(IDM_ARRANGE_TITLE, OnArrangeTitle)
	ON_UPDATE_COMMAND_UI(IDM_ARRANGE_TITLE, OnUpdateArrangeTitle)
	ON_COMMAND(IDM_ARRANGE_ALL, OnArrangeAll)
	ON_UPDATE_COMMAND_UI(IDM_ARRANGE_ALL, OnUpdateArrangeAll)
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(IDM_UPDATEEXTICONS, OnUpdateexticons)
	ON_UPDATE_COMMAND_UI(IDM_ALL_NORMAL, OnUpdatePopups)
	ON_UPDATE_COMMAND_UI(IDM_ALL_ONCEHIDE, OnUpdatePopups)
	ON_UPDATE_COMMAND_UI(IDM_ALL_TOP, OnUpdatePopups)
	ON_UPDATE_COMMAND_UI(IDM_ALL_TITLE, OnUpdatePopups)
	ON_UPDATE_COMMAND_UI(IDM_DATALIST, OnUpdatePopups)
	ON_UPDATE_COMMAND_UI(IDM_UPDATEEXTICONS, OnUpdateUpdateexticons)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( sm_unMsgFromTray, OnMsgFromTaskTray)
	ON_REGISTERED_MESSAGE( sm_unMsgFromMemo, OnMsgFromMemoChip)
	ON_REGISTERED_MESSAGE( sm_unMsgFromExplorer, OnMsgFromMemoExplorer)
	ON_COMMAND_RANGE( IDM_TEMPLET0MENU, IDM_TEMPLET9MENU, OnTempletMenu)
	ON_UPDATE_COMMAND_UI_RANGE( IDM_TEMPLET0MENU, IDM_TEMPLET9MENU, OnUpdateTempletMenu)
	ON_COMMAND_RANGE( IDM_TEMPLET0MENU_S, IDM_TEMPLET9MENU_S, OnSendTempletMenu)
	ON_UPDATE_COMMAND_UI_RANGE( IDM_TEMPLET0MENU_S, IDM_TEMPLET9MENU_S, OnUpdateTempletMenu)
	ON_COMMAND_RANGE( CATALOG_TOP, CATALOG_BOTTOM, OnCatalogMenu)
	ON_UPDATE_COMMAND_UI_RANGE( CATALOG_TOP, CATALOG_BOTTOM, OnUpdateCatalogMenu)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
	// TODO: この位置にメンバの初期化処理コードを追加してください。
	m_hIconHide		= NULL;
	m_hIconNormal	= NULL;
	m_hIconReceive	= NULL;
	m_hIconAbsence	= NULL;

	// 最後の通知日
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	m_nLastNotifyDay = cTime.GetDay();

	m_nTimerCount = 0;		// Dxlタイマとの差
	m_cLstMemoChip.RemoveAll();

	m_unMemoChipID = 0;

	m_blAboutDisplaied = false;
	m_blAllOnceHide = false;

	m_nResumeDelay = -1;
	m_nReceiveChip = 0;

	m_pcToolTip = NULL;

	m_blAbsence = FALSE;

	m_blLoadedMemoData = false;

	m_blStopedNetworkByApmSuspend = false;

	m_nDecrimentCount = 0;
	m_nRenotifyCount = 0;

	LoadFrame( IDR_MAINFRAME, WS_OVERLAPPEDWINDOW);
}

CMainFrame::~CMainFrame()
{
	if( m_pcToolTip)
	{
		delete m_pcToolTip;
		m_pcToolTip = NULL;
	}

	if( m_hIconHide)
	{
		DeleteObject( m_hIconHide);
		m_hIconHide = NULL;
	}
	if( m_hIconNormal)
	{
		DeleteObject( m_hIconNormal);
		m_hIconNormal = NULL;
	}
	if( m_hIconReceive)
	{
		DeleteObject( m_hIconReceive);
		m_hIconReceive = NULL;
	}
	if( m_hIconAbsence)
	{
		DeleteObject( m_hIconAbsence);
		m_hIconAbsence = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ

void CMainFrame::OnClose() 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	// 受信処理停止
	pcSOboe->SuspendReceive();

	QuitProcess();

	pcSOboe->EndNetwork();
	pcSOboe->CloseNetwork( GetSafeHwnd());

	CFrameWnd::OnClose();
}

void CMainFrame::ChackAndSelectLoadingFile( CString& cStrLoadFilePath)
{
	CString cStrDrive;
	CString cStrDir;
	CString cStrFileName;
	CString cStrBackupFile;

	_splitpath( cStrLoadFilePath, cStrDrive.GetBuffer( _MAX_DRIVE), cStrDir.GetBuffer( _MAX_DIR), cStrFileName.GetBuffer( _MAX_FNAME), NULL);
	cStrDrive.ReleaseBuffer();
	cStrDir.ReleaseBuffer();
	cStrFileName.ReleaseBuffer();
	_makepath( cStrBackupFile.GetBuffer( _MAX_PATH), cStrDrive, cStrDir, cStrFileName, TEXT( "BAK"));
	cStrBackupFile.ReleaseBuffer();

	CFileStatus cMainFile;
	CFileStatus cBackFile;

	LONGLONG llMainSize = 0;
	LONGLONG llBackSize = 0;


	if( CFile::GetStatus( cStrLoadFilePath, cMainFile))
	{
		llMainSize = cMainFile.m_size;
	}
	if( CFile::GetStatus( cStrBackupFile, cBackFile))
	{
		llBackSize = cBackFile.m_size;
	}

	if( llMainSize * 2 < llBackSize)
	{
		CString cStrMsg = TEXT( "前回のバックアップファイルよりデータファイルが極端に小さくなっています。\nデータファイルが破損している可能性もあります。\n\n復元しますか？");
		if( IDYES == MessageBox( cStrMsg, NULL, MB_YESNO | MB_ICONEXCLAMATION))
		{
			CTime cTime = CTime::GetCurrentTime();
			CString cStrNewBackUp;
			CString cStrNewBackUpFName;
			cStrNewBackUpFName.Format( TEXT( "OboeGaki%s.mrd"), cTime.Format( TEXT( "%y%m%d_%H%M")));

			_makepath( cStrNewBackUp.GetBuffer( _MAX_PATH), cStrDrive, cStrDir, cStrNewBackUpFName, TEXT( "BAK"));
			cStrNewBackUp.ReleaseBuffer();

			if( 0 != ::MoveFileEx( cStrLoadFilePath, cStrNewBackUp, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
			{
				cStrMsg = TEXT( "データファイルを以下のファイルに保存しました\nバックアップファイルからの復元を継続します。\n\n");
				cStrMsg += cStrBackupFile;

				MessageBox( cStrMsg, NULL, MB_YESNO | MB_ICONINFORMATION);
			}


			cStrLoadFilePath = cStrBackupFile;
		}
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: この位置に固有の作成用コードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	m_hIconHide		= (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_HIDE), IMAGE_ICON, 16, 16, 0);
	m_hIconNormal	= (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_MAINFRAME), IMAGE_ICON, 16, 16, 0);
	m_hIconReceive	= (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_RECEIVE), IMAGE_ICON, 16, 16, 0);
	m_hIconAbsence	= (HICON)LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDI_ABSENCE), IMAGE_ICON, 16, 16, 0);

	if( NULL == m_pcToolTip)
	{
		m_pcToolTip = new CToolTipCtrl;
	}
	m_pcToolTip->Create( NULL, TTS_ALWAYSTIP);
	m_pcToolTip->SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	m_pcToolTip->SetDelayTime( TTDT_INITIAL, pcSOboe->GetDelayTime());
	m_pcToolTip->SetDelayTime( TTDT_AUTOPOP, 5000);
	m_pcToolTip->Activate( TRUE);
	m_pcToolTip->SetMaxTipWidth( GetSystemMetrics( SM_CXSCREEN) / 2);


	// データの読み出しと復元
	CString	cStrSaveFile;
	cStrSaveFile = pcSOboe->GetSaveFilePath();
	ChackAndSelectLoadingFile( cStrSaveFile);
	while( FALSE == LoadMemoDatas( cStrSaveFile))
	{
		CString cStr;
		cStr.LoadString( IDS_FAILEDLOAD);
		int nResult = MessageBox( cStr, NULL, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_DEFBUTTON2);
		switch( nResult)
		{
		case IDABORT:
			PostQuitMessage( -1);
			return -1;
		case IDRETRY:
			continue;
		case IDIGNORE:
			break;
		}
		break;
	}
	m_blLoadedMemoData = true;

	m_uModifiers = pcSOboe->GetHKModifiers();
	m_uVirtKey = pcSOboe->GetHKVirtKey();

	if( pcSOboe->IsEnableNetwork())
	{
		if( pcSOboe->OpenNetwork( GetSafeHwnd()))
		{
			pcSOboe->StartNetwork();
		}
	}

	pcSOboe->LoadTemplet();

	if( m_uVirtKey)
	{
		VERIFY( RegisterHotKey( GetSafeHwnd(), 0, m_uModifiers, m_uVirtKey));
	}

	SetTimer( TIMER_ID, TIMER_INTERVAL, NULL);
	
	SetTrayIcon();

	return 0;
}

void CMainFrame::SetTrayIcon( void)
{
	NOTIFYICONDATA	stTnid;
	ZeroMemory( &stTnid, sizeof( NOTIFYICONDATA));
	stTnid.cbSize			= sizeof( NOTIFYICONDATA);
	stTnid.hWnd				= GetSafeHwnd();
	stTnid.uID				= TASKTRAY_ID;
	stTnid.uFlags			= NIF_MESSAGE | NIF_ICON | NIF_TIP;
	stTnid.uCallbackMessage	= sm_unMsgFromTray;
	CString	cStrTrayChip;
	if( m_blAllOnceHide)
	{
		stTnid.hIcon		= m_hIconHide;
		cStrTrayChip.LoadString( IDS_TRAYCHIP_HIDE);
	}
	else
	{
		stTnid.hIcon	= ( 0 < m_nReceiveChip) ? m_hIconReceive : ( FALSE == m_blAbsence) ? m_hIconNormal : m_hIconAbsence;

		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			cStrTrayChip.Format( IDS_TRAYCHIP_NORMAL, m_cLstMemoChip.GetCount());
			cSglk.Unlock();
		}
	}
	lstrcpy( stTnid.szTip, cStrTrayChip);

	// 追加を試みる
	for( int nIndex1 = 0; nIndex1 < 10; nIndex1++)
	{
		if( FALSE != Shell_NotifyIcon( NIM_ADD, &stTnid))
		{
			break;
		}
		// タイムアウトなら再度
		if( ERROR_TIMEOUT != GetLastError())
		{
			for( int nIndex = 0; nIndex < 10; nIndex++)
			{
				if( FALSE != Shell_NotifyIcon( NIM_MODIFY, &stTnid))
				{
					break;
				}
				if( ERROR_TIMEOUT != GetLastError())
				{
					break;
				}
				Sleep( 100);
			}
			break;
		}
		Sleep( 100);
	}
}

void CMainFrame::DelTrayIcon( void)
{
	NOTIFYICONDATA	stTnid;

	stTnid.cbSize			= sizeof( NOTIFYICONDATA);
	stTnid.hWnd				= GetSafeHwnd();
	stTnid.uID				= TASKTRAY_ID;
	stTnid.uFlags			= 0;
	Shell_NotifyIcon( NIM_DELETE, &stTnid);
}

LONG CMainFrame::OnMsgFromTaskTray( UINT wParam, LPARAM lParam)
{
	UINT	unMouseMsg = UINT( lParam);

	if( unMouseMsg == WM_LBUTTONDBLCLK || unMouseMsg == WM_LBUTTONDOWN || unMouseMsg == WM_RBUTTONDBLCLK || unMouseMsg == WM_RBUTTONDOWN)
	{
		if( m_nReceiveChip)
		{
			BOOL	blPopupReceive = FALSE;
			BOOL	blHideReceive = FALSE;

			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			blPopupReceive = pcSOboe->IsPopupReceive();
			if( !blPopupReceive)
			{
				blHideReceive = pcSOboe->IsHideReceive();
			}

			SetForegroundWindow();
			CMemoChip*	pcMemoChip;
			CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
			if( cSglk.Lock( INFINITE))
			{
				for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
				{
					pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
					if( pcMemoChip)
					{
						if( pcMemoChip->NoCheckReceive())
						{
							if( !blPopupReceive && blHideReceive)
							{
								pcMemoChip->RestoreOnceHide();
							}
							pcMemoChip->OneTimeTop();
							pcMemoChip->BringWindowToTop();
						}
					}
				}
				cSglk.Unlock();
			}
			return 0;
		}
	}

	if( unMouseMsg == WM_LBUTTONDBLCLK)
	{
		AllRestoreOnceHide();
		SetForegroundWindow();
		if( CMemoEditSheet::IsUsed())
		{
			MessageBeep( MB_ICONHAND);
		}
		else
		{
			PostMessage( WM_COMMAND, IDM_NEWMEMO, 0);
		}
		return 0;
	}

	if( unMouseMsg == WM_RBUTTONUP)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		AllRestoreOnceHide();

		POINT	stPoint;
		SetForegroundWindow();
		BringWindowToTop();
		GetCursorPos( &stPoint);

		CMenu cPopupMenu;
		cPopupMenu.LoadMenu( IDR_MAINFRAME);
		
		int nMenuIndex = ( pcSOboe->IsCompatibleOU() ? 2 : 0) + ( pcSOboe->IsFlatMenu() ? 1 : 0);
		CMenu*	pcMenu = cPopupMenu.GetSubMenu( nMenuIndex);
		pcMenu->SetDefaultItem( IDM_NEWMEMO, FALSE);

		if( pcSOboe->IsEnableSpMenu())
		{
			CMenu*	pcSpMenu = cPopupMenu.GetSubMenu( 4);

			if( pcSpMenu)
			{
				CString cStrMenu;
				cStrMenu.LoadString( IDS_SPESIALMENU);
				pcMenu->InsertMenu( IDM_NONPOPUP, MF_BYCOMMAND | MF_POPUP, ( UINT)pcSpMenu->GetSafeHmenu(), cStrMenu);
				// 拡張用のDLLがない場合はDisable
				pcMenu->EnableMenuItem( IDM_UPDATEEXTICONS, pcSOboe->IsExistUpdateDLL());
			}
		}
		else
		{
			// 拡張用のDLLがない場合はメニューを表示しない
			if( pcSOboe->IsExistUpdateDLL())
			{
				CString cStrMenu;
				cStrMenu.LoadString( IDS_UPDATEICONS);
				pcMenu->InsertMenu( IDM_NONPOPUP, MF_BYCOMMAND | MF_STRING, IDM_UPDATEEXTICONS, cStrMenu);
			}
		}

		// カタログ表示の処理
		CCatalogList	cCatalogList;
		if( pcSOboe->IsEnableCatalogView())
		{
			HDC	hDC = ::GetDC( GetSafeHwnd());
			if( hDC)
			{
				CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
				if( cSglk.Lock( 0))
				{
					POSITION	pos;
					CMemoChip*	pcMemoChip;
					int nCount = min( CATALOG_COUNT, m_cLstMemoChip.GetCount());
					for( int nIndex = 0; nIndex < nCount; nIndex++)
					{
						pos = m_cLstMemoChip.FindIndex( nIndex);
						if( pos)
						{
							pcMemoChip = m_cLstMemoChip.GetAt( pos);
							ASSERT( pcMemoChip);
							cCatalogList.AppendData( hDC, CATALOG_TOP + nIndex, pcMemoChip->AccessMemoData());
						}
					}
				}
				::ReleaseDC( GetSafeHwnd(), hDC);
			}

			HMENU	hCatalogMenu = ListUp( &cCatalogList, 0);
			if( NULL != hCatalogMenu)
			{
				CString	cStr;
				cStr.LoadString( IDS_CHIPCATALOG);
				pcMenu->InsertMenu( 3, MF_POPUP | MF_BYPOSITION, ( UINT)hCatalogMenu, cStr);
			}
		}
		// テンプレートのリストアップ
		if( pcSOboe->GetTempletCount())
		{
			CMenu	cTempletMemu;
			if( cTempletMemu.CreatePopupMenu())
			{
				CString		cStr;
				CString		cStrMenu;
				int nCount = pcSOboe->GetTempletCount();
				for( int nIndex = 0; nIndex < nCount; nIndex++)
				{
					if( pcSOboe->GetTempletName( nIndex, cStr))
					{
						cStrMenu.Format( "(&%d) %s", nIndex, cStr);
						cTempletMemu.AppendMenu( MF_BYCOMMAND, IDM_TEMPLET0MENU + nIndex, cStrMenu);
					}
				}
				cStr.LoadString( IDS_TEMPLETSUBMENU);
				pcMenu->InsertMenu( 1, MF_BYPOSITION | MF_POPUP, ( UINT)cTempletMemu.GetSafeHmenu(), cStr);
				cTempletMemu.Detach();
			}
			if( pcSOboe->IsStandbyNetwork())
			{
				CMenu	cSendTempletMemu;
				if( cSendTempletMemu.CreatePopupMenu())
				{
					CString		cStr;
					CString		cStrMenu;
					int nCount = pcSOboe->GetTempletCount();
					for( int nIndex = 0; nIndex < nCount; nIndex++)
					{
						if( pcSOboe->GetTempletName( nIndex, cStr))
						{
							cStrMenu.Format( "(&%d) %s", nIndex, cStr);
							cSendTempletMemu.AppendMenu( MF_BYCOMMAND, IDM_TEMPLET0MENU_S + nIndex, cStrMenu);
						}
					}
					cStr.LoadString( IDS_TEMPLETSUBMENU_S);
					pcMenu->InsertMenu( IDM_CLIENTRESERCH, MF_BYCOMMAND | MF_POPUP, ( UINT)cSendTempletMemu.GetSafeHmenu(), cStr);
					cSendTempletMemu.Detach();
				}
			}
		}
		pcMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, stPoint.x, stPoint.y, this, NULL);
		return 0;
	}
	else
	{
		if( unMouseMsg == WM_LBUTTONDOWN)
		{
			if( 0 != ( GetAsyncKeyState( VK_SHIFT) & ~1))
			{
				if( !m_blAllOnceHide)
				{
					PostMessage( WM_COMMAND, IDM_ALL_ONCEHIDE, 0);
				}
				else
				{
					AllRestoreOnceHide();
				}
				return 0;
			}
			else
			{
				if( 0 != ( GetAsyncKeyState( VK_CONTROL) & ~1))
				{
					AllRestoreOnceHide();
					SetForegroundWindow();
					CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
					if( cSglk.Lock( INFINITE))
					{
						CMemoChip*	pcMemoChip;
						for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
						{
							pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
							if( pcMemoChip)
							{
								pcMemoChip->OneTimeTop();
								pcMemoChip->BringWindowToTop();
							}
						}
						cSglk.Unlock();
					}
					return 0;
				}
			}
			AllRestoreOnceHide();
		}
	}

	return 0;
}

void CMainFrame::OnCatalogMenu( UINT nID)
{
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe->IsEnableCatalogView())
	{
		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( 0))
		{
			CMemoChip*	pcMemoChip;
			POSITION	pos;
			pos = m_cLstMemoChip.FindIndex( nID - CATALOG_TOP);
			if( NULL != pos)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( pos);
				ASSERT( pcMemoChip);
				if( ::IsWindow( pcMemoChip->GetSafeHwnd()))
				{
					pcMemoChip->SendMessage( WM_COMMAND, IDM_TONORMAL, 0L);
					pcMemoChip->SetActiveWindow();
					pcMemoChip->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					return;
				}
			}
			/*
			UINT unID = nID - CATALOG_TOP;
			int nCount = m_cLstMemoChip.GetCount();
			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pos = m_cLstMemoChip.FindIndex( nIndex);
				if( NULL != pos)
				{
					pcMemoChip = m_cLstMemoChip.GetAt( pos);
					if( NULL != pcMemoChip && unID == pcMemoChip->GetID())
					{
						pcMemoChip->SendMessage( WM_COMMAND, IDM_TONORMAL, 0L);
						pcMemoChip->SetActiveWindow();
						pcMemoChip->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					}
				}
			}
			*/
		}
	}
	MessageBeep( MB_ICONEXCLAMATION);
}

void CMainFrame::OnUpdateCatalogMenu( CCmdUI* pCmdUI)
{
}

LONG CMainFrame::OnMsgFromMemoChip( UINT wParam, LPARAM lParam)
{
	switch( wParam)
	{
	case NMFM_DELSAVELOG:
		LoggingMemoChip( lParam);
	case NMFM_DELSAVE:
		SaveMemoChipForDel( lParam);
	case NMFM_DELONLY:
		DelMemoChip( lParam);
		break;
	case NMFM_DELLOG:
		LoggingMemoChip( lParam);
		DelMemoChip( lParam);
		break;
	case NMFM_CREATE:
		if( NULL != lParam)
		{
			CMemoData* pMemoData = ( CMemoData*)lParam;
			CreateChipFromMemoData( *pMemoData);
		}
		break;
	case NMFM_CREATEEDIT:
		if( NULL != lParam)
		{
			CMemoData* pMemoData = ( CMemoData*)lParam;
			EditAndCreate( *pMemoData);
		}
		break;
	case NMFM_CHECKEDRECEIVE:
		m_nReceiveChip--;
		// 受信中の小紙片がないので、再通知をしない
		if( 0 >= m_nReceiveChip)m_nRenotifyCount = 0;
		SetTrayIcon();
		break;
	case NMFM_SENDREPLY:
		SendReply( lParam);
		break;
	case NMFM_RECEIVE:
		{
			RECEIVESOURCE* pRcvSrc = ( RECEIVESOURCE*)GlobalLock( ( HGLOBAL)lParam);
			if( pRcvSrc)
			{
				LPCSTR		lpcszData = &( ( LPCSTR)pRcvSrc)[ pRcvSrc->nOffsetReceiveData];
				int			nSize = pRcvSrc->nReceiveDataSize;

				ENTRYDATA*	pEntryData = NULL;
				int			nEntryDataSize = 0;
				if( 0 < pRcvSrc->nOffsetEntryData)
				{
					pEntryData = ( ENTRYDATA*)&( ( LPCSTR)pRcvSrc)[ pRcvSrc->nOffsetEntryData];
					nEntryDataSize = pRcvSrc->nEntryDataSize;
				}
				if( !lstrcmp( lpcszData, "OboeData"))
				{
					INNERSENDDATA	stInnerData;
					if( CNetExLib::ExcangeReceiveData( &stInnerData, ( SENDDATA*)lpcszData, nSize))
					{
						CEntryData*	pcEntryData = NULL;
						if( pEntryData)
						{
							pcEntryData = new CEntryData;
							pcEntryData->SetEntryData( pEntryData, nEntryDataSize);
						}
						ReceiveMemoData( stInnerData, pcEntryData);
						if( pcEntryData)
						{
							delete pcEntryData;
						}
					}
				}
				else
				{
					if( !lstrcmp( lpcszData, "OboeRslt"))
					{
						INNERRESULTDATA	stInnerResult;
						if( CNetExLib::ExcangeReceiveResultData( &stInnerResult, ( RESULTDATA*)lpcszData, nSize))
						{
							ReceiveResult( stInnerResult);
						}
					}
				}
				GlobalUnlock( ( HGLOBAL)lParam);
				GlobalFree( ( HGLOBAL)lParam);
			}
		}
		break;
	case NMFM_SAVEDATAS:
		SaveMemoDatas();
		break;
	case NMFM_CREATE_BIN:
		{
			CREATESOURCE* pstCreateSrc = ( CREATESOURCE*)GlobalLock( ( HGLOBAL)lParam);
			if( pstCreateSrc)
			{
				CMemoData* pcMemoData = CMemoData::CreateMemoFromBinData( ( LPBYTE)pstCreateSrc->lpabyData, pstCreateSrc->dwDataSize);
				if( pcMemoData)
				{
					if( pstCreateSrc->blEdit)
					{
						EditAndCreate( *pcMemoData);
					}
					else
					{
						CreateChipFromMemoData( *pcMemoData, pstCreateSrc->blCalcRect, pstCreateSrc->blDelOnQuit);
					}

					delete pcMemoData;
					pcMemoData = NULL;
				}
				GlobalUnlock( ( HGLOBAL)lParam);
				GlobalFree( ( HGLOBAL)lParam);
			}
		}
		break;

#ifdef	_KEYCONTROL_ENABLE_
	case NMFM_PREVCHIP:
		ChangeFocus( lParam, TRUE);
		break;
	case NMFM_NEXTCHIP:
		ChangeFocus( lParam, FALSE);
		break;
#endif	//_KEYCONTROL_ENABLE_
	}

	return 0;
}

LONG CMainFrame::OnMsgFromMemoExplorer( UINT wParam, LPARAM lParam)
{
	SetTrayIcon();

	return 0;
}

void CMainFrame::CheckRenotify()
{
	if( 0 < m_nRenotifyCount)
	{
		m_nRenotifyCount--;
		if( 0 >= m_nRenotifyCount)
		{
			if( 0 < m_nReceiveChip)
			{
				/*
				for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
				{
					pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
					if( pcMemoChip)
					{
						if( pcMemoChip->NoCheckReceive())
						{
							CMemoData cMemoData;
							pcMemoChip->GetMemoData( cMemoData);
							cStr = cMemoData->GetTitle();
						}
					}
				}
				*/

				CString cStr;
				cStr.LoadString( IDS_RENOTIFY);
				CRenotifyWnd* pcWnd = NULL;
				pcWnd = new CRenotifyWnd;
				pcWnd->Create( cStr, this);
				pcWnd->ShowWindow( SW_SHOWNOACTIVATE);

				m_nRenotifyCount = RENOTIFY_COUNT;
			}
		}
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( 0))
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		CMemoChip*	pcMemoChip;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		// Popup通知か、非Popup通知でも通知window有効？
		if( pcSOboe->IsPopupReceive() || ( !pcSOboe->IsPopupReceive() && pcSOboe->IsSubNotify()))
		{
			// 受信小片が未チェックの場合再通知を行う
			CheckRenotify();
		}

		m_nTimerCount++;
		for( int nIndex = 0; nIndex < m_cLstMemoChip.GetCount(); nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( TIMER_COUNT <= m_nTimerCount)
				{
					pcMemoChip->CheckTimer( cTime);
				}
				pcMemoChip->DxlCheckTimer( cTime);
			}
		}
		if( TIMER_COUNT <= m_nTimerCount)m_nTimerCount = 0;

		if( m_nLastNotifyDay != cTime.GetDay())
		{
			m_nLastNotifyDay = cTime.GetDay();
			for( int nIndex = 0; nIndex < m_cLstMemoChip.GetCount(); nIndex++)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
				if( pcMemoChip)pcMemoChip->CheckTimerOnDayChange( cTime);
			}
		}

		if( 0 <= m_nResumeDelay)
		{
			if( 0 == m_nResumeDelay)
			{
				// サスペンド終了
				if( pcSOboe->IsEnableNetwork())
				{
					if( pcSOboe->OpenNetwork( GetSafeHwnd()))
					{
						pcSOboe->StartNetwork();
					}
				}
			}
			m_nResumeDelay--;
		}

		m_nDecrimentCount++;
		if( _DECRIMENTSPAN < m_nDecrimentCount)
		{
			pcSOboe->DecrimentImportCount();
			m_nDecrimentCount= 0;
		}

		cSglk.Unlock();
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnNewmemo() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	/*
	if( GetKeyState( VK_SHIFT) & ~1)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		pcSOboe->ExEdit( !pcSOboe->IsExEdit());
	}
	else
	*/
	{
		CMemoData	cMemoData;

		cMemoData.SetDefaultStyle();
		EditAndCreate( cMemoData);
	}
}

void CMainFrame::OnUpdateNewmemo(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( !CMemoEditSheet::IsUsed());
}

BOOL CMainFrame::EditAndCreate( const CMemoData& cMemo)
{
	if( CMemoEditSheet::IsUsed())return FALSE;

	CMemoData cMemoData;
	cMemoData = cMemo;
/*
	COleDateTime	cTime = COleDateTime::GetCurrentTime();
	cMemoData.SetTimerMonth( cTime.GetMonth());
	cMemoData.SetTimerDay( cTime.GetDay());
*/

	CString	cStrCaption;
	cStrCaption.LoadString( IDS_CAPTION_NEW);

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe->IsExEdit())
	{
		CResizableMemoEditDialog cMemoEdit( cStrCaption, this);
		cMemoEdit.SetMemoData( cMemoData);
		cMemoEdit.ResizeLock( TRUE);
		while( TRUE)
		{
			if( IDOK != cMemoEdit.DoModal())break;
			if( !cMemoEdit.IsMemoEmpty())
			{
				cMemoEdit.GetMemoData( cMemoData);

				CreateChipFromMemoData( cMemoData, TRUE, cMemoEdit.IsDelOnQuit());

				return TRUE;
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MEMOEMPTY);
				if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON1))break;
			}
		}
	}
	else
	{
		CMemoEditSheet	cMemoEdit( cStrCaption, this);
		cMemoEdit.SetMemoData( cMemoData);
		cMemoEdit.ResizeLock( TRUE);
		while( TRUE)
		{
			if( IDOK != cMemoEdit.DoModal())break;
			if( !cMemoEdit.IsMemoEmpty())
			{
				cMemoEdit.GetMemoData( cMemoData);

				CreateChipFromMemoData( cMemoData, TRUE, cMemoEdit.IsDelOnQuit());

				return TRUE;
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MEMOEMPTY);
				if( IDYES != MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION | MB_DEFBUTTON1))break;
			}
		}
	}

	return FALSE;
}

BOOL CMainFrame::AddMemoChip( CMemoChip* pcMemoChip, BOOL blTaskTrayUpdate)
{
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		m_cLstMemoChip.AddTail( pcMemoChip);
		cSglk.Unlock();
	}
	if( blTaskTrayUpdate)
	{
		SetTrayIcon();
	}

	return TRUE;
}

BOOL CMainFrame::LoggingMemoChip( UINT unDelWndID)
{
	bool		blMatch = false;
	CMemoData	cMemoData;

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( unDelWndID == pcMemoChip->GetID())
				{
					pcMemoChip->GetMemoData( cMemoData);
					blMatch = true;
					break;
				}
			}
		}
		cSglk.Unlock();
	}

	if( blMatch)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		if( !pcSOboe->IsLoggingDelChip())return TRUE;

		CString	cPathName;
		cPathName = pcSOboe->GetDelLogFilePath();
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


			SYSTEMTIME	stSysTime;
			GetLocalTime( &stSysTime);
			COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
			CString	cStrDelTime;
			cStrDelTime = cTime.Format( "create:%%02d/%%02d/%%02d\r\ndelete:%y/%m/%d - %H:%M:%S\r\nData : %%s\r\n");

			int nYear;
			int nMonth;
			int nDay;
			cMemoData.GetCreateDate( nYear, nMonth, nDay);

			CString	cStrWork;
			CString	cStrLog;
			cMemoData.GetTitle( cStrWork);
			cStrLog.Format( cStrDelTime, nYear % 100, nMonth, nDay, cStrWork);
			cLogFile.Write( cStrLog, cStrLog.GetLength());	// 削除時間

			cStrWork = cMemoData.GetMemo();
			cLogFile.Write( cStrWork, cStrWork.GetLength());
			CString	cStrSep( "\r\n-------------------------------\r\n");
			cLogFile.Write( cStrSep, cStrSep.GetLength());
			cLogFile.Close();

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMainFrame::SaveMemoChipForDel( UINT unDelWndID)
{
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( unDelWndID == pcMemoChip->GetID())
			{
				StockMemoData( *pcMemoChip);
				break;
			}
		}
		cSglk.Unlock();
	}

	return TRUE;
}

BOOL CMainFrame::StockMemoData( const CMemoChip& cMemoChip)
{
	CFile		cSaveFile;

	try
	{
		CString	cStrStockFile;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		cStrStockFile = pcSOboe->GetStockFilePath();
		// 何度も何度もオープンするのはダサいような気がするもの、まぁ、しゃぁないね。
		if( cSaveFile.Open( cStrStockFile, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
		{
			cSaveFile.Seek( 0, CFile::end);
			CArchive	cArchive( &cSaveFile, CArchive::store);

			CMemoChip	cMemoChipStock;
			CMemoData	cMemoData;

//			cMemoChipStock = cMemoChip;
//			cMemoChipStock.GetMemoData( cMemoData);
			cMemoChip.GetMemoData( cMemoData);
			cMemoData.SetCreateDate();
			cMemoChipStock.SetMemoData( cMemoData);

			cArchive.WriteObject( &cMemoChipStock);

			cArchive.Close();
			cSaveFile.Close();
		}

		return TRUE;
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
		e->Delete();
	}

	return FALSE;
}

BOOL CMainFrame::DelMemoChip( UINT unDelWndID)
{
	bool	blDelete = false;
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( unDelWndID == pcMemoChip->GetID())
				{
					m_cLstMemoChip.RemoveAt( m_cLstMemoChip.FindIndex( nIndex));
					pcMemoChip->SmartDelete();
					pcMemoChip = NULL;
					blDelete = true;
					break;
				}
			}
		}
		cSglk.Unlock();
	}
	if( blDelete)
	{
		SaveMemoDatas();
		SetTrayIcon();
		return TRUE;
	}

	return FALSE;
}

BOOL CMainFrame::SendReply( UINT unReplyEndID)
{
	CMemoChip* pcMemoChipSrc = NULL;

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip* pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( unReplyEndID == pcMemoChip->GetID())
				{
					pcMemoChipSrc = pcMemoChip;
					break;
				}
			}
		}
		cSglk.Unlock();
	}

	if( NULL != pcMemoChipSrc)
	{
		CMemoSendSheet	cMemoSend( IDS_CAPTION_SEND, this, TRUE);

		CMemoData	cMemoData;

		pcMemoChipSrc->GetMemoData( cMemoData);
		CString	cStrTitle;
		cMemoData.GetTitle( cStrTitle);

		if( -1 == cStrTitle.Find( "Re:"))
		{
			CString	cStr;
			cStr.Format( "Re: %s", cStrTitle);
			cStrTitle = cStr.Left( 64);
			cMemoData.SetTitle( cStrTitle);
		}

		char	chData;
		char	chDataOld = 0;
		CString	cStrData;
		cStrData = "> ";
		for( int nIndex = 0; nIndex < cMemoData.GetMemoLength(); nIndex++)
		{
			chData = cMemoData.GetMemo()[ nIndex];
			if( '\n' == chDataOld)cStrData += "> ";
			cStrData += chData;
			chDataOld = chData;
		}
		cStrData += "\r\n";
		cMemoData.SetMemo( cStrData);

		cMemoSend.SetMemoData( cMemoData);
		cMemoSend.DelOnQuit( pcMemoChipSrc->IsDelOnQuit());
		cMemoSend.AddEntryData( pcMemoChipSrc->GetEntryData());
		cMemoSend.SetActivePage( 0);
		while( TRUE)
		{
			if( IDOK == cMemoSend.DoModal())
			{
				if( cMemoSend.IsMemoEmpty())
				{
					CString	cStr;
					cStr.LoadString( IDS_MEMOEMPTY);
					if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
					{
						break;
					}
					cMemoSend.SetActivePage( 0);
					continue;
				}
				if( cMemoSend.IsDistributeEmpty())
				{
					CString	cStr;
					cStr.LoadString( IDS_DISTRIBUTEEMPTY);
					if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
					{
						break;
					}
					cMemoSend.SetActivePage( 3);
					continue;
				}
				// すわ！送信
				{
					INNERSENDDATA	stSendData;
					cMemoSend.GetSendData( stSendData);

					CMemoChip*	pcMemoChip;
					pcMemoChip = new CMemoChip;
					pcMemoChip->SetMemoData( stSendData.m_cMemoData);
					pcMemoChip->DelOnQuit( stSendData.m_blDelOnQuit);	// あぁ、これ忘れった。
					pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), TRUE, m_hIconNormal, m_pcToolTip);
					m_unMemoChipID++;
					pcMemoChip->CalcDefaultRect();
					pcMemoChip->GetMemoData( stSendData.m_cMemoData);
					if( stSendData.m_blPack && ( stSendData.m_blCheckOpen || stSendData.m_blChackResult))
					{
						pcMemoChip->SetContextID( stSendData.m_cStrResultID);
						pcMemoChip->ShowWindow( SW_NORMAL);
						AddMemoChip( pcMemoChip);
						SaveMemoDatas();
					}

					CSendWorkerDialog*	pcSendWorker;
					pcSendWorker = new CSendWorkerDialog( this);

					if( pcSendWorker)
					{
						pcSendWorker->SetSendData( stSendData);
						int nCount = cMemoSend.GetEntryCount();
						for( int nIndex = 0; nIndex < nCount; nIndex++)
						{
							pcSendWorker->AddSendEntry( cMemoSend.GetEntryData( nIndex));
						}
						if( pcSendWorker->Create( CSendWorkerDialog::IDD))
						{
							if( cMemoSend.IsDeleteAfterSend())
							{
								CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
								if( cSglk.Lock( INFINITE))
								{
									CMemoChip* pcMemoChip;
									int nCount = m_cLstMemoChip.GetCount();
									for( int nIndex = 0; nIndex < nCount; nIndex++)
									{
										pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
										if( pcMemoChip)
										{
											if( pcMemoChipSrc->GetID() == pcMemoChip->GetID())
											{
												StockMemoData( *pcMemoChip);
												m_cLstMemoChip.RemoveAt( m_cLstMemoChip.FindIndex( nIndex));
												pcMemoChip->SmartDelete();
												break;
											}
										}
									}
									cSglk.Unlock();
								}
							}
						}
					}

					if( !stSendData.m_blPack || ( !stSendData.m_blCheckOpen && !stSendData.m_blChackResult))
					{
						StockMemoData( *pcMemoChip);
						pcMemoChip->SmartDelete();
					}
				}
			}
			break;
		}
	}
	return TRUE;
}

BOOL CMainFrame::EditAndSend( const CMemoData& cMemo, int nActivePage)
{
	BOOL	blResult = FALSE;
	if( CMemoSendSheet::IsUsed())return blResult;

	CMemoData cMemoData;
	cMemoData = cMemo;
	
	CMemoSendSheet	cMemoSend( IDS_CAPTION_SEND, this, TRUE);
	cMemoSend.SetMemoData( cMemoData);
	cMemoSend.SetActivePage( nActivePage);

	while( TRUE)
	{
		if( IDOK == cMemoSend.DoModal())
		{
			if( cMemoSend.IsMemoEmpty())
			{
				CString	cStr;
				cStr.LoadString( IDS_MEMOEMPTY);
				if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
				{
					break;
				}
				cMemoSend.SetActivePage( 0);
				continue;
			}
			if( cMemoSend.IsDistributeEmpty())
			{
				CString	cStr;
				cStr.LoadString( IDS_DISTRIBUTEEMPTY);
				if( IDNO == MessageBox( cStr, NULL, MB_YESNO | MB_ICONEXCLAMATION))
				{
					break;
				}
				cMemoSend.SetActivePage( 3);
				continue;
			}
			// すわ！送信
			{
				INNERSENDDATA	stSendData;
				cMemoSend.GetSendData( stSendData);

				CMemoChip*	pcMemoChip;
				pcMemoChip = new CMemoChip;
				pcMemoChip->SetMemoData( stSendData.m_cMemoData);
				pcMemoChip->DelOnQuit( stSendData.m_blDelOnQuit);
				pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), TRUE, m_hIconNormal, m_pcToolTip);
				m_unMemoChipID++;
				pcMemoChip->CalcDefaultRect();
				pcMemoChip->GetMemoData( stSendData.m_cMemoData);
				if( stSendData.m_blPack && ( stSendData.m_blCheckOpen || stSendData.m_blChackResult))
				{
					pcMemoChip->SetContextID( stSendData.m_cStrResultID);
					pcMemoChip->ShowWindow( SW_NORMAL);
					AddMemoChip( pcMemoChip);
				}
				else
				{
					StockMemoData( *pcMemoChip);
					pcMemoChip->SmartDelete();
				}

				CSendWorkerDialog*	pcSendWorker;
				pcSendWorker = new CSendWorkerDialog( this);

				if( pcSendWorker)
				{
					pcSendWorker->SetSendData( stSendData);
					int nCount = cMemoSend.GetEntryCount();
					for( int nIndex = 0; nIndex < nCount; nIndex++)
					{
						pcSendWorker->AddSendEntry( cMemoSend.GetEntryData( nIndex));
					}
					pcSendWorker->Create( CSendWorkerDialog::IDD);
					blResult = TRUE;
				}
			}
		}
		break;
	}
	return blResult;
}


void CMainFrame::OnDatalist() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CDataListDialog	cDataDlg( &m_cLstMemoChip, this);

	cDataDlg.DoModal();
}

BOOL CMainFrame::CreateChipFromMemoData( const CMemoData& cMemoData, BOOL blCalcRect, BOOL blDelOnQuit)
{
	CString	cStr;
	cStr = cMemoData.GetMemo();
	if( cStr.IsEmpty())return FALSE;

	CMemoChip*	pcMemoChip;
	pcMemoChip = new CMemoChip;

	pcMemoChip->SetMemoData( cMemoData);
	pcMemoChip->DelOnQuit( blDelOnQuit);

	pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), FALSE, m_hIconNormal, m_pcToolTip);
	m_unMemoChipID++;
	if( blCalcRect)
	{
		pcMemoChip->CalcDefaultRect();
	}
	pcMemoChip->ActiveZOrder();
	AddMemoChip( pcMemoChip);
	SaveMemoDatas();

	return TRUE;
}

void CMainFrame::OnPaste() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( OpenClipboard())
	{
		if( IsClipboardFormatAvailable( CF_TEXT))
		{
			HANDLE hMem = GetClipboardData( CF_TEXT);
			if( NULL != hMem)
			{
				CString	cStrMemo;
				cStrMemo = ( LPSTR)GlobalLock( hMem);
				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);
				if( pcSOboe->IsAutoTriming())
				{
					cStrMemo.TrimLeft();
					cStrMemo.TrimRight();
				}

				CMemoData	cMemo;
				cMemo.SetDefaultStyle();
				cMemo.SetMemo( cStrMemo);
				CreateChipFromMemoData( cMemo);
				GlobalUnlock( hMem);
			}
		}
		else
		{
			if( IsClipboardFormatAvailable( CMemoData::GetClipboardFormat()))
			{
				HANDLE hMem = GetClipboardData( CMemoData::GetClipboardFormat());
				if( NULL != hMem)
				{
					DWORD	dwSize = GlobalSize( hMem);
					LPBYTE lpszData = ( LPBYTE)GlobalLock( hMem);
					CMemoData* pcMemoData = CMemoData::CreateMemoFromBinData( lpszData, dwSize);
					GlobalUnlock( hMem);
					if( pcMemoData)
					{
						CMemoChip*	pcMemoChip;
						pcMemoChip = new CMemoChip;
						pcMemoChip->SetMemoData( *pcMemoData);
						pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), FALSE, m_hIconNormal, m_pcToolTip);
						m_unMemoChipID++;
						pcMemoChip->ActiveZOrder();
						AddMemoChip( pcMemoChip);
						delete pcMemoData;
					}
				}
			}
		}

		CloseClipboard();
	}
}

void CMainFrame::OnUpdatePaste(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( IsClipboardFormatAvailable( CF_TEXT) || IsClipboardFormatAvailable( CMemoData::GetClipboardFormat()));
}

BOOL CMainFrame::BackupDataFile( void)
{
	// データファイルを随時バックアップします？
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	CString	cStrSaveFile;
	cStrSaveFile = pcSOboe->GetSaveFilePath();
	if( cStrSaveFile.IsEmpty())return FALSE;

	char	szBackupFile[ _MAX_PATH];
	char	szDrive[ _MAX_DRIVE];
	char	szPath[ _MAX_DIR];
	char	szFile[ _MAX_FNAME];
	char	szExt[ _MAX_EXT];
	WIN32_FIND_DATA	stFindData;
	HANDLE			hFind;

	int		nGenerationCount = pcSOboe->GetGenerationCount();
	if( 0 < nGenerationCount)
	{
		BOOL blDayDiff = TRUE;

		_splitpath( cStrSaveFile, szDrive, szPath, szFile, NULL);
		_makepath( szBackupFile, szDrive, szPath, szFile, _T( "BK1"));
		hFind = FindFirstFile( szBackupFile, &stFindData);
		if( INVALID_HANDLE_VALUE != hFind)
		{
			FindClose( hFind);

			COleDateTime cBk1Time;
			cBk1Time = stFindData.ftLastWriteTime;

			_splitpath( cStrSaveFile, szDrive, szPath, szFile, NULL);
			_makepath( szBackupFile, szDrive, szPath, szFile, _T( "BAK"));
			hFind = FindFirstFile( szBackupFile, &stFindData);
			if( INVALID_HANDLE_VALUE != hFind)
			{
				FindClose( hFind);
				COleDateTime cBakTime;
				cBakTime = stFindData.ftLastWriteTime;

				COleDateTimeSpan cSpan;
				cSpan = cBakTime - cBk1Time;

				TRACE( _T( "%s\n"), cBakTime.Format( _T( "%Y/%m/%d %H:%M:%S")));
				TRACE( _T( "%s\n"), cBk1Time.Format( _T( "%Y/%m/%d %H:%M:%S")));

				if( 1 > cSpan.GetTotalDays() && -1 < cSpan.GetTotalDays())
				{
					blDayDiff = FALSE;
				}
			}
		}

		if( FALSE != blDayDiff)
		{
			CString	cStrBackUpFile;
			for( int nIndex = 0 ; nIndex < nGenerationCount; nIndex++)
			{
				_splitpath( cStrSaveFile, szDrive, szPath, szFile, NULL);
				wsprintf( szExt, "BK%d", nGenerationCount - ( nIndex + 1));
				_makepath( szBackupFile, szDrive, szPath, szFile, szExt);
				cStrBackUpFile = szBackupFile;

				hFind = FindFirstFile( cStrBackUpFile, &stFindData);
				if( INVALID_HANDLE_VALUE != hFind)
				{
					FindClose( hFind);
					if( 0 == ( FILE_ATTRIBUTE_DIRECTORY & stFindData.dwFileAttributes))
					{
						wsprintf( szExt, "BK%d", nGenerationCount - nIndex);
						_makepath( szBackupFile, szDrive, szPath, szFile, szExt);

						MoveFileEx( cStrBackUpFile, szBackupFile, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
						/*
						CopyFile( cStrBackUpFile, szBackupFile, FALSE);
						// セキュリティの移行
						DWORD nGetLength = 0;
						GetFileSecurity( cStrBackUpFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, 0, &nGetLength);
						if( 0 < nGetLength)
						{
							PSECURITY_DESCRIPTOR	pstSecurityDescriptor;
							pstSecurityDescriptor = malloc( nGetLength);
							if( pstSecurityDescriptor)
							{
								ZeroMemory( pstSecurityDescriptor, nGetLength);
								if( GetFileSecurity( cStrBackUpFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor, nGetLength, &nGetLength))
								{
									SetFileSecurity( szBackupFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor);
								}
								free( pstSecurityDescriptor);
							}
						}
						*/
					}
				}
			}
			_makepath( szBackupFile, szDrive, szPath, szFile, "BAK");
			cStrBackUpFile = szBackupFile;
			hFind = FindFirstFile( cStrBackUpFile, &stFindData);
			if( INVALID_HANDLE_VALUE != hFind)
			{
				FindClose( hFind);
				if( 0 == ( FILE_ATTRIBUTE_DIRECTORY & stFindData.dwFileAttributes))
				{
					_makepath( szBackupFile, szDrive, szPath, szFile, "BK1");

					MoveFileEx( cStrBackUpFile, szBackupFile, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
					/*
					CopyFile( cStrBackUpFile, szBackupFile, FALSE);
					// セキュリティの移行
					DWORD nGetLength = 0;
					GetFileSecurity( cStrBackUpFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, 0, &nGetLength);
					if( 0 < nGetLength)
					{
						PSECURITY_DESCRIPTOR	pstSecurityDescriptor;
						pstSecurityDescriptor = malloc( nGetLength);
						if( pstSecurityDescriptor)
						{
							ZeroMemory( pstSecurityDescriptor, nGetLength);
							if( GetFileSecurity( cStrBackUpFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor, nGetLength, &nGetLength))
							{
								SetFileSecurity( szBackupFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor);
							}
							free( pstSecurityDescriptor);
						}
					}
					*/
				}
			}
		}
	}

	hFind = FindFirstFile( cStrSaveFile, &stFindData);
	if( INVALID_HANDLE_VALUE != hFind)
	{
		FindClose( hFind);
		if( 0 == ( FILE_ATTRIBUTE_DIRECTORY & stFindData.dwFileAttributes))
		{

			_splitpath( cStrSaveFile, szDrive, szPath, szFile, NULL);
			_makepath( szBackupFile, szDrive, szPath, szFile, "BAK");

			BOOL nResult = CopyFile( cStrSaveFile, szBackupFile, FALSE);
			// セキュリティの移行
			DWORD nGetLength = 0;
			GetFileSecurity( cStrSaveFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, 0, &nGetLength);
			if( 0 < nGetLength)
			{
				PSECURITY_DESCRIPTOR	pstSecurityDescriptor;
				pstSecurityDescriptor = malloc( nGetLength);
				if( pstSecurityDescriptor)
				{
					ZeroMemory( pstSecurityDescriptor, nGetLength);
					if( GetFileSecurity( cStrSaveFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor, nGetLength, &nGetLength))
					{
						SetFileSecurity( szBackupFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, pstSecurityDescriptor);
					}
					free( pstSecurityDescriptor);
				}
			}

			return nResult;
		}
	}
	return FALSE;
}

BOOL CMainFrame::SaveMemoDatas( void)
{
	// ロード前に書き込まれると、データを壊すことになるからね！
	// ロード前は書き込めない様にするんだ。
	if( false == m_blLoadedMemoData)return FALSE;

	CFile		cSaveFile;

	try
	{
		CString	cStrTempFile;
		// ワークへデータ保存
		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			char	szPath[ _MAX_PATH];
			GetTempPath( _MAX_PATH, szPath);
			char	szTempPath[ _MAX_PATH];
			GetTempFileName( szPath, "Obe", 0, szTempPath);
			if( cSaveFile.Open( szTempPath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
			{
				CString cStrVer;
				cStrVer.LoadString( IDS_VERHEADER);
				int	nVersion = VER_CUR;
				cSaveFile.Write( cStrVer, cStrVer.GetLength());
				cSaveFile.Write( &nVersion, sizeof( int));

				CArchive	cArchive( &cSaveFile, CArchive::store);

				CMemoChip*	pcMemoChip;
				int nCount = m_cLstMemoChip.GetCount();
				cArchive << DWORD( nCount);
				for( int nIndex = 0; nIndex < nCount; nIndex++)
				{
					pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
					if( pcMemoChip)
					{
						cArchive.WriteObject( pcMemoChip);
					}
				}
				cArchive.Close();
				cSaveFile.Close();
				cStrTempFile = szTempPath;
			}
			cSglk.Unlock();
		}
		// ワークデータを本データへ移行する
		// 指定されているパスは書き込めるのかをテスト
		if( !cStrTempFile.IsEmpty())
		{
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			CString	cStrSaveFile = pcSOboe->GetSaveFilePath();
			int nFileCount = 0;
			while( !cSaveFile.Open( cStrSaveFile, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
			{
				// 書き込めないパスであった場合、使用できるパスを生成する
				char	szPath[ _MAX_PATH];
				GetModuleFileName( NULL, szPath, _MAX_PATH);

				char	szDrive[ _MAX_DRIVE];
				char	szFolder[ _MAX_DIR];
				char	szFile[ _MAX_PATH];
				wsprintf( szFile, "Refuge%02d", nFileCount);
				nFileCount++;
				_splitpath( szPath, szDrive, szFolder, NULL, NULL);
				_makepath( szPath, szDrive, szFolder, szFile, "mrm");
				if( 100 <= nFileCount)
				{
					// １００回の挑戦もむなしく書き込めるファイルを発見できなかった……
					// このような状況が発生することは考えにくいけど、仕方が無いので
					// テンポラリファイルを本データファイルに仕上げろ！
					_splitpath( cStrTempFile, szDrive, szFolder, szFile, NULL);
					_makepath( szPath, szDrive, szFolder, szFile, "mrm");
					cStrSaveFile = szPath;
					break;
				}
				cStrSaveFile = szPath;
			}
			cSaveFile.Close();
			if( 0 != CopyFile( cStrTempFile, cStrSaveFile, FALSE))
			{
				DeleteFile( cStrTempFile);
			}
			if( cStrSaveFile != pcSOboe->GetSaveFilePath())
			{
				CString	cStrOld = pcSOboe->GetSaveFilePath();
				pcSOboe->SetSaveFilePath( cStrSaveFile);
				CString	cStrMsg;
				cStrMsg.Format( IDS_WARNINGCREATEDATAFILE, cStrOld, cStrSaveFile);

				MessageBox( cStrMsg, NULL, MB_OK | MB_ICONINFORMATION);
			}
		}
/*
		CString	cStrSaveFile;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		cStrSaveFile = pcSOboe->GetSaveFilePath();
		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			int	nFileCount = 0;
			while( !cSaveFile.Open( cStrSaveFile, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
			{
				char	szPath[ _MAX_PATH];
				GetModuleFileName( NULL, szPath, _MAX_PATH);

				char	szDrive[ _MAX_DRIVE];
				char	szFolder[ _MAX_DIR];
				char	szFile[ _MAX_PATH];
				wsprintf( szFile, "Refuge%02d", nFileCount);
				nFileCount++;
				_splitpath( szPath, szDrive, szFolder, NULL, NULL);
				_makepath( szPath, szDrive, szFolder, szFile, "mrm");
				cStrSaveFile = szPath;
			}

			CString cStrVer;
			cStrVer.LoadString( IDS_VERHEADER);
			int	nVersion = VER_CUR;
			cSaveFile.Write( cStrVer, cStrVer.GetLength());
			cSaveFile.Write( &nVersion, sizeof( int));

			CArchive	cArchive( &cSaveFile, CArchive::store);

			CMemoChip*	pcMemoChip;
			int nCount = m_cLstMemoChip.GetCount();
			cArchive << DWORD( nCount);
			for( int nIndex = 0; nIndex < nCount; nIndex++)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
				cArchive.WriteObject( pcMemoChip);
			}
			cSglk.Unlock();

			cArchive.Close();
			cSaveFile.Close();
		}

		if( cStrSaveFile != pcSOboe->GetSaveFilePath())
		{
			CString	cStrOld = pcSOboe->GetSaveFilePath();
			pcSOboe->SetSaveFilePath( cStrSaveFile);
			CString	cStrMsg;
			cStrMsg.Format( IDS_WARNINGCREATEDATAFILE, cStrOld, cStrSaveFile);

			MessageBox( cStrMsg, NULL, MB_OK | MB_ICONINFORMATION);
		}
*/
		return TRUE;
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

	return FALSE;
}

BOOL CMainFrame::ImportChip( const CString& cStr)
{
	return LoadMemoDatas( cStr, TRUE);
}

BOOL CMainFrame::LoadMemoDatas( const CString& cStrLoadFile, BOOL blImport/* = FALSE*/)
{
	CFile			cLoadFile;
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	try
	{
		if( cLoadFile.Open( cStrLoadFile, CFile::modeRead | CFile::typeBinary))
		{
			cLoadFile.Seek( 0, CFile::begin);
			CString	cStrVer;
			cStrVer.LoadString( IDS_VERHEADER);
			int	nLenVer = cStrVer.GetLength();
			char* pszVer = new char [ nLenVer + 1];
			cLoadFile.Read( pszVer, nLenVer);
			pszVer[ nLenVer] = NULL;
			CString	cStrGetVer;
			cStrGetVer = pszVer;
			delete [] pszVer;
			int	nVersion;
			cLoadFile.Read( &nVersion, sizeof( int));

			if( cStrGetVer != cStrVer)
			{
				CString	cStrError;
				cStrError.LoadString( IDS_FORMATMISMATCH);
				MessageBox( cStrError, NULL, MB_ICONEXCLAMATION);
			}
			else
			{
				switch( nVersion)
				{
				case VER_120:	// 古すぎるバージョン
					{
						CString	cStrError;
						cStrError.LoadString( IDS_NOTSUPPORTFORMAT);
						MessageBox( cStrError, NULL, MB_ICONEXCLAMATION);					
					}
					break;
				case VER_CUR:
				case VER_160:
				case VER_145:
				case VER_142:
				case VER_141:
				case VER_129:
					{
						CArchive	cArchive( &cLoadFile, CArchive::load);
						CMemoChip*	pcMemoChip;

						CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
						ASSERT( pcSOboe);

						if( !blImport)
						{
							m_blAllOnceHide = ( ( NULL != pcSOboe) ? pcSOboe->IsHideStart() : FALSE) ? true : false;
						}

						DWORD	dwCount;
						cArchive >> dwCount;
						for( int nIndex = 0; nIndex < int( dwCount); nIndex++)
						{
							pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
							if( NULL == pcMemoChip)break;
							// インポート時にはＩＤを消す：重複の解消の為
							if( blImport)pcMemoChip->SetContextID( NULL);
							pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), ( blImport) ? false : m_blAllOnceHide, m_hIconNormal, m_pcToolTip);
							m_unMemoChipID++;
							pcMemoChip->CheckTimerOnDayChange( cTime);
							AddMemoChip( pcMemoChip, FALSE);
						}

						cArchive.Close();
					}
					break;
				default:
					{
						CString	cStrError;
						cStrError.LoadString( IDS_VERSIONMISMATCH);
						if( IDCANCEL == MessageBox( cStrError, NULL, MB_ICONEXCLAMATION | MB_OKCANCEL))
						{
							exit( -1);
							return TRUE;
						}
					}
					break;
				}
			}
			cLoadFile.Close();
		}
		return TRUE;
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		CString cStr;
		e->GetErrorMessage( cStr.GetBuffer( 1024), 1024);
		cStr.ReleaseBuffer();
		MessageBox( cStr, NULL, MB_OK | MB_ICONHAND);

		e->Delete();
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		CString cStr;
		e->GetErrorMessage( cStr.GetBuffer( 1024), 1024);
		cStr.ReleaseBuffer();
		MessageBox( cStr, NULL, MB_OK | MB_ICONHAND);

		e->Delete();
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		CString cStr;
		e->GetErrorMessage( cStr.GetBuffer( 1024), 1024);
		cStr.ReleaseBuffer();
		MessageBox( cStr, NULL, MB_OK | MB_ICONHAND);

		e->Delete();
	}
	return FALSE;
}

void CMainFrame::OnAppAbout() 
{
    if( m_blAboutDisplaied)return;
	m_blAboutDisplaied = true;
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	OSVERSIONINFO	stOSVersion;

	stOSVersion.dwOSVersionInfoSize = sizeof( OSVERSIONINFO);

	BOOL blResult = GetVersionEx( &stOSVersion);

	CDialog*	pcAbout;
	if( VER_PLATFORM_WIN32_NT == stOSVersion.dwPlatformId || blResult == 0)
	{
		pcAbout = new CAboutDialog( CAboutDialog::WINTYPE_NT, this);
	}
	else
	{
//		if( VER_PLATFORM_WIN32_WINDOWS == stOSVersion.dwPlatformId)
//		{
			pcAbout = new CAboutDialog( CAboutDialog::WINTYPE_98, this);
//		}
	}

	pcAbout->DoModal();

	delete pcAbout;

	m_blAboutDisplaied = false;
}

void CMainFrame::OnUpdateAppAbout(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( !m_blAboutDisplaied);
}

void CMainFrame::OnAppExit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CString	cStr;
	cStr.LoadString( IDS_EXITINFO);
	if( IDYES == MessageBox( cStr, NULL, MB_YESNO | MB_ICONQUESTION))
	{
		PostMessage( WM_CLOSE, 0, 0L);
	}
}

void CMainFrame::OnAllIcon() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ResizeAllMemoChip( ALL_ICON);
}

void CMainFrame::OnAllNormal()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ResizeAllMemoChip( ALL_NORMAL);
}

void CMainFrame::OnAllTitle()
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	ResizeAllMemoChip( ALL_TITLE);
}

void CMainFrame::ResizeAllMemoChip( int nType)
{
	WPARAM	wParam;
	switch( nType)
	{
	case ALL_NORMAL:
	default:
		wParam = IDM_TONORMAL;
		break;
	case ALL_ICON:
		wParam = IDM_TOICON;
		break;
	case ALL_TITLE:
		wParam = IDM_TOTITLE;
		break;
	}

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		const CMemoData*	pcMemoData;
		for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
			if( pcMemoChip)
			{
				pcMemoData = pcMemoChip->AccessMemoData();
				if( pcMemoData->IsShow())
				{
					if( ALL_NORMAL == nType && pcMemoData->IsPassWordLock())continue;
					if( ALL_ICON == nType && CMemoData::WINDOW_TYPE_TITLE == pcMemoData->GetWindowType())continue;
					if( ALL_TITLE == nType && CMemoData::WINDOW_TYPE_ICON == pcMemoData->GetWindowType())continue;
					pcMemoChip->DrawResizeFrame( FALSE);
					pcMemoChip->PostMessage( WM_COMMAND, wParam, 0L);
				}
			}
		}
		cSglk.Unlock();
	}
}

void CMainFrame::OnAllOncehide() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( false == m_blAllOnceHide)
	{
		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			CMemoChip*	pcMemoChip;
			for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
				if( pcMemoChip)pcMemoChip->OnceHide();
			}
			cSglk.Unlock();
		}
		m_blAllOnceHide = true;

		SetTrayIcon();

#ifndef	_DEBUG
		// あるMLでお勉強しました。
		// ワーキングセットを強制パージするお呪い（“のろい”ではなく“まじない”です。“おのろい”じゃヘンだもン）です。
		// ＮＴだけに効きます。
		SetProcessWorkingSetSize( GetCurrentProcess(), 0xffffffff, 0xffffffff);
#endif
	}
}

void CMainFrame::OnAllTop() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		for( int nIndex = 0; nIndex < m_cLstMemoChip.GetCount(); nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				pcMemoChip->BringWindowToTop();
				/*
				pcMemoChip->SetActiveWindow();
				pcMemoChip->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				*/
			}
		}
		cSglk.Unlock();
	}
}

void CMainFrame::OnUpdatePopups(CCmdUI* pCmdUI)
{
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		pCmdUI->Enable( ( 0 >= m_cLstMemoChip.GetCount()) ? FALSE : TRUE);
	}
}

void CMainFrame::AllRestoreOnceHide( void)
{
	if( !m_blAllOnceHide)return;

	CMemoChip*	pcMemoChip;
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
			if( pcMemoChip)
			{
				pcMemoChip->RestoreOnceHide();
			}
		}
		cSglk.Unlock();
	}
	m_blAllOnceHide = false;

	SetTrayIcon();
}

void CMainFrame::OnMemoimport() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	char	szFolder[ _MAX_PATH];
	GetCurrentDirectory( _MAX_PATH, szFolder);

	CString	cStr;
	cStr.LoadString( IDS_EXPORTFILTER);
	CFileDialog	cFileDlg( TRUE, "mrd", NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, cStr, this);
	if( IDOK == cFileDlg.DoModal())
	{
		LoadMemoDatas( cFileDlg.GetPathName(), TRUE);
	}	

	SetCurrentDirectory( szFolder);
}

void CMainFrame::OnDellist() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CDelListDialog::IsUsed())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CDelListDialog	cDelListDlg( pcSOboe->GetStockFilePath(), this);
	cDelListDlg.DoModal();
}

void CMainFrame::OnUpdateDellist(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( !CDelListDialog::IsUsed());
}

BOOL CMainFrame::ReceiveMemoData( INNERSENDDATA& stInnerData, const CEntryData* pcEntryData)
{
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	// 署名の処理
	{
		CString	cStrAppend;
		if( stInnerData.m_blAppendSig)
		{
			cStrAppend += stInnerData.m_cStrSignature;
		}
		if( stInnerData.m_blAppendSendTime)
		{
			if( !cStrAppend.IsEmpty())cStrAppend += _T( "\r\n");
			cStrAppend += cTime.Format( _T( "%Y/%m/%d - %H:%M:%S"));
		}
		if( !cStrAppend.IsEmpty())
		{
			CString	cStr;
			if( stInnerData.m_nAppendPos)
			{
				cStr = stInnerData.m_cMemoData.GetMemo();
				cStr += _T( "\r\n");
				cStr += cStrAppend;
			}
			else
			{
				cStr = cStrAppend;
				cStr += _T( "\r\n");
				cStr += stInnerData.m_cMemoData.GetMemo();
			}
			stInnerData.m_cMemoData.SetMemo( cStr);
		}
	}

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	// 履歴
	if( pcSOboe->IsLoggingReceiveChip())
	{
		CString	cPathName;
		cPathName = pcSOboe->GetReceiveLogFilePath();
		CFile	cLogFile;
		if( cLogFile.Open( cPathName, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
		{
			if( 1024 * 1024 /*MAX_FILESIZE*/ <= cLogFile.GetLength())
			{
				CString	cStr;
				cStr.LoadString( IDS_RECEIVELOGTOOBIG);
				if( IDYES == MessageBox( cStr, NULL, MB_YESNO | MB_ICONQUESTION))
				{
					cLogFile.SetLength( 0);
				}
			}
			cLogFile.Seek( 0, CFile::end);
			CString	cStrLog;
			SYSTEMTIME	stSysTime;
			GetLocalTime( &stSysTime);
			COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
			CString	cStrReceiveTime;
			cStrReceiveTime = cTime.Format( "%y/%m/%d - %H:%M:%S");
			CString	cStrTitle;
			stInnerData.m_cMemoData.GetTitle( cStrTitle);
			cStrLog.Format( "%s\r\nFrom : %s\r\nTo : %s\r\nData：%s", cStrReceiveTime, stInnerData.m_cStrSignature, stInnerData.m_cStrDistribute, cStrTitle);
			cLogFile.Write( cStrLog, cStrLog.GetLength());
			if( pcSOboe->IsFullLoggingReceiveChip())
			{
				cStrLog = "\r\nMemo:\r\n";
				cLogFile.Write( cStrLog, cStrLog.GetLength());
				cLogFile.Write( stInnerData.m_cMemoData.GetMemo(), stInnerData.m_cMemoData.GetMemoLength());
			}
			cStrLog = "\r\n-----------------------------------\r\n";
			cLogFile.Write( cStrLog, cStrLog.GetLength());
			cLogFile.Close();
		}
	}

	// 関連付けを拒否
	if( !pcSOboe->IsReceptionLink())
	{
		stInnerData.m_cMemoData.Link( FALSE);
	}
	// タイマを拒否
	if( !pcSOboe->IsReceptionTimer())
	{
		stInnerData.m_cMemoData.Timer( FALSE);
	}
	// DXLを拒否
	if( 0 != stInnerData.m_cMemoData.GetDxlID())
	{
		if( !pcSOboe->IsReceptionDxl())
		{
			stInnerData.m_cMemoData.SetDxlID( 0);
			if( !stInnerData.m_cStrDxlDummy.IsEmpty())
			{
				stInnerData.m_cMemoData.SetMemo( stInnerData.m_cStrDxlDummy);
			}
		}
		else
		{
			// あるいは再生可能なDxlが存在していない
			if( 0 > pcSOboe->FindDxlID( stInnerData.m_cMemoData.GetDxlID()))
			{
				if( !stInnerData.m_cStrDxlDummy.IsEmpty())
				{
					stInnerData.m_cMemoData.SetMemo( stInnerData.m_cStrDxlDummy);
				}
			}
		}
	}

	CMemoChip*	pcMemoChip;
	pcMemoChip = new CMemoChip;

	// バージョン差異の通知
	if( VERSION_CODE < stInnerData.m_unVersion)
	{
		CString	cStrWork;
		cStrWork = stInnerData.m_cMemoData.GetMemo();
		CString	cStr;
		cStr.Format( IDS_VEROLDNOTIFY, stInnerData.m_cStrVersion);
		cStrWork += cStr;
		stInnerData.m_cMemoData.SetMemo( cStrWork);
	}

	pcMemoChip->SetMemoData( stInnerData.m_cMemoData);
	// 相手の指定による終了時削除
	pcMemoChip->DelOnQuit( stInnerData.m_blDelOnQuit);
	// 自己設定による終了時削除
	if( pcSOboe->IsDelOnQuit2Rcv())
	{
		pcMemoChip->DelOnQuit( TRUE);
	}
	pcMemoChip->SetEntryData( pcEntryData);
	pcMemoChip->SetReceiveTime( cTime);
	if( stInnerData.m_blPack)
	{
		pcMemoChip->Pack( stInnerData.m_blPassWord);
		pcMemoChip->SetResultID( stInnerData.m_cStrResultID);
		if( stInnerData.m_blChackResult)
		{
			pcMemoChip->SetResults( stInnerData.m_stResults);
		}
		else
		{
			pcMemoChip->CheckOpen( stInnerData.m_blCheckOpen);
		}
	}
	else
	{
		if( m_blAbsence)pcMemoChip->Pack( FALSE);
	}
	
	BOOL	blPopupReceive = FALSE;
	BOOL	blHideReceive = FALSE;
	BOOL	blSubNotify = FALSE;

	blPopupReceive = pcSOboe->IsPopupReceive();
	if( !blPopupReceive && !m_blAbsence)
	{
		blHideReceive = pcSOboe->IsHideReceive();
		blSubNotify = pcSOboe->IsSubNotify();
	}

	if( pcMemoChip->Create( m_unMemoChipID, GetSafeHwnd(), TRUE, m_hIconNormal, m_pcToolTip))
	{
		m_unMemoChipID++;
		if( stInnerData.m_blSendResize)
		{
			pcMemoChip->CalcDefaultRect( stInnerData.m_blSendCenter);
			if( pcMemoChip->IsPack() && stInnerData.m_blSendCenter)
			{
				// NormalRectをNULLに初期化しておくと次のCalcDefaultRectでセンタリングされるンだ。
				// 正直パッチ臭いケドぉ～、しょうがないじゃン！
				CMemoData	cMemoData;
				pcMemoChip->GetMemoData( cMemoData);
				cMemoData.SetNormalRect( CRect( 0, 0, 0, 0));
				pcMemoChip->SetMemoData( cMemoData);
				// ……………………………………………………………………………………でもキライだ。
			}
		}
		else
		{
			if( stInnerData.m_blSendCenter)
			{
				pcMemoChip->CenterWindow();
			}
		}
		PlaySound( "Oboe_Receive", NULL, SND_ALIAS | SND_ASYNC | SND_NODEFAULT | SND_APPLICATION);
		// Popupの通知？
		if( blPopupReceive)
		{
			BringWindowToTop();
			pcMemoChip->SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		else
		{
			pcMemoChip->SetWindowPos( &wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
		}
		AddMemoChip( pcMemoChip);
		pcMemoChip->ReceiveNotify();

		// 受信小片の
		m_nReceiveChip++;
		m_nRenotifyCount = RENOTIFY_COUNT;
		SetTrayIcon();

		if( !blHideReceive)pcMemoChip->RestoreOnceHide();
		if( !blPopupReceive)
		{
			if( blSubNotify)
			{
				CString	cStr;
				stInnerData.m_cMemoData.GetTitle( cStr);

				CNotifyWnd*	pcWnd = NULL;
				pcWnd = new CNotifyWnd;
				pcWnd->Create( cStr, pcMemoChip);
				pcWnd->ShowWindow( SW_SHOWNOACTIVATE);
			}
		}
	}
	else
	{
		delete pcMemoChip;
	}

	// 受信後保存？
	if( pcSOboe->IsReceiveAndSave())
	{
		// 保存ね
		SaveMemoDatas();
	}

	return TRUE;
}

BOOL CMainFrame::ReceiveResult( INNERRESULTDATA& stInnerResult)
{
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CString	cStr;
		cStr.Format( "\r\n%s - %s", stInnerResult.m_cStrSignature, stInnerResult.m_cStrResult);

		CMemoChip*	pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		BOOL blMatch = FALSE;
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( pcMemoChip->IsEqualContextID( stInnerResult.m_cStrChipID))
				{
					if( CMemoEditSheet::IsUsed())
					{
						if( pcMemoChip->GetSafeHwnd() == CMemoEditSheet::GetUseWnd() ||
							pcMemoChip->GetSafeHwnd() == CResizableMemoEditDialog::GetUseWnd())
						{
							nIndex = nCount;
							break;
						}
					}
					blMatch = TRUE;
					pcMemoChip->AddResult( cStr);
					
					break;
				}
			}
		}
		if( FALSE == blMatch)
		{
			MessageBox( cStr, NULL, MB_OK);
		}
		cSglk.Unlock();
	}
	return TRUE;
}

void CMainFrame::OnSendDirect() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CMemoData	cMemoData;

	cMemoData.SetDefaultStyle();
	EditAndSend( cMemoData, 3);
}

void CMainFrame::OnUpdateSendDirect(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	BOOL	blSend = ( pcSOboe) ? pcSOboe->IsStandbyNetwork() : FALSE;

	pCmdUI->Enable( !CMemoEditSheet::IsUsed() && blSend);
}

void CMainFrame::ImeClose()
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

long CMainFrame::OnHotKey( WPARAM wParam, LPARAM lParam)
{
	//WM_HOTKEY
	if( m_uModifiers == ( UINT)LOWORD(lParam))
	{
		if( m_uVirtKey == ( UINT)HIWORD( lParam))
		{
			ImeClose();
			PostMessage( sm_unMsgFromTray, TASKTRAY_ID, WM_RBUTTONUP);
		}
	}
	return 0;
}

long CMainFrame::OnPowerBroadcast( WPARAM wParam, LPARAM lParam)
{
	if( PBT_APMSUSPEND == wParam)
	{
		// サスペンドスタート
		SaveMemoDatas();
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		if( pcSOboe->IsStandbyNetwork())
		{
			pcSOboe->EndNetwork();
			pcSOboe->CloseNetwork( GetSafeHwnd());

			m_blStopedNetworkByApmSuspend = true;
		}
	}
	else
	{
		if( PBT_APMRESUMESUSPEND == wParam)
		{
			// レジュームからの復帰時にタイマ経過チェックを実施させる
			m_nLastNotifyDay = -1;
			if( true == m_blStopedNetworkByApmSuspend)
			{
				m_blStopedNetworkByApmSuspend = false;

				CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
				ASSERT( pcSOboe);
				m_nResumeDelay = pcSOboe->GetResumeDelay();
				if( 0 >= m_nResumeDelay)
				{
					m_nResumeDelay = -1;
					// サスペンド終了
					CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
					ASSERT( pcSOboe);
					if( pcSOboe->IsEnableNetwork())
					{
						if( pcSOboe->OpenNetwork( GetSafeHwnd()))
						{
							pcSOboe->StartNetwork();
						}
					}
				}
			}
		}
	}
	return TRUE;
}

void CMainFrame::OnArrangeIcon() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		if( 0 >= m_cLstMemoChip.GetCount())return;
		// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		// リソースチェックを含むDLLを明示的にリンクします。
		HINSTANCE		hInst = NULL;
		hInst = LoadLibrary( pcSOboe->ArrangeIconDLL());
		// リンクが正常になされた？
		if( NULL != hInst)
		{
			ARRANGESTRUCTVERSION	lpArrangeStructVersion;
			// ヴァージョンの確認
			lpArrangeStructVersion = ( ARRANGESTRUCTVERSION)GetProcAddress( hInst, "GetArrangeSpecification");
			if( lpArrangeStructVersion)
			{
				ARRANGESPEC	stSpecification;
				lpArrangeStructVersion( &stSpecification);
				if( ( UINT)__ARRAGESTRUCT_VERSION200 >= stSpecification.unUpperStructVersion &&
					( UINT)__ARRAGESTRUCT_VERSION100 <= stSpecification.unLowerStructVersion)
				{
					// リソースチェックのDLLから、目的の関数ポインタを取得します。
					ARRANGEICONFUNC lpArrageIconFunc;
					lpArrageIconFunc = ( ARRANGEICONFUNC)GetProcAddress( hInst, "ArrangeIcon");
					if( NULL != lpArrageIconFunc)
					{
						int					nIconCount = m_cLstMemoChip.GetCount();

						if( ( UINT)__ARRAGESTRUCT_VERSION200 <= stSpecification.unUpperStructVersion)
						{
							ArrangeChip( stSpecification.unUpperStructVersion, lpArrageIconFunc, ARG_ICON);
						}
						else
						{
							ARRANGESTRUCT*		pArrangeData;
							SIZE				stSize;

							pArrangeData = new ARRANGESTRUCT[ nIconCount];

							CMemoChip*	pcMemoChip;
							CMemoData	cMemoData;
							for( int nIndex = 0; nIndex < nIconCount; nIndex++)
							{
								pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
								if( pcMemoChip)
								{
									pcMemoChip->GetMemoData( cMemoData);
									pArrangeData[ nIndex].stRctChip		= cMemoData.GetNormalRect();
									pArrangeData[ nIndex].stPntIcon.x	= cMemoData.GetIconRect().left;
									pArrangeData[ nIndex].stPntIcon.y	= cMemoData.GetIconRect().top;
									cMemoData.GetForeColor( pArrangeData[ nIndex].clrText);
									cMemoData.GetBackColor( pArrangeData[ nIndex].clrBack);
									pArrangeData[ nIndex].blIcon		= ( CMemoData::WINDOW_TYPE_ICON == cMemoData.GetWindowType());
									pArrangeData[ nIndex].nIconIndex	= cMemoData.GetIconIndex();
									pArrangeData[ nIndex].blHide		= !cMemoData.IsShow();
									pArrangeData[ nIndex].blPosLock		= cMemoData.IsPosLock();
								}
							}
							stSize.cx = 16 + GetSystemMetrics( SM_CXBORDER) * 2;
							stSize.cy = 16 + GetSystemMetrics( SM_CYBORDER) * 2;

							int nIconMax = _MAX_ICONINDEX + 1;
							const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
							if( pcIconImgs)nIconMax += pcIconImgs->GetExtendCount();

							if( FALSE == lpArrageIconFunc( ( UINT)__ARRAGESTRUCT_VERSION100, nIconCount, pArrangeData, &stSize, nIconMax))
							{
								CRect	cRetc;
								const CMemoData*	pcMemoData;
								for( int nIndex = 0; nIndex < nIconCount; nIndex++)
								{
									pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
									ASSERT( pcMemoChip);
									pcMemoData = pcMemoChip->AccessMemoData();
									if( CMemoData::WINDOW_TYPE_ICON == pcMemoData->GetWindowType())
									{
										pcMemoChip->ArrangeIcon( pArrangeData[ nIndex].stPntIcon);
									}
								}
							}
							delete [] pArrangeData;
							pArrangeData = NULL;
						}
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
		cSglk.Unlock();
	}
}

void CMainFrame::OnUpdateArrangeIcon(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		BOOL	blEnable = FALSE;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		if( !pcSOboe->ArrangeIconDLL().IsEmpty())
		{
			if( 0 < m_cLstMemoChip.GetCount())
			{
				blEnable = TRUE;
			}
		}

		pCmdUI->Enable( blEnable);
	}
}

BOOL CMainFrame::ArrangeChip( UINT unVersion, LPVOID lpvArgFunc, int nCmd)
{
	ARRANGESTRUCT200*	pArrangeData = NULL;
	int		nIconCount = m_cLstMemoChip.GetCount();

	// 整列用のデータ作成
	{
		CMemoChip*	pcMemoChip;
		CMemoData	cMemoData;
		int			nOffset = sizeof( ARRANGESTRUCT200) * nIconCount;

		CMemFile	cMemFile;

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		BYTE*	pabyData;
		ARRANGESTRUCT200	stArrangeData;
		for( int nIndex = 0; nIndex < nIconCount; nIndex++)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				ZeroMemory( &stArrangeData, sizeof( ARRANGESTRUCT200));
				pcMemoChip->GetMemoData( cMemoData);
				stArrangeData.stRctNormal		= cMemoData.GetNormalRect();
				stArrangeData.stRctTitle		= cMemoData.GetTitleRect();
				stArrangeData.stPntIcon			= cMemoData.GetIconRect().TopLeft();
				cMemoData.GetForeColor( stArrangeData.clrText);
				cMemoData.GetBackColor( stArrangeData.clrBack);
				stArrangeData.nWindowType		= cMemoData.GetWindowType();
				stArrangeData.nIconIndex		= cMemoData.GetIconIndex();
				stArrangeData.blHide			= !cMemoData.IsShow();
				stArrangeData.blPosLock			= cMemoData.IsPosLock();

				stArrangeData.blDelOnQuit		= pcMemoChip->IsDelOnQuit();
				stArrangeData.blTimerEnabel		= cMemoData.IsTimer();
				stArrangeData.blLinkEnabel		= cMemoData.IsLink();

				stArrangeData.unDxlID			= cMemoData.GetDxlID();
				stArrangeData.nDxlDataSize		= 0;
				stArrangeData.nDxlDataOffset	= 0;
				if( 0 != stArrangeData.unDxlID)
				{
					stArrangeData.nDxlDataSize = cMemoData.GetDxlExtData();
					if( 0 < stArrangeData.nDxlDataSize)
					{
						pabyData = new BYTE[ stArrangeData.nDxlDataSize];
						if( pabyData)
						{
							stArrangeData.nDxlDataOffset = nOffset;
							cMemoData.GetDxlExtData( pabyData, stArrangeData.nDxlDataSize);

							cMemFile.Seek( nOffset, CFile::begin);
							cMemFile.Write( pabyData, stArrangeData.nDxlDataSize);

							nOffset += stArrangeData.nDxlDataSize;
							delete [] pabyData;
						}
						else
						{
							stArrangeData.nDxlDataSize = 0;
						}
					}
				}
				cMemFile.Seek( nIndex * sizeof( ARRANGESTRUCT200), CFile::begin);
				cMemFile.Write( &stArrangeData, sizeof( ARRANGESTRUCT200));
			/*
				pcMemoChip->GetMemoData( cMemoData);
				pArrangeData[ nIndex].stRctChip			= cMemoData.GetNormalRect();
				pArrangeData[ nIndex].stPntIcon			= cMemoData.GetIconRect().TopLeft();
				pArrangeData[ nIndex].stRctTitle		= cMemoData.GetTitleRect();
				cMemoData.GetForeColor( pArrangeData[ nIndex].clrText);
				cMemoData.GetBackColor( pArrangeData[ nIndex].clrBack);
				pArrangeData[ nIndex].nWindowType		= cMemoData.GetWindowType();
				pArrangeData[ nIndex].nIconIndex		= cMemoData.GetIconIndex();
				pArrangeData[ nIndex].blHide			= !cMemoData.IsShow();
				pArrangeData[ nIndex].blPosLock			= cMemoData.IsPosLock();

				pArrangeData[ nIndex].blDelOnQuit		= pcMemoChip->IsDelOnQuit();
				pArrangeData[ nIndex].blTimerEnabel		= cMemoData.IsTimer();
				pArrangeData[ nIndex].blLinkEnabel		= cMemoData.IsLink();

				pArrangeData[ nIndex].unDxlID			= cMemoData.GetDxlID();
				pArrangeData[ nIndex].nDxlDataSize		= 0;
				pArrangeData[ nIndex].nDxlDataOffset	= 0;
				if( 0 != pArrangeData[ nIndex].unDxlID)
				{
					pArrangeData[ nIndex].nDxlDataSize = cMemoData.GetDxlExtData();
					nNeedSize += pArrangeData[ nIndex].nDxlDataSize;
					// 取り出すサイズが大きい場合に拡張するする！
					if( nNeedSize > nSize)
					{
						nSize = nNeedSize + 256;
						pabyData = ( BYTE*)realloc( pabyData, ( sizeof( ARRANGESTRUCT200) * nIconCount) + nSize);
						if( NULL == pabyData)
						{
							pabyData = ( BYTE*)pArrangeData;
							if( NULL != pabyData)
							{
								free( pabyData);
								pabyData = NULL;
							}
							return FALSE;
						}
						pArrangeData = ( ARRANGESTRUCT200*)pabyData;
					}
					pArrangeData[ nIndex].nDxlDataOffset = nOffset;
					cMemoData.GetDxlExtData( &pabyData[ pArrangeData[ nIndex].nDxlDataOffset], pArrangeData[ nIndex].nDxlDataSize);
					nOffset += pArrangeData[ nIndex].nDxlDataSize;
				}
			*/
			}
		}

		int nLength = cMemFile.GetLength();

		pArrangeData = ( ARRANGESTRUCT200*)malloc( nLength);
		cMemFile.SeekToBegin();
		cMemFile.Read( pArrangeData, nLength);
	}

	// せいれつよびだしぃ！
	if( pArrangeData)
	{
		SIZE	stSize;
		stSize.cx = 16;
		stSize.cy = 16;
		BOOL	blResult = TRUE;

		int nIconMax = _MAX_ICONINDEX + 1;

		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);
		const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
		if( pcIconImgs)nIconMax += pcIconImgs->GetExtendCount();

		if( ARG_NORMAL == nCmd)blResult = ( ( ARRANGECHIPFUNC)lpvArgFunc)( ( UINT)__ARRAGESTRUCT_VERSION200, nIconCount, pArrangeData, nIconMax);
		if( ARG_TITLE == nCmd)blResult = ( ( ARRANGETITLEFUNC)lpvArgFunc)( ( UINT)__ARRAGESTRUCT_VERSION200, nIconCount, pArrangeData, nIconMax);
		if( ARG_ICON == nCmd || ARG_ALL == nCmd)blResult = ( ( ARRANGEICONFUNC)lpvArgFunc)( ( UINT)__ARRAGESTRUCT_VERSION200, nIconCount, pArrangeData, &stSize, nIconMax);

		if( FALSE == blResult)
		{
			CMemoChip* pcMemoChip;
			const CMemoData*	pcMemoData;
			for( int nIndex = 0; nIndex < nIconCount; nIndex++)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
				ASSERT( pcMemoChip);
				pcMemoData = pcMemoChip->AccessMemoData();
				if( ARG_ALL == nCmd || ARG_NORMAL == nCmd)
//				if( ARG_ALL == nCmd || ( ARG_NORMAL == nCmd && CMemoData::WINDOW_TYPE_NORMAL == pcMemoData->GetWindowType()))
				{
					pcMemoChip->ArrangeChip( pArrangeData[ nIndex].stRctNormal);
				}
				if( ARG_ALL == nCmd || ARG_TITLE == nCmd)
//				if( ARG_ALL == nCmd || (  ARG_TITLE == nCmd && CMemoData::WINDOW_TYPE_TITLE == pcMemoData->GetWindowType()))
				{
					pcMemoChip->ArrangeTitle( pArrangeData[ nIndex].stRctTitle);
				}
				if( ARG_ALL == nCmd || ARG_ICON == nCmd)
//				if( ARG_ALL == nCmd || ( ARG_ICON == nCmd && CMemoData::WINDOW_TYPE_ICON == pcMemoData->GetWindowType()))
				{
					pcMemoChip->ArrangeIcon( pArrangeData[ nIndex].stPntIcon);
				}
			}
		}
		free( pArrangeData);
		pArrangeData = NULL;
	}
	return TRUE;
}

void CMainFrame::OnArrangeNormal() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		if( 0 >= m_cLstMemoChip.GetCount())return;
		// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		// リソースチェックを含むDLLを明示的にリンクします。
		HINSTANCE		hInst = NULL;
		hInst = LoadLibrary( pcSOboe->ArrangeNormalDLL());
		// リンクが正常になされた？
		if( NULL != hInst)
		{
			ARRANGESTRUCTVERSION	lpArrangeStructVersion;
			// ヴァージョンの確認
			lpArrangeStructVersion = ( ARRANGESTRUCTVERSION)GetProcAddress( hInst, "GetArrangeSpecification");
			if( lpArrangeStructVersion)
			{
				ARRANGESPEC	stSpecification;
				lpArrangeStructVersion( &stSpecification);
				if( ( UINT)__ARRAGESTRUCT_VERSION200 >= stSpecification.unUpperStructVersion &&
					( UINT)__ARRAGESTRUCT_VERSION100 <= stSpecification.unLowerStructVersion)
				{
					// リソースチェックのDLLから、目的の関数ポインタを取得します。
					ARRANGECHIPFUNC lpArrageChipFunc;
					lpArrageChipFunc = ( ARRANGECHIPFUNC)GetProcAddress( hInst, "ArrangeChip");
					if( NULL != lpArrageChipFunc)
					{
						int	nIconCount = m_cLstMemoChip.GetCount();

						if( ( UINT)__ARRAGESTRUCT_VERSION200 <= stSpecification.unUpperStructVersion)
						{
							ArrangeChip( stSpecification.unUpperStructVersion, lpArrageChipFunc, ARG_NORMAL);
						}
						else
						{
							ARRANGESTRUCT*	pArrangeData;
							SIZE			stSize;

							pArrangeData = new ARRANGESTRUCT[ nIconCount];

							if( pArrangeData)
							{
								CMemoChip*	pcMemoChip;
								CMemoData	cMemoData;
								for( int nIndex = 0; nIndex < nIconCount; nIndex++)
								{
									pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
									if( pcMemoChip)
									{
										pcMemoChip->GetMemoData( cMemoData);
										pArrangeData[ nIndex].stRctChip		= cMemoData.GetNormalRect();
										pArrangeData[ nIndex].stPntIcon.x	= cMemoData.GetIconRect().left;
										pArrangeData[ nIndex].stPntIcon.y	= cMemoData.GetIconRect().top;
										cMemoData.GetForeColor( pArrangeData[ nIndex].clrText);
										cMemoData.GetBackColor( pArrangeData[ nIndex].clrBack);
										pArrangeData[ nIndex].blIcon		= ( CMemoData::WINDOW_TYPE_ICON == cMemoData.GetWindowType());
										pArrangeData[ nIndex].nIconIndex	= cMemoData.GetIconIndex();
										pArrangeData[ nIndex].blHide		= !cMemoData.IsShow();
										pArrangeData[ nIndex].blPosLock		= cMemoData.IsPosLock();
									}
								}
								stSize.cx = 16;
								stSize.cy = 16;

								int nIconMax = _MAX_ICONINDEX + 1;
								const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
								if( pcIconImgs)nIconMax += pcIconImgs->GetExtendCount();

								if( FALSE == lpArrageChipFunc( ( UINT)__ARRAGESTRUCT_VERSION100, nIconCount, pArrangeData, nIconMax))
								{
									CRect	cRetc;
									const CMemoData* pcMemoData;
									for( int nIndex = 0; nIndex < nIconCount; nIndex++)
									{
										pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
										ASSERT( pcMemoChip);
										pcMemoData = pcMemoChip->AccessMemoData();
										if( CMemoData::WINDOW_TYPE_NORMAL == pcMemoData->GetWindowType())
										{
											pcMemoChip->ArrangeChip( pArrangeData[ nIndex].stRctChip);
										}
									}
								}
								delete [] pArrangeData;
								pArrangeData = NULL;
							}
						}
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
		cSglk.Unlock();
	}
}

void CMainFrame::OnUpdateArrangeNormal(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		BOOL	blEnable = FALSE;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		if( !pcSOboe->ArrangeNormalDLL().IsEmpty())
		{
			if( 0 < m_cLstMemoChip.GetCount())
			{
				blEnable = TRUE;
			}
		}

		pCmdUI->Enable( blEnable);
	}
}

void CMainFrame::OnArrangeTitle() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		if( 0 >= m_cLstMemoChip.GetCount())return;
		// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		// リソースチェックを含むDLLを明示的にリンクします。
		HINSTANCE		hInst = NULL;
		hInst = LoadLibrary( pcSOboe->ArrangeTitleDLL());
		// リンクが正常になされた？
		if( NULL != hInst)
		{
			ARRANGESTRUCTVERSION	lpArrangeStructVersion;
			// ヴァージョンの確認
			lpArrangeStructVersion = ( ARRANGESTRUCTVERSION)GetProcAddress( hInst, "GetArrangeSpecification");
			if( lpArrangeStructVersion)
			{
				ARRANGESPEC	stSpecification;
				lpArrangeStructVersion( &stSpecification);
				if( ( UINT)__ARRAGESTRUCT_VERSION200 <= stSpecification.unUpperStructVersion &&
					( UINT)__ARRAGESTRUCT_VERSION200 >= stSpecification.unLowerStructVersion)
				{
					// リソースチェックのDLLから、目的の関数ポインタを取得します。
					ARRANGECHIPFUNC lpArrageChipFunc;
					lpArrageChipFunc = ( ARRANGECHIPFUNC)GetProcAddress( hInst, "ArrangeTitle");
					if( NULL != lpArrageChipFunc)
					{
						ArrangeChip( stSpecification.unUpperStructVersion, lpArrageChipFunc, ARG_TITLE);
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
		cSglk.Unlock();
	}
}

void CMainFrame::OnUpdateArrangeTitle(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		BOOL	blEnable = FALSE;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		if( !pcSOboe->ArrangeTitleDLL().IsEmpty())
		{
			if( 0 < m_cLstMemoChip.GetCount())
			{
				blEnable = TRUE;
			}
		}

		pCmdUI->Enable( blEnable);
	}
}

void CMainFrame::OnArrangeAll() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		if( 0 >= m_cLstMemoChip.GetCount())return;
		// TODO: この位置にコマンド ハンドラ用のコードを追加してください
		// リソースチェックを含むDLLを明示的にリンクします。
		HINSTANCE		hInst = NULL;
		hInst = LoadLibrary( pcSOboe->ArrangeTitleDLL());
		// リンクが正常になされた？
		if( NULL != hInst)
		{
			ARRANGESTRUCTVERSION	lpArrangeStructVersion;
			// ヴァージョンの確認
			lpArrangeStructVersion = ( ARRANGESTRUCTVERSION)GetProcAddress( hInst, "GetArrangeSpecification");
			if( lpArrangeStructVersion)
			{
				ARRANGESPEC	stSpecification;
				lpArrangeStructVersion( &stSpecification);
				if( ( UINT)__ARRAGESTRUCT_VERSION200 >= stSpecification.unUpperStructVersion &&
					( UINT)__ARRAGESTRUCT_VERSION100 <= stSpecification.unLowerStructVersion)
				{
					// リソースチェックのDLLから、目的の関数ポインタを取得します。
					ARRANGECHIPFUNC lpArrageChipFunc;
					lpArrageChipFunc = ( ARRANGECHIPFUNC)GetProcAddress( hInst, "ArrangeAll");
					if( NULL != lpArrageChipFunc)
					{
						ArrangeChip( stSpecification.unUpperStructVersion, lpArrageChipFunc, ARG_TITLE);
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
		cSglk.Unlock();
	}
}

void CMainFrame::OnUpdateArrangeAll(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		BOOL	blEnable = FALSE;
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		ASSERT( pcSOboe);

		if( !pcSOboe->ArrangeAllDLL().IsEmpty())
		{
			if( 0 < m_cLstMemoChip.GetCount())
			{
				blEnable = TRUE;
			}
		}

		pCmdUI->Enable( blEnable);
	}
}

void CMainFrame::OnClientreserch() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pcSOboe->EndNetwork();
	Sleep( 200);
	pcSOboe->StartNetwork( m_blAbsence);
}

void CMainFrame::OnUpdateClientreserch(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	BOOL	blEnable = FALSE;
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	blEnable = pcSOboe->IsStandbyNetwork();

	pCmdUI->Enable( blEnable);
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	if( CMemoData::GetClipboardFormat() == pCopyDataStruct->dwData)
	{
		CMemoData*	pcMemoData = CMemoData::CreateMemoFromBinData( ( LPBYTE)pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		if( pcMemoData)
		{
			CreateChipFromMemoData( *pcMemoData);
			delete pcMemoData;

			return TRUE;
		}
	}
	
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

void CMainFrame::OnNonpopup() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->PopupReceive( !pcSOboe->IsPopupReceive());
}

void CMainFrame::OnUpdateNonpopup(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pCmdUI->Enable( pcSOboe->IsStandbyNetwork() && !m_blAbsence);
	pCmdUI->SetCheck( !pcSOboe->IsPopupReceive());
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( m_pcToolTip)m_pcToolTip->RelayEvent(pMsg);
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnCustom() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CSOboeApp::IsCustomizing())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( IDOK == pcSOboe->DoCustomize( this))
	{
		if( m_uVirtKey)VERIFY( UnregisterHotKey( GetSafeHwnd(), 0));

		m_uModifiers = pcSOboe->GetHKModifiers();
		m_uVirtKey = pcSOboe->GetHKVirtKey();

		pcSOboe->LoadTemplet();

		if( m_uVirtKey)VERIFY( RegisterHotKey( GetSafeHwnd(), 0, m_uModifiers, m_uVirtKey));
	}
}

void CMainFrame::OnUpdateCustom(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( !CSOboeApp::IsCustomizing());
}

void CMainFrame::OnTempletMenu( UINT nID)
{
	if( CMemoEditSheet::IsUsed())return;

	if( IDM_TEMPLET0MENU > nID || IDM_TEMPLET9MENU < nID)return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	int	nCount = pcSOboe->GetTempletCount();
	int nIndex = nID - IDM_TEMPLET0MENU;
	if( nIndex < nCount)
	{
		const CMemoData*	pcMemoData;
		pcMemoData = pcSOboe->GetTemplet( nIndex);

		EditAndCreate( *pcMemoData);
	}
}

void CMainFrame::OnUpdateTempletMenu( CCmdUI* pCmdUI)
{
	if( ( IDM_TEMPLET0MENU <= pCmdUI->m_nID && IDM_TEMPLET9MENU >= pCmdUI->m_nID) ||
		( IDM_TEMPLET0MENU_S <= pCmdUI->m_nID && IDM_TEMPLET9MENU_S >= pCmdUI->m_nID))
	{
		pCmdUI->Enable( !CMemoEditSheet::IsUsed());
	}
}

void CMainFrame::OnSendTempletMenu( UINT nID)
{
	if( CMemoEditSheet::IsUsed())return;

	if( IDM_TEMPLET0MENU_S > nID || IDM_TEMPLET9MENU_S < nID)return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	int	nCount = pcSOboe->GetTempletCount();
	int nIndex = nID - IDM_TEMPLET0MENU_S;
	if( nIndex < nCount)
	{
		const CMemoData*	pcMemoData;
		pcMemoData = pcSOboe->GetTemplet( nIndex);

		EditAndSend( *pcMemoData);
	}
}

void CMainFrame::OnEndSession(BOOL bEnding)
{
	if (bEnding)
	{
		QuitProcess();
	}
}

BOOL CMainFrame::OnQueryEndSession() 
{
	if (!CFrameWnd::OnQueryEndSession())
		return FALSE;
	
	// TODO: この位置に特殊なクエリー終了セッション用のコードを追加してください。
	BOOL blResult = TRUE;

	// 受信処理停止
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pcSOboe->SuspendReceive();
	pcSOboe->EndNetwork();
	pcSOboe->CloseNetwork( GetSafeHwnd());

	// 未開封小片があるかどうかをチェックします。
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		if( m_nReceiveChip)
		{
			BOOL	blPopupReceive = FALSE;
			BOOL	blHideReceive = FALSE;

			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			blPopupReceive = pcSOboe->IsPopupReceive();
			if( !blPopupReceive)
			{
				blHideReceive = pcSOboe->IsHideReceive();
			}
			CMemoChip*	pcMemoChip;
			int nCount = m_cLstMemoChip.GetCount();
			for( int nIndex = nCount - 1; nIndex >= 0; nIndex--)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
				if( pcMemoChip)
				{
					if( pcMemoChip->NoCheckReceive())
					{
						if( !blPopupReceive && blHideReceive)
						{
							pcMemoChip->RestoreOnceHide();
						}
						pcMemoChip->OneTimeTop();
						pcMemoChip->BringWindowToTop();
						blResult = FALSE;
					}
				}
			}
		}
		cSglk.Unlock();
	}

	return blResult;
}

BOOL CMainFrame::QuitProcess( void)
{
	// タイマの停止
	KillTimer( TIMER_ID);
	// ショートカット停止
	if( m_uVirtKey)
	{
		VERIFY( UnregisterHotKey( GetSafeHwnd(), 0));
	}

	// データ処理を最優先事項で処理
	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = nCount - 1; nIndex >= 0; nIndex--)
		{
			pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( nIndex));
			if( pcMemoChip)
			{
				if( pcMemoChip->IsDelOnQuit())
				{
					StockMemoData( *pcMemoChip);
					m_cLstMemoChip.RemoveAt( m_cLstMemoChip.FindIndex( nIndex));
					pcMemoChip->SmartDelete();
				}
			}
		}
		cSglk.Unlock();
	}
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( m_blLoadedMemoData)
	{
		if( pcSOboe->IsMakeBackupFile())
		{
			BackupDataFile();
		}
		SaveMemoDatas();
	}
	// 終了のための保存処理をしたので次には保存せん様にする
	m_blLoadedMemoData = false;

	if( cSglk.Lock( INFINITE))
	{
		CMemoChip*	pcMemoChip;
		while( m_cLstMemoChip.GetCount())
		{
			pcMemoChip = m_cLstMemoChip.GetHead();
			m_cLstMemoChip.RemoveHead();
			if( pcMemoChip)
			{
				pcMemoChip->SmartDelete();
				pcMemoChip = NULL;
			}
		}
		cSglk.Unlock();
	}

	DelTrayIcon();

	DestroyIcon( m_hIconHide);
	DestroyIcon( m_hIconNormal);
	DestroyIcon( m_hIconReceive);
	DestroyIcon( m_hIconAbsence);

	return TRUE;
}

void CMainFrame::OnAbsence() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	m_blAbsence = !m_blAbsence;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pcSOboe->NotifyAbsence( m_blAbsence);

	SetTrayIcon();
}

void CMainFrame::OnUpdateAbsence(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pCmdUI->Enable( pcSOboe->IsStandbyNetwork());
	pCmdUI->SetCheck( m_blAbsence);
}

void CMainFrame::OnHelp() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	pcSOboe->WinHelp( 1);
}

BOOL CMainFrame::ChangeFocus( UINT unDelWndID, BOOL blPrev)
{
	BOOL	blResult = FALSE;

	CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
	if( cSglk.Lock( INFINITE))
	{
		POSITION	Pos;
		CMemoChip*	pcMemoChip = NULL;
		int nCount = m_cLstMemoChip.GetCount();
		for( int nIndex = 0; nIndex < nCount; nIndex++)
		{
			Pos = m_cLstMemoChip.FindIndex( nIndex);
			pcMemoChip = m_cLstMemoChip.GetAt( Pos);
			if( pcMemoChip)
			{
				if( unDelWndID == pcMemoChip->GetID())
				{
					if( TRUE == blPrev)
					{
						m_cLstMemoChip.GetPrev( Pos);
						if( NULL == Pos)
							pcMemoChip = m_cLstMemoChip.GetTail();
						else
							pcMemoChip = m_cLstMemoChip.GetAt( Pos);
					}
					else
					{
						m_cLstMemoChip.GetNext( Pos);
						if( NULL == Pos)
							pcMemoChip = m_cLstMemoChip.GetHead();
						else
							pcMemoChip = m_cLstMemoChip.GetAt( Pos);
					}
					if( pcMemoChip)
					{
						pcMemoChip->SetActiveWindow();
						pcMemoChip->SetWindowPos( &wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
					}
					blResult = TRUE;
				}
			}
		}
		cSglk.Unlock();
	}

	return blResult;
}

void CMainFrame::OnSavenow() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	SaveMemoDatas();
}

void CMainFrame::OnStopnxl() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe->IsEnableNetwork())
	{
		if( pcSOboe->IsStandbyNetwork())
		{
			pcSOboe->EndNetwork();
			pcSOboe->CloseNetwork( GetSafeHwnd());
		}
	}
}

void CMainFrame::OnStartnxl() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe->IsEnableNetwork())
	{
		if( !pcSOboe->IsStandbyNetwork())
		{
			if( pcSOboe->OpenNetwork( GetSafeHwnd()))
			{
				pcSOboe->StartNetwork();
			}
		}
	}
}

void CMainFrame::OnUpdateStopnxl(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	BOOL	blStandby = FALSE;
	if( pcSOboe->IsEnableNetwork())
	{
		 blStandby = pcSOboe->IsStandbyNetwork();
	}
	pCmdUI->Enable( blStandby);
}

void CMainFrame::OnUpdateStartnxl(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	BOOL	blStandby = FALSE;
	if( pcSOboe->IsEnableNetwork())
	{
		 blStandby = !pcSOboe->IsStandbyNetwork();
	}
	pCmdUI->Enable( blStandby);
}

void CMainFrame::OnExedit() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->ExEdit( !pcSOboe->IsExEdit());
}

void CMainFrame::OnUpdateExedit(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pCmdUI->SetCheck( pcSOboe->IsExEdit());
}

void CMainFrame::OnBigbutton() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->BigButton( !pcSOboe->IsBigButton());
}

void CMainFrame::OnUpdateBigbutton(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pCmdUI->Enable( pcSOboe->IsExEdit());
	pCmdUI->SetCheck( pcSOboe->IsBigButton());
}

void CMainFrame::OnColorprint() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pcSOboe->ColorPrint( !pcSOboe->IsColorPrint());
}

void CMainFrame::OnUpdateColorprint(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	pCmdUI->SetCheck( pcSOboe->IsColorPrint());
}

void CMainFrame::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
//	lpwndpos->flags |= SWP_HIDEWINDOW;
}


HMENU CMainFrame::ListUp( const CCatalogList* pcCatalogList, int nIndex)
{
	if( CATALOG_COUNT <= nIndex)return NULL;

	CMenu	cMenu;
	if( cMenu.CreatePopupMenu())
	{
		POSITION	pos;
		MENUITEM*	pstMenuItem;
		int nOffset = 0;
		for( ; nOffset < CATALOG_GROUP_COUNT; nOffset++)
		{
			pos = pcCatalogList->FindIndex( nIndex + nOffset);
			if( NULL == pos)break;
			pstMenuItem = pcCatalogList->GetAt( pos);
			cMenu.AppendMenu( MF_OWNERDRAW | MF_ENABLED, pstMenuItem->unItemID, ( LPCSTR)pstMenuItem);
		}
		if( NULL != pos)
		{
			HMENU hSubMenu = ListUp( pcCatalogList, nIndex + nOffset);
			if( hSubMenu)
			{
				CString	cStr;
				cStr.LoadString( IDS_OTHERCHIP);
				/*
				cMenu.InsertMenu( 0, MF_SEPARATOR | MF_BYPOSITION);
				cMenu.InsertMenu( 0, MF_POPUP | MF_BYPOSITION, ( UINT)hSubMenu, cStr);
				*/
				cMenu.AppendMenu( MF_SEPARATOR | MF_BYPOSITION);
				cMenu.AppendMenu( MF_POPUP | MF_BYPOSITION, ( UINT)hSubMenu, cStr);
			}
		}
		if( 0 < cMenu.GetMenuItemCount())
		{
			return cMenu.Detach();
		}
	}
	return NULL;
}

void CMainFrame::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	if( ODT_MENU != lpDrawItemStruct->CtlType)
	{
		CFrameWnd::OnDrawItem( nIDCtl, lpDrawItemStruct);
		return;
	}

	COLORREF	clrBkColor;
	COLORREF	clrTextColor;

	if( lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		clrBkColor = GetSysColor( COLOR_HIGHLIGHT);
		clrTextColor = GetSysColor( COLOR_HIGHLIGHTTEXT);
	}
	else
	{
		if( lpDrawItemStruct->itemState & ODS_DISABLED)
		{
			clrBkColor = ::GetBkColor( lpDrawItemStruct->hDC);
			clrTextColor = ::GetTextColor( lpDrawItemStruct->hDC);
		}
		else
		{
			clrBkColor = GetSysColor( COLOR_MENU);
			clrTextColor = GetSysColor( COLOR_MENUTEXT);
		}
	}

	CBrush cBrush( clrBkColor);
	::FillRect( lpDrawItemStruct->hDC, &lpDrawItemStruct->rcItem, ( HBRUSH)cBrush.GetSafeHandle());

	if( 0 <= lpDrawItemStruct->itemID)
	{
		int nSaveID = SaveDC( lpDrawItemStruct->hDC);
		if( nSaveID)
		{
			MENUITEM*	pstMenuItem = ( MENUITEM*)lpDrawItemStruct->itemData;

			::SetBkColor( lpDrawItemStruct->hDC, pstMenuItem->clrBack);
			::SetTextColor( lpDrawItemStruct->hDC, pstMenuItem->clrFore);
			CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
			ASSERT( pcSOboe);
			const CIconImgList* pcIconImgs = pcSOboe->GetIconList();
			if( pcIconImgs)
			{
				CRect	cRect;
				cRect = lpDrawItemStruct->rcItem;
				cRect.right = cRect.left + _ICON_WIDTH;
				pcIconImgs->Draw( pstMenuItem->nIndex, lpDrawItemStruct->hDC, cRect);
			}
			else
			{
				HDC	hMemDC = ::CreateCompatibleDC( lpDrawItemStruct->hDC);
				if( hMemDC)
				{
					HBITMAP	hBitmap;
					HBITMAP	hBitmapOld;
					hBitmap = ::LoadBitmap( AfxGetResourceHandle(), MAKEINTRESOURCE( IDB_ICONIMG));
					if( hBitmap)
					{
						hBitmapOld = ( HBITMAP)::SelectObject( hMemDC, hBitmap);
						::BitBlt( lpDrawItemStruct->hDC, lpDrawItemStruct->rcItem.left + GetSystemMetrics( SM_CXBORDER), lpDrawItemStruct->rcItem.top, _ICON_WIDTH, _ICON_HEIGHT, hMemDC, _ICON_WIDTH * pstMenuItem->nIndex, 0, SRCCOPY);
						::SelectObject( hMemDC, hBitmapOld);
						::DeleteObject( hBitmap);
					}
					::DeleteDC( hMemDC);
				}
			}

			LOGFONT	stLogFont;
			ZeroMemory( &stLogFont, sizeof( LOGFONT));
			stLogFont.lfCharSet = DEFAULT_CHARSET;
			stLogFont.lfHeight = _ICON_HEIGHT - ( GetSystemMetrics( SM_CYBORDER) * 2);
			lstrcpy( stLogFont.lfFaceName, pstMenuItem->lfFaceName);
			HFONT hFont = ::CreateFontIndirect( &stLogFont);

			::SelectObject( lpDrawItemStruct->hDC, hFont);
			::SetBkColor( lpDrawItemStruct->hDC, clrBkColor);
			::SetTextColor( lpDrawItemStruct->hDC, clrTextColor);

			lpDrawItemStruct->rcItem.left += _ICON_WIDTH;
			lpDrawItemStruct->rcItem.left += GetSystemMetrics( SM_CXBORDER);
			lpDrawItemStruct->rcItem.left += GetSystemMetrics( SM_CXFRAME);
			::DrawText( lpDrawItemStruct->hDC, pstMenuItem->cStrTitle, -1, &lpDrawItemStruct->rcItem, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			::RestoreDC( lpDrawItemStruct->hDC, nSaveID);

			::DeleteObject( hFont);
		}
	}
}

void CMainFrame::OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	if( ODT_MENU != lpMeasureItemStruct->CtlType)
	{
		CFrameWnd::OnMeasureItem( nIDCtl, lpMeasureItemStruct);
		return;
	}

	if( ODT_MENU != lpMeasureItemStruct->CtlType)return;
	if( NULL != lpMeasureItemStruct->itemData)
	{
		MENUITEM*	pstMenuItem = ( MENUITEM*)lpMeasureItemStruct->itemData;

		lpMeasureItemStruct->itemWidth = pstMenuItem->stSize.cx;
		lpMeasureItemStruct->itemHeight = pstMenuItem->stSize.cy;
	}
}

void CMainFrame::OnUpdateexticons() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	if( CSOboeApp::IsUpdating())return;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	if( pcSOboe->DoUpdateIcon( this))
	{
		const CMemoData* pcMemoData;
		CMemoChip* pcMemoChip;
		CSingleLock cSglk( &m_cLstMemoChip.m_cCriticalSection);
		if( cSglk.Lock( INFINITE))
		{
			for( int i = 0; i < m_cLstMemoChip.GetCount(); i++)
			{
				pcMemoChip = m_cLstMemoChip.GetAt( m_cLstMemoChip.FindIndex( i));
				if( pcMemoChip)
				{
					pcMemoData = pcMemoChip->AccessMemoData();
					ASSERT( pcMemoData);

					if( CMemoData::WINDOW_TYPE_TITLE == pcMemoData->GetWindowType() ||
						CMemoData::WINDOW_TYPE_ICON == pcMemoData->GetWindowType())
					{
						pcMemoChip->InvalidateRect( NULL);
					}
				}
			}
			cSglk.Unlock();
		}
	}
}

void CMainFrame::OnUpdateUpdateexticons(CCmdUI* pCmdUI) 
{
	// TODO: この位置に command update UI ハンドラ用のコードを追加してください
	pCmdUI->Enable( !CSOboeApp::IsUpdating());
}
