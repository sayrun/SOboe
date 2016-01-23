#ifndef	__OBOE_NXL_IF_H__
#define	__OBOE_NXL_IF_H__

// �\���̂̃o�[�W����
#define	_NXL_IF_DATA_VER		0x00000100

// ��M�ʒmRegisterWindowMessage�p
typedef int (CALLBACK* RECEIVEDATA)( const LPVOID lpvEntry, int nEntrySize, const LPVOID lpvData, int nSize);

#define	_NXL_SEND					0x0000000000000001
#define	_NXL_RECEIVE				0x0000000000000002
#define	_NXL_SENDRECEIVE			( _NXL_SEND | _NXL_RECEIVE)
#define	_NXL_ENTRYGROUP				0x0000000000000004
#define	_NXL_SUPPORTRESOLUTION		0x0000000000000008
#define	_NXL_RESERVED				0x0000000000000010
#define	_NXL_SUPPORTORIGINALICON	0x0000000000000020
#define	_NXL_SUPPORTINPUTENTRY		0x0000000000000040
#define	_NXL_SUPPORTBULKSEND		0x0000000000000080

#pragma pack( push, __NetExLib, 1)
typedef struct tagNXLSPECIFICATION
{
	UINT	unLowerStructVersion;		// �����\�ȍŌÃo�[�W����
	UINT	unUpperStructVersion;		// �����\�ȍŐV�o�[�W����
	char	szProtcolString[ 50];		// ��������
	UINT	unNxlID;					// �v���g�R���ŗLID T.Chiba���s�ɂ��
	UINT	unNxlStyle;					// Nxl�̏������e
}NXLSPECIFICATION;

#define	_SENDDATA_VER100		0x0100
// ���M�f�[�^�e�[�u��
// ������f�[�^�́A�ȉ��̂悤�Ɏ�o��
// �^�C�g���̗�
//	int nOffset = pstSendDataEx->m_nszMemoTitle;
//	char* pszMemoTitle = NULL;
//	if( 0 <= nOffset)
//	{
//		pszMemoTitle = ( ( char*)pstSendDataEx)[ nOffset];
//	}
// ���Ȃ݂ɁA�e�f�[�^�̏��Ԃ͖��Ȃ�
typedef	struct tagSENDDATA
{
	// �t�H�[�}�b�g�R�[�h
	char			szHeader[ 9];		// "OboeData\0"
	// �\���̏��
	int				nSize;
	UINT			unStructVersion;

	// �o�[�W�������ْʒm
	UINT			m_unVersion;		// �o�[�W�����ԍ�
	int				m_nszVersion;		// �o�[�W��������:offset from *SENDDATA

	// ��{�ݒ�̃f�[�^
	int				m_nszMemoTitle;		// �^�C�g��:offset from *SENDDATA
	int				m_nszMemoData;		// ���e:offset from *SENDDATA

	// �������
	int				m_nstLogFont;		// �t�H���g�\����(LOGFONT):offset from *SENDDATA
	COLORREF		m_clrForeColor;		// �����F
	COLORREF		m_clrBackColor;		// �w�i�F
	int				m_nIconIndex;		// �A�C�R�����

	// �֘A�t��
	BOOL			m_blShellExec;		// �֘A�t���̗L��
	int				m_nszProgName;		// �֘A�t���v���O����:offset from *SENDDATA
	int				m_nszFileName;		// �֘A�t���t�@�C��:offset from *SENDDATA

	// �^�C�}�֘A
	BOOL			m_blTimerEnable;	// �^�C�}�̗L��
	int				m_nHour;			// ����
	int				m_nMin;				// ��
	int				m_nTimerType;		// �^�C�}�̎��
	int				m_nMonth;			// �ʒm��
	int				m_nDay;				// �ʒm��
	int				m_nNotifyShift;		// �ʒm�������j���������ꍇ�̃V�t�g:1.40
	int				m_nBeforeNotify;	// �����O����̒ʒm:1.40
	BOOL			m_blExpireTimer;	// �o�ߌ�̃^�C�}�[:1.40
	int				m_nszWaveFile;		// �T�E���h�t�@�C��:1.40:offset from *SENDDATA
	int				m_nWeekDay;			// �ʒm�j��

	// ���M�̃f�[�^
	BOOL			m_blSendMaster;		// �����̗L��
	int				m_nAppendPos;		// �����ǉ��ʒu
	int				m_nszSignature;		// ����:offset from *SENDDATA
	BOOL			m_blSendTime;		// ���M����
	BOOL			m_blSendResize;		// ���M��̃��T�C�Y
	RECT			m_stRctNormal;		// �\���T�C�Y
	BOOL			m_blSendCenter;		// �Z���^�����O��M:1.40
	BOOL			m_blPack;			// ���Ԃ����M:1.40
	BOOL			m_blPassWord;		// �p�X���[�h�v��:1.40
	BOOL			m_blCheckOpen;		// �J���m�F:1.40
	BOOL			m_blDelOnQuit;		// �I�����̍폜
	int				m_nszFrom;			// ���M��:offset from *SENDDATA
	int				m_nszTo;			// ���M��ꗗ:offset from *SENDDATA
										// ���M��ꗗ�́A�A�h���X�݂̂ŁA","�J���}�ŋ�؂鎖

	// �l�b�g���[�N���c
	BOOL			m_blCheckYesNo;		// �͂��^�������m�F:1.40
	BOOL			m_blOtherResult;	// ���̑��̉�:1.42
	int				m_nszResult1;		// �I�����P:offset from *SENDDATA
	int				m_nszResult2;		// �I�����P:offset from *SENDDATA
	int				m_nszResult3;		// ���̑��̉��f�t�H���g:offset from *SENDDATA
	int				m_nszCHIPID;		// �����R�[�h�i������j:offset from *SENDDATA
	int				m_nResultTime;		// �����L�����ԁF�|�P�͖�����

	// ���Ђ̍쐬����
	int				m_nCreateYear;		// �쐬�̔N:1.40
	int				m_nCreateMonth;		// �쐬�̌�:1.40
	int				m_nCreateDay;		// �쐬�̓�:1.40
	int				m_nZapDays;			// �폜����:1.40�G�g�p���Ȃ�

	// �`��g��
	UINT			m_unDxlID;			// Dxl��ID
	int				m_nszDxlDummy;		// Dxl���g�p���̕\������:offset from *SENDDATA
	int				m_nDxlDataSize;		// dxl�̊g�����T�C�Y
	int				m_nabyDxlData;		// dxl�̊g�����:offset from *SENDDATA

	// ���̌��ɉσf�[�^���i�[�����
}SENDDATA;

