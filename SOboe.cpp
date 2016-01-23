// SOboe.cpp : アプリケーション用クラスの機能定義を行います。
//

#include "stdafx.h"
#include "SOboe.h"
#include "lmcons.h"
#include "MainFrm.h"
#include "IconImgList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSOboeApp

BEGIN_MESSAGE_MAP(CSOboeApp, CWinApp)
	//{{AFX_MSG_MAP(CSOboeApp)
	//}}AFX_MSG_MAP
	// 標準のファイル基本ドキュメント コマンド
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSOboeApp クラスの構築
CSemaphore	CSOboeApp::sm_cSmpDoCustomize( 1, 1, "SOBOE|EDIT#SEMAPHORE");
CSemaphore	CSOboeApp::sm_cSmpDoUpdating( 1, 1, "SOBOE|EDIT#SEMAPHORE");
UINT	CSOboeApp::m_unOboeFmt = RegisterClipboardFormat( "OboeMemo#Binary");
struct CSOboeApp::COMMAND	CSOboeApp::stCommands[ _COMMAND_MAX] = 
{
	{ "[CreateChip]", IDM_NEWMEMO},
	{ "[CreateChipFromClipBoard]", IDM_PASTE},
	{ "[ChipList]", IDM_DATALIST},
	{ "[DelChipList]", IDM_DELLIST},
	{ "[ChipSend]", IDM_SEND_DIRECT},
	{ "[ChipAllTop]", IDM_ALL_TOP},
	{ "[ChipAllIcon]", IDM_ALL_ICON},
	{ "[ChipAllTitle]", IDM_ALL_TITLE},
	{ "[ChipAllRestore]", IDM_ALL_NORMAL},
	{ "[ChipAllOnceHide]", IDM_ALL_ONCEHIDE},
	{ "[ArrangeTitle]", IDM_ARRANGE_TITLE},
	{ "[ArrangeIcon]", IDM_ARRANGE_ICON},
	{ "[ArrangeChip]", IDM_ARRANGE_NORMAL},
	{ "[ArrangeAll]", IDM_ARRANGE_ALL},
	{ "[CustomOboe]", IDM_CUSTOM}
};

CSOboeApp::CSOboeApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
	m_blInitialized = FALSE;

	// for DDE
	m_dwIdInst			= 0;
	m_hszTopic			= NULL;
	m_hszAppName		= NULL;
	m_blSetWarehouse	= FALSE;

	char	szCompname[ MAX_COMPUTERNAME_LENGTH + 1];
	DWORD	dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	m_cStrCompName = ( 0 >= GetComputerName( szCompname, &dwSize)) ? "Default" : szCompname;
	/* 上のコードはつまりこういう事なんだよぉン
	if( 0 >= GetComputerName( szCompname, &dwSize))
	{
		m_cStrCompName = "Default";
	}
	else
	{
		m_cStrCompName = szCompname;
	}
	*/

	char	szUser[ UNLEN + 1];
	dwSize = UNLEN + 1;
	m_cStrUserName = ( 0 >= GetUserName( szUser, &dwSize)) ? "Anonymous" : szUser;
	/* 上のコードはつまりこういう事なんだよぉン
	if( 0 >= GetUserName( szUser, &dwSize))
	{
		m_cStrUserName = "Anonymous";
	}
	else
	{
		m_cStrUserName = szUser;
	}
	*/
	m_cStrEntryName = m_cStrUserName;

	// DXLの強制初期化
	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		m_acDrawExlib[ nIndex].SetPath( _T(""));
		m_acDrawExlib[ nIndex].SetMenuString( _T(""));
	}
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		m_acNetExlib[ nIndex].SetPath( _T(""));
	}

	// SHIFTを押しながらの移動時のグリッド移動値
	m_nShiftStepX = 10;
	m_nShiftStepY = 10;

	m_hNetInitDll = NULL;
	m_blStandbyNetwork = FALSE;
	// データはバックアップしなくてもイイでしょ？
	// 将来に向けてのコードって奴ねッ
	// と思ったけど、保存するようにデフォルトを変更！
	// バックアップは終了時のみ作成
	m_blMakeBackupFile = TRUE;

	m_blSuspendReceive = FALSE;
	m_blEnableSpMenu = FALSE;

	m_blAbsence = FALSE;

	m_dwInitalDeray = 150;

	m_pcIconImgs = NULL;

	m_nImportCount = 0;
}

/////////////////////////////////////////////////////////////////////////////
// 唯一の CSOboeApp オブジェクト

CSOboeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSOboeApp クラスの初期化

BOOL CSOboeApp::InitInstance()
{
	// 重複起動のチェック
	// プロセス終了時にハンドルが自動開放されるので、面倒は見ない
	HANDLE	hMutex = CreateMutex( NULL, TRUE, "hPrevInstance#Memo|Shin");
	// 重複？ンじゃ、終了ね。
	if( ERROR_ALREADY_EXISTS == GetLastError())
	{
		AfxMessageBox( IDS_ALREADY_EXEC, MB_OK);
		return FALSE;
	}

	if (!AfxOleInit())
	{
		AfxMessageBox("OLE Initialized error");
		return FALSE;
	}

	// マシンタイマのチェック
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
	if( COleDateTime::valid != cTime.GetStatus())
	{
		AfxMessageBox( IDS_ILLEGALTIME, MB_OK);
		return FALSE;
	}

	// 標準的な初期化処理
	// もしこれらの機能を使用せず、実行ファイルのサイズを小さく
	//  したければ以下の特定の初期化ルーチンの中から不必要なもの
	//  を削除してください。

#ifdef _AFXDLL
	Enable3dControls();			// 共有 DLL の中で MFC を使用する場合にはここを呼び出してください。
#else
	Enable3dControlsStatic();	// MFC と静的にリンクしている場合にはここを呼び出してください。
#endif

	if( !DdeInitialize(&m_dwIdInst, reinterpret_cast<PFNCALLBACK>( DdeCallback), CBF_FAIL_ADVISES | CBF_FAIL_SELFCONNECTIONS | APPCMD_FILTERINITS, 0))
	{
		m_hszTopic			= DdeCreateStringHandle( m_dwIdInst, "OboeServer", CP_WINANSI);
		m_hszAppName		= DdeCreateStringHandle( m_dwIdInst, "Oboe", CP_WINANSI);
		DdeNameService( m_dwIdInst, m_hszAppName, 0, DNS_REGISTER);
	}

	// 初期化してからデータ読みをする…
	InitializeProfile();
	LoadProfile();

	LoadDrawExLibs();
	if( m_blEnableNetwork)
	{
		if( FALSE == LoadNetExLibs())
		{
			m_blEnableNetwork = FALSE;
		}
	}

	m_pcIconImgs = new CIconImgList;
	if( m_pcIconImgs)m_pcIconImgs->LoadExtIconData();

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd = new CMainFrame;
	if( ::IsWindow( m_pMainWnd->GetSafeHwnd()))
	{
		m_pMainWnd->ShowWindow( SW_HIDE);
		m_pMainWnd->UpdateWindow();

		m_blInitialized = TRUE;
#ifndef	_DEBUG
		// あるMLでお勉強しました。
		// ワーキングセットを強制パージするお呪い（“のろい”ではなく“まじない”です。“おのろい”じゃヘンだもン）です。
		// ＮＴだけに効きます。
		SetProcessWorkingSetSize( GetCurrentProcess(), 0xffffffff, 0xffffffff);
#endif
	}

	return TRUE;
}

