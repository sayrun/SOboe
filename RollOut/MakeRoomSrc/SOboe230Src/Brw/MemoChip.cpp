// MemoChip.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Brw.h"
#include "MemoChip.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL( CMemoChip, CWnd, VERSIONABLE_SCHEMA | _SCHEMA_ID)
/////////////////////////////////////////////////////////////////////////////
// CMemoChip
CMemoChip::CMemoChip()
{
	m_blDataAppry	= FALSE;
	m_blMinSize		= FALSE;
	m_blClose		= FALSE;
	m_blFlash		= FALSE;
	m_nTodayTimer	= 0;
	m_cPntStart		= CPoint( -1, -1);
	m_pcWndParent	= NULL;
	m_blShow		= TRUE;

	m_blUndo		= FALSE;
	m_cStrUndo		= _T("");

	m_blFrameDraw	= TRUE;

	m_blBeforeHide	= FALSE;

	m_blChipPosLock	= FALSE;

	m_blSysResize	= FALSE;

	m_blCheckOpen	= FALSE;	// 開封確認:1.40
	m_blCheckYesNo	= FALSE;	// はい／いいえ確認:1.40
	m_cStrSignature.Empty();	// 送り主
	m_cStrComp.Empty();			// 送り元
	m_cStrCaption.Empty();		// 
	m_cStrPeerAddres.Empty();		// 
	m_cStrUser.Empty();			// 送り元

	m_blCheckedReceive = FALSE;

	m_blMove = FALSE;
}

CMemoChip::CMemoChip( CMemoChip& cMemoChip)
{
	m_blDataAppry		= cMemoChip.m_blDataAppry;
	m_blMinSize			= cMemoChip.m_blMinSize;
	m_blClose			= cMemoChip.m_blClose;
	m_blFlash			= cMemoChip.m_blFlash;
	m_nTodayTimer		= cMemoChip.m_nTodayTimer;
	m_cPntStart			= cMemoChip.m_cPntStart;
	m_cMemoData			= cMemoChip.m_cMemoData;
	m_pcWndParent		= cMemoChip.m_pcWndParent;
	m_blShow			= cMemoChip.m_blShow;
	m_blChipPosLock		= cMemoChip.m_blChipPosLock;

	m_blCheckOpen		= cMemoChip.m_blCheckOpen;		// 開封確認:1.40
	m_blCheckYesNo		= cMemoChip.m_blCheckYesNo;		// はい／いいえ確認:1.40
	m_cStrSignature		= cMemoChip.m_cStrSignature;	// 送り主
	m_cStrComp			= cMemoChip.m_cStrComp;			// 送り元
	m_cStrCaption		= cMemoChip.m_cStrCaption;		// 
	m_cStrPeerAddres	= cMemoChip.m_cStrPeerAddres;	// 
	m_cStrUser			= cMemoChip.m_cStrUser;

	m_cMemoData			= cMemoChip.m_cMemoData;
}

CMemoChip::~CMemoChip()
{
}

BEGIN_MESSAGE_MAP(CMemoChip, CWnd)
	//{{AFX_MSG_MAP(CMemoChip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMemoChip メッセージ ハンドラ

void CMemoChip::SetMemoData( CMemoData& cMemoData)
{
	m_cMemoData = cMemoData;
	m_blDataAppry = TRUE;

	// すでにすぎた時間である事を検査
	CTime	cTime;
	cTime = CTime::GetCurrentTime();
	m_nTodayTimer = cTime.GetDay();
	if( m_cMemoData.IsTimer())
	{
		if( m_cMemoData.GetTimerHour() > cTime.GetHour())m_nTodayTimer = 0;
		if( m_cMemoData.GetTimerHour() == cTime.GetHour() && m_cMemoData.GetTimerMin() > cTime.GetMinute())m_nTodayTimer = 0;
	}
}

BOOL CMemoChip::GetMemoData( CMemoData& cMemoData)
{
	if( FALSE == m_blDataAppry)return FALSE;

	cMemoData = m_cMemoData;

	return TRUE;
}

