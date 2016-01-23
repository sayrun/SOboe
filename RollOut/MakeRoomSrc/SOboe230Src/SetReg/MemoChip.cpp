// MemoChip.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MemoChip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoChip
IMPLEMENT_SERIAL( CMemoChip, CCmdTarget/*CWnd*/, VERSIONABLE_SCHEMA | _SCHEMA_ID)

CMemoChip::CMemoChip()
{
	// 移動に関するフラグ
}

CMemoChip::~CMemoChip()
{
}

const CMemoChip& CMemoChip::operator=( const CMemoChip& cMemoChip)
{
	m_cMemoData		= cMemoChip.m_cMemoData;

	return *this;
}

void CMemoChip::Serialize( CArchive& ar )
{
	try
	{
		// なぜにここにデータ読み出し機構があるかといえば、互換性のためです
		// 旧バージョンはこのクラスも永続性のデータを保持していたのです。
		// 面倒なので、永続性の全てはCMemoDataへ移行しました。
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

BEGIN_MESSAGE_MAP(CMemoChip, CWnd)
	//{{AFX_MSG_MAP(CMemoChip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoChip メッセージ ハンドラ
