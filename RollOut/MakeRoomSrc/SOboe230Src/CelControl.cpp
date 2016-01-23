#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include "CelControl.h"

CSemaphore CCelControl::sm_cSmpDoCel;

// 使用中？
BOOL CCelControl::IsUsed()
{
	BOOL blResult;

	CSingleLock	cSlk( &sm_cSmpDoCel);

	blResult = cSlk.Lock( 0);

	return !blResult;
}

CCelControl::CCelControl( void)
{
	m_blValid = FALSE;
	m_cStrSeparator.Empty();
	for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
	{
		m_acStrMenu[ nIndex].Empty();
		m_acStrPath[ nIndex].Empty();
		m_cStrSeparator += '0';
	}
}

BOOL CCelControl::SetSelData( int nIndex, LPCSTR lpcszPath, LPCSTR lpcszMenu)
{
	if( _CEL_MAX <= nIndex && 0 > nIndex)return FALSE;
	if( 0 >= lstrlen( lpcszPath))return FALSE;

	BOOL	blResult = FALSE;

	OFSTRUCT	of;
	if( HFILE_ERROR != OpenFile( lpcszPath, &of, OF_READ | OF_EXIST))
	{
		HINSTANCE hInst = LoadLibrary( of.szPathName);
		if( NULL != hInst)
		{
			// ヴァージョンの確認
			CHIPEXSPECIFICATION	lpChipExSpecification;
			lpChipExSpecification = ( CHIPEXSPECIFICATION)GetProcAddress( hInst, "ChipExSpecification");
			if( lpChipExSpecification)
			{
				SPECIFICATION	stSpecification;
				lpChipExSpecification( &stSpecification);
				if( ( UINT)_IF_DATA_VER <= stSpecification.unUpperStructVersion && ( UINT)_IF_DATA_VER >= stSpecification.unLowerStructVersion)
				{
					m_acStrPath[ nIndex] = of.szPathName;
					m_acStrMenu[ nIndex] = stSpecification.szMenuString;
					if( NULL != lpcszMenu && 0 < lstrlen( lpcszMenu))
					{
						m_acStrMenu[ nIndex] = lpcszMenu;
					}
					else
					{
						m_acStrMenu[ nIndex] = stSpecification.szMenuString;
					}
					blResult = TRUE;
					m_blValid = TRUE;
				}
			}
			FreeLibrary( hInst);
		}
	}
	return blResult;
}

BOOL CCelControl::SetMenuSeparator( LPCSTR lpcsz)
{
	if( NULL == lpcsz)return FALSE;
	if( _CEL_MAX > lstrlen( lpcsz))return FALSE;

	m_cStrSeparator = lpcsz;

	return TRUE;
}

HMENU CCelControl::GetMenu( void) const
{
	if( FALSE == m_blValid)return NULL;

	HMENU	hMenu = CreateMenu();
	if( hMenu)
	{
		for( int nIndex = 0; nIndex < _CEL_MAX; nIndex++)
		{
			if( !m_acStrMenu[ nIndex].IsEmpty())
			{
				if( '0' != m_cStrSeparator[ nIndex])
				{
					AppendMenu( hMenu, MF_SEPARATOR, 0, NULL);
				}
				AppendMenu( hMenu, MF_STRING, _CEL_MENUTOP + nIndex, m_acStrMenu[ nIndex]);
			}
		}
	}
	return hMenu;
}

DWORD CCelControl::DoCel( HWND hWnd, UINT unMenuID, CMemoData& cMemoData) const
{
	DWORD dwResult = _CELCTL_NOP;
	CSingleLock	cSlk( &sm_cSmpDoCel);
	if( cSlk.Lock( 0))
	{
		if( _CEL_MENUTOP <= unMenuID && _CEL_MENUBOTTOM >= unMenuID)
		{
			int	nIndex = unMenuID - _CEL_MENUTOP;
			if( !m_acStrPath[ nIndex].IsEmpty())
			{
				HINSTANCE		hInst = NULL;
				hInst = LoadLibrary( m_acStrPath[ nIndex]);
				if( hInst)
				{
					// リソースチェックのDLLから、目的の関数ポインタを取得します。
					CHIPEXPROC	lpChipExProc;
					lpChipExProc = ( CHIPEXPROC)GetProcAddress( hInst, "ChipExProc");
					if( NULL != lpChipExProc)
					{
						IFDATA	stIfData;
						MakeIFData( stIfData, cMemoData);
						if( !lpChipExProc( hWnd, nIndex, &stIfData))
						{
							dwResult = ModifyIFData( cMemoData, stIfData);
						}
						DeleteIFData( stIfData);
					}
					FreeLibrary( hInst);
				}
				else
				{
					dwResult |= _CELCTL_ERROR;
				}
			}
			else
			{
				dwResult |= _CELCTL_ERROR;
			}
		}
		else
		{
			dwResult |= _CELCTL_ERROR;
		}
	}
	return dwResult;
}

