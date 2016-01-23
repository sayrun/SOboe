// ProfileData.cpp: CProfileData クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SetReg.h"
#include "ProfileData.h"
#include "LMCONS.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CProfileData::CProfileData()
{
	char	szUser[ UNLEN + 1];
	DWORD	dwSize = UNLEN + 1;
	m_cStrUserName = ( 0 >= GetUserName( szUser, &dwSize)) ? "Anonymous" : szUser;

	char	szCompname[ MAX_COMPUTERNAME_LENGTH + 1];
	dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	m_cStrCompName = ( 0 >= GetComputerName( szCompname, &dwSize)) ? "Default" : szCompname;

	InitializeProfile();
}

CProfileData::~CProfileData()
{

}

BOOL CProfileData::GetProfileName( CString& cStrProfile, BOOL blMasterFile)
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
	cStrProfile = szPath;

	return TRUE;
}

BOOL CProfileData::IsMultiUserMode( void)
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

BOOL CProfileData::LoadProfile( void)
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
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrSaveFilePath = szWork;
		if( m_cStrSaveFilePath.IsEmpty())
		{
			char	szFileName[ _MAX_PATH];
			GetModuleFileName( NULL, szFileName, _MAX_PATH);
			char	szDrive[_MAX_DRIVE];
			char	szFolder[_MAX_DIR];
		   _splitpath( szFileName, szDrive, szFolder, NULL, NULL);
		   _makepath( szFileName, szDrive, szFolder, "OboeGaki", "mrm");
			m_cStrSaveFilePath = szFileName;
		}

		// デフォルトのフォント
		cStrKey.LoadString( IDS_REG_LOGFONT);
		if( !GetPrivateProfileStruct( m_cStrUserName, cStrKey, &m_stLogFont, sizeof( LOGFONT), cStrProfile))
		{
			LOGFONT		stLogFont = { -19, 0, 0, 0, 400, FALSE, FALSE, FALSE, 128, 3, 2, 1, 50, "ＭＳ Ｐゴシック"};
			CopyMemory( &m_stLogFont, &stLogFont, sizeof( LOGFONT));
		}
		
		// デフォルトの文字色
		cStrKey.LoadString( IDS_REG_FORECOLOR);
		m_clrForeColor = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrForeColor)
		{
			m_clrForeColor = GetSysColor( COLOR_WINDOWTEXT);
		}
		// デフォルトの背景色
		cStrKey.LoadString( IDS_REG_BACKCOLOR);
		m_clrBackColor = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrBackColor)
		{
			m_clrBackColor = GetSysColor( COLOR_WINDOW);
		}
		// デフォルトのアイコン
		cStrKey.LoadString( IDS_REG_ICONINDEX);
		m_nIconIndex = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0, cStrProfile);


		// 設定タイマ経過後文字色
		cStrKey.LoadString( IDS_REG_AFTERFORECOLOR);
		m_clrAfterFore = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrAfterFore)
		{
			m_clrAfterFore = RGB( 0xFF, 0x00, 0x00);
		}
		// 設定タイマ経過後背景色
		cStrKey.LoadString( IDS_REG_AFTERBACKCOLOR);
		m_clrAfterBack = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrAfterBack)
		{
			m_clrAfterBack = RGB( 0xFF, 0xFF, 0xFF);
		}

		// 設定タイマ事前通知文字色
		cStrKey.LoadString( IDS_REG_BEFOREFORECOLOR);
		m_clrBeforeFore = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrBeforeFore)
		{
			m_clrBeforeFore = RGB( 0xFF, 0xFF, 0x00);
		}
		// 設定タイマ事前通知背景色
		cStrKey.LoadString( IDS_REG_BEFOREBACKCOLOR);
		m_clrBeforeBack = GetPrivateProfileInt( m_cStrUserName, cStrKey, CLR_INVALID, cStrProfile);
		if( CLR_INVALID == m_clrBeforeFore)
		{
			m_clrBeforeBack = RGB( 0x00, 0x00, 0x00);
		}
	
		// 削除データ保存パス
		cStrKey.LoadString( IDS_REG_WAREHOUSEPATH);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrStockFilePath = szWork;
		if( m_cStrStockFilePath.IsEmpty())
		{
			char	szFileName[ _MAX_PATH];
			GetModuleFileName( NULL, szFileName, _MAX_PATH);
			char	szDrive[_MAX_DRIVE];
			char	szFolder[_MAX_DIR];
		   _splitpath( szFileName, szDrive, szFolder, NULL, NULL);
		   _makepath( szFileName, szDrive, szFolder, "Dust", "mrz");
			m_cStrStockFilePath = szFileName;
		}

		// 大きさ再計算
		cStrKey.LoadString( IDS_REG_AUTORESIZE);
		m_blAutoResize = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// オートトリミング
		cStrKey.LoadString( IDS_REG_AUTOTRIMING);
		m_blAutoTriming = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// 編集時のIME自動ON
		cStrKey.LoadString( IDS_REG_AUTOIMEOPEN);
		m_blAutoIMEOpen = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// メニューは階層化しない
		cStrKey.LoadString( IDS_REG_FLATMENU);
		m_blFlatMenu = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// 起動時の一時非表示
		cStrKey.LoadString( IDS_REG_HIDESTART);
		m_blHideStart = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 削除データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGDELCHIP);
		m_blLoggingDelChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);		
		// 通知後非表示へ戻す
		cStrKey.LoadString( IDS_REG_HIDEBACK);
		m_blHideback = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);		
		// バージョン通知
		cStrKey.LoadString( IDS_REG_NOTIFIYVERSION);
		m_blNotifyVersion = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);		
		// 削除時のメッセージ
		cStrKey.LoadString( IDS_REG_NOCHECKDELETE);
		m_blNoCheckDelete = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// 送信データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGSENDCHIP);
		m_blLoggingSendChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// 送信データの全ログ採り
		cStrKey.LoadString( IDS_REG_FULLLOGGINGSENDCHIP);
		m_blFullLoggingSendChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 受信データのログ採り
		cStrKey.LoadString( IDS_REG_LOGGINGRECEIVECHIP);
		m_blLoggingReceiveChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
		// 受信データの全ログ採り
		cStrKey.LoadString( IDS_REG_FULLLOGGINGRECEIVECHIP);
		m_blFullLoggingReceiveChip = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 送信時の終了時削除
		cStrKey.LoadString( IDS_REG_CHECKDELONQUIT);
		m_blCheckDelOnQuit = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 送信時元小片を削除
		cStrKey.LoadString( IDS_REG_DELETEAFTERSEND);
		m_blDeleteAfterSend = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 受信小片に「終了時削除」を適用
		cStrKey.LoadString( IDS_REG_DELONQUIT2RCV);
		m_blDelOnQuit2Rcv= GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 関連付けの受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONLINK);
		m_blReceptionLink = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// タイマ受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONTIMER);
		m_blReceptionTimer = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// タイマ受け入れ
		cStrKey.LoadString( IDS_REG_RECEPTIONDXL);
		m_blReceptionDxl = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);

		// Popup受信
		cStrKey.LoadString( IDS_REG_POPUPRECEIVE);
		m_blPopupReceive = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
			// 受信小片非表示
			cStrKey.LoadString( IDS_REG_HIDERECEIVE);
			m_blHideReceive = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);
			// 通知Windowあり
			cStrKey.LoadString( IDS_REG_NOTIFYRECEIVE);
			m_blSubNotify = GetPrivateProfileInt( m_cStrUserName, cStrKey, TRUE, cStrProfile);

		// 送信時の署名添付
		cStrKey.LoadString( IDS_REG_APPENDSIGNATURE);
		m_blAppendSignature = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 送信時添付
		cStrKey.LoadString( IDS_REG_APPENDTIME);
		m_blAppendTime = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
			// 添付位置
			cStrKey.LoadString( IDS_REG_APPENDPOS);
			m_nAppendPos = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 裏返し送信
		cStrKey.LoadString( IDS_REG_PACKSEND);
		m_blPackSend = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
			// 表返し確認
			cStrKey.LoadString( IDS_REG_OPENCHECK);
			m_blNotifyOpen = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);
		// 署名
		cStrKey.LoadString( IDS_REG_SIGNATURE);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrSignature = szWork;
		if( m_cStrSignature.IsEmpty())
		{
			m_cStrSignature.Format( "%s in %s", m_cStrUserName, m_cStrCompName);
		}
		
		// 左Ｗクリック
		cStrKey.LoadString( IDS_REG_LDBLCLKFUNC);
		m_nLDblClkFunc = GetPrivateProfileInt( m_cStrUserName, cStrKey, DBLCLKFUNC_ICON, cStrProfile);
		// 中Ｗクリック
		cStrKey.LoadString( IDS_REG_MDBLCLKFUNC);
		m_nMDblClkFunc = GetPrivateProfileInt( m_cStrUserName, cStrKey, DBLCLKFUNC_EDIT, cStrProfile);

		// 小片リストのアイコン
		cStrKey.LoadString( IDS_REG_DATALISTFILTER);
		m_nDataListFilter = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0, cStrProfile);
		// 小片リストの逆順設定
		cStrKey.LoadString( IDS_REG_DATALISTREVERSE);
		m_blDataListReverse = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);

		// 削除済みリストの逆順設定
		cStrKey.LoadString( IDS_REG_DELLISTREVERSE);
		m_blDelListReverse = GetPrivateProfileInt( m_cStrUserName, cStrKey, FALSE, cStrProfile);

		// ホットキーの情報
		cStrKey.LoadString( IDS_REG_HOTKEY);
		m_dwHotKey = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0x30041, cStrProfile);

		// サスペンド復帰からネットワーク初期化までの時間
		cStrKey.LoadString( IDS_REG_RESUMEDELAY);
		m_nResumeDelay = GetPrivateProfileInt( m_cStrUserName, cStrKey, -1, cStrProfile);

		// 小片整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGENORMALDLL);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrArrangeNormalDLL = szWork;
		// タイトル化小片整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGETITLEDLL);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrArrangeTitleDLL = szWork;
		// アイコン整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGEICONDLL);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrArrangeIconDLL = szWork;
		// 状態非依存整列用のDLLの読み出し
		cStrKey.LoadString( IDS_REG_ARRANGEALLDLL);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrArrangeAllDLL = szWork;

		// DXLの読み出し
		OFSTRUCT	ofDxl;
		for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
		{
			cStrKey.Format( IDS_REG_DXLPATH, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			if( HFILE_ERROR != OpenFile( szWork, &ofDxl, OF_READ | OF_EXIST))
			{
				m_cStrDxlPath[ nIndex] = szWork;

				cStrKey.Format( IDS_REG_DXLMENU, nIndex);
				GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
				m_cStrDxlMenuString[ nIndex] = szWork;
			}
			else
			{
				m_cStrDxlPath[ nIndex] = _T("");
				m_cStrDxlMenuString[ nIndex] = _T("");
			}
		}
		// デフォルトのDXL
		cStrKey.LoadString( IDS_REG_DEFAULTDXLID);
		m_unDefaultDxlID = GetPrivateProfileInt( m_cStrUserName, cStrKey, 0, cStrProfile);

		// デフォルトのNXL
		cStrKey.LoadString( IDS_REG_DEFAULTNXL);
		m_nDefaultNxl = GetPrivateProfileInt( m_cStrUserName, cStrKey, -1, cStrProfile);
		// ネットワークの有無
		cStrKey.LoadString( IDS_REG_NETWORKENABLE);
		m_blEnableNetwork = ( 0 != GetPrivateProfileInt( m_cStrUserName, cStrKey, 0, cStrProfile)) ? 1 : 0;
		// NXLの読み出し
		OFSTRUCT	ofNxl;
		for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
		{
			cStrKey.Format( IDS_REG_NXLPATH, nIndex);
			GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
			m_cStrNxlPath[ nIndex] = ( HFILE_ERROR != OpenFile( szWork, &ofNxl, OF_READ | OF_EXIST)) ?  szWork : _T( "");
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
			m_cStrCelPath[ nIndex] = cStrPath;
			m_cStrCelMenuString[ nIndex] = cStrMenu;
		}
		cStrKey.LoadString( IDS_REG_CELMENUSEPARATOR);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrMenuSeparator = szWork;
		for( int nIndex = ( _CEL_MAX - m_cStrMenuSeparator.GetLength()); nIndex >= 0; nIndex--)
		{
			m_cStrMenuSeparator += '0';
		}


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

		// テンプレート
		cStrKey.LoadString( IDS_REG_ENABLETEMPLET);
		m_blEnableTemplet = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blCompatibleOU, cStrProfile);
		// テンプレートファイル
		cStrKey.LoadString( IDS_REG_TEMPLETFILE);
		GetPrivateProfileString( m_cStrUserName, cStrKey, "", szWork, 2048, cStrProfile);
		m_cStrTempletFile = szWork;


		// 覚え書ユーティリティとの互換処理
		cStrKey.LoadString( IDS_REG_COMPATIBLEOU);
		m_blCompatibleOU = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blCompatibleOU, cStrProfile);

	
		// ユーザー設定による定義色
		for( int nIndex = 0; nIndex < 16; nIndex++)
		{
			cStrKey.Format( IDS_REG_USERCOLORS, nIndex);
			m_clrCustomColor[ nIndex] = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_clrCustomColor[ nIndex], cStrProfile);
		}

		// データバックアップの世代数
		cStrKey.LoadString( IDS_REG_GENERATIONCOUNT);
		m_nGenerationCount = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_nGenerationCount, cStrProfile);

		// 受信時の保存指定
		cStrKey.LoadString( IDS_REG_RECEIVEANDSAVE);
		m_blReceiveAndSave = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blReceiveAndSave, cStrProfile);
		// 編集時の保存指定
		cStrKey.LoadString( IDS_REG_MODIFYANDSAVE);
		m_blModifyAndSave = GetPrivateProfileInt( m_cStrUserName, cStrKey, m_blModifyAndSave, cStrProfile);


		return TRUE;
	}

	return FALSE;
}