typedef	struct tagRESULTDATA
{
	// �t�H�[�}�b�g�R�[�h
	char			szHeader[ 9];		// "OboeRslt\0"
	// �\���̏��
	int				nSize;
	UINT			unStructVersion;

	int				m_nszSignature;
	int				m_nszCHIPID;		// �����R�[�h�i������j:offset from *RESULTDATA
	int				m_nszResult;		// ���Ԏ����惓�Foffset from *RESULTDATA

	// ���̌��ɉσf�[�^���i�[�����
}RESULTDATA;

typedef struct tagIMAGEDATA
{
	SIZE		stSize;
	int			nCount;
	HBITMAP		hImage;
	COLORREF	clrMask;
}IMAGEDATA;

#define	_STATUS_NONE	0
#define	_STATUS_NORMAL	1
#define	_STATUS_ABSENCE	2

// �G���g���f�[�^
#define	_ENTRYSTRUCT_VER100	0x0100
typedef struct tagENTRYDATA
{
	int		nSize;
	UINT	unStructVersion;
	UINT	unNxlID;
	int		nEntryStat;
	int		nParentGroupCount;
	int		nOffsetParentGroup;
	int		nOffsetEntryName;
}ENTRYDATA;

#define	_GROUPSTRUCT_VER100	0x0100
typedef struct tagGROUPDATA
{
	int		nSize;
	UINT	unStructVersion;
	int		nParentGroup;
	int		nGroupStatus;
	int		nOffsetGroupName;
}GROUPDATA;

#pragma pack( pop, __NetExLib)

