#include "stdafx.h"
#include "soboe.h"
#include "MemoData.h"
#include <mbctype.h>

// モドギ暗号化に使用するテーブル
CString CMemoData::sm_cStrData = _T( "!#$%&+=-[(<>)]*?");
UINT CMemoData::m_unOboeChip = RegisterClipboardFormat( "OboeGakiChipData100");

CMemoData::CMemoData() : CObject()
{
	// 基本設定
	{
		m_cStrTitle.Empty();			// タイトル
		m_cStrMemo.Empty();				// 表示内容

		m_pstLogFont	= NULL;			// 表示フォント
		m_clrForeColor	= CLR_INVALID;	// 文字色
		m_clrBackColor	= CLR_INVALID;	// 背景色

		m_nIconIndex	= -1;			// アイコン種別
	}

	// 拡張設定
	{
		m_blLink			= FALSE;	// 関連付け有功無効
		m_cStrFile.Empty();				// ファイル
		m_cStrParameters.Empty();		// パラメータ
		m_blTimerExec		= FALSE;	// タイマで起動

		m_blPassWordLock	= FALSE;	// パスワードの有功無効
		m_cStrPassWord.Empty();			// パスワード

		// 描画拡張
		m_unDxlID			= 0;		// Dxl-ID
		m_unDxlExtDataSize	= 0;		// Dxl 拡張データサイズ
		m_pabyDxlExtData	= NULL;		// Dxl 拡張データ
	}

	// タイマ
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

		m_blTimer		= FALSE;				// タイマ通知有功
		m_nTimerType	= TIMERTYPE_EVERYDAY;	// タイマ種別
		m_nNotifyShift	= NOTIFYSHIFT_NOMOVE;	// 通知日の移動処理
		m_nMonth		= cTime.GetMonth();		// 通知月
		m_nDay			= cTime.GetDay();		// 通知日
		m_nHour			= 0;					// 通知時
		m_nMin			= 0;					// 通知分
		m_nWeek			= WEEK_DISABLE;			// 通知曜日

		m_nBeforeNotify	= 0;					// 事前通知
		m_blAfterNotify	= FALSE;				// 経過通知

		m_cStrNotifySound.Empty();				// 通知サウンドファイル
	}

	// Window関係
	{
		m_cRctNormal.SetRectEmpty();
		m_cRctTitle.SetRectEmpty();
		m_cRctIcon.SetRectEmpty();

		m_nWindowType = WINDOW_TYPE_NORMAL;	// 形状
		m_blPosLock = FALSE;

		m_blShow = TRUE;
		m_nZOrder = ZORDER_NORMAL;

		m_blSeeThrough = FALSE;

		m_byAlphaBlend = ALPHABLEND_000;
	}

	// 作成日付記憶
	SetCreateDate();

	// 結果受信用ＩＤ
	m_pcStrContextID = NULL;
}

CMemoData::~CMemoData()
{
	if( m_pstLogFont)
	{
		delete m_pstLogFont;
		m_pstLogFont = NULL;
	}
	if( m_pabyDxlExtData)
	{
		delete [] m_pabyDxlExtData;
		m_pabyDxlExtData = NULL;
	}
	if( m_pcStrContextID)
	{
		delete m_pcStrContextID;
		m_pcStrContextID = NULL;
	}
}

// スタイルの初期化
BOOL CMemoData::SetDefaultStyle( void)
{
	// このような関数はない方がステキだとは思うし、
	// コンストラクタで処理させれば、使用しない様に記述できるんだけれどね。
	// でも、毎度毎度コンストラクタでこの処理がされるかと思うとねぇ……。

	CSOboeApp* pcSOboe = ( CSOboeApp*)AfxGetApp();
	ASSERT( pcSOboe);
	if( pcSOboe)
	{
		// 表示フォント
		if( NULL == m_pstLogFont)
		{
			m_pstLogFont = new LOGFONT;
			if( NULL == m_pstLogFont)return TRUE;
		}
		pcSOboe->GetDefaultFont( m_pstLogFont);

		m_clrForeColor = pcSOboe->GetDefaultForeColor();	// 文字色
		m_clrBackColor = pcSOboe->GetDefaultBackColor();	// 背景色

		m_nIconIndex = pcSOboe->GetDefaultIconIndex();		// アイコン種別

		m_unDxlID = pcSOboe->GetDefaultDxlID(); //GetDefaultDxlID

		return FALSE;
	}
	return TRUE;
}

