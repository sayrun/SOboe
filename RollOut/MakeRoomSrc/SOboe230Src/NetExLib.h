#ifndef	__NETEXLIB_H__
#define	__NETEXLIB_H__

#include "NetEx_IF.h"
#include "MemoData.h"
#include "GroupEntry.h"

typedef struct tagINNERRESULTS
{
	CString		m_cStrResult1;
	CString		m_cStrResult2;
	BOOL		m_blOtherResult;
		CString		m_cStrResult3;
}INNERRESULTS;

typedef	struct tagINNERSENDDATA
{
	CString		m_cStrFrom;
	CString		m_cStrDistribute;

	BOOL		m_blAppendSig;
	int			m_nAppendPos;
	CString		m_cStrSignature;

	BOOL		m_blAppendSendTime;

	BOOL		m_blSendResize;
	BOOL		m_blSendCenter;

	BOOL		m_blPack;
		BOOL		m_blPassWord;
		BOOL		m_blCheckOpen;
	BOOL		m_blChackResult;
		CString			m_cStrResultID;
		INNERRESULTS	m_stResults;

	BOOL		m_blDelOnQuit;

	int			m_nCreateYear;
	int			m_nCreateMonth;
	int			m_nCreateDay;

	UINT		m_unVersion;
	CString		m_cStrVersion;

	CString		m_cStrDxlDummy;

	CMemoData	m_cMemoData;
}INNERSENDDATA;

typedef	struct tagINNERRESULTDATA
{
	CString			m_cStrSignature;
	CString			m_cStrChipID;
	CString			m_cStrResult;
}INNERRESULTDATA;

class CNetExLib : public CObject
{
public:
	CNetExLib();
	~CNetExLib();

	BOOL SetPath( LPCSTR lpcszPath);
	BOOL LoadNxl( void);
	BOOL FreeNxl( void);

	const CString& GetLibName( void) const;
	BOOL IsSupportGrouping( void) const;
	BOOL IsSupportSend( void) const;
	BOOL IsSupportOriginalImage( void) const;
	BOOL IsSupportExtData( void) const;
	BOOL IsSupportInputEntry( void) const;
	BOOL IsSupportBulkSend( void) const;

	BOOL IsAssignFunction( void) const;

	BOOL IsInitialied( void) const;

	UINT GetNxlID( void) const;

	BOOL About( HWND hWnd);

	BOOL Initialize( HWND hWnd, LPCSTR lpcszEntryName);
	BOOL UnInitialize( HWND hWnd);

	BOOL IsNxlActive( void) const;
	BOOL IsReadyReceive( void) const;
	BOOL IsReadySend( void) const;

	BOOL StartEntry( BOOL blAbsence = FALSE);
	BOOL AbsenceEntry( BOOL blAbsence);
	BOOL ExitEntry();

	BOOL LockEntryList();
	BOOL UnlockEntryList();
	int GetGroupCount();
	BOOL GetGroupData( int nIndex, CGroupData& cGroupData);
	int GetEntryCount();
	BOOL GetEntryData( int nIndex, CEntryData& cEntryData);
	BOOL CompareEntryData( const CEntryData& cEntryDataDst, const CEntryData& cEntryDataSrc);
	void GetEntryInfo( CString& cStrInfo, const CEntryData& cEntryDataDst);

	BOOL DoInputEntry( HWND hWnd, CEntryData& cEntryData);

	BOOL GetEntryImage( CSize& cSize, HBITMAP& hBitmal, COLORREF& clrMask);

	int SendMemoData( const CEntryData* pcEntry, const INNERSENDDATA* pstInnerSendData, HANDLE hCancelEvent);
	int SendMemoDataInBulk( const CList < CEntryData*, CEntryData*&>& cEntryList, int* panResult, int nCount, const INNERSENDDATA* pstInnerSendData, HANDLE hCancelEvent);
	int SendResultData( const CEntryData* pcEntry, const INNERRESULTDATA* pstInnerResultData, HANDLE hCancelEvent);

	BOOL GetErrorMsg( int nCode, CString& cStrErrorMsg);

	static BOOL ExcangeReceiveData( INNERSENDDATA* pstInnerSendData, const SENDDATA* pstSendData, int nSendDataSize);
	static BOOL ExcangeReceiveResultData( INNERRESULTDATA* pstInnerResultData, const RESULTDATA* pstResultData, int pstResultDataSize);

protected:
	int ExchangeSendData( SENDDATA* pstSendData, int nSendDataSize, const INNERSENDDATA* pstInnerSendData);
	int ExchangeSendResultData( RESULTDATA* pstResultData, int nResultDataSize, const INNERRESULTDATA* pstInnerResultData);
	int CalcSendData( const INNERSENDDATA* pstInnerSendData);
	int CalcSendResultData( const INNERRESULTDATA* pstInnerSendData);

