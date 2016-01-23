#ifndef	__PRINTERDC_H__
#define	__PRINTERDC_H__

class CPrinterDC : public CDC
{
public:
	CPrinterDC( void);

	BOOL Attach( HDC hDC, const RECT& stRect);

	BOOL PrintForm( void);
	BOOL PrintChip( const CMemoData* pcMemoData);
	BOOL PrintChip( const CString& cStrTitle, const CString& cStrMemo, COLORREF clrFore, COLORREF clrBack, const CString& cStrFontFace);

protected:
	CSize	m_cPaperSize;
	CRect	m_cPaperRect;
	BOOL	m_blAttach;
	int		m_nCurrentPos;
	int		m_nCenterPos;
	BOOL	m_blColor;

	CRect	m_cRctPage;
	int		m_nPage;
};

#endif	//__PRINTERDC_H__