int CSOboeApp::ExitInstance() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	if( m_dwIdInst)
	{
		DdeNameService( m_dwIdInst, m_hszAppName, 0, DNS_UNREGISTER);
		if( NULL != m_hszTopic)DdeFreeStringHandle( m_dwIdInst, m_hszTopic);
		if( NULL != m_hszAppName)DdeFreeStringHandle( m_dwIdInst, m_hszAppName);
		DdeUninitialize( m_dwIdInst);
		m_dwIdInst = 0;
	}

	if( m_blInitialized)
	{
		FreeNetExLibs();
		FreeDrawExLibs();
		SaveProfile();
	}
	FreeTemplet();

	if( m_pcIconImgs)
	{
		delete m_pcIconImgs;
		m_pcIconImgs = NULL;
	}

	return CWinApp::ExitInstance();
}

CDrawExLib* CSOboeApp::GetDrawExLib( int nIndex)
{
	if( 0 <= nIndex && _DRAWEXLIB_MAX > nIndex)
	{
		if( m_acDrawExlib[ nIndex].IsFunctionAssign())
		{
			return &m_acDrawExlib[ nIndex];
		}
	}
	return NULL;
}


UINT CSOboeApp::GetDefaultDxlID( void) const
{
	UINT unResult = 0;

	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		if( m_acDrawExlib[ nIndex].IsFunctionAssign())
		{
			if( m_unDefaultDxlID == m_acDrawExlib[ nIndex].GetDxlID())
			{
				unResult = m_unDefaultDxlID;
				break;
			}
		}
	}

	return unResult;
}

void CSOboeApp::SetDefaultDxlID( UINT unDxlID)
{
	CString cStrData;
	CString	cStrProfile;
	CString cStrKey;

	GetProfileName( cStrProfile);

	cStrKey.LoadString( IDS_REG_DEFAULTDXLID);
	cStrData.Format( "%d", unDxlID);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	m_unDefaultDxlID = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_unDefaultDxlID, cStrProfile);
}


// 通信拡張
CNetExLib* CSOboeApp::GetNetExLib( int nIndex)
{
	if( 0 <= nIndex && _NETEXLIB_MAX > nIndex)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			return &m_acNetExlib[ nIndex];
		}
	}
	return NULL;
}

CNetExLib* CSOboeApp::GetDefaultNetExLib( void)
{
	if( 0 <= m_nDefaultNxl && m_nDefaultNxl < _NETEXLIB_MAX)
	{
		if( m_acNetExlib[ m_nDefaultNxl].IsAssignFunction())
		{
			if( m_acNetExlib[ m_nDefaultNxl].IsSupportSend())
			{
				return &m_acNetExlib[ m_nDefaultNxl];
			}
		}
	}
	return NULL;
}

BOOL CSOboeApp::LoadDrawExLibs( void)
{
	// 設定しにしたがったDLLの読み出し
	BOOL	blResult = FALSE;

	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		blResult |= m_acDrawExlib[ nIndex].LoadDxl();

		m_acDrawExlib[ nIndex].GetDxlID();
	}

	return blResult;
}

BOOL CSOboeApp::FreeDrawExLibs( void)
{
	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		m_acDrawExlib[ nIndex].FreeDxl();
	}

	return TRUE;
}

BOOL CSOboeApp::LoadNetExLibs( void)
{
	BOOL	blResult = FALSE;
	BOOL	blLoad;

	// ネットワーク初期化用のDLLってのは、Winsockの初期化をするンだけれどね。
	// 初期化を内部で行っても良かったんだけれど、外部DLLにしておけばさぁ
	// このDLLで色々な初期化を行えるじゃない？Winsockゃ無い物とかも……そんな訳。
	{
		if( m_hNetInitDll)
		{
			FreeLibrary( m_hNetInitDll);
			m_hNetInitDll = NULL;
		}
		m_hNetInitDll = LoadLibrary( "NetInit.dll");
		if( NULL == m_hNetInitDll)return FALSE;
		INITIALIZEWINSOCK*	pfnInitWinsock;
		pfnInitWinsock = ( INITIALIZEWINSOCK*)GetProcAddress( m_hNetInitDll, "InitializeWinsock");
		if( NULL == pfnInitWinsock)return FALSE;
		WORD wVersion;
		pfnInitWinsock( &wVersion);
	}

	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		blLoad = m_acNetExlib[ nIndex].LoadNxl();
		blResult |= blLoad;
	}

	return blResult;
}

BOOL CSOboeApp::FreeNetExLibs( void)
{
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		m_acNetExlib[ nIndex].FreeNxl();
	}

	if( m_hNetInitDll)
	{
		TERMINATEWINSOCK*	pfnTermWinsock;
		pfnTermWinsock = ( TERMINATEWINSOCK*)GetProcAddress( m_hNetInitDll, "TerminateWinsock");
		if( NULL != pfnTermWinsock)pfnTermWinsock();

		FreeLibrary( m_hNetInitDll);
		m_hNetInitDll = NULL;
	}

	return TRUE;
}

BOOL CSOboeApp::OpenNetwork( HWND hWnd)
{
	m_blStandbyNetwork = FALSE;
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			m_acNetExlib[ nIndex].Initialize( hWnd, GetEntryName());
			m_blStandbyNetwork |= m_acNetExlib[ nIndex].IsSupportSend();
		}
	}

	return TRUE;
}

BOOL CSOboeApp::StartNetwork( BOOL blAbsence)
{
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			m_acNetExlib[ nIndex].StartEntry( blAbsence);
		}
	}
	return TRUE;
}

BOOL CSOboeApp::EndNetwork( void)
{
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			m_acNetExlib[ nIndex].ExitEntry();
		}
	}
	return TRUE;
}

BOOL CSOboeApp::CloseNetwork( HWND hWnd)
{
	m_blStandbyNetwork = FALSE;
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			m_acNetExlib[ nIndex].UnInitialize( hWnd);
		}
	}

	return TRUE;
}

BOOL CSOboeApp::NotifyAbsence( BOOL blAbsence)
{
	m_blAbsence = blAbsence;
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		if( m_acNetExlib[ nIndex].IsAssignFunction())
		{
			m_acNetExlib[ nIndex].AbsenceEntry( blAbsence);
		}
	}

	return TRUE;
}

CString CSOboeApp::GetStockFilePath( void) const
{
	if( !m_blSetWarehouse && IsCompatibleOU())
	{
		HKEY	hChildKey;
		LONG	lResult;
		CString	cStrOpenKey;
		CString	cStrResult;

		cStrResult.Empty();

		cStrOpenKey.LoadString( IDS_REG_DATAFILE);
		lResult = RegOpenKeyEx( HKEY_CURRENT_USER, cStrOpenKey, 0, KEY_READ, &hChildKey);
		if( ERROR_SUCCESS == lResult)
		{
			CString	cStrValue;
			DWORD	dwDataType;
			DWORD	dwDataLength;
			cStrValue.LoadString( IDS_REG_WAREHOUSEPATH);
			lResult = RegQueryValueEx( hChildKey, cStrValue, NULL, &dwDataType, NULL, &dwDataLength);
			if( ERROR_SUCCESS == lResult)
			{
				if( REG_SZ == dwDataType && 0 < dwDataLength)
				{
					char*	pszData;
					pszData = new char[ dwDataLength + 1];
					lResult = RegQueryValueEx( hChildKey, cStrValue, NULL, &dwDataType, ( PBYTE)pszData, &dwDataLength);
					if( ERROR_SUCCESS == lResult)
					{
						cStrResult = pszData;
					}
					delete [] pszData;
				}
			}
			RegCloseKey( hChildKey);
		}
		if( !cStrResult.IsEmpty())return cStrResult;
	}
	return m_cStrStockFilePath;
}

BOOL CSOboeApp::SetStockFilePath( LPCSTR cStrStockFilePath)
{
	CFile	cFileTest;
	if( !cFileTest.Open( cStrStockFilePath, CFile::modeReadWrite | CFile::typeBinary))
	{
		if( !cFileTest.Open( cStrStockFilePath, CFile::modeWrite | CFile::modeNoTruncate | CFile::modeCreate))
		{
			return FALSE;
		}
		cFileTest.Close();
		CFile::Remove( cStrStockFilePath);
	}
	m_cStrStockFilePath = cStrStockFilePath;
	m_blSetWarehouse = TRUE;
	return TRUE;
}

