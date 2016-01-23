#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "ChipExLIb.h"

CSemaphore CChipExLib::sm_cSmpDoCall;

BOOL CChipExLib::LoadLibrary()
{
	if( !m_cStrPath.IsEmpty())
	{
		m_hInstance = ::LoadLibrary( m_cStrPath);

		CHIPEXSPECIFICATION	lpChipExSpecification;
		// ヴァージョンの確認
		lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( m_hInstance, "ChipExSpecification");
		if( lpChipExSpecification)
		{
			lpChipExSpecification( &m_stSpecification);
			return TRUE;
		}
		::FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	return FALSE;
}

BOOL CChipExLib::FreeLibrary()
{
	if( m_hInstance)
	{
		::FreeLibrary( m_hInstance);
		m_hInstance = NULL;
	}
	return TRUE;
}

BOOL CChipExLib::ChipExProc( HWND hWnd, CMemoData& cMemoData, BOOL& blReSize, BOOL& blAutoReSize)
{
	BOOL	blResult = FALSE;

	if( m_hInstance)
	{
		if( ( UINT)_IF_DATA_VER <= m_stSpecification.unUpperStructVersion && ( UINT)_IF_DATA_VER >= m_stSpecification.unLowerStructVersion)
		{
			// リソースチェックのDLLから、目的の関数ポインタを取得します。
			CHIPEXPROC	lpChipExProc;
			lpChipExProc = ( CHIPEXPROC)GetProcAddress( m_hInstance, "ChipExProc");
			if( NULL != lpChipExProc)
			{
				IFDATA	stIFData;
				CreateIFData( stIFData, cMemoData);
				if( !lpChipExProc( hWnd, m_nIndex, &stIFData))
				{
					if( ModifyIFData( stIFData, cMemoData, blReSize, blAutoReSize))
					{
						blResult = TRUE;
					}
				}
				ReleaseIFData( stIFData);
			}
		}
	}
	return blResult;
}

BOOL CChipExLib::CreateIFData( IFDATA& stIFData, const CMemoData& cMemoData)
{
	CString	cStr;
	CString	cStrSlave;

	// この構造体のサイズ
	stIFData.m_nSize		= sizeof( IFDATA);
	// この構造体のバージョン：_IF_DATA_VERを指定の事
	stIFData.m_nVersion		= _IF_DATA_VER;

	// DLL側での変更フラグ
	stIFData.m_unModify		= 0;

	// 小片のタイトル
	cMemoData.GetTitle( cStr);
	lstrcpy( stIFData.m_achszMemoTitle, cStr);

	// 小片の内容
	DWORD	dwLength = cMemoData.GetMemoLength() + 1;
	stIFData.m_hachszMemoData = GlobalAlloc( GHND, dwLength);
	LPSTR lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszMemoData);
	lstrcpy( lpsz, cMemoData.GetMemo());
	GlobalUnlock( stIFData.m_hachszMemoData);

	// テキスト色
	cMemoData.GetForeColor( stIFData.m_clrTextColor);
	// 背景色
	cMemoData.GetBackColor( stIFData.m_clrBackColor);

	// フォントスタイル
	cMemoData.GetFont( &stIFData.m_stLogFont);

	// Ｚオーダー
	stIFData.m_nZOder	= cMemoData.GetZOrder();

	// パスワード
	cStr = cMemoData.GetPassWord();
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszPassWord = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszPassWord);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszPassWord);
	
	// 関連付け使用の有無
	stIFData.m_blShellExec = cMemoData.GetLinkData( cStr, cStrSlave);
	// 関連付けのタイマでの起動
	stIFData.m_blTimerExec = cMemoData.IsTimerExec();
	// 関連付けのファイル名称
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszFileName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszFileName);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszFileName);
	// 関連付けのプログラム名称
	dwLength = cStrSlave.GetLength() + 1;
	stIFData.m_hachszProgName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszProgName);
	lstrcpy( lpsz, cStrSlave);
	GlobalUnlock( stIFData.m_hachszProgName);

	// タイマ使用の有無
	stIFData.m_blTimerEnable	= cMemoData.IsTimer();
	// タイマの時
	stIFData.m_nHour			= cMemoData.GetTimerHour();
	// タイマの分
	stIFData.m_nMin				= cMemoData.GetTimerMin();
	// タイマタイプ 0/1/2 = 毎日/曜日/月日
	stIFData.m_nTimerType		= cMemoData.GetTimerType();
	// タイマの月
	stIFData.m_nMonth			= cMemoData.GetTimerMonth();
	// タイマの日
	stIFData.m_nDay				= cMemoData.GetTimerDay();
	// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	stIFData.m_nWeekDay			= cMemoData.GetWeek();
	// 通知日が日曜日だった場合のシフト
	stIFData.m_nNotifyShift		= cMemoData.GetNotifyShift();
	// ｎ日前からの通知
	stIFData.m_nBeforeNotify	= cMemoData.GetBeforeNotify();
	// 経過後のタイマー
	stIFData.m_blExpireTimer	= cMemoData.IsAfterNotify();
	// サウンドファイル
	cMemoData.GetNotifySound( cStr);
	dwLength = cStr.GetLength() + 1;
	stIFData.m_hachszSoundFile = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIFData.m_hachszSoundFile);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIFData.m_hachszSoundFile);

	// アイコン種別
	stIFData.m_nIconIndex		= cMemoData.GetIconIndex();

	// 小片サイズ
	stIFData.m_stRctChip		= cMemoData.GetNormalRect();
	// アイコン位置
	stIFData.m_stPntIcon		= cMemoData.GetIconRect().TopLeft();

	return FALSE;
}

