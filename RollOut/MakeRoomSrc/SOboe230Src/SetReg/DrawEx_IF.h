#ifndef	__OBOE_DXL_IF_H__
#define	__OBOE_DXL_IF_H__

// �\���̂̃o�[�W����
#define	_DXL_IF_DATA_VER		0x00000100

typedef struct tagDXLIFDATA
{
	int				m_nSize;				// ���̍\���̂̃T�C�Y
	int				m_nVersion;				// ���̍\���̂̃o�[�W�����F_IF_DATA_VER���w��̎�

	char			m_achszMemoTitle[64];	// ���Ђ̃^�C�g��
	HGLOBAL			m_hachszMemoData;		// ���Ђ̓��e
	COLORREF		m_clrTextColor;			// �e�L�X�g�F
	COLORREF		m_clrBackColor;			// �w�i�F
	LOGFONT			m_stLogFont;			// �t�H���g�X�^�C��
}DXLIFDATA;

// ���̂f�d�k���T�|�[�g���Ȃ��@�\
#define	_UNSUPPORT_RESEIZE		0x0000000000000001		// ���T�C�Y�͋֎~
#define	_UNSUPPORT_CLICK		0x0000000000000002		// �N���b�N�͏������Ȃ�
#define	_UNSUPPORT_MENU			0x0000000000000004		// ���j���[�͎g�p���Ȃ�
#define	_UNSUPPORT_EDITHEPLER	0x0000000000000008		// ���͕⏕�͂Ȃ�

#define	_DT_EXCHANGE			0x0000000000000001		// �����ϊ��̂݁F�`��͊o����
#define	_DT_OWNERDRAW			0x0000000000000002		// �`�揈���L��

typedef struct tagDXLSPECIFICATION
{
	UINT	unLowerStructVersion;	// �����\�ȍŌÃo�[�W����
	UINT	unUpperStructVersion;	// �����\�ȍŐV�o�[�W����
	char	szMenuString[ 50];		// �f�t�H���g�̏�������
	UINT	unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	UINT	unDrawType;				// dxl�̏������e
	UINT	unUnsupportMenu;		// �T�|�[�g���Ȃ��@�\
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
#define	_DXL_MENUBOTTOM			10009

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
__declspec( dllexport ) BOOL CalcDefaultRect( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// �\���F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) BOOL DrawChip( DWORD dwDxlWord, HWND hWnd, HDC hDC);
// �}�E�X�N���b�N
__declspec( dllexport ) BOOL MouseClik( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
// �T�C�Y�ύX�F_DT_OWNERDRAW�w�莞�̂ݏ���
__declspec( dllexport ) BOOL Resize( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
// �f�[�^�X�V�F�ҏW�ɂ�蔭��
__declspec( dllexport ) BOOL Update( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
// ���M���镶����
__declspec( dllexport ) int GetSendString( DWORD dwDxlWord, LPSTR lpszData, int nSize);
// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
__declspec( dllexport ) int GetMemoString( DWORD dwDxlWord, LPSTR lpszData, int nSize);
// �|�b�v�A�b�v���j���[
__declspec( dllexport ) HMENU GetDrawExMenu( DWORD dwDxlWord);
// �|�b�v�A�b�v���j���[
__declspec( dllexport ) BOOL DoDrawExMenu( DWORD dwDxlWord, UINT unMenuID);
// ����ҏW�F���̓w���p�[
__declspec( dllexport ) int EditHelper( HWND hWnd, HGLOBAL* hgMem);

#ifdef	__cplusplus
}
#endif

#endif	//__OBOE_DXL_IF_H__