void CMemoData::RestoreFromOldFormat( CArchive& ar, UINT unVersion)
{
	int		nDummy;
	BOOL	blDummy;
	CString	cStrDummy;
	CRect	cRctDummy;

	BOOL blWork;
	ar >> blWork;
	m_nWindowType = ( FALSE != blWork) ? CMemoData::WINDOW_TYPE_ICON : CMemoData::WINDOW_TYPE_NORMAL;

	ar >> blWork;	// Dummy
	ar >> m_blShow;
	ar >> m_blPosLock;

	ar >> m_cStrTitle;			// メモのタイトル
	ar >> m_cStrMemo;			// メモした内容

	if( m_pstLogFont)
	{
		delete m_pstLogFont;
		m_pstLogFont = NULL;
	}
	m_pstLogFont = new LOGFONT;
	ar.Read( m_pstLogFont, sizeof( LOGFONT));	// Font情報
	
	ar >> m_clrForeColor;				// 文字色
	ar >> m_clrBackColor;				// バックカラー
	ar >> nDummy;						// 情報固有識別ＩＤ

	// タイマー通知関係
	ar >> m_blTimer;				// タイマー使用設定
	ar >> m_nTimerType;				// タイマーのタイプ 0=毎日 1-7=曜日 8=月日
	ar >> m_nHour;						// 通知時刻
	ar >> m_nMin;						// 同上
	ar >> m_nMonth;					// 月日タイプの月
	ar >> m_nDay;					// 月日タイプの日

	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		BYTE	byWork;
		ar >> byWork;					// 曜日:1.40
		m_nWeek = byWork;
		ar >> m_cStrNotifySound;		// 通知音声:1.40
		ar >> m_blAfterNotify;			// 経過後の通知:1.40
		ar >> m_nBeforeNotify;			// 前日通知:1.40
		ar >> m_nNotifyShift;			// 休日移動:1.40
	}
	else
	{
		if( 0 < m_nTimerType && 8 > m_nTimerType)
		{
			m_nWeek = ( BYTE)0x01 << ( m_nTimerType - 1);
			m_nTimerType = CMemoData::TIMERTYPE_WEEK;
		}
		else
		{
			if( 8 == m_nTimerType)m_nTimerType = CMemoData::TIMERTYPE_DAY;
		}
	}
	// Window状態情報
	ar >> m_nIconIndex;					// 表示アイコン
	ar >> blDummy;						// 表示Window 状態
	ar >> m_cRctNormal;					// 通常時のWindow Size
	ar >> m_cRctIcon;					// 最小化時のサイズ
	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		ar >> blDummy;					// 裏返し:1.40
		ar >> blDummy;					// オープン時パスワード:1.40
	}
	// パスワード
	ar >> m_blPassWordLock;				// パスワード設定
	ar >> m_cStrPassWord;				// パスワード
	ar >> blDummy;						// チップヘルプ
	// Ｚオーダー
	ar >> m_nZOrder;						// 常に先頭
	// 関連付け機能のデータ
	ar >> m_cStrParameters;
	ar >> m_cStrFile;
	ar >> m_blTimerExec;
	ar >> m_blLink;

	if( _SCHEMA_ID_TYPE1 < unVersion)
	{
		int	nCreateDate;
		ar >> nCreateDate;					// 有効期限の年
		m_uniCreateDate.m_btCreateYear = nCreateDate;
		ar >> nCreateDate;					// 有効期限の月
		m_uniCreateDate.m_btCreateMonth = nCreateDate;
		ar >> nCreateDate;					// 有効期限の日
		m_uniCreateDate.m_btCreateDay = nCreateDate;
		ar >> nDummy;						// 削除期日

		ar >> m_blSeeThrough;
		ar >> m_unDxlID;						// 描画拡張Dxl ID 1.60以降
		ar >> cStrDummy;
		ar >> cRctDummy;
	}

	// 1.60 later
	if( _SCHEMA_ID_TYPE2 < unVersion)
	{
		ar >> m_unDxlExtDataSize;
		if( 0 < m_unDxlExtDataSize)
		{
			if( m_pabyDxlExtData)
			{
				delete [] m_pabyDxlExtData;
				m_pabyDxlExtData = NULL;
			}
			m_pabyDxlExtData = new BYTE [ m_unDxlExtDataSize];
			ar.Read( m_pabyDxlExtData, m_unDxlExtDataSize);
		}
		ar >> nDummy;
		ar >> nDummy;
		ar >> nDummy;
		ar >> nDummy;
	}
}