	BOOL AssignProcAddress( void);

protected:
	// 機能確認
	typedef	BOOL ( *NETEXSPECIFICATION)( NXLSPECIFICATION* pNxlSpecification);
	// 設定変更用
	typedef	BOOL ( *NETEXCONFIGURE)( HWND hWnd);
	// Copyright表示用
	typedef	BOOL ( *NETEXABOUT)( HWND hWnd);

	// 初期化
	typedef BOOL ( *INITIALIZENETEX)( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData);
	// 終了
	typedef BOOL ( *UNINITIALIZENETEX)( HWND hWnd);
	// 状態の確認
	typedef DWORD ( *GETNXLSTATUS)( void);
	// エントリ開始
	typedef BOOL ( *STARTENTRY)( void);
	// エントリ開始：留守通知付き
	typedef BOOL ( *STARTENTRYEX)( BOOL blAbsence);
	// 留守
	typedef BOOL ( *ABSENCEENTRY)( BOOL blAbsence);
	// エントリ終了
	typedef BOOL ( *EXITENTRY)( void);

	// エントリリスト更新の中断、取得の開始のため
	typedef BOOL ( *LOCKENTRYLIST)( void);
	// エントリリスト更新の再開
	typedef BOOL ( *UNLOCKENTRYLIST)( void);
	// エントリーイメージの取得
	typedef BOOL ( *GETENTRYIMAGE)( IMAGEDATA* lpImageData);
	// グループ数を取得
	typedef int ( *GETGROUPCOUNT)( void);
	// グループ情報を取得
	typedef int ( *GETGROUPDATA)( int nIndex, GROUPDATA* pstGroupData, int nSize);
	// リストに存在するエントリー個数取得
	typedef int ( *GETENTRYCOUNT)( void);
	// エントリー情報の取得
	typedef int ( *GETENTRYDATA)( int nIndex, ENTRYDATA* ppstEntry, int nSize);
	// エントリ情報の比較
	typedef int ( *COMPAREENTRYDATA)( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize);
	// エントリ情報の表示
	typedef int ( *GETENTRYINFO)( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize);
	// エントリーの直接取得
	typedef int ( *INPUTENTRY)( HWND hWnd, HGLOBAL* hEntry);
	// 送信設定の取得
	typedef int ( *GETEXTDATA)( HWND hWnd, HGLOBAL* phExt);

	// データ送信
	typedef int ( *SENDMEMODATA)( const ENTRYDATA* pstEntry, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
	// データ送信（一括
	typedef int ( *SENDMEMODATAINBULK)( const ENTRYDATA** pstEntry, int* panResult, int nCount, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
	// 応答送信
	typedef int ( *SENDRESULTDATA)( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent);

	// 固有エラーメッセージ
	typedef int ( *GETERRORMSG)( int nEcode, char* pszErrorMsg, int nSize);



	HINSTANCE	m_hInstance;			// 
	CString		m_cStrLibName;
	CString		m_cStrNxlPath;

	UINT		m_unNxlID;
	UINT		m_unNxlStyle;
	UINT		m_unVersion;

	BOOL		m_blInitialized;
	BOOL		m_blFunctionAssign;


	INITIALIZENETEX		m_lpfInitializeNetEx;
	UNINITIALIZENETEX	m_lpfUnInitializeNetEx;
	GETNXLSTATUS		m_lpfGetNxlStatus;
	STARTENTRY			m_lpfStartEntry;
	STARTENTRYEX		m_lpfStartEntryEx;
	ABSENCEENTRY		m_lpfAbsenceEntry;
	EXITENTRY			m_lpfExitEntry;

	LOCKENTRYLIST		m_lpfLockEntryList;
	UNLOCKENTRYLIST		m_lpfUnlockEntryList;
	GETENTRYIMAGE		m_lpfGetEntryImage;
	GETGROUPCOUNT		m_lpfGetGroupCount;
	GETGROUPDATA		m_lpfGetGroupData;
	GETENTRYCOUNT		m_lpfGetEntryCount;
	GETENTRYDATA		m_lpfGetEntryData;
	COMPAREENTRYDATA	m_lpfCompareEntryData;
	GETENTRYINFO		m_lpfGetEntryInfo;
	INPUTENTRY			m_lpfInputEntry;
	GETEXTDATA			m_lpfGetExtData;

	SENDMEMODATA		m_lpfSendMemoData;
	SENDMEMODATAINBULK	m_lpfnSendMemoDataInBluk;
	SENDRESULTDATA		m_lpfSendResultData;

	GETERRORMSG			m_lpfGetErrorMsg;
};

#endif	//__NETEXLIB_H__