#define	_SEND_SUCCESS	0		// ���M�͖����Ɋ�������
#define	_SEND_ABSENCE	1		// ���M�������A����͗��炾�����I
#define	_SEND_FORCEDEND	2		// nxl���J������A�����𒆒f����
#define	_SEND_ERROR		3		// �ʐM���ɃG���[����������
#define	_SEND_TIMEOUT	4		// �������^�C���A�E�g����
#define	_SEND_CANCEL	5		// ���[�U�ɂ��L�����Z������A�����𒆒f����
#define	_SEND_USER		100		// ����ȏオ�eNxl�̃G���[�R�[�h�Ƃ���F�ꉞ���̐��͈͂ɂ��Ă���

#define	_NXLSTATUS_ACTIVE			0x000000001
#define	_NXLSTATUS_READY_RECEIVE	0x000000002
#define	_NXLSTATUS_READY_SEND		0x000000004

#ifdef	__cplusplus
extern "C"
{
#endif

// �@�\�m�F
__declspec( dllexport ) BOOL NetExSpecification( NXLSPECIFICATION* pNxlSpecification);
// �ݒ�ύX�p
__declspec( dllexport ) BOOL NetExConfigure( HWND hWnd);
// Copyright�\���p
__declspec( dllexport ) BOOL NetExAbout( HWND hWnd);
// ������
__declspec( dllexport ) BOOL InitializeNetEx( HWND hWnd, LPCSTR lpcszEntryName, RECEIVEDATA lpfnRcvData);
// �I��
__declspec( dllexport ) BOOL UnInitializeNetEx( HWND hWnd);
// ��Ԃ̊m�F
__declspec( dllexport ) DWORD GetNxlStatus( void);
// �G���g���J�n
__declspec( dllexport ) BOOL StartEntry( void);
// �G���g���J�n�F����ʒm�t��
__declspec( dllexport ) BOOL StartEntryEx( BOOL blAbsence);
// ����
__declspec( dllexport ) BOOL AbsenceEntry( BOOL blAbsence);
// �G���g���I��
__declspec( dllexport ) BOOL ExitEntry( void);
// �G���g�����X�g�X�V�̒��f�A�擾�̊J�n�̂���
__declspec( dllexport ) BOOL LockEntryList( void);
// �G���g�����X�g�X�V�̍ĊJ
__declspec( dllexport ) BOOL UnlockEntryList( void);
// �G���g���[�C���[�W�̎擾
__declspec( dllexport ) BOOL GetEntryImage( IMAGEDATA* lpImageData);
// �O���[�v�����擾
__declspec( dllexport ) int GetGroupCount( void);
// �O���[�v�����擾
__declspec( dllexport ) int GetGroupData( int nIndex, GROUPDATA* pstGroupData, int nSize);
// ���X�g�ɑ��݂���G���g�����擾
__declspec( dllexport ) int GetEntryCount( void);
// �G���g�����̎擾
__declspec( dllexport ) int GetEntryData( int nIndex, ENTRYDATA* pstEntry, int nSize);
// �G���g�����̔�r
__declspec( dllexport ) int CompareEntryData( const ENTRYDATA* pstEntryDst, int nDstSize, const ENTRYDATA* pstEntrySrc, int nSrcSize);
// �G���g�����̕\��
__declspec( dllexport ) int GetEntryInfo( LPSTR lpszEntryInfo, int nEntryInfoSize, const ENTRYDATA* pstEntry, int nSize);
// �G���g���[�̒��ڎ擾
__declspec( dllexport ) int InputEntry( HWND hWnd, HGLOBAL* hEntry);
// �f�[�^���M
__declspec( dllexport ) int SendMemoData( const ENTRYDATA* pstEntry, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
// �f�[�^���M�i�ꊇ
__declspec( dllexport ) int SendMemoDataInBluk( const ENTRYDATA** pstEntry, int* panResult, int nCount, const SENDDATA* pSendData, int nSendDataSize, HANDLE hCancelEvent);
// �������M
__declspec( dllexport ) int SendResultData( const ENTRYDATA* pstEntry, const RESULTDATA* pResultData, int nResultDataSize, HANDLE hCancelEvent);
// �ŗL�G���[���b�Z�[�W
__declspec( dllexport ) int GetErrorMsg( int nEcode, char* pszErrorMsg, int nSize);

#ifdef	__cplusplus
}
#endif

#endif	__OBOE_NXL_IF_H__