BOOL CSOboeApp::LoadProfile( BOOL blAllLoad)
{
	CString	cStrProfile;
	GetProfileName( cStrProfile);

	OFSTRUCT	of;
	if( HFILE_ERROR != OpenFile( cStrProfile, &of, OF_READ | OF_EXIST))
	{
		CString	cStrKey;
		char	szWork[ 2048];

		// データ保存のパス取得
		cStrKey.LoadString( IDS_REG_FILEPATH);
		if( 0 < GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrSaveFilePath = szWork;
		}
		// エクスポートのデフォルトフォルダ
		cStrKey.LoadString( IDS_REG_EXPORTFOLDER);
		if( 0 < GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrExportFolder = szWork;
		}

		// デフォルトのフォント
		LOGFONT		stLogFont;
		cStrKey.LoadString( IDS_REG_LOGFONT);
		if( GetPrivateProfileStruct( m_cStrUserName, cStrKey, &stLogFont, sizeof( LOGFONT), cStrProfile))
		{
			CopyMemory( &m_stLOGFONT, &stLogFont, sizeof( LOGFONT));
		}
		
		// デフォルトの文字色
		cStrKey.LoadString( IDS_REG_FORECOLOR);
		m_clrForeColor = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrForeColor, cStrProfile);
		// デフォルトの背景色
		cStrKey.LoadString( IDS_REG_BACKCOLOR);
		m_clrBackColor = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrBackColor, cStrProfile);
		// デフォルトのアイコン
		cStrKey.LoadString( IDS_REG_ICONINDEX);
		m_nIconIndex = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nIconIndex, cStrProfile);


		// 設定タイマ経過後文字色
		cStrKey.LoadString( IDS_REG_AFTERFORECOLOR);
		m_clrAfterFore = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrAfterFore, cStrProfile);
		// 設定タイマ経過後背景色
		cStrKey.LoadString( IDS_REG_AFTERBACKCOLOR);
		m_clrAfterBack = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrAfterBack, cStrProfile);

		// 設定タイマ事前通知文字色
		cStrKey.LoadString( IDS_REG_BEFOREFORECOLOR);
		m_clrBeforeFore = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrBeforeFore, cStrProfile);
		// 設定タイマ事前通知背景色
		cStrKey.LoadString( IDS_REG_BEFOREBACKCOLOR);
		m_clrBeforeBack = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrBeforeBack, cStrProfile);
	
		// 削除データ保存パス
		cStrKey.LoadString( IDS_REG_WAREHOUSEPATH);
		if( 0 < GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrStockFilePath = szWork;
		}

		// 大きさ再計算
		cStrKey.LoadString( IDS_REG_AUTORESIZE);
		m_blAutoResize = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAutoResize, cStrProfile);
		// オートトリミング
		cStrKey.LoadString( IDS_REG_AUTOTRIMING);
		m_blAutoTriming = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAutoTriming, cStrProfile);
		// 編集時のIME自動ON
		cStrKey.LoadString( IDS_REG_AUTOIMEOPEN);
		m_blAutoIMEOpen = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAutoIMEOpen, cStrProfile);
		// 編集画面のスクロール有無
		cStrKey.LoadString( IDS_REG_AUTOSCROLL);
		m_blAutoScroll = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAutoScroll, cStrProfile);
		// メニューは階層化しない
		cStrKey.LoadString( IDS_REG_FLATMENU);
		m_blFlatMenu = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blFlatMenu, cStrProfile);
		// 起動時は一時非表示
		cStrKey.LoadString( IDS_REG_HIDESTART);
		m_blHideStart = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blHideStart, cStrProfile);
		// 削除データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGDELCHIP);
		m_blLoggingDelChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blLoggingDelChip, cStrProfile);
		// 非表示は非表示へ
		cStrKey.LoadString( IDS_REG_HIDEBACK);
		m_blHideback = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blHideback, cStrProfile);
		// ヴァージョン通知
		cStrKey.LoadString( IDS_REG_NOTIFIYVERSION);
		m_blNotifyVersion = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blNotifyVersion, cStrProfile);
		// 削除時のメッセージ
		cStrKey.LoadString( IDS_REG_NOCHECKDELETE);
		m_blNoCheckDelete = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blNoCheckDelete, cStrProfile);
		// 送信データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGSENDCHIP);
		m_blLoggingSendChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blLoggingSendChip, cStrProfile);
		// 送信データの全ログ採り
		cStrKey.LoadString( IDS_REG_FULLLOGGINGSENDCHIP);
		m_blFullLoggingSendChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blFullLoggingSendChip, cStrProfile);
		// 受信データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGRECEIVECHIP);
		m_blLoggingReceiveChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blLoggingReceiveChip, cStrProfile);
		// 受信データの全ログ採り
		cStrKey.LoadString( IDS_REG_FULLLOGGINGRECEIVECHIP);
		m_blFullLoggingReceiveChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blFullLoggingReceiveChip, cStrProfile);
		// 送信時の終了時削除
		cStrKey.LoadString( IDS_REG_CHECKDELONQUIT);
		m_blCheckDelOnQuit = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blCheckDelOnQuit, cStrProfile);
		// 受信小片へ「終了時削除」の適用
		cStrKey.LoadString( IDS_REG_DELONQUIT2RCV);
		m_blDelOnQuit2Rcv = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blDelOnQuit2Rcv, cStrProfile);
		// 送信時元小片を削除
		cStrKey.LoadString( IDS_REG_DELETEAFTERSEND);
		m_blDeleteAfterSend = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blDeleteAfterSend, cStrProfile);
		// 関連付けの受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONLINK);
		m_blReceptionLink = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blReceptionLink, cStrProfile);
		// タイマ受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONTIMER);
		m_blReceptionTimer = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blReceptionTimer, cStrProfile);
		// タイマ受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONDXL);
		m_blReceptionDxl = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blReceptionDxl, cStrProfile);


		// 非Popup受信
		cStrKey.LoadString( IDS_REG_POPUPRECEIVE);
		m_blPopupReceive = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blPopupReceive, cStrProfile);
			// 受信小片非表示
			cStrKey.LoadString( IDS_REG_HIDERECEIVE);
			m_blHideReceive = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blHideReceive, cStrProfile);
			// 通知Windowあり
			cStrKey.LoadString( IDS_REG_NOTIFYRECEIVE);
			m_blSubNotify = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blSubNotify, cStrProfile);

		// 送信時の署名添付
		cStrKey.LoadString( IDS_REG_APPENDSIGNATURE);
		m_blAppendSignature = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAppendSignature, cStrProfile);
		// 送信時添付
		cStrKey.LoadString( IDS_REG_APPENDTIME);
		m_blAppendTime = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blAppendTime, cStrProfile);
			// 添付位置
			cStrKey.LoadString( IDS_REG_APPENDPOS);
			m_nAppendPos = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nAppendPos, cStrProfile);
		// 裏返し送信
		cStrKey.LoadString( IDS_REG_PACKSEND);
		m_blPackSend = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blPackSend, cStrProfile);
			// 表返し確認
			cStrKey.LoadString( IDS_REG_OPENCHECK);
			m_blNotifyOpen = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blNotifyOpen, cStrProfile);
		// 署名
		cStrKey.LoadString( IDS_REG_SIGNATURE);
		if( 0 < GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrSignature = szWork;
		}
		// 署名が取得できなかったので作成する
		if( m_cStrSignature.IsEmpty())
		{
			m_cStrSignature = m_cStrUserName;
			m_cStrSignature += TEXT( " in ");
			m_cStrSignature += m_cStrCompName;
		}
		
		// 左Ｗクリック
		cStrKey.LoadString( IDS_REG_LDBLCLKFUNC);
		m_nLDblClkFunc = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nLDblClkFunc, cStrProfile);
		// 中Ｗクリック
		cStrKey.LoadString( IDS_REG_MDBLCLKFUNC);
		m_nMDblClkFunc = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nMDblClkFunc, cStrProfile);

		// 小片リストのアイコン
		cStrKey.LoadString( IDS_REG_DATALISTFILTER);
		m_nDataListFilter = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0, cStrProfile);
		// 小片リストの逆順設定
		cStrKey.LoadString( IDS_REG_DATALISTREVERSE);
		m_blDataListReverse = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blDataListReverse, cStrProfile);

		// 削除済みリストの逆順設定
		cStrKey.LoadString( IDS_REG_DELLISTREVERSE);
		m_blDelListReverse = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);

		// ホットキーの情報
		cStrKey.LoadString( IDS_REG_HOTKEY);
		m_dwHotKey = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0x30041, cStrProfile);

		// サスペンド復帰からネットワーク初期化までの時間
		cStrKey.LoadString( IDS_REG_RESUMEDELAY);
		m_nResumeDelay = GetPrivateProfileInt( m_cStrUserName, cStrKey, -1, cStrProfile);

		// 超拡張メニュー
		cStrKey.LoadString( IDS_REG_ENABLESPMENU);
		m_blEnableSpMenu = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blEnableSpMenu, cStrProfile);

		// 小片整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGENORMALDLL);
		if( 0 <= GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrArrangeNormalDLL = szWork;
		}
		// 整列小片整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGETITLEDLL);
		if( 0 <= GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrArrangeTitleDLL = szWork;
		}
		// アイコン整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGEICONDLL);
		if( 0 <= GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrArrangeIconDLL = szWork;
		}
		// 状態非依存整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGEALLDLL);
		if( 0 <= GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			m_cStrArrangeAllDLL = szWork;
		}

		// デフォルトのNXL
		cStrKey.LoadString( IDS_REG_DEFAULTDXLID);
		m_unDefaultDxlID = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_unDefaultDxlID, cStrProfile);

		// DXLの読み出し
		OFSTRUCT	ofDxl;
		for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
		{
			cStrKey.Format( IDS_REG_DXLPATH, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			if( HFILE_ERROR != OpenFile( szWork, &ofDxl, OF_READ | OF_EXIST))
			{
				m_acDrawExlib[ nIndex].SetPath( szWork);

				cStrKey.Format( IDS_REG_DXLMENU, nIndex);
				GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
				m_acDrawExlib[ nIndex].SetMenuString( szWork);
			}
			else
			{
				m_acDrawExlib[ nIndex].SetPath( _T(""));
				m_acDrawExlib[ nIndex].SetMenuString( _T(""));
			}
		}
		// デフォルトのNXL
		cStrKey.LoadString( IDS_REG_DEFAULTNXL);
		m_nDefaultNxl = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nDefaultNxl, cStrProfile);

		// ネットワークの有無
		cStrKey.LoadString( IDS_REG_NETWORKENABLE);
		m_blEnableNetwork = GetPrivateProfileInt( m_cStrUserName, cStrKey, -1, cStrProfile);
		// NXLの読み出し
		OFSTRUCT	ofNxl;
		for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
		{
			cStrKey.Format( IDS_REG_NXLPATH, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			if( HFILE_ERROR != OpenFile( szWork, &ofNxl, OF_READ | OF_EXIST))
			{
				m_acNetExlib[ nIndex].SetPath( szWork);
			}
			else
			{
				m_acNetExlib[ nIndex].SetPath( _T(""));
			}
		}

		// Celの読み出し
		CString	cStrPath;
		CString	cStrMenu;
		for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
		{
			cStrKey.Format( IDS_REG_CELPATH, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			cStrPath = szWork;
			cStrKey.Format( IDS_REG_CELMENU, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			cStrMenu = szWork;
			m_cCelControl.SetSelData( nIndex, cStrPath, cStrMenu);
		}
		cStrKey.LoadString( IDS_REG_CELMENUSEPARATOR);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		cStrMenu = szWork;
		for( int nIndex = ( _CEL_MAX - cStrMenu.GetLength()); nIndex >= 0; nIndex--)
		{
			cStrMenu += '0';
		}
		m_cCelControl.SetMenuSeparator( cStrMenu);


		cStrKey.LoadString( IDS_REG_GLOBALPASSWORD);
		m_cStrGlobalPassWord.Empty();
		if( GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile))
		{
			char	chData;
			int	nLen = lstrlen( szWork);
			for( int nIndex = 0; nIndex < nLen; nIndex+=2)
			{
				chData = ( ( 0x0F & szWork[ nIndex]) << 4) | ( 0x0F & szWork[ nIndex + 1]);
				chData = ~chData;
				m_cStrGlobalPassWord += chData;
			}
		}

		
		// テンプレートファイル
		cStrKey.LoadString( IDS_REG_ENABLETEMPLET);
		m_blEnableTemplet = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blEnableTemplet, cStrProfile);
		// テンプレートファイル
		cStrKey.LoadString( IDS_REG_TEMPLETFILE);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrTempletFile = szWork;

		// ユーザー設定による定義色
		for( int nIndex = 0; nIndex < ( _USERCOLOR_MAX - 2); nIndex++)
		{
			cStrKey.Format( IDS_REG_USERCOLORS, nIndex);
			m_clrCustomColor[ nIndex] = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrCustomColor[ nIndex], cStrProfile);
		}

		// 覚え書ユーティリティとの互換処理
		cStrKey.LoadString( IDS_REG_COMPATIBLEOU);
		m_blCompatibleOU = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blCompatibleOU, cStrProfile);

		// Nxlの情報に基づくグループ分け
		cStrKey.LoadString( IDS_REG_ENABLENXLGROUP);
		m_blEnableNxlGroup = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blEnableNxlGroup, cStrProfile);

		// データバックアップの世代数
		cStrKey.LoadString( IDS_REG_GENERATIONCOUNT);
		m_nGenerationCount = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nGenerationCount, cStrProfile);

		// 編集ダイアログの種類？
		cStrKey.LoadString( IDS_REG_EXEDIT);
		m_blExEdit = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blExEdit, cStrProfile);

			// 編集ダイアログの種類？ツールボタンの大小
			cStrKey.LoadString( IDS_REG_BIGBUTTON);
			m_blBigButton = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blBigButton, cStrProfile);

			// 編集ダイアログの種類？大きさ？
			RECT	stRect;
			cStrKey.LoadString( IDS_REG_EXEDITRECT);
			if( GetPrivateProfileStruct( m_cStrUserName, cStrKey, &stRect, sizeof( RECT), cStrProfile))
			{
				m_cRctEditWindow = stRect;
			}

		// ツールチップの最初の時間ね
		cStrKey.LoadString( IDS_REG_INITALDERAY);
		m_dwInitalDeray = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_dwInitalDeray, cStrProfile);

		// 送信結果ダイアログの自動終了
		cStrKey.LoadString( IDS_REG_SENDDLGAUTOCLOSE);
		m_blSendDlgAutoClose = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blSendDlgAutoClose, cStrProfile);

		// 印刷のカラーorモノクロ指定
		cStrKey.LoadString( IDS_REG_COLORPRINT);
		m_blColorPrint = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blColorPrint, cStrProfile);

		// タイトル一覧表示
		cStrKey.LoadString( IDS_REG_CATALOG);
		m_blCatalog = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blCatalog, cStrProfile);
		
		// 受信時の保存指定
		cStrKey.LoadString( IDS_REG_RECEIVEANDSAVE);
		m_blReceiveAndSave = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blReceiveAndSave, cStrProfile);

		// 編集時の保存指定
		cStrKey.LoadString( IDS_REG_MODIFYANDSAVE);
		m_blModifyAndSave = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blModifyAndSave, cStrProfile);

		// 送信リストの展開
		cStrKey.LoadString( IDS_REG_EXPANDSENDTREE);
		m_blExpandSendTree = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blExpandSendTree, cStrProfile);

		return TRUE;
	}

	return FALSE;
}