void CMemoData::Restore( CArchive& ar, UINT unVersion)
{
	CString	cStrWork;
	CString	cStrDummy;
	int		nDummy;

	try
	{
		if( ar.IsLoading())
		{
			if( _SCHEMA_ID_TYPE3 >= unVersion)
			{
				RestoreFromOldFormat( ar, unVersion);
				return;
			}
			// 読み出し側
			// Window関連
			{
				ar >> m_cRctNormal;
				ar >> m_cRctTitle;
				ar >> m_cRctIcon;
				ar >> m_nWindowType;
				ar >> m_blPosLock;
				ar >> m_blShow;
				ar >> m_nZOrder;

				ar >> m_blSeeThrough;
			}

			ar >> m_cStrTitle;			// タイトル
			// 表示内容
			ar >> cStrWork;
			Decode( m_cStrMemo, cStrWork);

			// 表示スタイル
			// 表示フォント
			BOOL blFont;
			ar >> blFont;
			if( blFont)
			{
				if( NULL == m_pstLogFont)
				{
					m_pstLogFont = new LOGFONT;
				}
				ar.Read( m_pstLogFont, sizeof( LOGFONT));
			}
			ar >> m_clrForeColor;		// 文字色
			ar >> m_clrBackColor;		// 背景色
			ar >> m_nIconIndex;			// アイコン種別

			// 関連付け
			ar >> m_blLink;				// 関連付け有功無効
			// ファイル
			ar >> cStrWork;
			Decode( m_cStrFile, cStrWork);
			// パラメータ
			ar >> cStrWork;
			Decode( m_cStrParameters, cStrWork);
			ar >> m_blTimerExec;		// タイマで起動

			// パスワード関連
			ar >> m_blPassWordLock;		// パスワードの有功無効
			// パスワード
			ar >> cStrWork;
			Decode( m_cStrPassWord, cStrWork);

			// 描画拡張
			ar >> m_unDxlID;			// 描画拡張ID
			ar >> m_unDxlExtDataSize;	// DXLに関する特殊保存データサイズ
			// DXLに関する特殊保存データ
			if( 0 < m_unDxlExtDataSize)
			{
				if( m_pabyDxlExtData)
				{
					delete [] m_pabyDxlExtData;
					m_pabyDxlExtData = NULL;
				}
				m_pabyDxlExtData = new BYTE [ m_unDxlExtDataSize];
				ar.Read( m_pabyDxlExtData, m_unDxlExtDataSize);
			}

			// タイマ
			ar >> m_blTimer;			// タイマ通知有功
			ar >> m_nTimerType;			// タイマ種別
			ar >> m_nNotifyShift;		// 通知日の移動処理
			ar >> m_nMonth;				// 通知月
			ar >> m_nDay;				// 通知日
			ar >> m_nHour;				// 通知時
			ar >> m_nMin;				// 通知分
			ar >> m_nWeek;				// 通知曜日
			ar >> m_nBeforeNotify;		// 事前通知
			ar >> m_blAfterNotify;		// 経過通知
			ar >> m_cStrNotifySound;	// 通知サウンドファイル

			ar >> m_uniCreateDate.m_unCreateDate;		// 作成日付

			// 決議などで使用するＩＤ
			ar >> cStrDummy;
			if( !cStrDummy.IsEmpty())
			{
				SetContextID( cStrDummy);
			}
			// 簡単拡張用・ファイルバージョンを変更しないでの拡張
			ar >> cStrDummy;
			ar >> cStrDummy;
			ar >> nDummy;
			// 透過率
			m_byAlphaBlend = LOBYTE( LOWORD( nDummy));
			ar >> nDummy;
			ar >> nDummy;
			ar >> nDummy;
		}
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}

void CMemoData::Store( CArchive& ar )
{
	CString	cStrWork;
	CString	cStrDummy;
	int		nDummy;

	try
	{
		if( ar.IsStoring())
		{
			// 保存側
			// Window関連
			{
				ar << m_cRctNormal;
				ar << m_cRctTitle;
				ar << m_cRctIcon;
				ar << m_nWindowType;
				ar << m_blPosLock;
				ar << m_blShow;
				ar << m_nZOrder;

				ar << m_blSeeThrough;
			}

			ar << m_cStrTitle;			// タイトル
			// 表示内容
			Encode( cStrWork, m_cStrMemo);
			ar << cStrWork;

			// 表示スタイル
			// 表示フォント
			if( m_pstLogFont)
			{
				ar << BOOL( TRUE);
				ar.Write( m_pstLogFont, sizeof( LOGFONT));
			}
			else
			{
				ar << BOOL( FALSE);
			}
			ar << m_clrForeColor;		// 文字色
			ar << m_clrBackColor;		// 背景色
			ar << m_nIconIndex;			// アイコン種別

			// 関連付け
			ar << m_blLink;				// 関連付け有功無効
			// ファイル
			Encode( cStrWork, m_cStrFile);
			ar << cStrWork;
			// パラメータ
			Encode( cStrWork, m_cStrParameters);
			ar << cStrWork;
			ar << m_blTimerExec;		// タイマで起動

			// パスワード関連
			ar << m_blPassWordLock;		// パスワードの有功無効
			// パスワード
			Encode( cStrWork, m_cStrPassWord);
			ar << cStrWork;

			// 描画拡張
			ar << m_unDxlID;			// 描画拡張ID
			ar << m_unDxlExtDataSize;	// DXLに関する特殊保存データサイズ
			// DXLに関する特殊保存データ
			if( 0 < m_unDxlExtDataSize)
			{
				if( m_pabyDxlExtData)
				{
					ar.Write( m_pabyDxlExtData, m_unDxlExtDataSize);
				}
			}

			// タイマ
			ar << m_blTimer;			// タイマ通知有功
			ar << m_nTimerType;			// タイマ種別
			ar << m_nNotifyShift;		// 通知日の移動処理
			ar << m_nMonth;				// 通知月
			ar << m_nDay;				// 通知日
			ar << m_nHour;				// 通知時
			ar << m_nMin;				// 通知分
			ar << m_nWeek;				// 通知曜日
			ar << m_nBeforeNotify;		// 事前通知
			ar << m_blAfterNotify;		// 経過通知
			ar << m_cStrNotifySound;	// 通知サウンドファイル

			ar << m_uniCreateDate.m_unCreateDate;	// 作成日付

			cStrDummy.Empty();
			// 決議などで使用するＩＤの記録
			if( NULL != m_pcStrContextID)
			{
				ar << *m_pcStrContextID;
			}
			else
			{
				ar << cStrDummy;
			}
			// 簡単拡張用・ファイルバージョンを変更しないでの拡張
			ar << cStrDummy;
			ar << cStrDummy;
			nDummy = MAKELONG( MAKEWORD( m_byAlphaBlend, 0), 0);;
			ar << nDummy;
			nDummy = -1;
			ar << nDummy;
			nDummy = 0;
			ar << nDummy;
			ar << nDummy;
		}
	}
	catch( CFileException* e)
	{
#ifdef	_DEBUG
		afxDump << "FileException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CMemoryException* e)
	{
#ifdef	_DEBUG
		afxDump << "MemoryException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
	catch( CArchiveException* e)
	{
#ifdef	_DEBUG
		afxDump << "ArchiveException " << __FILE__ << " in " << __LINE__ <<"\n!";
#endif
	}
}

// 保存時に、ちょとした「暗号化」モドキを施すために使用
// 暗号化はBASE16とでも言ったところかなぁ……
void CMemoData::Encode( CString& cStrDst, const CString& cStrSrc)
{
	// 1999/03/05 masさんによる高速化のため修正提案による
	int	nLength = cStrSrc.GetLength();

	cStrDst.Empty();
	int		nData;
	char	*pszData = new char[ nLength * 2 + 1];
	ZeroMemory( pszData, nLength * 2 + 1);

	for( int nIndex = 0; nIndex < nLength; nIndex++)
	{
		nData = cStrSrc[ nIndex];
		pszData[ nIndex * 2]		= sm_cStrData[ nData & 0x0F];
		pszData[ nIndex * 2 + 1]	= sm_cStrData[ ( nData >> 4) & 0x0F];
	}

	cStrDst = pszData;
	delete [] pszData;
	/*
	この方式ではアロケーション作業が細かく入る可能性がある
	CString	cStr;
	int nLength = cStrSrc.GetLength();

	cStrDst.Empty();
	int nData;
	for( int nIndex = 0; nIndex < nLength; nIndex++)
	{
		nData = cStrSrc[ nIndex];
		cStrDst += sm_cStrData[ nData & 0x0F];
		cStrDst += sm_cStrData[ ( nData >> 4) & 0x0F];
	}
	*/
}

void CMemoData::Decode( CString& cStrDst, const CString& cStrSrc)
{
	cStrDst.Empty();
	int nLength = cStrSrc.GetLength();
	if( 0 < nLength)
	{
		char*	pszData;
		pszData = new char[ nLength];
		ZeroMemory( pszData, nLength);

		int		nData;
		char	chData;
		for( int nIndex = 0; nIndex < nLength; nIndex += 2)
		{
			nData = sm_cStrData.Find( cStrSrc[ nIndex]);
			chData = nData;
			nData = sm_cStrData.Find( cStrSrc[ nIndex + 1]);
			chData |= ( nData << 4);
			pszData[ nIndex / 2] = chData;
		}

		cStrDst = pszData;
		delete [] pszData;
	}
}

BOOL CMemoData::CopyMember( const CMemoData& cMemoData)
{
	// 基本設定
	{
		m_cStrTitle		= cMemoData.m_cStrTitle;	// タイトル
		m_cStrMemo		= cMemoData.m_cStrMemo;		// 表示内容

		// 表示フォント
		if( NULL != cMemoData.m_pstLogFont)
		{
			if( NULL == m_pstLogFont)
			{
				m_pstLogFont = new LOGFONT;
			}
			*m_pstLogFont = *cMemoData.m_pstLogFont;
		}
		else
		{
			if( m_pstLogFont)
			{
				delete m_pstLogFont;
				m_pstLogFont = NULL;
			}
		}
		m_clrForeColor	= cMemoData.m_clrForeColor;	// 文字色
		m_clrBackColor	= cMemoData.m_clrBackColor;	// 背景色
		m_nIconIndex	= cMemoData.m_nIconIndex;	// アイコン種別
	}

	// 拡張設定
	{
		m_blLink			= cMemoData.m_blLink;			// 関連付け有功無効
		m_cStrFile			= cMemoData.m_cStrFile;			// ファイル
		m_cStrParameters	= cMemoData.m_cStrParameters;	// パラメータ
		m_blTimerExec		= cMemoData.m_blTimerExec;		// タイマで起動

		m_blPassWordLock	= cMemoData.m_blPassWordLock;	// パスワードの有功無効
		m_cStrPassWord		= cMemoData.m_cStrPassWord;		// パスワード

		// 描画拡張
		m_unDxlID			= cMemoData.m_unDxlID;			// Dxl-ID
		// Dxl 拡張データ
		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		if( cMemoData.m_unDxlID)
		{
			m_unDxlExtDataSize	= cMemoData.m_unDxlExtDataSize;	// Dxl 拡張データサイズ
			if( cMemoData.m_unDxlExtDataSize)
			{
				m_pabyDxlExtData = new BYTE[ cMemoData.m_unDxlExtDataSize];
				CopyMemory( m_pabyDxlExtData, cMemoData.m_pabyDxlExtData, cMemoData.m_unDxlExtDataSize);
			}
		}
		else
		{
			m_unDxlExtDataSize = 0;
		}
	}

	// タイマ
	{
		m_blTimer		= cMemoData.m_blTimer;			// タイマ通知有功
		m_nTimerType	= cMemoData.m_nTimerType;		// タイマ種別
		m_nNotifyShift	= cMemoData.m_nNotifyShift;		// 通知日の移動処理
		m_nMonth		= cMemoData.m_nMonth;			// 通知月
		m_nDay			= cMemoData.m_nDay;				// 通知日
		m_nHour			= cMemoData.m_nHour;			// 通知時
		m_nMin			= cMemoData.m_nMin;				// 通知分
		m_nWeek			= cMemoData.m_nWeek;			// 通知曜日

		m_nBeforeNotify	= cMemoData.m_nBeforeNotify;	// 事前通知
		m_blAfterNotify	= cMemoData.m_blAfterNotify;	// 経過通知

		m_cStrNotifySound	= cMemoData.m_cStrNotifySound;	// 通知サウンドファイル
	}

	// Window関連
	{
		m_cRctNormal	= cMemoData.m_cRctNormal;
		m_cRctTitle		= cMemoData.m_cRctTitle;
		m_cRctIcon		= cMemoData.m_cRctIcon;
		m_nWindowType	= cMemoData.m_nWindowType;
		m_blPosLock		= cMemoData.m_blPosLock;
		m_blShow		= cMemoData.m_blShow;
		m_nZOrder		= cMemoData.m_nZOrder;

		m_blSeeThrough	= cMemoData.m_blSeeThrough;

		m_byAlphaBlend	= cMemoData.m_byAlphaBlend;
	}

	// 作成日付記憶
	m_uniCreateDate.m_unCreateDate = cMemoData.m_uniCreateDate.m_unCreateDate;

	return TRUE;
}

void CMemoData::SetTitle( const CString& cStrTitle)
{
	// タイトルは最長６３固定です
	if( _MAX_TITLE_LEN >= cStrTitle.GetLength())
	{
		m_cStrTitle = cStrTitle;
	}
	else
	{
		char	szDummy[ _MAX_TITLE_LEN + 1]; 
		ZeroMemory( szDummy, _MAX_TITLE_LEN + 1);
		for( int nIndex = 0; nIndex < ( _MAX_TITLE_LEN + 1); nIndex++)
		{
			szDummy[ nIndex] = cStrTitle[ nIndex];
			if( _ismbblead( szDummy[ nIndex]))
			{
				if( _ismbbtrail( cStrTitle[ nIndex + 1]))
				{
					nIndex++;
					if( nIndex >= _MAX_TITLE_LEN)
					{
						szDummy[ _MAX_TITLE_LEN] = 0;
						break;
					}
					szDummy[ nIndex] = cStrTitle[ nIndex];
				}
			}
			else
			{
				if( 0 == szDummy[ nIndex])break;
			}
		}
		m_cStrTitle = szDummy;
	}
	m_cStrTitle.Replace( '\t', ' ');
	m_cStrTitle.TrimLeft();
	m_cStrTitle.TrimRight();
}

BOOL CMemoData::GetTitle( CString& cStrTitle) const
{
	// タイトルが無ければ、先頭行を返す
	if( m_cStrTitle.IsEmpty())
	{
		cStrTitle = m_cStrMemo;
		cStrTitle.TrimLeft();
		int nIndex = cStrTitle.Find( "\r\n");
		if( 0 < nIndex)cStrTitle = cStrTitle.Left( nIndex);
		if( _MAX_TITLE_LEN < cStrTitle.GetLength())
		{
			char	szDummy[ _MAX_TITLE_LEN + 1]; 
			ZeroMemory( szDummy, _MAX_TITLE_LEN + 1);
			for( int nIndex = 0; nIndex < ( _MAX_TITLE_LEN + 1); nIndex++)
			{
				szDummy[ nIndex] = cStrTitle[ nIndex];
				if( _ismbblead( szDummy[ nIndex]))
				{
					if( _ismbbtrail( cStrTitle[ nIndex + 1]))
					{
						nIndex++;
						if( nIndex >= _MAX_TITLE_LEN)
						{
							szDummy[ _MAX_TITLE_LEN] = 0;
							break;
						}
						szDummy[ nIndex] = cStrTitle[ nIndex];
					}
				}
				else
				{
					if( 0 == szDummy[ nIndex])break;
				}
			}
			cStrTitle = szDummy;
		}
		cStrTitle.Replace( '\t', ' ');

		/*
		if( nIndex >= 1 && cStrTitle.GetLength() >= nIndex)
		{
			char chChar = cStrTitle[ nIndex - 1];
			if( _ismbblead( chChar))
			{
				if( _ismbbtrail( cStrTitle[ nIndex]))
				{
					nIndex--;
				}
			}
		}
		CString	cStr;
		cStr = cStrTitle.Left( nIndex);
		cStrTitle = cStrTitle.Left( nIndex);
#if	_DEBUG
		int nLen = cStrTitle.GetLength();
		nLen = cStr.GetLength();
		char*	pcszData;
		pcszData = new char [ nLen + 10];
		lstrcpyn( pcszData, cStrTitle, nLen);
		delete [] pcszData;
#endif
		*/
		return FALSE;
	}
	// タイトルは設定されているので、それを返します
	cStrTitle = m_cStrTitle;
	return TRUE;
}

void CMemoData::SetFont( const LOGFONT* pstLogFont)
{
	if( NULL == pstLogFont)
	{
		m_pstLogFont = NULL;
	}
	else
	{
		if( NULL == m_pstLogFont)
		{
			// フォント領域が無ければ、新規確保
			m_pstLogFont = new LOGFONT;
		}
		*m_pstLogFont = *pstLogFont;
	}
}

BOOL CMemoData::GetFont( LOGFONT* const pstLogFont) const
{
	if( NULL == m_pstLogFont)
	{
		// フォントを持っていない場合は、デフォルトを返す
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			pcSOboe->GetDefaultFont( pstLogFont);
		}
		return FALSE;
	}
	*pstLogFont = *m_pstLogFont;
	return TRUE;
}

BOOL CMemoData::GetForeColor( COLORREF& clrFore) const
{
	if( CLR_INVALID == m_clrForeColor)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			clrFore = pcSOboe->GetDefaultForeColor();
		}
		return FALSE;
	}
	clrFore = m_clrForeColor;
	return TRUE;
}

