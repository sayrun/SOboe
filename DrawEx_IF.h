#ifndef	__OBOE_DXL_IF_H__
#define	__OBOE_DXL_IF_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// �\���̂̃o�[�W����
#define	_DXL_IF_DATA_VER_100	0x00000100
#define	_DXL_IF_DATA_VER_200	0x00000200

#if ( _DXLVER >= 0x200)
	#define	_DXL_IF_DATA_VER	_DXL_IF_DATA_VER_200
#else
	#define	_DXL_IF_DATA_VER	_DXL_IF_DATA_VER_100
#endif


#if ( _DXLVER >= 0x200)
#define	_GET_MEMO			1
#define	_GET_TITLE			2
#define	_GET_FONT			3
#define	_GET_FORECOLOR		4
#define	_GET_BACKCOLOR		5
#define	_GET_ICON			6
#define	_GET_ZORDER			7
#define	_GET_WINDOWTYPE		8
#define	_GET_RECT_NORMAL	9
#define	_GET_POINT_TITLE	10
#define	_GET_POINT_ICON		11
#define _GET_RECT_TITLE		12
#define _GET_RECT_ICON		13

#define	_GET_TIMERENABLE	20
#define	_GET_TIMER_TYPE		21
#define	_GET_TIMER_MONTH	22
#define	_GET_TIMER_DAY		23
#define	_GET_TIMER_HOUR		24
#define	_GET_TIMER_MIN		25
#define	_GET_TIMER_WEEK		26
#define	_GET_TIMER_SOUND	27

#define	_GET_LINKENABLE		40
#define	_GET_LINK_FILE		41
#define	_GET_LINK_PARAM		42

#define	_GET_CREATE_YEAR	60
#define	_GET_CREATE_MONTH	61
#define	_GET_CREATE_DAY		62

#define	_GET_DEF_FONT		80
#define	_GET_DEF_FORECLR	81
#define	_GET_DEF_BACKCLR	82

#define	_GET_VERSION		100

typedef int (CALLBACK* GETMEMODATA)( LPVOID lp, int nGetType, LPVOID lpData, int nDataSize);
#endif

#pragma pack(8)
typedef struct tagDXLSAVEDATA
{
	int		m_nDataSize;
	BYTE	m_abyData[1];
}DXLSAVEDATA;

typedef struct tagDXLIFDATA100
{
	int				m_nSize;				// ���̍\���̂̃T�C�Y
	int				m_nVersion;				// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�

	char			m_achszMemoTitle[64];	// ���Ђ̃^�C�g��
	LPCSTR			m_lpcszMemoData;		// ���Ђ̓��e
	COLORREF		m_clrTextColor;			// �e�L�X�g�F
	COLORREF		m_clrBackColor;			// �w�i�F
	LOGFONT			m_stLogFont;			// �t�H���g�X�^�C��
	BOOL			m_blZoom;				// ���Ё^�A�C�R��
	DXLSAVEDATA*	m_pstSaveData;			// 
}DXLIFDATA100;

#if ( _DXLVER >= 0x200)
typedef struct tagDXLIFDATA200
{
	int				m_nSize;				// ���̍\���̂̃T�C�Y
	int				m_nVersion;				// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�

	char			m_achszMemoTitle[64];	// ���Ђ̃^�C�g��
	LPCSTR			m_lpcszMemoData;		// ���Ђ̓��e
	COLORREF		m_clrTextColor;			// �e�L�X�g�F
	COLORREF		m_clrBackColor;			// �w�i�F
	LOGFONT			m_stLogFont;			// �t�H���g�X�^�C��
	BOOL			m_blZoom;				// ���Ё^�A�C�R��
	DXLSAVEDATA*	m_pstSaveData;			// 
	GETMEMODATA		m_lpfnGetMemoData;		// callback
	LPVOID			m_lpGetMemoID;			// for callback:�����ɐG��Ǝ��ʂ��I���񂪂��c�c���( ^<
}DXLIFDATA200;
#endif

#if ( _DXLVER >= 0x200)
	typedef DXLIFDATA200 DXLIFDATA;
#else
	typedef DXLIFDATA100 DXLIFDATA;
#endif

typedef struct tagEDITDATA
{
	int		m_nSelStart;
	int		m_nSelEnd;
	HGLOBAL	m_hEdit;
	int		m_nSize;
	HGLOBAL m_hSaveData;
}EDITDATA;
#pragma pack()

// ���̂c�w�k�̋@�\
#define	_DXL_NOUSERRESEIZE			0x0000000000000001		// ���T�C�Y�͋֎~
#define	_DXL_MOUSECLICK				0x0000000000000002		// �}�E�X�N���b�N����������
#define	_DXL_USEMENU				0x0000000000000004		// ���j���[���g�p����
#define	_DXL_EDITHEPLER				0x0000000000000008		// ���͕⏕���s��
#define	_DXL_NATIVEDATA				0x0000000000000010		// �ŗL�f�[�^�ۑ��f�[�^
#define	_DXL_NOEXSEND				0x0000000000000020		// ���M���͏����^�g���Ȃǂ̂̓T�|�[�g���Ȃ�
#define	_DXL_TIMER					0x0000000000000040		// �^�C�}�@�\���g�p����
#define	_DXL_NEEDSTRING				0x0000000000000080		// �`�揈�����ɕ������K�v�Ƃ���
#define	_DXL_NOALPHABLEND			0x0000000000000100		// AlphaBlend���֎~

