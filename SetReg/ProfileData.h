// ProfileData.h: CProfileData クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROFILEDATA_H__38AFB2BC_7C6A_11D2_91F8_004005469C82__INCLUDED_)
#define AFX_PROFILEDATA_H__38AFB2BC_7C6A_11D2_91F8_004005469C82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProfileData  
{
public:
	CProfileData();
	virtual ~CProfileData();

	BOOL LoadProfile( void);
	BOOL SaveProfile( void);

	enum
	{
		DBLCLKFUNC_EDIT,
		DBLCLKFUNC_LINKEXEC,
		DBLCLKFUNC_ICON,
		DBLCLKFUNC_SEND
	};

protected:
	BOOL InitializeProfile( void);
	BOOL GetProfileName( CString& cStrProfile, BOOL blMasterFile = FALSE);
	BOOL IsMultiUserMode( void);

public:
	CString		m_cStrUserName;
	CString		m_cStrCompName;


	CString		m_cStrSaveFilePath;
	CString		m_cStrStockFilePath;

	LOGFONT		m_stLogFont;
	COLORREF	m_clrForeColor;
	COLORREF	m_clrBackColor;
	int			m_nIconIndex;

	COLORREF	m_clrAfterFore;
	COLORREF	m_clrAfterBack;
	COLORREF	m_clrBeforeFore;
	COLORREF	m_clrBeforeBack;

	BOOL		m_blAutoResize;
	BOOL		m_blAutoTriming;
	BOOL		m_blAutoIMEOpen;
	BOOL		m_blFlatMenu;
	BOOL		m_blHideStart;
	BOOL		m_blHideback;
	BOOL		m_blNoCheckDelete;

	BOOL		m_blLoggingDelChip;
	BOOL		m_blLoggingSendChip;
	BOOL		m_blFullLoggingSendChip;
	BOOL		m_blLoggingReceiveChip;
	BOOL		m_blFullLoggingReceiveChip;

	BOOL		m_blNotifyVersion;

	BOOL		m_blPopupReceive;
	BOOL		m_blHideReceive;
	BOOL		m_blSubNotify;
	BOOL		m_blCheckDelOnQuit;
	BOOL		m_blDeleteAfterSend;
	BOOL		m_blDelOnQuit2Rcv;

	BOOL		m_blAppendSignature;
	BOOL		m_blAppendTime;
	BOOL		m_nAppendPos;
	
	BOOL		m_blPackSend;
	BOOL		m_blNotifyOpen;

	CString		m_cStrSignature;

	int			m_nLDblClkFunc;
	int			m_nMDblClkFunc;

	int			m_nDataListFilter;
	BOOL		m_blDataListReverse;
	BOOL		m_blDelListReverse;

	DWORD		m_dwHotKey;

	int			m_nResumeDelay;

	CString		m_cStrArrangeNormalDLL;
	CString		m_cStrArrangeTitleDLL;
	CString		m_cStrArrangeIconDLL;
	CString		m_cStrArrangeAllDLL;

	CString		m_cStrDxlPath[ _DRAWEXLIB_MAX];
	CString		m_cStrDxlMenuString[ _DRAWEXLIB_MAX];
	UINT		m_unDefaultDxlID;

	BOOL		m_blEnableNetwork;
	CString		m_cStrNxlPath[ _NETEXLIB_MAX];
	int			m_nDefaultNxl;

	CString		m_cStrCelPath[ _CEL_MAX];
	CString		m_cStrCelMenuString[ _CEL_MAX];
	CString		m_cStrMenuSeparator;

	CString		m_cStrGlobalPassWord;

	BOOL		m_blReceptionLink;
	BOOL		m_blReceptionTimer;
	BOOL		m_blReceptionDxl;

	BOOL		m_blEnableTemplet;
	CString		m_cStrTempletFile;

	BOOL		m_blCompatibleOU;

	COLORREF	m_clrCustomColor[ _USERCOLOR_MAX];

	int			m_nGenerationCount;

	BOOL		m_blReceiveAndSave;
	BOOL		m_blModifyAndSave;
};

#endif // !defined(AFX_PROFILEDATA_H__38AFB2BC_7C6A_11D2_91F8_004005469C82__INCLUDED_)