BOOL CMemoData::GetBackColor( COLORREF& clrBack) const
{
	if( CLR_INVALID == m_clrBackColor)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			clrBack = pcSOboe->GetDefaultBackColor();
		}
		return FALSE;
	}
	clrBack = m_clrBackColor;
	return TRUE;
}

int CMemoData::GetIconIndex( void) const
{
	if( 0 > m_nIconIndex)
	{
		CSOboeApp*	pcSOboe = ( CSOboeApp*)AfxGetApp();
		if( pcSOboe)
		{
			return pcSOboe->GetDefaultIconIndex();
		}
	}/*
	if( _MAX_ICONINDEX < m_nIconIndex)
	{
		return _MAX_ICONINDEX + 1;
	}*/
	return m_nIconIndex;
}

BOOL CMemoData::GetLinkData( CString& cStrFile, CString& cStrParameters) const
{
	cStrFile = m_cStrFile;
	cStrParameters = m_cStrParameters;

	BOOL blResult = ( cStrFile.IsEmpty() && cStrParameters.IsEmpty()) ? FALSE : m_blLink;

	return blResult;
}

int CMemoData::SetTimerType( int nType)
{
	int	nResult;

	nResult = m_nTimerType;

	if( TIMERTYPE_EVERYDAY != nType && TIMERTYPE_WEEK != nType && TIMERTYPE_DAY != nType)
	{
		m_nTimerType = TIMERTYPE_EVERYDAY;
	}
	else
	{
		m_nTimerType = nType;
	}

	return nResult;
}

