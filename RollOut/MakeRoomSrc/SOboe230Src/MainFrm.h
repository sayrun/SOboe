// MainFrm.h : CMainFrame �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__386BA431_26B0_11D2_90A3_00804C15184E__INCLUDED_)
#define AFX_MAINFRM_H__386BA431_26B0_11D2_90A3_00804C15184E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MemoChipList.h"
#include "NetExLib.h"
#include "CatalogList.h"

#define	TASKTRAY_ID			7126
#define	TIMER_ID			7127
#define	TIMER_COUNT			2
#define	TIMER_INTERVAL		500

#define	RENOTIFY_COUNT		( 5 * 60) * ( 1000 / TIMER_INTERVAL)

#define	_MAX_TEMPLTE		10

#define	CATALOG_GROUP_COUNT	20
#define	CATALOG_COUNT		200
#define	CATALOG_TOP			1000
#define	CATALOG_BOTTOM		( CATALOG_TOP + CATALOG_COUNT)

#define	_DECRIMENTSPAN		60

typedef struct tagRECEIVESOURCE
{
	int		nEntryDataSize;
	int		nOffsetEntryData;
	int		nReceiveDataSize;
	int		nOffsetReceiveData;
}RECEIVESOURCE;

struct CREATESOURCE
{
	DWORD dwSize;
	BOOL blCalcRect;
	BOOL blDelOnQuit;
	BOOL blEdit;
	DWORD dwDataSize;
	BYTE lpabyData[ 1];
};

class CMainFrame : public CFrameWnd
{
protected: // �V���A���C�Y�@�\�݂̂���쐬���܂��B
	DECLARE_DYNCREATE(CMainFrame)

// �A�g���r���[�g
public:

// �I�y���[�V����
public:
	CMainFrame();

	BOOL CreateChipFromMemoData( const CMemoData& cMemoData, BOOL blCalcRect = TRUE, BOOL blDelOnQuit = FALSE);
	BOOL EditAndCreate( const CMemoData& cMemo);

	BOOL ImportChip( const CString& cStr);

	enum
	{
		NMFM_DELSAVELOG,		// lParam��ID
		NMFM_DELSAVE,			// lParam��ID
		NMFM_DELONLY,			// lParam��ID
		NMFM_DELLOG,			// lParam��ID
		NMFM_CREATE,			// lParam��CMemoData*
		NMFM_CREATEEDIT,		// lParam��CMemoData*
		NMFM_CHECKEDRECEIVE,	// lParam��ID
		NMFM_SENDREPLY,			// lParam��ID
		NMFM_RECEIVE,			// lParam��SENDDATA*orRESULTDATA*��HGLOBAL
		NMFM_SAVEDATAS,			//
		NMFM_CREATE_BIN,		// lParam��CREATESRC��HGLOBAL�ō폜�̓��b�Z�[�W��M��
#ifdef	_KEYCONTROL_ENABLE_
		NMFM_PREVCHIP,			// lParam��ID
		NMFM_NEXTCHIP			// lParam��ID
#endif
	};
	static UINT GetNotifyMsgFromChip( void)
	{
		return sm_unMsgFromMemo;
	};

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMainFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// �I�y���[�V����
protected:
	// �^�X�N�g���C�̃A�C�R��
	void SetTrayIcon( void);
	void DelTrayIcon( void);

	BOOL BackupDataFile( void);
	BOOL SaveMemoDatas( void);
	BOOL LoadMemoDatas( const CString& cStrLoadFile, BOOL blImport = FALSE);


	BOOL AddMemoChip( CMemoChip* pcMemoChip, BOOL blTaskTrayUpdate = TRUE);
	BOOL LoggingMemoChip( UINT unWndID);
	BOOL SaveMemoChipForDel( UINT unWndID);
	BOOL StockMemoData( const CMemoChip& cMemoChip);
	BOOL DelMemoChip( UINT unDelWndID);
	BOOL SendReply( UINT unReplyEndID);
	BOOL EditAndSend( const CMemoData& cMemo, int nActivePage = 0);

	BOOL ChangeFocus( UINT unDelWndID, BOOL blPrev);

	void CheckRenotify();


	BOOL QuitProcess( void);

	enum
	{
		ALL_NORMAL,
		ALL_ICON,
		ALL_TITLE
	};
	void ResizeAllMemoChip( int nType);
	void AllRestoreOnceHide( void);

	BOOL ReceiveMemoData( INNERSENDDATA& stInnerData, const CEntryData* pcEntryData);
	BOOL ReceiveResult( INNERRESULTDATA& stInnerResult);

	void ImeClose( void);

	// ����
	typedef	BOOL ( *ARRANGECHIPFUNC)( UINT, int, LPVOID/*ARRANGESTRUCT**/, int);
	typedef	BOOL ( *ARRANGEICONFUNC)( UINT, int, LPVOID/*ARRANGESTRUCT**/, LPSIZE, int);
	typedef	BOOL ( *ARRANGETITLEFUNC)( UINT, int, LPVOID/*ARRANGESTRUCT**/, int);
	typedef	BOOL ( *ARRANGESTRUCTVERSION)( LPVOID/*SPECIFICATION**/);