BOOL CCelControl::MakeIFData( IFDATA& stIfData, CMemoData& cMemoData) const
{
	// この構造体のサイズ
	stIfData.m_nSize		= sizeof( IFDATA);
	// この構造体のバージョン：_IF_DATA_VERを指定の事
	stIfData.m_nVersion		= _IF_DATA_VER;

	// DLL側での変更フラグ
	stIfData.m_unModify		= 0;

	// 小片のタイトル
	CString	cStr;
	cMemoData.GetTitle( cStr);
	lstrcpy( stIfData.m_achszMemoTitle, cStr);

	// 小片の内容
	DWORD	dwLength = cMemoData.GetMemoLength() + 1;
	stIfData.m_hachszMemoData = GlobalAlloc( GHND, dwLength);
	LPSTR lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszMemoData);
	lstrcpy( lpsz, cMemoData.GetMemo());
	GlobalUnlock( stIfData.m_hachszMemoData);

	// テキスト色
	cMemoData.GetForeColor( stIfData.m_clrTextColor);
	// 背景色
	cMemoData.GetBackColor( stIfData.m_clrBackColor);

	// フォントスタイル
	cMemoData.GetFont( &stIfData.m_stLogFont);

	// Ｚオーダー
	stIfData.m_nZOder	= cMemoData.GetZOrder();

	// パスワード
	dwLength = cMemoData.GetPassWord().GetLength() + 1;
	stIfData.m_hachszPassWord = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszPassWord);
	lstrcpy( lpsz, cMemoData.GetPassWord());
	GlobalUnlock( stIfData.m_hachszPassWord);
	
	// 関連付け使用の有無
	stIfData.m_blShellExec = cMemoData.IsLink();
	// 関連付けのタイマでの起動
	stIfData.m_blTimerExec = cMemoData.IsTimerExec();
	// 関連付けのファイル名称
	CString	cStrParam;
	cMemoData.GetLinkData( cStr, cStrParam);
	dwLength = cStr.GetLength() + 1;
	stIfData.m_hachszFileName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszFileName);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIfData.m_hachszFileName);
	// 関連付けのプログラム名称
	dwLength = cStrParam.GetLength() + 1;
	stIfData.m_hachszProgName = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszProgName);
	lstrcpy( lpsz, cStrParam);
	GlobalUnlock( stIfData.m_hachszProgName);

	// タイマ使用の有無
	stIfData.m_blTimerEnable	= cMemoData.IsTimer();
	// タイマの時
	stIfData.m_nHour			= cMemoData.GetTimerHour();
	// タイマの分
	stIfData.m_nMin				= cMemoData.GetTimerMin();
	// タイマタイプ 0/1/2 = 毎日/曜日/月日
	stIfData.m_nTimerType		= cMemoData.GetTimerType();
	// タイマの月
	stIfData.m_nMonth			= cMemoData.GetTimerMonth();
	// タイマの日
	stIfData.m_nDay				= cMemoData.GetTimerDay();
	// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
	stIfData.m_nWeekDay			= cMemoData.GetWeek();
	// 通知日が日曜日だった場合のシフト
	stIfData.m_nNotifyShift		= cMemoData.GetNotifyShift();
	// ｎ日前からの通知
	stIfData.m_nBeforeNotify	= cMemoData.GetBeforeNotify();
	// 経過後のタイマー
	stIfData.m_blExpireTimer	= cMemoData.IsAfterNotify();
	// サウンドファイル
	cMemoData.GetNotifySound( cStr);
	dwLength = cStr.GetLength() + 1;
	stIfData.m_hachszSoundFile = GlobalAlloc( GHND, dwLength);
	lpsz = ( LPSTR)GlobalLock( stIfData.m_hachszSoundFile);
	lstrcpy( lpsz, cStr);
	GlobalUnlock( stIfData.m_hachszSoundFile);

	// アイコン種別
	stIfData.m_nIconIndex		= cMemoData.GetIconIndex();

	// 小片サイズ
	CRect	cRect = cMemoData.GetNormalRect();
	stIfData.m_stRctChip.left	= cRect.left;
	stIfData.m_stRctChip.top	= cRect.top;
	stIfData.m_stRctChip.right	= cRect.right;
	stIfData.m_stRctChip.bottom	= cRect.bottom;
	// アイコン位置
	cRect = cMemoData.GetIconRect();
	stIfData.m_stPntIcon.x		= cRect.left;
	stIfData.m_stPntIcon.y		= cRect.top;

	return TRUE;
}

BOOL CCelControl::DeleteIFData( IFDATA& stIfData) const
{
	// 小片の内容
	GlobalFree( stIfData.m_hachszMemoData);
	// 関連付けのファイル名称
	GlobalFree( stIfData.m_hachszFileName);
	// 関連付けのプログラム名称
	GlobalFree( stIfData.m_hachszProgName);
	// パスワード
	GlobalFree( stIfData.m_hachszPassWord);
	// サウンドファイル
	GlobalFree( stIfData.m_hachszSoundFile);

	return TRUE;
}

