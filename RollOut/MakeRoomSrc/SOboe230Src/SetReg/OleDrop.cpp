#include "stdafx.h"
#include "OleDrop.h"
#include "MemoListBox.h"

BOOL COleDrop::OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point )
{
	if( !m_blDropEnable)return 0;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoListBox)))return 0;

	CMemoListBox* pcMemoListBox = (CMemoListBox*)pWnd;
	return pcMemoListBox->OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT COleDrop::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if( !m_blDropEnable)return DROPEFFECT_NONE;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoListBox)))return DROPEFFECT_NONE;

	CMemoListBox* pcMemoListBox = (CMemoListBox*)pWnd;
	return pcMemoListBox->OnDragEnter(pDataObject, dwKeyState, point);
}

DROPEFFECT COleDrop::OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	if( !m_blDropEnable)return DROPEFFECT_NONE;
	if (!pWnd->IsKindOf(RUNTIME_CLASS(CMemoListBox)))return DROPEFFECT_NONE;

	CMemoListBox* pcMemoListBox = (CMemoListBox*)pWnd;
	return pcMemoListBox->OnDragEnter(pDataObject, dwKeyState, point);
}