	enum
	{
		ARG_NORMAL,
		ARG_TITLE,
		ARG_ICON,
		ARG_ALL
	};
	BOOL ArrangeChip( UINT unVersion, LPVOID lpvArgFunc, int nCmd);

	HMENU ListUp( const CCatalogList* pcCatalogList, int nIndex);

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	afx_msg long OnPowerBroadcast( WPARAM wParam, LPARAM lParam);
	afx_msg long OnHotKey( WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMainFrame)
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNewmemo();
	afx_msg void OnDatalist();
	afx_msg void OnPaste();
	afx_msg void OnUpdatePaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNewmemo(CCmdUI* pCmdUI);
	afx_msg void OnAppAbout();
	afx_msg void OnAppExit();
	afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	afx_msg void OnAllIcon();
	afx_msg void OnUpdatePopups(CCmdUI* pCmdUI);
	afx_msg void OnAllNormal();
	afx_msg void OnAllOncehide();
	afx_msg void OnAllTop();
	afx_msg void OnAllTitle();
	afx_msg void OnMemoimport();
	afx_msg void OnDellist();
	afx_msg void OnUpdateDellist(CCmdUI* pCmdUI);
	afx_msg void OnSendDirect();
	afx_msg void OnUpdateSendDirect(CCmdUI* pCmdUI);
	afx_msg void OnArrangeIcon();
	afx_msg void OnUpdateArrangeIcon(CCmdUI* pCmdUI);
	afx_msg void OnArrangeNormal();
	afx_msg void OnUpdateArrangeNormal(CCmdUI* pCmdUI);
	afx_msg void OnClientreserch();
	afx_msg void OnUpdateClientreserch(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnNonpopup();
	afx_msg void OnUpdateNonpopup(CCmdUI* pCmdUI);
	afx_msg void OnCustom();
	afx_msg void OnUpdateCustom(CCmdUI* pCmdUI);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnAbsence();
	afx_msg void OnUpdateAbsence(CCmdUI* pCmdUI);
	afx_msg void OnHelp();
	afx_msg void OnSavenow();
	afx_msg void OnStopnxl();
	afx_msg void OnStartnxl();
	afx_msg void OnUpdateStopnxl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartnxl(CCmdUI* pCmdUI);
	afx_msg void OnExedit();
	afx_msg void OnUpdateExedit(CCmdUI* pCmdUI);
	afx_msg void OnBigbutton();
	afx_msg void OnUpdateBigbutton(CCmdUI* pCmdUI);
	afx_msg void OnColorprint();
	afx_msg void OnUpdateColorprint(CCmdUI* pCmdUI);
	afx_msg void OnArrangeTitle();
	afx_msg void OnUpdateArrangeTitle(CCmdUI* pCmdUI);
	afx_msg void OnArrangeAll();
	afx_msg void OnUpdateArrangeAll(CCmdUI* pCmdUI);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnUpdateexticons();
	afx_msg void OnUpdateUpdateexticons(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LONG OnMsgFromTaskTray( UINT wParam, LPARAM lParam);
	afx_msg LONG OnMsgFromMemoChip( UINT wParam, LPARAM lParam);
	afx_msg LONG OnMsgFromMemoExplorer( UINT wParam, LPARAM lParam);
	afx_msg void OnTempletMenu( UINT nID);
	afx_msg void OnUpdateTempletMenu( CCmdUI* pCmdUI);
	afx_msg void OnSendTempletMenu( UINT nID);
	afx_msg void OnCatalogMenu( UINT nID);
	afx_msg void OnUpdateCatalogMenu( CCmdUI* pCmdUI);
	afx_msg void OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	DECLARE_MESSAGE_MAP()
protected:
	CMemoChipList	m_cLstMemoChip;
	// �^�X�N�g���C�̃A�C�R���V���[�Y
	HICON	m_hIconHide;
	HICON	m_hIconNormal;
	HICON	m_hIconReceive;
	HICON	m_hIconAbsence;

	int		m_nLastNotifyDay;	// �Ō�̒ʒm��
	int		m_nTimerCount;		// Dxl�^�C�}�Ƃ̍�

	// �^�X�N�g���C����̒ʒm���b�Z�[�W
	static UINT	sm_unMsgFromTray;

	UINT	m_unMemoChipID;		// ���Ђ̑������̂h�c

	// ���Ђ���̒ʒm���b�Z�[�W
	static UINT	sm_unMsgFromMemo;

	// explorer�̑h���Ή�
	static UINT sm_unMsgFromExplorer;

	bool	m_blAboutDisplaied; //About��ʗp�v���e�N�g
	bool	m_blAllOnceHide;	// �S�Ă̈ꎞ�\��

	UINT	m_uModifiers;
	UINT	m_uVirtKey;

	int		m_nResumeDelay;

	int		m_nReceiveChip;

	BOOL	m_blAbsence;

	CToolTipCtrl*	m_pcToolTip;

	bool	m_blLoadedMemoData;

	bool	m_blStopedNetworkByApmSuspend;

	int		m_nDecrimentCount;

	int m_nRenotifyCount;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MAINFRM_H__386BA431_26B0_11D2_90A3_00804C15184E__INCLUDED_)