int CMemoData::SetNotifyShift( int nShift)
{
	int	nResult;

	nResult = m_nNotifyShift;

	if( NOTIFYSHIFT_NOMOVE != nShift &&	// 移動無し
		NOTIFYSHIFT_TOSAT != nShift &&	// 土曜日へ
		NOTIFYSHIFT_TOFRI != nShift &&	// 金曜日へ
		NOTIFYSHIFT_TOMON_FROMSATSUN != nShift &&	// 月曜日へ 土曜日／日曜日
		NOTIFYSHIFT_TOMON_FROMSUN != nShift)	// 月曜日へ 日曜日
	{
		m_nNotifyShift = NOTIFYSHIFT_NOMOVE;
	}
	else
	{
		m_nNotifyShift = nShift;
	}

	return nResult;
}

// 通知月
int CMemoData::SetTimerMonth( int nMonth)
{
	int nResult;

	nResult = m_nMonth;

	if( 0 > nMonth || 12 < nMonth)
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		m_nMonth = cTime.GetMonth();
	}
	else
	{
		m_nMonth = nMonth;
	}

	return nResult;
}

// 通知日
int CMemoData::SetTimerDay( int nDay)
{
	int nResult;

	nResult = m_nDay;

	if( 0 >= nDay || 31 < nDay)
	{
		SYSTEMTIME	stSysTime;
		GetLocalTime( &stSysTime);
		COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();
		m_nDay = cTime.GetDay();
	}
	else
	{
		m_nDay = nDay;
	}

	return nResult;
}

