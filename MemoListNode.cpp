#include "stdafx.h"
#include "resource.h"
#include "MemoData.h"
#include "MemoListNode.h"

CMemoListNode::CMemoListNode( const CMemoChip* pcMemo, int nDataType)
{
	m_pabyDxlSaveData= NULL;
	if( pcMemo)
	{
		SetData( pcMemo, nDataType);
	}
}

CMemoListNode::~CMemoListNode()
{
	if( m_pabyDxlSaveData)
	{
		delete [] m_pabyDxlSaveData;
		m_pabyDxlSaveData = NULL;
	}
}

BOOL CMemoListNode::SetData( const CMemoChip* pcMemo, int nDataType)
{
	if( NULL == pcMemo)return FALSE;

	CMemoData cMemoData;
	pcMemo->GetMemoData( cMemoData);

	cMemoData.GetTitle( m_cStrTitle);
	m_cStrMemo = cMemoData.GetMemo();
	int nYear;
	int nMonth;
	int nDay;
	cMemoData.GetCreateDate( nYear, nMonth, nDay);
	m_cStrDate.Format( ( DATA_DISPLAY == nDataType) ? IDS_MEMOCREATEDAY : IDS_MEMOSTOCKDAY, nYear, nMonth, nDay);
	m_nIconIndex = cMemoData.GetIconIndex();
	cMemoData.GetForeColor( m_clrFore);
	cMemoData.GetBackColor( m_clrBack);
	cMemoData.GetFont( &m_stLogFont);
	m_blLink = ( FALSE != cMemoData.IsLink());
	m_blSeeThrough = ( FALSE != cMemoData.IsSeeThrough());
	m_blPassWordLock = ( FALSE != cMemoData.IsPassWordLock());
	m_blShow = ( FALSE != cMemoData.IsShow());

	m_unDxlID = cMemoData.GetDxlID();
	if( 0 != m_unDxlID)
	{
		m_nDxlSaveDataSize = cMemoData.GetDxlExtData();
		if( m_nDxlSaveDataSize)
		{
			m_pabyDxlSaveData = new BYTE [ m_nDxlSaveDataSize];
			cMemoData.GetDxlExtData( m_pabyDxlSaveData, m_nDxlSaveDataSize);
		}
	}

	m_hWnd = pcMemo->GetSafeHwnd();

	m_blDelOnQuit = ( TRUE == pcMemo->IsDelOnQuit());

	return TRUE;
}

BOOL CMemoListNode::GetData( CMemoData& cMemoData)
{
	if( m_blPassWordLock)
	{
		CString	cStr;
		cStr.LoadString( IDS_DONOTVIEW);
		cMemoData.SetMemo( cStr);
	}
	else
	{
		cMemoData.SetMemo( m_cStrMemo);
	}
	cMemoData.SetTitle( m_cStrTitle);
	cMemoData.SetForeColor( m_clrFore);
	cMemoData.SetBackColor( m_clrBack);
	cMemoData.SetFont( &m_stLogFont);
	cMemoData.SetDxlID( m_unDxlID);
	if( 0 != m_unDxlID && 0 < m_nDxlSaveDataSize)
	{
		cMemoData.SetDxlExtData( m_pabyDxlSaveData, m_nDxlSaveDataSize);
	}

	return TRUE;
}