BOOL CChipExLib::ReleaseIFData( IFDATA& stIFData)
{
	// 小片の内容
	GlobalFree( stIFData.m_hachszMemoData);
	// 関連付けのファイル名称
	GlobalFree( stIFData.m_hachszFileName);
	// 関連付けのプログラム名称
	GlobalFree( stIFData.m_hachszProgName);
	// パスワード
	GlobalFree( stIFData.m_hachszPassWord);
	// サウンドファイル
	GlobalFree( stIFData.m_hachszSoundFile);

	return FALSE;
}

BOOL CChipExLib::ModifyIFData( IFDATA& stIFData, CMemoData& cMemoData, BOOL& blResize, BOOL& blAutoResize)
{
	CString	cStr;
	CString	cStrSlave;
	BOOL	blResult = FALSE;
	LPCSTR	lpcsz;

	if( _MODIFY_MEMO & stIFData.m_unModify)
	{
		// 小片のタイトル
		cMemoData.GetTitle( cStr);
		if( cStr != stIFData.m_achszMemoTitle)
		{
			cMemoData.SetTitle( stIFData.m_achszMemoTitle);
			blResult = TRUE;
		}

		// 小片の内容
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszMemoData);
		if( lstrlen( lpcsz))
		{
			cMemoData.SetMemo( lpcsz);
			blResult = TRUE;
		}
		GlobalUnlock( stIFData.m_hachszMemoData);
	}
	if( _MODIFY_COLOR & stIFData.m_unModify)
	{
		// 文字色
		cMemoData.SetForeColor( stIFData.m_clrTextColor);
		// バックカラー
		cMemoData.SetBackColor( stIFData.m_clrBackColor);
		blResult = TRUE;
	}
	if( _MODIFY_FONT & stIFData.m_unModify)
	{
		// フォントスタイル
		cMemoData.SetFont( &stIFData.m_stLogFont);
		blResult = TRUE;
	}
	if( _MODIFY_PASSWORD & stIFData.m_unModify)
	{
		// 小片の内容
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszMemoData);
		if( 4 <= lstrlen( lpcsz))
		{
			cMemoData.SetPassWord( lpcsz);
			blResult = TRUE;
		}
		GlobalUnlock( stIFData.m_hachszMemoData);
	}
	if( _MODIFY_ZODER & stIFData.m_unModify)
	{
		// Ｚオーダー
		cMemoData.SetZOrder( stIFData.m_nZOder);
		blResult = TRUE;
	}

	if( _MODIFY_SHELL & stIFData.m_unModify)
	{
		// 関連付け使用の有無
		cMemoData.Link( stIFData.m_blShellExec);
		// 関連付けのタイマでの起動
		cMemoData.TimerExec( stIFData.m_blTimerExec);
		// 関連付けのファイル名称
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszFileName);
		cStr = lpcsz;
		GlobalUnlock( stIFData.m_hachszFileName);
		// 関連付けのプログラム名称
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszProgName);
		cStrSlave = lpcsz;
		GlobalUnlock( stIFData.m_hachszProgName);
		cMemoData.SetLinkData( cStr, cStrSlave);
		blResult = TRUE;
	}

	if( _MODIFY_TIMER & stIFData.m_unModify)
	{
		// タイマ使用の有無
		cMemoData.Timer( stIFData.m_blTimerEnable);
		// タイマの時
		cMemoData.SetTimerHour( stIFData.m_nHour);
		// タイマの分
		cMemoData.SetTimerMin( stIFData.m_nMin);
		// タイマタイプ 0/1/2 = 毎日/曜日/月日
		cMemoData.SetTimerType( stIFData.m_nTimerType);
		// タイマの月
		cMemoData.SetTimerMonth( stIFData.m_nMonth);
		// タイマの日
		cMemoData.SetTimerDay( stIFData.m_nDay);
		// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
		cMemoData.SetWeek( stIFData.m_nWeekDay);
		// 通知日が日曜日だった場合のシフト
		cMemoData.SetNotifyShift( stIFData.m_nNotifyShift);
		// ｎ日前からの通知
		cMemoData.SetBeforeNotify( stIFData.m_nBeforeNotify);
		// 経過後のタイマー
		cMemoData.AfterNotify( stIFData.m_blExpireTimer);
		// サウンドファイル
		lpcsz = ( LPCSTR)GlobalLock( stIFData.m_hachszSoundFile);
		cStr = lpcsz;
		GlobalUnlock( stIFData.m_hachszSoundFile);
		cMemoData.SetNotifySound( cStr);

		blResult = TRUE;
	}

	if( _MODIFY_ICON & stIFData.m_unModify)
	{
		// アイコン種別
		cMemoData.SetIconIndex( stIFData.m_nIconIndex);
		blResult = TRUE;
	}

	if( _MODIFY_POINTICON & stIFData.m_unModify)
	{
		// アイコン位置
		CSize cSize;
		cSize = cMemoData.GetIconRect().Size();

		CRect	cRect;
		cRect.left		= stIFData.m_stPntIcon.x;
		cRect.top		= stIFData.m_stPntIcon.y;
		cRect.right		= cRect.left + cSize.cx;
		cRect.bottom	= cRect.top + cSize.cy;
		cMemoData.SetIconRect( cRect);

		blResult = TRUE;
	}

	if( _MODIFY_RECTCHIP & stIFData.m_unModify)
	{
		// 小片サイズ
		cMemoData.SetNormalRect( stIFData.m_stRctChip);
		blResize = TRUE;
		blResult = TRUE;
	}
	else
	{
		if( _MODIFY_AUTORESIZE & stIFData.m_unModify)
		{
			blAutoResize = TRUE;
			blResult = TRUE;
		}
	}

	return blResult;
}