// 通知日の内部変換
int CMemoData::ExchangeNotifyDay( const COleDateTime& cTimeNow, const int nGetType) const
{
	COleDateTime	cTimeCheck( cTimeNow.GetYear(), ( ( 0 != m_nMonth) ? m_nMonth : cTimeNow.GetMonth()), m_nDay, cTimeNow.GetHour(), cTimeNow.GetMinute(), cTimeNow.GetSecond());
	int nDayOfWeek = cTimeCheck.GetDayOfWeek();
	switch( m_nNotifyShift)
	{
	case NOTIFYSHIFT_NOMOVE:	// 移動無し
		break;
	case NOTIFYSHIFT_TOSAT:	// 土曜日へ移動
		if( 1 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 1, 0, 0, 0);
		break;
	case NOTIFYSHIFT_TOFRI:	// 金曜日へ移動
		if( 1 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 2, 0, 0, 0);
		if( 7 == nDayOfWeek)cTimeCheck -= COleDateTimeSpan( 1, 0, 0, 0);
		break;
	case NOTIFYSHIFT_TOMON_FROMSATSUN: // 月曜日へ移動：土／日曜日
		if( 7 == nDayOfWeek)cTimeCheck += COleDateTimeSpan( 2, 0, 0, 0);
	case NOTIFYSHIFT_TOMON_FROMSUN:	// 月曜日へ移動：日曜日
		if( 1 == nDayOfWeek)cTimeCheck += COleDateTimeSpan( 1, 0, 0, 0);
		break;
	}

	int nResult;
	if( GET_MONTH == nGetType)
	{
		nResult = ( 0 == m_nMonth) ? 0 : cTimeCheck.GetMonth();
	}
	else
	{
		nResult = cTimeCheck.GetDay();
	}

	return nResult;
}

// 曜日の通知
int CMemoData::SetWeek( int nWeek)
{
	int	nResult;

	nResult = m_nWeek;

	if( WEEK_DISABLE == ( WEEK_ALL & nWeek))
	{
		m_nWeek = WEEK_DISABLE;
	}
	else
	{
		m_nWeek = nWeek;
	}

	return nWeek;
}

BOOL CMemoData::GetNotifySound( CString& cStrNotifySound) const
{
	BOOL	blResult = TRUE;

	cStrNotifySound = m_cStrNotifySound;

	WIN32_FIND_DATA	stFindData;
	HANDLE hHandle = FindFirstFile( cStrNotifySound, &stFindData);
	if( INVALID_HANDLE_VALUE != hHandle)
	{
		blResult = FALSE;
		FindClose( hHandle);
	}
	return blResult;
}

BOOL CMemoData::SetDxlExtData( const BYTE* pabyDxlExtData, const UINT unSize)
{
	try
	{
		if( m_pabyDxlExtData)
		{
			delete [] m_pabyDxlExtData;
			m_pabyDxlExtData = NULL;
		}
		m_unDxlExtDataSize = unSize;
		if( unSize)
		{
			m_pabyDxlExtData = new BYTE [ unSize];
			CopyMemory( m_pabyDxlExtData, pabyDxlExtData, unSize);
		}
		return TRUE;
	}
	catch( CMemoryException* e)
	{
		e->Delete();
		return FALSE;
	}
}

UINT CMemoData::GetDxlExtData( BYTE* pabyDxlExtData, UINT unSize) const
{
	if( NULL == pabyDxlExtData || 0 == unSize)return m_unDxlExtDataSize;

	UINT unCopySize = min( unSize, m_unDxlExtDataSize);
	if( 0 < unCopySize)
	{
		CopyMemory( pabyDxlExtData, m_pabyDxlExtData, unCopySize);
	}
	return unCopySize;
}

// 作成日付
void CMemoData::SetCreateDate( void)
{
	SYSTEMTIME	stSysTime;
	GetLocalTime( &stSysTime);
	COleDateTime cTime( stSysTime);// = COleDateTime::GetCurrentTime();

	m_uniCreateDate.m_btCreateYear = cTime.GetYear();
	m_uniCreateDate.m_btCreateMonth = cTime.GetMonth();
	m_uniCreateDate.m_btCreateDay = cTime.GetDay();
}

void CMemoData::GetCreateDate( int& nYear, int& nMonth, int& nDay) const
{
	nYear = m_uniCreateDate.m_btCreateYear;
	nMonth = m_uniCreateDate.m_btCreateMonth;
	nDay = m_uniCreateDate.m_btCreateDay;
}

int CMemoData::SetWindowType( int nWindowType)
{
	int nResult;

	nResult = m_nWindowType;
	m_nWindowType = nWindowType;

	return nResult;
}