void CMemoChip::Serialize( CArchive& ar )
{
	try
	{
		CObject::Serialize( ar );
		if( ar.IsStoring())
		{
			m_cMemoData.Store( ar);
		}
		else
		{
			UINT unVersion = ar.GetObjectSchema();
			m_cMemoData.Restore( ar, unVersion);
		}
	}
	catch( CFileException e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}


const BOOL CMemoChip::IsPassWordLock( LPCSTR lpcszPassWord/*=NULL*/)
{
	if( !m_cMemoData.IsPassWordLock())return FALSE;
	if( NULL == lpcszPassWord)return TRUE;
	if( m_cMemoData.GetPassWord() == lpcszPassWord)return FALSE;
	return TRUE;
}

int CMemoChip::GetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord)
{
	// この構造体のバージョン：_IF_DATA_VERを指定の事
	if( _BRW_IF_DATA_OLD_VER != pstIFData->m_nVersion &&
		_BRW_IF_DATA_300_VER != pstIFData->m_nVersion &&
		_BRW_IF_DATA_CUR_VER != pstIFData->m_nVersion)return -1;

	// 小片のタイトル
	CString	cStr;
	m_cMemoData.GetTitle( cStr);
	lstrcpyn( pstIFData->m_achszMemoTitle, cStr, _MAX_TITLE_LEN + 1);

	// テキスト色
	m_cMemoData.GetForeColor( pstIFData->m_clrTextColor);
	// 背景色
	m_cMemoData.GetBackColor( pstIFData->m_clrBackColor);

	// フォントスタイル
	m_cMemoData.GetFont( &pstIFData->m_stLogFont);

	LPSTR	lpsz;
	DWORD	dwLength;
	if( IsPassWordLock( lpcszPassWord))
	{
		CString	cStr;
		cStr.LoadString( IDS_PWPROTECT);

		// 小片の内容
		DWORD	dwLength = cStr.GetLength() + 1;
		pstIFData->m_hachszMemoData = GlobalAlloc( GHND, dwLength);
		LPSTR lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszMemoData);
		lstrcpy( lpsz, cStr);
		GlobalUnlock( pstIFData->m_hachszMemoData);
		return -2;
	}
	// パスワード
	dwLength = m_cMemoData.GetPassWord().GetLength();
	pstIFData->m_hachszPassWord = NULL;
	if( dwLength)
	{
		pstIFData->m_hachszPassWord = GlobalAlloc( GHND, dwLength + 1);
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszPassWord);
		lstrcpy( lpsz, m_cMemoData.GetPassWord());
		GlobalUnlock( pstIFData->m_hachszPassWord);
	}

	// 小片の内容
	dwLength = m_cMemoData.GetMemoLength() + 1;
	pstIFData->m_hachszMemoData = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszMemoData);
	lstrcpy( lpsz, m_cMemoData.GetMemo());
	GlobalUnlock( pstIFData->m_hachszMemoData);

	// Ｚオーダー
	pstIFData->m_nZOder	= m_cMemoData.GetZOrder();

	// 関連付け使用の有無
	pstIFData->m_blShellExec = m_cMemoData.IsLink();
	// 関連付けのタイマでの起動
	pstIFData->m_blTimerExec = m_cMemoData.IsTimerExec();

	CString	cStrProg;
	CString	cStrParam;
	m_cMemoData.GetLinkData( cStrProg, cStrParam);
	// 関連付けのプログラム名称
	dwLength = cStrProg.GetLength();
	pstIFData->m_hachszProgName = NULL;
	if( dwLength)
	{
		pstIFData->m_hachszProgName = GlobalAlloc( GHND, dwLength + 1);
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszProgName);
		lstrcpy( lpsz, cStrProg);
		GlobalUnlock( pstIFData->m_hachszProgName);
	}
	// 関連付けのファイル名称
	dwLength = cStrParam.GetLength();
	pstIFData->m_hachszFileName = NULL;
	if( dwLength)
	{
		pstIFData->m_hachszFileName = GlobalAlloc( GHND, dwLength + 1);
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszFileName);
		lstrcpy( lpsz, cStrParam);
		GlobalUnlock( pstIFData->m_hachszFileName);
	}

	// タイマ使用の有無
	pstIFData->m_blTimerEnable	= m_cMemoData.IsTimer();
	// タイマの時
	pstIFData->m_nHour			= m_cMemoData.GetTimerHour();
	// タイマの分
	pstIFData->m_nMin			= m_cMemoData.GetTimerMin();
	// タイマタイプ 0/1/2 = 毎日/曜日/月日
	pstIFData->m_nTimerType		= m_cMemoData.GetTimerType();
	// タイマの月
	pstIFData->m_nMonth			= m_cMemoData.GetTimerMonth();
	// タイマの日
	pstIFData->m_nDay			= m_cMemoData.GetTimerMonth();
	// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	pstIFData->m_nWeekDay		= m_cMemoData.GetWeek();
	// 通知日が日曜日だった場合のシフト
	pstIFData->m_nNotifyShift	= m_cMemoData.GetNotifyShift();
	// ｎ日前からの通知
	pstIFData->m_nBeforeNotify	= m_cMemoData.GetBeforeNotify();
	// 経過後のタイマー
	pstIFData->m_blExpireTimer	= m_cMemoData.IsAfterNotify();
	// サウンドファイル
	CString	cStrSound;
	m_cMemoData.GetNotifySound( cStrSound);
	dwLength = cStrSound.GetLength();
	pstIFData->m_hachszSoundFile = NULL;
	if( dwLength)
	{
		pstIFData->m_hachszSoundFile = GlobalAlloc( GHND, dwLength + 1);
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszSoundFile);
		lstrcpy( lpsz, cStrSound);
		GlobalUnlock( pstIFData->m_hachszSoundFile);
	}

	// 表示情報
	pstIFData->m_blMinSize		= ( CMemoData::WINDOW_TYPE_NORMAL != m_cMemoData.GetWindowType());
	pstIFData->m_blShow			= m_cMemoData.IsShow();
	// 表示位置固定
	pstIFData->m_blChipPosLock	= m_cMemoData.IsPosLock();
	// アイコン種別
	pstIFData->m_nIconIndex		= m_cMemoData.GetIconIndex();

	// 小片サイズ
	pstIFData->m_stRctChip		= m_cMemoData.GetNormalRect();
	// アイコン位置
	CRect	cRect = m_cMemoData.GetIconRect();
	pstIFData->m_stPntIcon.x		= cRect.left;
	pstIFData->m_stPntIcon.y		= cRect.top;

	// 削除年月日
	m_cMemoData.GetCreateDate( pstIFData->m_nDeleteYear, pstIFData->m_nDeleteMonth, pstIFData->m_nDeleteDay);

	if( _BRW_IF_DATA_300_VER <= pstIFData->m_nVersion)
	{
		// シースルーフラグ
		pstIFData->m_blSeeTrough		= m_cMemoData.IsSeeThrough();
		// DxlのユニークなID
		pstIFData->m_unDXLID			= m_cMemoData.GetDxlID();
		if( _BRW_IF_DATA_CUR_VER <= pstIFData->m_nVersion)
		{
			pstIFData->m_nDxlSaveDataSize	= m_cMemoData.GetDxlExtData();
			if( pstIFData->m_nDxlSaveDataSize)
			{
				pstIFData->m_hDxlSaveData	= GlobalAlloc( GHND, pstIFData->m_nDxlSaveDataSize);
				LPBYTE	lpbyData = ( LPBYTE)GlobalLock( pstIFData->m_hDxlSaveData);
				if( lpbyData)
				{
					m_cMemoData.GetDxlExtData( lpbyData, pstIFData->m_nDxlSaveDataSize);
					GlobalUnlock( pstIFData->m_hDxlSaveData);
				}
			}
			else
			{
				pstIFData->m_hDxlSaveData	= NULL;
			}
		}
	}

	return FALSE;
}