#define	_DT_EXCHANGE				0x0000000000000001		// �����ϊ��̂݁F�`��͊o����
#define	_DT_OWNERDRAW				0x0000000000000002		// �`�揈���L��

#define	_TM_REDRAW					0x0000000000000001		// �ĕ`��
#define	_TM_CALCRECT				0x0000000000000002		// �傫���Čv�Z
#define	_TM_NOTIFYBLINK				0x0000000000000004		// �u�����N
#if ( _DXLVER >= 0x200)
#define	_TM_TONORMAL				0x0000000000000010		// �ʏ���
#define	_TM_TOICON					0x0000000000000020		// �A�C�R����
#define	_TM_TOTITLE					0x0000000000000040		// �^�C�g����
#endif

typedef struct tagDXLSPECIFICATION
{
	UINT	unLowerStructVersion;	// �����\�ȍŌÃo�[�W����
	UINT	unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
	char	szMenuString[ 50];		// �f�t�H���g�̏�������
	UINT	unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	UINT	unDrawType;				// dxl�̏������e
	UINT	unStyle;				// Dxl�̃X�^�C��
}DXLSPECIFICATION;

#define	_DXL_MENUTOP			10000
#define	_DXL_MENU0				10000
#define	_DXL_MENU1				10001
#define	_DXL_MENU2				10002
#define	_DXL_MENU3				10003
#define	_DXL_MENU4				10004
#define	_DXL_MENU5				10005
#define	_DXL_MENU6				10006
#define	_DXL_MENU7				10007
#define	_DXL_MENU8				10008
#define	_DXL_MENU9				10009
#define	_DXL_MENU10				10010
#define	_DXL_MENU11				10011
#define	_DXL_MENU12				10012
#define	_DXL_MENU13				10013
#define	_DXL_MENU14				10014
#define	_DXL_MENU15				10015
#define	_DXL_MENU16				10016
#define	_DXL_MENU17				10017
#define	_DXL_MENU18				10018
#define	_DXL_MENU19				10019
#define	_DXL_MENUBOTTOM			10019

#ifdef	__cplusplus
extern "C"
{
#endif


// �@�\�m�F
__declspec( dllexport ) BOOL DrawExSpecification( DXLSPECIFICATION* pDxlSpecification);
// �ݒ�ύX�p
__declspec( dllexport ) BOOL DrawExConfigure( HWND hWnd);
// Copyright�\���p
__declspec( dllexport ) BOOL DrawExAbout( HWND hWnd);
// ����������
__declspec( dllexport ) BOOL InitializeChip( DWORD* pdwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// �I������
__declspec( dllexport ) BOOL UnInitializeChip( DWORD dwDxlWord, HWND hWnd);
// �傫���Čv�Z�F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) BOOL CalcDefaultRect( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize, LPCSTR lpcszMemoData);
// �\���F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) BOOL DrawChip( DWORD dwDxlWord, HWND hWnd, HDC hDC, LPCSTR lpcszMemoData);
// �}�E�X�N���b�N
__declspec( dllexport ) BOOL MouseClik( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// �T�C�Y�ύX�F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) BOOL Resize( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// ���Љ��^�A�C�R�����ɌĂ΂��F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) void Zoom( DWORD dwDxlWord, HWND hWnd, BOOL blZoom);
// �f�[�^�X�V�F�ҏW�ɂ�蔭��
__declspec( dllexport ) BOOL Update( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// ���M���镶����
__declspec( dllexport ) int ExchangeSendString( HGLOBAL* phgMem);
// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
__declspec( dllexport ) int GetMemoString( DWORD dwDxlWord, LPSTR lpszData, int nSize, LPCSTR lpcszMemoData);
// �|�b�v�A�b�v���j���[
__declspec( dllexport ) HMENU GetDrawExMenu( DWORD dwDxlWord);
// �|�b�v�A�b�v���j���[
__declspec( dllexport ) DWORD DoDrawExMenu( DWORD dwDxlWord, HWND hWnd, UINT unMenuID);
// ����ҏW�F���̓w���p�[
__declspec( dllexport ) int EditHelper( HWND hWnd, EDITDATA* pstEditData);
// �f�[�^�ۑ����̕ҏW
__declspec( dllexport ) BOOL GetSaveData( DWORD dwDxlWord, HWND hWnd, HGLOBAL* phGMem);
// �^�C�}�����擾
__declspec( dllexport ) int GetTimerSpan( DWORD dwDxlWord);
// �^�C�}����
__declspec( dllexport ) DWORD DoTimer( DWORD dwDxlWord);


#if ( _DXLVER >= 0x200)
// �ȍ~�̃C���^�[�t�F�C�X�́A�V���o�����ŗ��p
// �}�E�X�N���b�N�F�g��
// ���̃C���^�[�t�F�C�X�������ꍇ�́A��MouseClik���Ăяo����܂��B
__declspec( dllexport ) DWORD MouseClikEx( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// ���M���镶����F�g��
// ���̃C���^�[�t�F�C�X�������ꍇ�́A��ExchangeSendString���Ăяo����܂��B
__declspec( dllexport ) int ExchangeSendStringEx( EDITDATA* pstEditData);
// ���Ђ̃A�N�e�B�u�F�V�K
__declspec( dllexport ) void Active( DWORD dwDxlWord, HWND hWnd, BOOL blActive);
#endif

#ifdef	__cplusplus
}
#endif

#endif	//__OBOE_DXL_IF_H__