int CMemoData::SetZOrder( int nZOrder)
{
	int nResult;

	nResult = m_nZOrder;
	if( ZORDER_NORMAL != nZOrder && ZORDER_TOPMOST != nZOrder && ZORDER_BOTTOMMOST != nZOrder)
	{
		m_nZOrder = ZORDER_NORMAL;
	}
	else
	{
		m_nZOrder = nZOrder;
	}

	return nResult;
}

CRect CMemoData::SetNormalRect( const CRect& cRctNormal)
{
	CRect cRct;

	cRct = m_cRctNormal;
	m_cRctNormal = cRctNormal;

	return cRct;
}

CRect CMemoData::SetTitleRect( const CRect& cRctTitle)
{
	CRect cRct;

	cRct = m_cRctTitle;
	m_cRctTitle = cRctTitle;

	return cRct;
}

CRect CMemoData::SetIconRect( const CRect& cRctIcon)
{
	CRect cRct;

	cRct = m_cRctIcon;
	m_cRctIcon = cRctIcon;

	return cRct;
}

int	CMemoData::CalcDDDataSize( void)
{
	int	nDDDataSize;

	nDDDataSize = sizeof( DDDATA);

	nDDDataSize += m_cStrTitle.GetLength() + 1;

	nDDDataSize += m_cStrMemo.GetLength() + 1;

	if( m_pstLogFont)
	{
		nDDDataSize += sizeof( LOGFONT);
	}


	nDDDataSize += m_cStrFile.GetLength() + 1;
	nDDDataSize += m_cStrParameters.GetLength() + 1;

	nDDDataSize += m_cStrPassWord.GetLength() + 1;

	if( m_unDxlID)
	{
		nDDDataSize += m_unDxlExtDataSize;
	}

	nDDDataSize += m_cStrNotifySound.GetLength() + 1;

	return nDDDataSize;
}

int CMemoData::GetDDData( DDDATA* pstDDData, int nSize)
{
	int	nResult = CalcDDDataSize();
	if( nSize < nResult)return nResult;

	CString	cStr;
	int		nExtendDataPos = sizeof( DDDATA);
	char*	pWorkPointer = ( char*)pstDDData;

	pstDDData->m_nSize	= nSize;
	lstrcpy( pstDDData->m_szDataFmt, "OboeGakiChipData100");

	pstDDData->m_stRctNormal	= m_cRctNormal;
	pstDDData->m_stRctTitle		= m_cRctTitle;
	pstDDData->m_stRctIcon		= m_cRctIcon;
	pstDDData->m_nWindowType	= m_nWindowType;

	pstDDData->m_blShow			= m_blShow;
	pstDDData->m_nZOrder		= m_nZOrder;
	pstDDData->m_blPosLock		= m_blPosLock;
	pstDDData->m_blSeeThrough	= m_blSeeThrough;

	pstDDData->m_nszTitle = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrTitle);
	nExtendDataPos += m_cStrTitle.GetLength() + 1;

	pstDDData->m_nszMemo = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrMemo);
	nExtendDataPos += m_cStrMemo.GetLength() + 1;

	if( m_pstLogFont)
	{
		pstDDData->m_nstLogFont = nExtendDataPos;
		CopyMemory( &pWorkPointer[ nExtendDataPos], m_pstLogFont, sizeof( LOGFONT));
		nExtendDataPos += sizeof( LOGFONT);
	}
	else
	{
		pstDDData->m_nstLogFont = -1;
	}
	pstDDData->m_clrForeColor = m_clrForeColor;
	pstDDData->m_clrBackColor = m_clrBackColor;
	pstDDData->m_nIconIndex = m_nIconIndex;


	pstDDData->m_blLink = m_blLink;
	pstDDData->m_nszFile = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrFile);
	nExtendDataPos += m_cStrFile.GetLength() + 1;

	pstDDData->m_nszParam = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrParameters);
	nExtendDataPos += m_cStrParameters.GetLength() + 1;

	pstDDData->m_blTimerExec = m_blTimerExec;

	pstDDData->m_blPassWordLock = m_blPassWordLock;

	pstDDData->m_nszPassWord = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrPassWord);
	nExtendDataPos += m_cStrPassWord.GetLength() + 1;

	pstDDData->m_unDxlID = m_unDxlID;
	pstDDData->m_unDxlExtDataSize = m_unDxlExtDataSize;
	if( 0 < m_unDxlExtDataSize)
	{
		pstDDData->m_nabyDxlExtData = nExtendDataPos;
		CopyMemory( &pWorkPointer[ nExtendDataPos], m_pabyDxlExtData, m_unDxlExtDataSize);
		nExtendDataPos += pstDDData->m_unDxlExtDataSize;
	}
	else
	{
		pstDDData->m_nabyDxlExtData = -1;
	}


	pstDDData->m_blTimer		= m_blTimer;			// タイマ通知有功
	pstDDData->m_nTimerType		= m_nTimerType;			// タイマ種別
	pstDDData->m_nNotifyShift	= m_nNotifyShift;		// 通知日の移動処理
	pstDDData->m_nMonth			= m_nMonth;				// 通知月
	pstDDData->m_nDay			= m_nDay;				// 通知日
	pstDDData->m_nHour			= m_nHour;				// 通知時
	pstDDData->m_nMin			= m_nMin;				// 通知分
	pstDDData->m_nWeek			= m_nWeek;				// 通知曜日
	pstDDData->m_nBeforeNotify	= m_nBeforeNotify;		// 事前通知
	pstDDData->m_blAfterNotify	= m_blAfterNotify;		// 経過通知

	pstDDData->m_nszNotifySound = nExtendDataPos;
	lstrcpy( ( LPSTR)&pWorkPointer[ nExtendDataPos], m_cStrNotifySound);
	nExtendDataPos += m_cStrNotifySound.GetLength() + 1;

	pstDDData->m_nCreateYear	= m_uniCreateDate.m_btCreateYear;
	pstDDData->m_nCreateMonth	= m_uniCreateDate.m_btCreateMonth;
	pstDDData->m_nCreateDay		= m_uniCreateDate.m_btCreateDay;

	return nResult;
}

