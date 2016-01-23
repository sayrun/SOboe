#ifndef	__OLEDROP_H__
#define	__OLEDROP_H__

class COleDrop : public COleDropTarget
{
public:
	COleDrop() : COleDropTarget()
	{
		m_blDropEnable = TRUE;
	};

	BOOL DropEnable( BOOL blEnable)
	{
		BOOL blResult = m_blDropEnable;
		m_blDropEnable = blEnable;

		return blResult;
	}

	BOOL OnDrop( CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point );
	DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
	DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point );
protected:
	BOOL	m_blDropEnable;
};
#endif	//__OLEDROP_H__