BOOL CSOboeApp::SetSaveFilePath( const CString& cStrFile)
{
	if( m_cStrSaveFilePath != cStrFile)
	{
		m_cStrSaveFilePath = cStrFile;
		// データ保存のパス取得
		CString	cStrProfile;
		GetProfileName( cStrProfile);
		CString	cStrKey;
		cStrKey.LoadString( IDS_REG_FILEPATH);
		WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrSaveFilePath, cStrProfile);
		return TRUE;
	}
	return FALSE;
}

BOOL CSOboeApp::SaveProfile( void)
{
	CString	cStrProfile;
	GetProfileName( cStrProfile);

	CString	cStrKey;
	CString	cStrData;

	// 非Popup受信
	cStrKey.LoadString( IDS_REG_POPUPRECEIVE);
	cStrData.Format( "%d", m_blPopupReceive);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 送信時の署名添付
	cStrKey.LoadString( IDS_REG_APPENDSIGNATURE);
	cStrData.Format( "%d", m_blAppendSignature);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 送信時添付
	cStrKey.LoadString( IDS_REG_APPENDTIME);
	cStrData.Format( "%d", m_blAppendTime);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		// 添付位置
		cStrKey.LoadString( IDS_REG_APPENDPOS);
		cStrData.Format( "%d", m_nAppendPos);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 編集画面のスクロール有無
	cStrKey.LoadString( IDS_REG_AUTOSCROLL);
	cStrData.Format( "%d", m_blAutoScroll);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 小片リストのアイコン
	cStrKey.LoadString( IDS_REG_DATALISTFILTER);
	cStrData.Format( "%d", m_nDataListFilter);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 小片リストの逆順設定
	cStrKey.LoadString( IDS_REG_DATALISTREVERSE);
	cStrData.Format( "%d", m_blDataListReverse);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 削除済みリストの逆順設定
	cStrKey.LoadString( IDS_REG_DELLISTREVERSE);
	cStrData.Format( "%d", m_blDelListReverse);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// エクスポートのデフォルトフォルダ
	cStrKey.LoadString( IDS_REG_EXPORTFOLDER);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrExportFolder, cStrProfile);
	
	// ユーザー設定による定義色
	for( int nIndex = 0; nIndex < ( _USERCOLOR_MAX - 2); nIndex++)
	{
		cStrKey.Format( IDS_REG_USERCOLORS, nIndex);
		cStrData.Format( "0x%08X", m_clrCustomColor[ nIndex]);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	}

	// 編集ダイアログの種類？
	cStrKey.LoadString( IDS_REG_EXEDIT);
	cStrData.Format( "%d", m_blExEdit);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

		// 編集ダイアログの種類？ツールボタンの大小
		cStrKey.LoadString( IDS_REG_BIGBUTTON);
		cStrData.Format( "%d", m_blBigButton);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

		// 編集ダイアログの種類？大きさ？
		cStrKey.LoadString( IDS_REG_EXEDITRECT);
		WritePrivateProfileStruct( m_cStrUserName, cStrKey, ( LPRECT)m_cRctEditWindow, sizeof( RECT), cStrProfile);

	// 送信結果ダイアログの自動終了
	cStrKey.LoadString( IDS_REG_SENDDLGAUTOCLOSE);
	cStrData.Format( "%d", m_blSendDlgAutoClose);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 印刷のカラーorモノクロ指定
	cStrKey.LoadString( IDS_REG_COLORPRINT);
	cStrData.Format( "%d", m_blColorPrint);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	return TRUE;
}