BOOL CProfileData::SaveProfile( void)
{
	CString	cStrProfile;
	GetProfileName( cStrProfile);

	CString	cStrKey;
	CString	cStrData;

	// データ保存のパス取得
	cStrKey.LoadString( IDS_REG_FILEPATH);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrSaveFilePath, cStrProfile);

	// デフォルトのフォント
	cStrKey.LoadString( IDS_REG_LOGFONT);
	WritePrivateProfileStruct( m_cStrUserName, cStrKey, &m_stLogFont, sizeof( LOGFONT), cStrProfile);
	
	// デフォルトの文字色
	cStrKey.LoadString( IDS_REG_FORECOLOR);
	cStrData.Format( "0x%08X", m_clrForeColor);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// デフォルトの背景色
	cStrKey.LoadString( IDS_REG_BACKCOLOR);
	cStrData.Format( "0x%08X", m_clrBackColor);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// デフォルトのアイコン
	cStrKey.LoadString( IDS_REG_ICONINDEX);
	cStrData.Format( "%d", m_nIconIndex);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 設定タイマ経過後文字色
	cStrKey.LoadString( IDS_REG_AFTERFORECOLOR);
	cStrData.Format( "0x%08X", m_clrAfterFore);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 設定タイマ経過後背景色
	cStrKey.LoadString( IDS_REG_AFTERBACKCOLOR);
	cStrData.Format( "0x%08X", m_clrAfterBack);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 設定タイマ事前通知文字色
	cStrKey.LoadString( IDS_REG_BEFOREFORECOLOR);
	cStrData.Format( "0x%08X", m_clrBeforeFore);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 設定タイマ事前通知背景色
	cStrKey.LoadString( IDS_REG_BEFOREBACKCOLOR);
	cStrData.Format( "0x%08X", m_clrBeforeBack);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 削除データ保存パス
	cStrKey.LoadString( IDS_REG_WAREHOUSEPATH);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrStockFilePath, cStrProfile);

	// 大きさ再計算
	cStrKey.LoadString( IDS_REG_AUTORESIZE);
	cStrData.Format( "%d", m_blAutoResize);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// オートトリミング
	cStrKey.LoadString( IDS_REG_AUTOTRIMING);
	cStrData.Format( "%d", m_blAutoTriming);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 編集時のIME自動ON
	cStrKey.LoadString( IDS_REG_AUTOIMEOPEN);
	cStrData.Format( "%d", m_blAutoIMEOpen);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// メニューは階層化しない
	cStrKey.LoadString( IDS_REG_FLATMENU);
	cStrData.Format( "%d", m_blFlatMenu);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 起動時の一時非表示
	cStrKey.LoadString( IDS_REG_HIDESTART);
	cStrData.Format( "%d", m_blHideStart);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 削除データのログ採り
	cStrKey.LoadString( IDS_REG_LOGGINGDELCHIP);
	cStrData.Format( "%d", m_blLoggingDelChip);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 通知後非表示へ戻す
	cStrKey.LoadString( IDS_REG_HIDEBACK);
	cStrData.Format( "%d", m_blHideback);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// バージョン通知
	cStrKey.LoadString( IDS_REG_NOTIFIYVERSION);
	cStrData.Format( "%d", m_blNotifyVersion);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 削除時のメッセージ
	cStrKey.LoadString( IDS_REG_NOCHECKDELETE);
	cStrData.Format( "%d", m_blNoCheckDelete);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 送信データのログ採り
	cStrKey.LoadString( IDS_REG_LOGGINGSENDCHIP);
	cStrData.Format( "%d", m_blLoggingSendChip);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 送信データの全ログ採り
	cStrKey.LoadString( IDS_REG_FULLLOGGINGSENDCHIP);
	cStrData.Format( "%d", m_blFullLoggingSendChip);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 送信時の終了時削除
	cStrKey.LoadString( IDS_REG_CHECKDELONQUIT);
	cStrData.Format( "%d", m_blCheckDelOnQuit);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 送信時元小片を削除
	cStrKey.LoadString( IDS_REG_DELETEAFTERSEND);
	cStrData.Format( "%d", m_blDeleteAfterSend);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 受信小片へ「終了時削除」を適用
	cStrKey.LoadString( IDS_REG_DELONQUIT2RCV);
	cStrData.Format( "%d", m_blDelOnQuit2Rcv);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 受信データのログ採り
	cStrKey.LoadString( IDS_REG_LOGGINGRECEIVECHIP);
	cStrData.Format( "%d", m_blLoggingReceiveChip);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 受信データの全ログ採り
	cStrKey.LoadString( IDS_REG_FULLLOGGINGRECEIVECHIP);
	cStrData.Format( "%d", m_blFullLoggingReceiveChip);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 関連付けの受け入れ
	cStrKey.LoadString( IDS_REG_RECEPTIONLINK);
	cStrData.Format( "%d", m_blReceptionLink);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// タイマ受け入れ
	cStrKey.LoadString( IDS_REG_RECEPTIONTIMER);
	cStrData.Format( "%d", m_blReceptionTimer);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// DXL受け入れ
	cStrKey.LoadString( IDS_REG_RECEPTIONDXL);
	cStrData.Format( "%d", m_blReceptionDxl);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);


	// 非Popup受信
	cStrKey.LoadString( IDS_REG_POPUPRECEIVE);
	cStrData.Format( "%d", m_blPopupReceive);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		// 受信小片非表示
		cStrKey.LoadString( IDS_REG_HIDERECEIVE);
		cStrData.Format( "%d", m_blHideReceive);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		// 通知Windowあり
		cStrKey.LoadString( IDS_REG_NOTIFYRECEIVE);
		cStrData.Format( "%d", m_blSubNotify);
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
	// 裏返し送信
	cStrKey.LoadString( IDS_REG_PACKSEND);
	cStrData.Format( "%d", m_blPackSend);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		// 表返し確認
		cStrKey.LoadString( IDS_REG_OPENCHECK);
		cStrData.Format( "%d", m_blNotifyOpen);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 署名
	cStrKey.LoadString( IDS_REG_SIGNATURE);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrSignature, cStrProfile);

	// 左Ｗクリック
	cStrKey.LoadString( IDS_REG_LDBLCLKFUNC);
	cStrData.Format( "%d", m_nLDblClkFunc);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// 中Ｗクリック
	cStrKey.LoadString( IDS_REG_MDBLCLKFUNC);
	cStrData.Format( "%d", m_nMDblClkFunc);
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

	// ホットキーの情報
	cStrKey.LoadString( IDS_REG_HOTKEY);
	cStrData.Format( "0x%08X", m_dwHotKey);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// サスペンド復帰からネットワーク初期化までの時間
	cStrKey.LoadString( IDS_REG_RESUMEDELAY);
	cStrData.Format( "%d", m_nResumeDelay);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 小片整列用のDLL
	cStrKey.LoadString( IDS_REG_ARRANGENORMALDLL);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrArrangeNormalDLL, cStrProfile);
	// タイトル化小片整列用のDLL
	cStrKey.LoadString( IDS_REG_ARRANGETITLEDLL);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrArrangeTitleDLL, cStrProfile);
	// アイコン整列用のDLL
	cStrKey.LoadString( IDS_REG_ARRANGEICONDLL);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrArrangeIconDLL, cStrProfile);
	// 状態非依存整列用のDLL
	cStrKey.LoadString( IDS_REG_ARRANGEALLDLL);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrArrangeAllDLL, cStrProfile);

	// DXLの書き出し
	for( int nIndex = 0; nIndex < _DRAWEXLIB_MAX; nIndex++)
	{
		cStrKey.Format( IDS_REG_DXLPATH, nIndex);
		cStrData = m_cStrDxlPath[ nIndex];
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		cStrKey.Format( IDS_REG_DXLMENU, nIndex);
		cStrData = m_cStrDxlMenuString[ nIndex];
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	}
	// デフォルトのDxl
	cStrKey.LoadString( IDS_REG_DEFAULTDXLID);
	cStrData.Format( "%d", m_unDefaultDxlID);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	

	// ネットワークの設定
	cStrKey.LoadString( IDS_REG_NETWORKENABLE);
	cStrData.Format( "%d", m_blEnableNetwork);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// NXLの書き出し
	for( int nIndex = 0; nIndex < _NETEXLIB_MAX; nIndex++)
	{
		cStrKey.Format( IDS_REG_NXLPATH, nIndex);
		cStrData = m_cStrNxlPath[ nIndex];
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	}
	// デフォルトのNXL
	cStrKey.LoadString( IDS_REG_DEFAULTNXL);
	cStrData.Format( "%d", m_nDefaultNxl);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// Celの読み出し
	CString	cStrPath;
	CString	cStrMenu;
	for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
	{
		cStrKey.Format( IDS_REG_CELPATH, nIndex);
		cStrData = m_cStrCelPath[ nIndex];
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
		cStrKey.Format( IDS_REG_CELMENU, nIndex);
		cStrData = m_cStrCelMenuString[ nIndex];
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	}
	cStrKey.LoadString( IDS_REG_CELMENUSEPARATOR);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrMenuSeparator, cStrProfile);


	cStrKey.LoadString( IDS_REG_GLOBALPASSWORD);
	int nLength= m_cStrGlobalPassWord.GetLength();
	char*	pszData;
	BYTE	byData;
	pszData = new char[ ( nLength * 2) + 1];
	ZeroMemory( pszData, ( nLength * 2) + 1);
	UINT unSRand = GetTickCount();
	srand( unSRand);
	int nRnd;
	BYTE	byDataDummy;
	for( int nIndex = 0; nIndex < nLength; nIndex++)
	{
		byData = m_cStrGlobalPassWord[ nIndex];
		byData = ~byData;

		nRnd = rand() % 3;
		switch( nRnd)
		{
		case 0:
			byDataDummy = 0x30;
			break;
		case 1:
			byDataDummy = 0x40;
			break;
		case 2:
		default:
			byDataDummy = 0x60;
			break;
		}
		pszData[ nIndex * 2] = ( ( 0xF0 & byData) >> 4) | byDataDummy;

		nRnd = rand() % 3;
		switch( nRnd)
		{
		case 0:
			byDataDummy = 0x30;
			break;
		case 1:
			byDataDummy = 0x40;
			break;
		case 2:
		default:
			byDataDummy = 0x60;
			break;
		}
		pszData[ nIndex * 2 + 1] = ( 0x0F & byData) | byDataDummy;
	}
	WritePrivateProfileString( m_cStrUserName, cStrKey, pszData, cStrProfile);
	delete [] pszData;

	// テンプレート
	cStrKey.LoadString( IDS_REG_ENABLETEMPLET);
	cStrData.Format( "%d", m_blEnableTemplet);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	// テンプレートファイル
	cStrKey.LoadString( IDS_REG_TEMPLETFILE);
	WritePrivateProfileString( m_cStrUserName, cStrKey, m_cStrTempletFile, cStrProfile);

	// 覚え書ユーティリティとの互換処理
	cStrKey.LoadString( IDS_REG_COMPATIBLEOU);
	cStrData.Format( "%d", m_blCompatibleOU);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// ユーザー設定による定義色
	for( int nIndex = 0; nIndex < 16; nIndex++)
	{
		cStrKey.Format( IDS_REG_USERCOLORS, nIndex);
		cStrData.Format( "0x%08X", m_clrCustomColor[ nIndex]);
		WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);
	}
	// データバックアップの世代数
	cStrKey.LoadString( IDS_REG_GENERATIONCOUNT);
	cStrData.Format( "%d", m_nGenerationCount);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 受信時の保存指定
	cStrKey.LoadString( IDS_REG_RECEIVEANDSAVE);
	cStrData.Format( "%d", m_blReceiveAndSave);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	// 編集時の保存指定
	cStrKey.LoadString( IDS_REG_MODIFYANDSAVE);
	cStrData.Format( "%d", m_blModifyAndSave);
	WritePrivateProfileString( m_cStrUserName, cStrKey, cStrData, cStrProfile);

	return TRUE;
}

