#ifndef	__DRAWEXLIB_H__
#define	__DRAWEXLIB_H__

#include "DrawEx_IF.h"

class CDrawExLib : public CObject
{
public:
	CDrawExLib();
	~CDrawExLib();

	const CString& GetPath( void);
	BOOL SetPath( LPCSTR lpcsz);

	HINSTANCE GetInstance( void) const;
	const CString& GetMenuString( void);
	BOOL SetMenuString( LPCSTR lpcsz);
	UINT GetDataVersion( void) const;

	UINT GetDxlID( void) const;
	UINT GetDrawType( void) const;
	UINT GetUnsupportMenu( void) const;
	const CString& GetDefaultMenuString( void);

	BOOL LoadDxl( HWND hWnd);
	BOOL FreeDxl( void);

	BOOL IsFunctionAssign( void);

	BOOL AboutDrawExLib( HWND hWnd);
	BOOL Configure( HWND hWnd);

	const int UseStart( void);
	const int UseEnd( void);
	int GetUseCount( void) const;
protected:
	void DispErrorMsg( HWND hWnd, DWORD dwCode);

	// �@�\�m�F
	typedef	BOOL ( *DRAWEXSPECIFICATION)( DXLSPECIFICATION* pDxlSpecification);
	// Copyright�ύX�p
	typedef	BOOL ( *DRAWEXABOUT)( HWND hWnd);
	// �ݒ�ύX�p
	typedef BOOL ( *DRAWEXCONFIGURE)( HWND hWnd);

	UINT		m_unDxlDataVersion;		// �����\�ȍŐV�o�[�W����
	CString*	m_pcStrMenuString;		// �f�t�H���g�̏�������
	UINT		m_unDXLID;				// �Sdxl�ԂŃ��j�[�N�Ȃh�c�FT.Chiba�̔��s�ɂ��
	UINT		m_unDrawType;			// dxl�̏������e
	UINT		m_unUnsupportMenu;		// �T�|�[�g���Ȃ��@�\

	HINSTANCE	m_hInstance;			// 
	CString*	m_pcStrDxlPath;			// �t�@�C����
	CString		m_cStrDxlMenu;			// �f�t�H���g�̃��j��

	int			m_nUseCount;
};

#include "DrawExLib.inl"
#endif	//__DRAWEXLIB_H__