BOOL CSOboeApp::InitializeProfile( void)
{
	// 固定値データ
	// 14 は、CColorDialogのカスタム色16-2（2は COLOR_WINDOW & COLOR_WINDOWTEXTのため)
	COLORREF	clrCustomColor[ _USERCOLOR_MAX - 2] = 
	{
		0x009999ff,
		0x0099ff99,
		0x00ff9999,
		0x009955ff,
		0x0055ff99,
		0x00ff9955,
		0x005599ff,
		0x0099ff55,
		0x00ff5599,
		0x00ffffff,
		0x00ffffff,
		0x00ffffff,
		0x00ffffff,
		0x00ffffff
	};
	CopyMemory( m_clrCustomColor, clrCustomColor, sizeof( COLORREF) * 14);
	m_clrCustomColor[ 14] = GetSysColor( COLOR_WINDOW);
	m_clrCustomColor[ 15] = GetSysColor( COLOR_WINDOWTEXT);

	LOGFONT		stLogFont = { -19, 0, 0, 0, 400, FALSE, FALSE, FALSE, 128, 3, 2, 1, 50, "ＭＳ Ｐゴシック"};

	CopyMemory( &m_stLOGFONT, &stLogFont, sizeof( LOGFONT));
	m_clrForeColor = GetSysColor( COLOR_WINDOWTEXT);
	m_clrBackColor = GetSysColor( COLOR_WINDOW);
	m_nIconIndex = 0;

	m_clrAfterFore = RGB( 0xFF, 0xFF, 0x00);
	m_clrAfterBack = RGB( 0x00, 0x00, 0x00);
	m_clrBeforeFore = RGB( 0xFF, 0xFF, 0xFF);
	m_clrBeforeBack = RGB( 0xFF, 0x00, 0x00);

	m_cStrSignature.Format( "%s in %s", m_cStrUserName, m_cStrCompName);

	char	szFileName[ _MAX_PATH];
	GetModuleFileName( NULL, szFileName, _MAX_PATH);
	char	szDrive[_MAX_DRIVE];
	char	szFolder[_MAX_DIR];
	_splitpath( szFileName, szDrive, szFolder, NULL, NULL);

	_makepath( szFileName, szDrive, szFolder, IsMultiUserMode() ? m_cStrUserName : "OboeGaki", "mrm");
	m_cStrSaveFilePath = szFileName;

	_makepath( szFileName, szDrive, szFolder, IsMultiUserMode() ? m_cStrUserName : "Dust", "mrz");
	m_cStrStockFilePath = szFileName;

	_makepath( szFileName, szDrive, szFolder, "DelLog", "txt");
	m_cStrDelLogFilePath = szFileName;

	_makepath( szFileName, szDrive, szFolder, "SendLog", "txt");
	m_cStrSendLogFilePath = szFileName;

	_makepath( szFileName, szDrive, szFolder, "ReceiveLog", "txt");
	m_cStrReceiveLogFilePath = szFileName;

	_makepath( szFileName, szDrive, szFolder, NULL, NULL);
	m_cStrExportFolder = szFileName;

	m_blAutoResize = TRUE;
	m_blAutoTriming = TRUE;
	m_blAutoIMEOpen = TRUE;
	m_blAutoScroll = TRUE;
	m_blFlatMenu = FALSE;
	m_blHideStart = FALSE;
	m_blLoggingDelChip = TRUE;
	m_blHideback = TRUE;
	m_blNotifyVersion = TRUE;
	m_blNoCheckDelete = FALSE;
	m_blLoggingSendChip = TRUE;
	m_blFullLoggingSendChip = FALSE;
	m_blLoggingReceiveChip = TRUE;
	m_blFullLoggingReceiveChip = TRUE;
	m_blCheckDelOnQuit = FALSE;
	m_blDelOnQuit2Rcv = FALSE;
	m_blReceptionLink = FALSE;
	m_blReceptionTimer = FALSE;
	m_blReceptionDxl = FALSE;
	m_blDeleteAfterSend = FALSE;

	m_blPopupReceive = FALSE;
		m_blHideReceive = TRUE;
		m_blSubNotify = TRUE;

	m_blAppendSignature = TRUE;
	m_blAppendTime = FALSE;
	m_nAppendPos = 1;

	m_blPackSend = TRUE;
		m_blNotifyOpen = FALSE;

	m_cStrSignature.Format( "%s in %s", m_cStrUserName, m_cStrCompName);

	m_nLDblClkFunc = DBLCLKFUNC_ICON;
	m_nMDblClkFunc = DBLCLKFUNC_EDIT;

	m_nDataListFilter = 0;
	m_blDataListReverse = FALSE;
	m_blDelListReverse = FALSE;

	m_dwHotKey = 0;

	m_nResumeDelay = -1;

	m_blCompatibleOU = FALSE;

	m_cStrArrangeNormalDLL.Empty();
	m_cStrArrangeTitleDLL.Empty();
	m_cStrArrangeIconDLL.Empty();
	m_cStrArrangeAllDLL.Empty();

	m_blEnableTemplet = FALSE;

	m_blEnableNxlGroup = TRUE;

	m_blEnableSpMenu = FALSE;

	m_blExEdit = FALSE;
	m_blBigButton = FALSE;
	m_cRctEditWindow.SetRect( 160, 120, 480, 360);

	m_dwInitalDeray = 300;

	// データバックアップの世代数
	m_nGenerationCount = 3;

	{
		// DXLの読み出し
		for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
		{
			m_acDrawExlib[ nIndex].SetPath( _T(""));
			m_acDrawExlib[ nIndex].SetMenuString( _T(""));
		}
		// デフォルトのDXL
		m_unDefaultDxlID = 0;

		// ネットワークの有無
		m_blEnableNetwork = FALSE;
		// NXLの読み出し
		for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
		{
			m_acNetExlib[ nIndex].SetPath( _T(""));
		}
		// デフォルトのNXL
		m_nDefaultNxl = 0;

		// Celの読み出し
		for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
		{
			m_cCelControl.SetSelData( nIndex, _T( ""), _T( ""));
		}
		CString	cStr;
		for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
		{
			cStr += '0';
		}
		m_cCelControl.SetMenuSeparator( cStr);
	}

	m_blSendDlgAutoClose = FALSE;
	m_blColorPrint = FALSE;
	m_blCatalog = FALSE;

	m_blExpandSendTree = FALSE;

	return TRUE;
}