DWORD CCelControl::ModifyIFData( CMemoData& cMemoData, IFDATA& stIfData) const
{
	DWORD dwResult = _CELCTL_REDRAW;
	LPCSTR	lpcsz;

	if( _MODIFY_MEMO & stIfData.m_unModify)
	{
		// 小片のタイトル
		CString	cStrTitle;
		cMemoData.GetTitle( cStrTitle);
		if( lstrcmp( stIfData.m_achszMemoTitle, cStrTitle))
		{
			cMemoData.SetTitle( cStrTitle);
		}

		// 小片の内容
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszMemoData);
		if( lstrlen( lpcsz))
		{
			cMemoData.SetMemo( lpcsz);
		}
		GlobalUnlock( stIfData.m_hachszMemoData);
	}
	if( _MODIFY_COLOR & stIfData.m_unModify)
	{
		// 文字色
		cMemoData.SetForeColor( stIfData.m_clrTextColor);
		// バックカラー
		cMemoData.SetBackColor( stIfData.m_clrBackColor);
	}
	if( _MODIFY_FONT & stIfData.m_unModify)
	{
		// フォントスタイル
		cMemoData.SetFont( &stIfData.m_stLogFont);
	}
	if( _MODIFY_PASSWORD & stIfData.m_unModify)
	{
		// 小片の内容
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszMemoData);
		if( 4 <= lstrlen( lpcsz))
		{
			cMemoData.SetPassWord( lpcsz);
		}
		GlobalUnlock( stIfData.m_hachszMemoData);
	}
	if( _MODIFY_ZODER & stIfData.m_unModify)
	{
		// Ｚオーダー
		cMemoData.SetZOrder( stIfData.m_nZOder);
	}

	if( _MODIFY_SHELL & stIfData.m_unModify)
	{
		// 関連付け使用の有無
		cMemoData.Link( stIfData.m_blShellExec);
		// 関連付けのタイマでの起動
		cMemoData.TimerExec( stIfData.m_blTimerExec);
		// 関連付けのファイル名称
		LPCSTR lpcszFileName = ( LPCSTR)GlobalLock( stIfData.m_hachszFileName);
		// 関連付けのプログラム名称
		LPCSTR lpcszProgName = ( LPCSTR)GlobalLock( stIfData.m_hachszProgName);
		cMemoData.SetLinkData( lpcszProgName, lpcszFileName);
		GlobalUnlock( stIfData.m_hachszProgName);
		GlobalUnlock( stIfData.m_hachszFileName);
	}

	if( _MODIFY_TIMER & stIfData.m_unModify)
	{
		// タイマ使用の有無
		cMemoData.Timer( stIfData.m_blTimerEnable);
		// タイマの時
		cMemoData.SetTimerHour( stIfData.m_nHour);
		// タイマの分
		cMemoData.SetTimerMin( stIfData.m_nMin);
		// タイマタイプ 0/1/2 = 毎日/曜日/月日
		cMemoData.SetTimerType( stIfData.m_nTimerType);
		// タイマの月
		cMemoData.SetTimerMonth( stIfData.m_nMonth);
		// タイマの日
		cMemoData.SetTimerDay( stIfData.m_nDay);
		// 曜日種別タイマの曜日指定 0bit=日曜日 1bit月曜日...
		cMemoData.SetWeek( stIfData.m_nWeekDay);
		// 通知日が日曜日だった場合のシフト
		cMemoData.SetNotifyShift( stIfData.m_nNotifyShift);
		// ｎ日前からの通知
		cMemoData.SetBeforeNotify( stIfData.m_nBeforeNotify);
		// 経過後のタイマー
		cMemoData.AfterNotify( stIfData.m_blExpireTimer);
		// サウンドファイル
		lpcsz = ( LPCSTR)GlobalLock( stIfData.m_hachszSoundFile);
		cMemoData.SetNotifySound( lpcsz);
		GlobalUnlock( stIfData.m_hachszSoundFile);
	}

	if( _MODIFY_ICON & stIfData.m_unModify)
	{
		// アイコン種別
		cMemoData.SetIconIndex( stIfData.m_nIconIndex);
	}

	if( _MODIFY_POINTICON & stIfData.m_unModify)
	{
		// アイコン位置
		CRect	cRctIcon;
		cRctIcon.left	= stIfData.m_stPntIcon.x;
		cRctIcon.top	= stIfData.m_stPntIcon.y;
		cRctIcon.right	= cRctIcon.left + _ICON_WIDTH;
		cRctIcon.bottom	= cRctIcon.top + _ICON_HEIGHT;

		cMemoData.SetIconRect( cRctIcon);
	}

	if( _MODIFY_RECTCHIP & stIfData.m_unModify)
	{
		// 小片サイズ
		CRect	cRctNormal	= stIfData.m_stRctChip;
		cMemoData.SetNormalRect( cRctNormal);
		dwResult = _CELCTL_MOVE;
	}
	else
	{
		if( _MODIFY_AUTORESIZE & stIfData.m_unModify)
		{
			dwResult = _CELCTL_RESIZE;
		}
	}
	return dwResult;
}
