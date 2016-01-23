#ifndef	__DXLCONTROL_H__
#define	__DXLCONTROL_H__

#include "DrawEx_IF.h"
#include "DrawExLib.h"

class CMemoChip;

class CDxlControl : public CObject
{
public:
	CDxlControl( void);
	~CDxlControl( void);

	BOOL SetInstance( const CDrawExLib* cDrawExLib);
	HINSTANCE GetInstance( void) const;

	const CDxlControl& operator=( CDxlControl& cStrSrc);

	BOOL InitializeChip( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom);
	BOOL UnInitializeChip( HWND hWnd);

	BOOL CalcDefaultRect( HWND hWnd, SIZE* pSize, const CMemoData& cMemoData);
	BOOL DrawChip( HWND hWnd, HDC hDC, const CMemoData& cMemoData);
	DWORD MouseClik( HWND hWnd, POINT* pstPoint);
	BOOL Resize( HWND hWnd, SIZE* pstSize);
	BOOL Zoom( HWND hWnd, BOOL blZoom);
	BOOL UpDate( HWND hWnd, const CMemoChip& cMemoChip, BOOL blZoom);
	BOOL GetMemoString( CString& cStr, const CMemoData& cMemoData);
	BOOL DoEditHelper( HWND hWnd, HGLOBAL* phGloba);
	HMENU GetDrawExMenu( void);
	BOOL DoDrawExMenu( HWND hWnd, UINT unMenuID);
	BOOL GetSaveData( HWND hWnd, CMemoData& cMemoData);
	int GetTimerSpan( void) const;
	DWORD DoTimer( void) const;
	BOOL Active( HWND hWnd, BOOL blActive);

	// �`��
	BOOL IsDataExchange() const;
	BOOL IsOwnerDraw() const;

	// �T�|�[�g���j���[
	BOOL IsSupportResize() const;
	BOOL IsSupportClick() const;
	BOOL IsSupportMenu() const;
	BOOL IsSupportNativeData() const;
	BOOL IsSupportTimer() const;
	BOOL IsNeedString() const;
	BOOL IsNoAlphaBlend() const;

	DWORD CheckTimer( void);

	// ���j���[�g�p���H
	static BOOL IsUsed();
protected:
	// ����������
	typedef	BOOL ( *INITIALIZECHIP)( DWORD* pdwDxlWord, HWND hWnd, DXLIFDATA* pstDxlIfData);
	// �I������
	typedef	BOOL ( *UNINITIALIZECHIP)( DWORD dwDxlWord, HWND hWnd);
	// �傫���Čv�Z
	typedef	BOOL ( *CALCDEFAULTRECT)( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize, LPCSTR lpcszMemoData);
	// �\��
	typedef	BOOL ( *DRAWCHIP)( DWORD dwDxlWord, HWND hWnd, HDC hDC, LPCSTR lpcszMemoData);
	// �}�E�X�N���b�N
	typedef	BOOL ( *MOUSECLIK)( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
	// �T�C�Y�ύX
	typedef	BOOL ( *RESIZE)( DWORD dwDxlWord, HWND hWnd, SIZE* pstSize);
	// ���Љ��^�A�C�R�����ɌĂ΂��F_DT_OWNERDRAW�w�莞�̂ݏ���
	typedef	void ( *ZOOM)( DWORD dwDxlWord, HWND hWnd, BOOL blZoom);
	// �f�[�^�X�V�F�ҏW�ɂ�蔭��
	typedef	BOOL ( *UPDATE)( DWORD dwDxlWord, HWND hWnd, DXLIFDATA* pstGelIfData);
	// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
	typedef	int ( *GETMEMOSTRING)( DWORD dwDxlWord, LPSTR lpszData, int nSize, LPCSTR lpcszMemoData);
	// �|�b�v�A�b�v���j���[
	typedef	HMENU ( *GETDRAWEXMENU)( DWORD dwDxlWord);
	// �|�b�v�A�b�v���j���[
	typedef	DWORD ( *DODRAWEXMENU)( DWORD dwDxlWord, HWND hWnd, UINT unMenuID);
	// �f�[�^�ۑ����̕ҏW
	typedef BOOL ( *GETSAVEDATA)( DWORD dwDxlWord, HWND hWnd, HGLOBAL* phGMem);
	// �^�C�}�����擾
	typedef int ( *GETTIMERSPAN)( DWORD dwDxlWord);
	// �^�C�}����
	typedef DWORD ( *DOTIMER)( DWORD dwDxlWord);

	// �ȍ~�̃C���^�[�t�F�C�X�́A�V���o�����ŗ��p
	// �}�E�X�N���b�N�F�g��
	typedef DWORD ( *MOUSECLIKEX)( DWORD dwDxlWord, HWND hWnd, POINT* pstPoint);
	// ���Ђ̃A�N�e�B�u�F�V�K
	typedef void ( *ACTIVE)( DWORD dwDxlWord, HWND hWnd, BOOL blActive);

protected:
	int m_nTimerSpan;
	HINSTANCE			m_hInstance;
	DWORD				m_dwDxlWord;
	UINT				m_unDrawType;
	UINT				m_unStyle;
	UINT				m_unIFDataVer;

	static CSemaphore	sm_cSmpDoMenu;

	// ����������
	INITIALIZECHIP m_lpfInitializeChip;
	// �I������
	UNINITIALIZECHIP m_lpfUnInitializeChip;
	// �傫���Čv�Z
	CALCDEFAULTRECT m_lpfCalcDefaultRect;
	// �\��
	DRAWCHIP m_lpfDrawChip;
	// �}�E�X�N���b�N
	MOUSECLIK m_lpfMouseClik;
	// �T�C�Y�ύX
	RESIZE m_lpfResize;
	// ���Љ��^�A�C�R����
	ZOOM m_lpfZoom;
	// �f�[�^�X�V�F�ҏW�ɂ�蔭��
	UPDATE m_lpfUpdate;
	// �`�悷�镶����F_DT_EXCHANGE�w�莞�̂ݏ���
	GETMEMOSTRING m_lpfGetMemoString;
	// �|�b�v�A�b�v���j���[
	GETDRAWEXMENU m_lpfGetDrawExMenu;
	// �|�b�v�A�b�v���j���[
	DODRAWEXMENU m_lpfDoDrawExMenu;
	// �f�[�^�ۑ����̕ҏW
	GETSAVEDATA m_lpfGetSaveData;
	// �^�C�}�����擾
	GETTIMERSPAN m_lpfnGetTimerSpan;
	// �^�C�}����
	DOTIMER m_lpfnDoTimer;

	// �}�E�X�N���b�N�F�g��
	MOUSECLIKEX m_lpfMouseClikEx;
	// ���Ђ̃A�N�e�B�u�F�V�K
	ACTIVE m_lpfActive;
};

#include "DxlControl.inl"
#endif	//__DXLCONTROL_H__