BOOL CSOboeApp::OnDdeExecut( HDDEDATA hData)
{
	DWORD dwSize = DdeGetData( hData, NULL, 0, 0);
	char*	pszCommand = NULL;
	pszCommand = new char [ dwSize + 1];
	if( NULL == pszCommand)return FALSE;

	DdeGetData( hData, ( LPBYTE)pszCommand, dwSize + 1, 0);
	pszCommand[ dwSize] = 0;
	CString	cStr = pszCommand;
	delete [] pszCommand;

	for( int nIndex = 0; nIndex < _COMMAND_MAX; nIndex++)
	{
		if( -1 != cStr.Find( stCommands[ nIndex].lpcsz))
		{
			m_pMainWnd->SetForegroundWindow();
			m_pMainWnd->PostMessage( WM_COMMAND, stCommands[ nIndex].unCommand, 0L);
			return TRUE;
		}
	}
	if( -1 != cStr.Find( "ChipImport"))
	{
		int	nPos = cStr.Find( '(');
		if( -1 == nPos)
		{
			m_pMainWnd->SetForegroundWindow();
			m_pMainWnd->PostMessage( WM_COMMAND, IDM_MEMOIMPORT, 0L);
		}
		else
		{
			CString	cStrFileWork;
			CString	cStrFile;
			cStrFileWork = cStr.Mid( nPos + 1);
			nPos = cStrFileWork.ReverseFind( ')');
			cStrFile = cStrFileWork.Left( nPos);
			ASSERT( m_pMainWnd->IsKindOf( RUNTIME_CLASS( CMainFrame)));
			CMainFrame*	pMain;
			pMain = ( CMainFrame*)m_pMainWnd;
			pMain->ImportChip( cStrFile);
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CSOboeApp::CreateMemoFromBinData( const LPBYTE pabyData, DWORD dwSize, BOOL blCalcRect, BOOL blDelOnQuit, BOOL blEdit)
{
	BOOL blResult = FALSE;

	CMainFrame*	pMain;
	pMain = ( CMainFrame*)m_pMainWnd;

	DWORD dwCreateSrc = dwSize + sizeof( CREATESOURCE);
	HGLOBAL hGlocal = GlobalAlloc( GHND, dwCreateSrc);
	if( NULL != hGlocal)
	{
		CREATESOURCE* lpstCreateSrc = ( CREATESOURCE*)GlobalLock( hGlocal);
		if( NULL != lpstCreateSrc)
		{
			lpstCreateSrc->dwSize = dwCreateSrc;
			lpstCreateSrc->blCalcRect = blCalcRect;
			lpstCreateSrc->blDelOnQuit = blDelOnQuit;
			lpstCreateSrc->blEdit = blEdit;
			lpstCreateSrc->dwDataSize = dwSize;
			CopyMemory( lpstCreateSrc->lpabyData, pabyData, dwSize);

			GlobalUnlock( hGlocal);

			// 作成する小片が前列に出るように、覚え書本体を前に出す。
			{
				int nTargetID, nActiveId;

				nTargetID = ::GetWindowThreadProcessId( m_pMainWnd->GetSafeHwnd(), NULL );
				nActiveId = ::GetWindowThreadProcessId( ::GetForegroundWindow(), NULL );

				::AttachThreadInput( nTargetID, nActiveId, TRUE);
				::SetForegroundWindow( m_pMainWnd->GetSafeHwnd());
				::AttachThreadInput( nTargetID, nActiveId, FALSE);

				::SetWindowPos( m_pMainWnd->GetSafeHwnd(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
			}
			if( 0 != ::PostMessage( m_pMainWnd->GetSafeHwnd(), CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_CREATE_BIN, ( LPARAM)hGlocal))
			{
				hGlocal = NULL;
				blResult = TRUE;
			}
		}
		if( NULL != hGlocal)GlobalFree( hGlocal);
	}

	return blResult;


/*
	CMemoData*	pcMemoData = CMemoData::CreateMemoFromBinData( pabyData, dwSize);
	if( pcMemoData)
	{
		if( blEdit)
		{
			// こうしないとさぁ、ＤＤＥで応答できないでしょ？
			// メモデータは削除しちゃ駄目なんだよ。だって、POSTだしぃ
			if( 0 == ::PostMessage( m_pMainWnd->GetSafeHwnd(), CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_CREATEEDIT_BIN, ( LPARAM)pcMemoData))
			{
				// でも、送信失敗なら削除
				delete pcMemoData;
				return FALSE;
			}
			return TRUE;
		}

		// このような直接呼出しは問題ありのようにも見えるけどねぇ。
		pMain->CreateChipFromMemoData( *pcMemoData, blCalcRect, DelOnQuit);
		delete pcMemoData;

		return TRUE;
	}

	return FALSE;*/
}

void CSOboeApp::GetProfileName( CString& cStr, BOOL blMasterFile)
{
	char	szPath[ _MAX_PATH];
	GetModuleFileName( NULL, szPath, _MAX_PATH);

	char szDrive[ _MAX_DRIVE];
	char szDir[ _MAX_DIR];
	_splitpath( szPath, szDrive, szDir, NULL, NULL);
	_makepath( szPath, szDrive, szDir, "OboeGaki", "ini");

	if( !blMasterFile)
	{
		CString cStrDefault;
		CString cStrWork;

		cStrDefault.LoadString( IDS_REG_DEFAULT);
		cStrWork.LoadString( IDS_REG_MULTIUSER);
		if( 0 != GetPrivateProfileInt( cStrDefault, cStrWork, 0, szPath))
		{
			_makepath( cStrWork.GetBuffer( _MAX_PATH), szDrive, szDir, m_cStrUserName, "ini");
			cStrWork.ReleaseBuffer();

			GetPrivateProfileString( cStrDefault, m_cStrUserName, cStrWork, szPath, _MAX_PATH, szPath);
		}
	}
	cStr = szPath;
}

BOOL CSOboeApp::IsMultiUserMode( void)
{
	CString cStrOrgProfile;
	GetProfileName( cStrOrgProfile, TRUE);

	CString cStrDefault;
	CString cStrEntry;
	CString cStrLoadEntry;

	cStrDefault.LoadString( IDS_REG_DEFAULT);
	cStrEntry.LoadString( IDS_REG_MULTIUSER);
	return ( 0 != GetPrivateProfileInt( cStrDefault, cStrEntry, 0, cStrOrgProfile));
}

HDDEDATA CSOboeApp::OnDdeRequest( UINT wFmt, HSZ hszItem) const
{
	HDDEDATA	hResult = ( HDDEDATA)DDE_FNOTPROCESSED;
	if( CF_TEXT == wFmt)
	{
		DWORD dwLength = DdeQueryString( m_dwIdInst, hszItem, NULL, 0, CP_WINANSI);
		char*	pszItem;
		pszItem = new char [ dwLength + 1];
		if( pszItem)
		{
			DdeQueryString( m_dwIdInst, hszItem, pszItem, dwLength + 1, CP_WINANSI);

			if( !lstrcmp( pszItem, "WarehousePath"))
			{
				LPCSTR	lpcsz = GetStockFilePath();
				hResult = DdeCreateDataHandle( m_dwIdInst, ( LPBYTE)lpcsz, lstrlen( lpcsz) + 1, 0, hszItem, CF_TEXT, 0);
			}

			delete [] pszItem;
		}
	}
	return hResult;
}

BOOL CSOboeApp::OnDdePoke( UINT wFmt, HSZ hszItem, HDDEDATA hData)
{
	BOOL blResult = FALSE;

	if( CF_TEXT == wFmt)
	{
		DWORD dwLength = DdeQueryString( m_dwIdInst, hszItem, NULL, 0, CP_WINANSI);
		if( 0 < dwLength)
		{
			CString cStr;
			DdeQueryString( m_dwIdInst, hszItem, cStr.GetBuffer( dwLength + 1), dwLength + 1, CP_WINANSI);
			cStr.ReleaseBuffer();

			if( "WarehousePath" == cStr)
			{
				DWORD dwSize = DdeGetData( hData, NULL, 0, 0);
				char*	pszWarehousePath = NULL;
				pszWarehousePath = new char [ dwSize + 1];
				if( pszWarehousePath)
				{
					DdeGetData( hData, ( LPBYTE)pszWarehousePath, dwSize + 1, 0);
					pszWarehousePath[ dwSize] = 0;

					blResult = SetStockFilePath( pszWarehousePath);

					delete [] pszWarehousePath;
				}
			}
		}
	}
	else
	{
		if( CMemoData::GetClipboardFormat() == wFmt)
		{
			if( MAX_REJECTCOUNT > m_nImportCount)m_nImportCount++;
			if( MAX_FREEIMPORT >= m_nImportCount)
			{
				DWORD dwLength = DdeQueryString( m_dwIdInst, hszItem, NULL, 0, CP_WINANSI);
				if( 0 < dwLength)
				{
					CString cStr;
					DdeQueryString( m_dwIdInst, hszItem, cStr.GetBuffer( dwLength + 1), dwLength + 1, CP_WINANSI);
					cStr.ReleaseBuffer();

					if( "ChipBinary" == cStr.Left( 10))
					{
						cStr.MakeUpper();
						BOOL blCalcRect = ( 0 <= cStr.Find( "/R"));		// 大きさ再計算
						BOOL blDelOnQuit = ( 0 <= cStr.Find( "/D"));	// 終了時削除
						BOOL blEdit = ( 0 <= cStr.Find( "/E"));			// 編集
						BOOL blSend = ( 0 <= cStr.Find( "/S"));			// 送信編集

						if( FALSE != blSend)return FALSE;	// 送信編集は未サポートなのなの！

						DWORD dwSize = DdeGetData( hData, NULL, 0, 0);
						if( 0 < dwSize)
						{
							BYTE* pabyData = new BYTE [ dwSize];
							DdeGetData( hData, pabyData, dwSize, 0);
							blResult = CreateMemoFromBinData( pabyData, dwSize, blCalcRect, blDelOnQuit, blEdit);
							delete [] pabyData;
							pabyData = NULL;
						}
					}
				}
			}
		}
	}

	return blResult;
}

int CSOboeApp::ReceiveData( const LPVOID lpvEntry, int nEntrySize, const LPVOID lpvData, int nDataSize)
{
	if( NULL == lpvData)return FALSE;
	if( 0 >= nDataSize)return FALSE;
	if( TRUE == m_blSuspendReceive)return FALSE;

	CSingleLock	cSglLk( &m_cCrsRcvMemo);
	if( cSglLk.Lock())
	{
		int nAllocSize = sizeof( RECEIVESOURCE) + nEntrySize + nDataSize;
		HGLOBAL	hGlobal	= GlobalAlloc( GHND, nAllocSize);
		RECEIVESOURCE* pstReceiveSrc = ( RECEIVESOURCE*)GlobalLock( hGlobal);
		if( pstReceiveSrc)
		{
			pstReceiveSrc->nEntryDataSize = nEntrySize;
			if( 0 < pstReceiveSrc->nEntryDataSize)
			{
				pstReceiveSrc->nOffsetEntryData = sizeof( RECEIVESOURCE);
				CopyMemory( &( ( LPBYTE)pstReceiveSrc)[ pstReceiveSrc->nOffsetEntryData], lpvEntry, nEntrySize);
			}
			else
			{
				pstReceiveSrc->nOffsetEntryData = -1;
			}
			pstReceiveSrc->nReceiveDataSize = nDataSize;
			pstReceiveSrc->nOffsetReceiveData = sizeof( RECEIVESOURCE) + nEntrySize;
			CopyMemory( &( ( LPBYTE)pstReceiveSrc)[ pstReceiveSrc->nOffsetReceiveData], lpvData, nDataSize);
			GlobalUnlock( hGlobal);
			m_pMainWnd->PostMessage( CMainFrame::GetNotifyMsgFromChip(), CMainFrame::NMFM_RECEIVE, ( LPARAM)hGlobal);
			return TRUE;
		}
		// 本当はPostMessageなど使用せずにここから直で制作しようと思っていたのだけれど、
		// スタックの問題か……上手く行かない…仕方が無いのでメインスレッドに処理を移した。
	}
	return FALSE;
}

BOOL CSOboeApp::FreeTemplet( void)
{
	CMemoData*	pcMemoData;
	while( m_cLstSeeds.GetCount())
	{
		pcMemoData = m_cLstSeeds.RemoveHead();
		delete pcMemoData;
	}

	return TRUE;
}

BOOL CSOboeApp::LoadTemplet( void)
{
	BOOL	blResult = FALSE;

	FreeTemplet();

	if( !IsEnableTemplet())return FALSE;
	if( m_cStrTempletFile.IsEmpty())return FALSE;

	CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);

	try
	{
		CFile	cLoadFile;
		if( cLoadFile.Open( m_cStrTempletFile, CFile::modeRead | CFile::typeBinary))
		{
			CRect	cRctNormal;
			cRctNormal.SetRectEmpty();

			CMemoChip*	pcMemoChip;
			CMemoData*	pcMemoData;
			CArchive	cArchive( &cLoadFile, CArchive::load);
			for( int nIndex = 0; nIndex < _MAX_TEMPLTE; nIndex++)
			{
				pcMemoChip = ( CMemoChip*)cArchive.ReadObject( RUNTIME_CLASS( CMemoChip));
				if( NULL == pcMemoChip)break;
				pcMemoData = new CMemoData;
				pcMemoChip->GetMemoData( *pcMemoData);
				delete pcMemoChip;
				
				// 継承してはイケナイ部分を初期化！
				pcMemoData->SetNormalRect( cRctNormal);
				pcMemoData->SetTitleRect( cRctNormal);
				pcMemoData->SetIconRect( cRctNormal);
				pcMemoData->SetWindowType( CMemoData::WINDOW_TYPE_NORMAL);
				pcMemoData->PosLock( FALSE);
				pcMemoData->Show( TRUE);
				pcMemoData->SeeThrough( FALSE);

				m_cLstSeeds.AddTail( pcMemoData);
			}
			blResult = TRUE;
		}
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n";
#endif
		if( CArchiveException::endOfFile == e->m_cause)
		{
			blResult = TRUE;
		}
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

	return blResult;
}

BOOL CSOboeApp::GetTempletName( int nIndex, CString& cStr) const
{
	if( m_cLstSeeds.GetCount() <= nIndex)return FALSE;
	POSITION Pos = m_cLstSeeds.FindIndex( nIndex);
	if( NULL == Pos)return FALSE;
	const CMemoData* pcMemo = m_cLstSeeds.GetAt( Pos);
	if( NULL == pcMemo)return FALSE;
	pcMemo->GetTitle( cStr);
	return TRUE;
}

const CMemoData* CSOboeApp::GetTemplet( int nIndex) const
{
	if( nIndex >= m_cLstSeeds.GetCount())return NULL;
	if( m_cLstSeeds.GetCount() <= nIndex)return NULL;
	POSITION Pos = m_cLstSeeds.FindIndex( nIndex);
	if( NULL == Pos)return NULL;
	const CMemoData* pcMemo = m_cLstSeeds.GetAt( Pos);
	return pcMemo;
}

BOOL CSOboeApp::IsCustomizing( void)
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoCustomize);

	blResult = !cSlk.Lock( 0);

	return blResult;
}

int CSOboeApp::DoCustomize( CWnd* pcParent)
{
	int				nResult = IDCANCEL;

	CSingleLock cLock( &sm_cSmpDoCustomize);
	if( cLock.Lock( 0))
	{
		HINSTANCE		hInst = NULL;
		CString			cStrSetRegDll;

		cStrSetRegDll.LoadString( IDS_SETREG_DLL);
		hInst = LoadLibrary( cStrSetRegDll);
		// リンクが正常になされた？
		if( NULL != hInst)
		{
			GETSETREGVERSION	lpGetSetRegVersion;
			// ヴァージョンの確認
			lpGetSetRegVersion = ( GETSETREGVERSION)GetProcAddress( hInst, "GetSetRegVersion");
			if( lpGetSetRegVersion)
			{
				if( _CUSTOMIZEDLL_VER == lpGetSetRegVersion())
				{
					DOCUSTOMIZE	lpDoCustomize;
					lpDoCustomize = ( DOCUSTOMIZE)GetProcAddress( hInst, "DoCustomize");
					if( lpDoCustomize)
					{
						SaveProfile();
						nResult = lpDoCustomize( FALSE, pcParent->GetSafeHwnd());
						if( IDOK == nResult)LoadProfile( FALSE);
					}
					else
					{
						CString	cStr;
						cStr.LoadString( IDS_MISSMATCHEXTENDED);
						pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
					}
				}
				else
				{
					CString	cStr;
					cStr.LoadString( IDS_MISSMATCHEXTENEDEDVERSION);
					pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
				}
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MISSMATCHEXTENDED);
				pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
			}
			FreeLibrary( hInst);
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
			pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
	}

	return nResult;
}

BOOL CSOboeApp::IsUpdating( void)
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoUpdating);

	blResult = !cSlk.Lock( 0);

	return blResult;
}

BOOL CSOboeApp::IsExistUpdateDLL()
{
	HINSTANCE hInst = LoadLibrary( TEXT( "GetIcon.dll"));
	if( hInst)
	{
		FreeLibrary( hInst);
		return TRUE;
	}
	return FALSE;
}

int CSOboeApp::DoUpdateIcon( CWnd* pcParent)
{
	int nResult = IDCANCEL;

	CSingleLock cLock( &sm_cSmpDoUpdating);
	if( cLock.Lock( 0))
	{
		HINSTANCE hInst = LoadLibrary( TEXT( "GetIcon.dll"));
		if( hInst)
		{
			LPDOUPDATEICON lpfnDoUpdateIcon = ( LPDOUPDATEICON)GetProcAddress( hInst, TEXT( "DoUpdateIcon"));
			if( lpfnDoUpdateIcon)
			{
				if( IDOK == lpfnDoUpdateIcon( pcParent->GetSafeHwnd()))
				{
					if( UpdateIconList())
					{
						nResult = IDOK;
					}
				}
			}
			else
			{
				CString	cStr;
				cStr.LoadString( IDS_MISSMATCHEXTENDED);
				pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
			}
			FreeLibrary( hInst);
		}
		else
		{
			CString	cStr;
			cStr.LoadString( IDS_NOTFOUNDEXTENDEDDLL);
			pcParent->MessageBox( cStr, NULL, MB_ICONEXCLAMATION | MB_OK);
		}
	}

	return nResult;
}

BOOL CSOboeApp::UpdateIconList( void)
{
	if( m_pcIconImgs)
	{
		m_pcIconImgs->ReleaseExtIconData();
		m_pcIconImgs->LoadExtIconData();

		return TRUE;
	}
	return FALSE;
}

void CSOboeApp::DecrimentImportCount( void)
{
	if( 0 < m_nImportCount)m_nImportCount--;
}

HDDEDATA CALLBACK DdeCallback( UINT wType, UINT wFmt, HCONV hConv, HSZ hszTopic, HSZ hszItem, HDDEDATA hData, DWORD lData1, DWORD lData2)
{
	CSOboeApp*	pApp;
	pApp = ( CSOboeApp*)AfxGetApp();

	switch( wType)
	{
	case XTYP_CONNECT:
		if( !DdeCmpStringHandles( pApp->GetAppName(), hszItem))
		{
			if( !DdeCmpStringHandles( pApp->GetAppTopic(), hszTopic))
			{
				return ( HDDEDATA)TRUE;
			}
		}
		return ( HDDEDATA)DDE_FNOTPROCESSED;

	case XTYP_REQUEST:
		if( !DdeCmpStringHandles( pApp->GetAppTopic(), hszTopic))
		{
			return pApp->OnDdeRequest( wFmt, hszItem);
		}
		return ( HDDEDATA)DDE_FNOTPROCESSED;

	case XTYP_POKE:
		if( !DdeCmpStringHandles( pApp->GetAppTopic(), hszTopic))
		{
			return ( HDDEDATA)( pApp->OnDdePoke( wFmt, hszItem, hData) ? DDE_FACK : DDE_FNOTPROCESSED);
		}
		return ( HDDEDATA)DDE_FNOTPROCESSED;

	case XTYP_EXECUTE:
		if( !DdeCmpStringHandles( pApp->GetAppTopic(), hszTopic))
		{
			return ( HDDEDATA)( pApp->OnDdeExecut( hData) ? DDE_FACK : DDE_FNOTPROCESSED);
		}
		return ( HDDEDATA)DDE_FNOTPROCESSED;

	case XTYP_DISCONNECT:
		return ( HDDEDATA)TRUE;
	}
	return 0;
}