BOOL CProfileData::InitializeProfile( void)
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

	// 固定値データ
	LOGFONT		stLogFont = { -19, 0, 0, 0, 400, FALSE, FALSE, FALSE, 128, 3, 2, 1, 50, "ＭＳ Ｐゴシック"};

	CopyMemory( &m_stLogFont, &stLogFont, sizeof( LOGFONT));
	m_clrForeColor = GetSysColor( COLOR_WINDOWTEXT);
	m_clrBackColor = GetSysColor( COLOR_WINDOW);
	m_nIconIndex = 0;

	m_clrAfterFore = RGB( 0xFF, 0x00, 0x00);
	m_clrAfterBack = RGB( 0xFF, 0xFF, 0xFF);
	m_clrBeforeFore = RGB( 0xFF, 0xFF, 0x00);
	m_clrBeforeBack = RGB( 0x00, 0x00, 0x00);

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

	m_blAutoResize = TRUE;
	m_blAutoTriming = TRUE;
	m_blAutoIMEOpen = TRUE;

	m_blPopupReceive = FALSE;
		m_blHideReceive = TRUE;
		m_blSubNotify = TRUE;

	m_nLDblClkFunc = DBLCLKFUNC_ICON;
	m_nMDblClkFunc = DBLCLKFUNC_EDIT;

	m_nDataListFilter = 0;

	m_blDataListReverse = FALSE;
	m_blDelListReverse = FALSE;

	m_dwHotKey = 0;

	m_nResumeDelay = -1;

	m_blEnableNetwork = FALSE;

	m_blFlatMenu = FALSE;

	m_blHideback = TRUE;

	m_blNoCheckDelete = FALSE;

	m_blLoggingSendChip = TRUE;
	m_blFullLoggingSendChip = FALSE;

	m_blLoggingReceiveChip = TRUE;
	m_blFullLoggingReceiveChip = TRUE;

	m_blReceptionLink = FALSE;
	m_blReceptionTimer = FALSE;
	m_blReceptionDxl = FALSE;

	m_blCompatibleOU = FALSE;

	m_nGenerationCount = 3;

	m_blReceiveAndSave = FALSE;
	m_blModifyAndSave = FALSE;

	return TRUE;
}