BOOL CMemoChip::SetIFData( IFDATA* pstIFData, LPCSTR lpcszPassWord)
{
	if( NULL == pstIFData)return -1;
//	if( sizeof( IFDATA)  != pstIFData->m_nSize)return -1;
	if( _BRW_IF_DATA_OLD_VER != pstIFData->m_nVersion &&
		_BRW_IF_DATA_300_VER != pstIFData->m_nVersion &&
		_BRW_IF_DATA_CUR_VER != pstIFData->m_nVersion)return -1;
	if( IsPassWordLock( lpcszPassWord))return -2;
	if( NULL == pstIFData->m_hachszMemoData)return -1;

	// 小片のタイトル
	m_cMemoData.SetTitle( pstIFData->m_achszMemoTitle);

	// 小片の内容
	LPSTR lpsz;
	lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszMemoData);
	m_cMemoData.SetMemo( lpsz);
	GlobalUnlock( pstIFData->m_hachszMemoData);

	// テキスト色
	m_cMemoData.SetForeColor( pstIFData->m_clrTextColor);
	// 背景色
	m_cMemoData.SetBackColor( pstIFData->m_clrBackColor);

	// フォントスタイル
	m_cMemoData.SetFont( &pstIFData->m_stLogFont);

	// Ｚオーダー
	m_cMemoData.SetZOrder( pstIFData->m_nZOder);

	// パスワード
	CString	cStrPass;
	if( pstIFData->m_hachszPassWord)
	{
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszPassWord);
		cStrPass = lpsz;
		GlobalUnlock( pstIFData->m_hachszPassWord);
	}
	m_cMemoData.SetPassWord( cStrPass);
	
	// 関連付け使用の有無
	m_cMemoData.Link( pstIFData->m_blShellExec);
	// 関連付けのタイマでの起動
	m_cMemoData.TimerExec( pstIFData->m_blTimerExec);
	// 関連付けのプログラム名称
	CString	cStrProgName;
	if( pstIFData->m_hachszProgName)
	{
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszProgName);
		cStrProgName = lpsz;
		GlobalUnlock( pstIFData->m_hachszProgName);
	}
	// 関連付けのファイル名称
	CString	cStrFileName;
	if( pstIFData->m_hachszFileName)
	{
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszFileName);
		cStrFileName = lpsz;
		GlobalUnlock( pstIFData->m_hachszFileName);
	}
	m_cMemoData.SetLinkData( cStrProgName, cStrFileName);

	// タイマ使用の有無
	m_cMemoData.Timer( pstIFData->m_blTimerEnable);
	// タイマの時
	m_cMemoData.SetTimerHour( pstIFData->m_nHour);
	// タイマの分
	m_cMemoData.SetTimerMin( pstIFData->m_nMin);
	// タイマタイプ 0/1/2 = 毎日/曜日/月日
	m_cMemoData.SetTimerType( pstIFData->m_nTimerType);
	// タイマの月
	m_cMemoData.SetTimerMonth( pstIFData->m_nMonth);
	// タイマの日
	m_cMemoData.SetTimerDay( pstIFData->m_nDay);
	// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	m_cMemoData.SetWeek( pstIFData->m_nWeekDay);
	// 通知日が日曜日だった場合のシフト
	m_cMemoData.SetNotifyShift( pstIFData->m_nNotifyShift);
	// ｎ日前からの通知
	m_cMemoData.SetBeforeNotify( pstIFData->m_nBeforeNotify);
	// 経過後のタイマー
	m_cMemoData.AfterNotify( pstIFData->m_blExpireTimer);
	// サウンドファイル
	CString	cStrWaveFile;
	if( pstIFData->m_hachszSoundFile)
	{
		lpsz = ( LPSTR)GlobalLock( pstIFData->m_hachszSoundFile);
		cStrWaveFile = lpsz;
		GlobalUnlock( pstIFData->m_hachszSoundFile);
	}
	m_cMemoData.SetNotifySound( cStrWaveFile);

	// 表示情報
	m_cMemoData.SetWindowType( pstIFData->m_blMinSize ? CMemoData::WINDOW_TYPE_NORMAL : CMemoData::WINDOW_TYPE_ICON);
	m_cMemoData.Show( pstIFData->m_blShow);
	// 表示位置固定
	m_cMemoData.PosLock( pstIFData->m_blChipPosLock);
	// アイコン種別
	m_cMemoData.SetIconIndex( pstIFData->m_nIconIndex);

	// 小片サイズ
	m_cMemoData.SetNormalRect( pstIFData->m_stRctChip);
	// アイコン位置
	CRect	cRctIcon;
	cRctIcon.left	= pstIFData->m_stPntIcon.x;
	cRctIcon.right	= pstIFData->m_stPntIcon.x + _ICON_WIDTH;
	cRctIcon.top	= pstIFData->m_stPntIcon.y;
	cRctIcon.bottom	= pstIFData->m_stPntIcon.y + _ICON_HEIGHT;
	m_cMemoData.SetIconRect( cRctIcon);

	// 削除年月日
	m_cMemoData.SetCreateDate();
	/*
	m_cMemoData.m_nGoodForYear		= pstIFData->m_nDeleteYear;
	m_cMemoData.m_nGoodForMonth		= pstIFData->m_nDeleteMonth;
	m_cMemoData.m_nGoodForDay		= pstIFData->m_nDeleteDay;
	*/

	if( _BRW_IF_DATA_300_VER <= pstIFData->m_nVersion)
	{
		// シースルーフラグ
		m_cMemoData.SeeThrough( pstIFData->m_blSeeTrough);
		// DxlのユニークなID
		m_cMemoData.SetDxlID( pstIFData->m_unDXLID);
		if( _BRW_IF_DATA_CUR_VER <= pstIFData->m_nVersion)
		{
			if( pstIFData->m_nDxlSaveDataSize)
			{
				pstIFData->m_hDxlSaveData	= GlobalAlloc( GHND, pstIFData->m_nDxlSaveDataSize);
				LPBYTE	lpbyData = ( LPBYTE)GlobalLock( pstIFData->m_hDxlSaveData);
				if( lpbyData)
				{
					m_cMemoData.SetDxlExtData( lpbyData, pstIFData->m_nDxlSaveDataSize);
					GlobalUnlock( pstIFData->m_hDxlSaveData);
				}
			}
		}
	}

	m_blDataAppry					= TRUE;

	return FALSE;
}