CMemoData* CMemoData::CreateMemoFromBinData( BYTE* pabyData, int nSize)
{
	DDDATA*	pstDDData = ( DDDATA*)pabyData;

	if( nSize < pstDDData->m_nSize)return NULL;
	if( lstrcmp( pstDDData->m_szDataFmt, "OboeGakiChipData100"))return NULL;
	if( 0 > pstDDData->m_nszMemo)return NULL;

	CMemoData*	pcMemoData;
	pcMemoData = new CMemoData;

	pcMemoData->m_cRctNormal	= pstDDData->m_stRctNormal;
	pcMemoData->m_cRctTitle		= pstDDData->m_stRctTitle;
	pcMemoData->m_cRctIcon		= pstDDData->m_stRctIcon;
	pcMemoData->m_nWindowType	= pstDDData->m_nWindowType;

	pcMemoData->m_blShow		= pstDDData->m_blShow;
	pcMemoData->m_nZOrder		= pstDDData->m_nZOrder;
	pcMemoData->m_blPosLock		= pstDDData->m_blPosLock;
	pcMemoData->m_blSeeThrough	= pstDDData->m_blSeeThrough;

	CString	cStr;
	if( 0 <= pstDDData->m_nszTitle)
	{
		pcMemoData->m_cStrTitle = ( LPCSTR)&pabyData[ pstDDData->m_nszTitle];
	}
	if( 0 > pstDDData->m_nszMemo)return NULL;
	pcMemoData->m_cStrMemo = ( LPCSTR)&pabyData[ pstDDData->m_nszMemo];


	if( 0 <= pstDDData->m_nstLogFont)
	{
		if( pcMemoData->m_pstLogFont)
		{
			delete pcMemoData->m_pstLogFont;
			pcMemoData->m_pstLogFont = NULL;
		}
		pcMemoData->m_pstLogFont = new LOGFONT;
		CopyMemory( pcMemoData->m_pstLogFont, &pabyData[ pstDDData->m_nstLogFont], sizeof( LOGFONT));
	}
	pcMemoData->m_clrForeColor	= pstDDData->m_clrForeColor;
	pcMemoData->m_clrBackColor	= pstDDData->m_clrBackColor;
	pcMemoData->m_nIconIndex	= pstDDData->m_nIconIndex;


	if( 0 <= pstDDData->m_nszFile)
	{
		pcMemoData->m_cStrFile = ( LPCSTR)&pabyData[ pstDDData->m_nszFile];
	}
	if( 0 <= pstDDData->m_nszParam)
	{
		pcMemoData->m_cStrParameters = ( LPCSTR)&pabyData[ pstDDData->m_nszParam];
	}
	pcMemoData->m_blLink		= ( pcMemoData->m_cStrFile.IsEmpty() && pcMemoData->m_cStrParameters.IsEmpty() && pstDDData->m_blLink) ? FALSE : TRUE;
	pcMemoData->m_blTimerExec	= pstDDData->m_blTimerExec;

	if( 0 <= pstDDData->m_nszPassWord)
	{
		pcMemoData->m_cStrPassWord = ( LPCSTR)&pabyData[ pstDDData->m_nszPassWord];
	}
	pcMemoData->m_blPassWordLock = pcMemoData->m_cStrPassWord.IsEmpty() ? FALSE : pstDDData->m_blPassWordLock;

	pcMemoData->m_unDxlID	= pstDDData->m_unDxlID;
	if( pstDDData->m_unDxlID)
	{
		LPBYTE lpbyData = ( 0 <= pstDDData->m_nabyDxlExtData && 0 < pstDDData->m_unDxlExtDataSize) ? &pabyData[ pstDDData->m_nabyDxlExtData] : NULL;
		pcMemoData->SetDxlExtData( lpbyData, pstDDData->m_unDxlExtDataSize);
	}

	pcMemoData->m_blTimer		= pstDDData->m_blTimer;			// タイマ通知有功
	pcMemoData->m_nTimerType	= pstDDData->m_nTimerType;			// タイマ種別
	pcMemoData->m_nNotifyShift	= pstDDData->m_nNotifyShift;		// 通知日の移動処理
	pcMemoData->m_nMonth		= pstDDData->m_nMonth;				// 通知月
	pcMemoData->m_nDay			= pstDDData->m_nDay;				// 通知日
	pcMemoData->m_nHour			= pstDDData->m_nHour;				// 通知時
	pcMemoData->m_nMin			= pstDDData->m_nMin;				// 通知分
	pcMemoData->m_nWeek			= pstDDData->m_nWeek;				// 通知曜日
	pcMemoData->m_nBeforeNotify	= pstDDData->m_nBeforeNotify;		// 事前通知
	pcMemoData->m_blAfterNotify	= pstDDData->m_blAfterNotify;		// 経過通知
	// 通知サウンドファイル
	if( 0 <= pstDDData->m_nszNotifySound)
	{
		pcMemoData->m_cStrNotifySound = ( LPCSTR)&pabyData[ pstDDData->m_nszNotifySound];
	}

//	cMemoData.SetCreateDate( pstDDData->m_nCreateYear, pstDDData->m_nCreateMonth, pstDDData->m_nCreateDay);
	pcMemoData->SetCreateDate();

	return pcMemoData;
}

void CMemoData::SetContextID( LPCSTR lpcszID)
{
	if( NULL == lpcszID)
	{
		if( m_pcStrContextID)
		{
			delete m_pcStrContextID;
			m_pcStrContextID = NULL;
		}
	}
	else
	{
		if( NULL == m_pcStrContextID)
		{
			m_pcStrContextID = new CString;
		}
		*m_pcStrContextID = lpcszID;
	}
}
