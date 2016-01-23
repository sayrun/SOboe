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
	// �@�\�m�F
	typedef	BOOL ( *NETEXSPECIFICATION)( NXLSPECIFICATION* pNxlSpecification);
	// �ݒ�ύX�p
	typedef	BOOL ( *NETEXCONFIGURE)( HWND hWnd);
	// Copyright�\���p
	typedef	BOOL ( *NETEXABOUT)( HWND hWnd);

	// ������
	typedef BOOL ( *INITIALIZENETEX)( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData);
	// �I��
	typedef BOOL ( *UNINITIALIZENETEX)( HWND hWnd);
	// ��Ԃ̊m�F
	typedef DWORD ( *GETNXLSTATUS)( void);
	// �G���g���J�n
	typedef BOOL ( *STARTENTRY)( void);
	// �G���g���J�n�F����ʒm�t��
	typedef BOOL ( *STARTENTRYEX)( BOOL blAbsence);
	// ����
	typedef BOOL ( *ABSENCEENTRY)( BOOL blAbsence);
	// �G���g���I��
	typedef BOOL ( *EXITENTRY)( void);

	// �G���g�����X�g�X�V�̒��f�A�擾�̊J�n�̂���
	typedef BOOL ( *LOCKENTRYLIST)( void);
	// �G���g�����X�g�X�V�̍ĊJ
	typedef BOOL ( *UNLOCKENTRYLIST)( void);
	// �G���g���[�C���[�W�̎擾
	typedef BOOL ( *GETENTRYIMAGE)( IMAGEDATA* lpImageData);
	// �O���[�v�����擾
	typedef int ( *GETGROUPCOUNT)( void);
	// �O���[�v�����擾
	typedef int ( *GETGROUPDATA)( int nIndex, GROUPDATA* pstGroupData, int nSize);
	// ���X�g�ɑ��݂���G���g���[���擾
	typedef int ( *GETENTRYCOUNT)( void);
	// �G���g���[���̎擾
	typedef int ( *GETENTRYDATA)( int nIndex, ENTRYDATA* ppstEntry, int nSize);
	// �G���g�����̔�r
	typedef int ( *COMPAREENTRYDATA)( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize);
	// �G���g�����̕\��
	typedef int ( *GETENTRYINFO)( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize);
	// �G���g���[�̒��ڎ擾
	typedef int ( *INPUTENTRY)( HWND hWnd, HGLOBAL* hEntry);
	// ���M�ݒ�̎擾
	typedef int ( *GETEXTDATA)( HWND hWnd, HGLOBAL* phExt);

	// �f�[�^���M
	typedef int ( *SENDMEMODATA)( const ENTRYDATA* pstEntry, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
	// �f�[�^���M�i�ꊇ
	typedef int ( *SENDMEMODATAINBULK)( const ENTRYDATA** pstEntry, int* panResult, int nCount, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
	// �������M
	typedef int ( *SENDRESULTDATA)( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent);

	// �ŗL�G���[���b�Z�[�W
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
