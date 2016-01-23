#include "stdafx.h"
#include "SOboe.h"
#include "OleDrop.h"
#include "MemoChip.h"

BOOL COleDrop::OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	if( !m_blDropEnable)return 0;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoChip)))return 0;

	CMemoChip* pcMemoChip = (CMemoChip*)pWnd;
	return pcMemoChip->OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT COleDrop::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if( !m_blDropEnable)return DROPEFFECT_NONE;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoChip)))return DROPEFFECT_NONE;

	CMemoChip* pcMemoChip = (CMemoChip*)pWnd;
	return pcMemoChip->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT COleDrop::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if( !m_blDropEnable)return DROPEFFECT_NONE;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoChip)))return DROPEFFECT_NONE;

	CMemoChip* pcMemoChip = (CMemoChip*)pWnd;
	return pcMemoChip->OnDragEnter(pDataObject, dwKeyState, point);
}