BOOL CMemoChip::ReleaseIFData( IFDATA* pstIFData)
{
	// 小片の内容
	GlobalFree( pstIFData->m_hachszMemoData);
	pstIFData->m_hachszMemoData = NULL;
	// 関連付けのファイル名称
	GlobalFree( pstIFData->m_hachszFileName);
	pstIFData->m_hachszFileName = NULL;
	// 関連付けのプログラム名称
	GlobalFree( pstIFData->m_hachszProgName);
	pstIFData->m_hachszProgName = NULL;
	// パスワード
	GlobalFree( pstIFData->m_hachszPassWord);
	pstIFData->m_hachszPassWord = NULL;
	// サウンドファイル
	GlobalFree( pstIFData->m_hachszSoundFile);
	pstIFData->m_hachszSoundFile = NULL;

	if( _BRW_IF_DATA_300_VER <= pstIFData->m_nVersion)
	{
		pstIFData->m_nDxlSaveDataSize = 0;
		GlobalFree( pstIFData->m_hDxlSaveData);
		pstIFData->m_hDxlSaveData = NULL;
	}

	return FALSE;
}

BOOL CMemoChip::GetMemoTitle( LPSTR lpszTitle)
{
	CString	cStr;
	m_cMemoData.GetTitle( cStr);
	lstrcpyn( lpszTitle, cStr, _MAX_TITLE_LEN + 1);

	return TRUE;
}

int CMemoChip::GetMemo( LPSTR lpszData, int nSize, LPCSTR lpcszPassWord)
{
	if( IsPassWordLock( lpcszPassWord))
	{
		CString	cStr;
		cStr.LoadString( IDS_PWPROTECT);
		if( lpszData)
		{
			lstrcpyn( lpszData, cStr, nSize);
			return lstrlen( lpszData);
		}
		else
		{
			return cStr.GetLength();
		}
	}
	else
	{
		int nLength = m_cMemoData.GetMemoLength();
		if( NULL == lpszData)
		{
			return nLength;
		}
		else
		{
			lstrcpyn( lpszData, m_cMemoData.GetMemo(), nSize);
			return lstrlen( lpszData);
		}
	}
}

COLORREF CMemoChip::GetTextColor() const
{
	COLORREF	clrFore;
	m_cMemoData.GetForeColor( clrFore);
	return clrFore;
}

COLORREF CMemoChip::GetBackColor() const
{
	COLORREF	clrBack;
	m_cMemoData.GetBackColor( clrBack);
	return clrBack;
}

int CMemoChip::GetIconIndex() const
{
	return m_cMemoData.GetIconIndex();
}

BOOL CMemoChip::GetFont(LOGFONT * pstLogFont)
{
	m_cMemoData.GetFont( pstLogFont);

	return TRUE;
}

BOOL CMemoChip::IsLink() const
{
	return m_cMemoData.IsLink();
}
