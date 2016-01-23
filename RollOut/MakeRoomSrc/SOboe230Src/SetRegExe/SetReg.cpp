// SetReg.cpp : �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// �O���[�o���ϐ�:
HINSTANCE hInst;					// ���݂̃C���X�^���X
TCHAR szTitle[MAX_LOADSTRING];				// �^�C�g�� �o�[ �e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// �^�C�g�� �o�[ �e�L�X�g

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̑O�錾:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	// ��d�N���̗}��
	HANDLE	hPrevMutex = CreateMutex( NULL, TRUE, "hPrevInstance#Memo|Shin");
	// �d���H������A�I���ˁB
	if( ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox( NULL, "�o�������N������Ă��܂�\n\n��Ɋo�������I�����ĉ������B", "�o�������W�X�g���ݒ�", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
		return FALSE;
	}

	HRESULT hResult = CoInitialize( NULL);

	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o�� �X�g�����O�����������܂�
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SETREG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// �A�v���P�[�V�����̏��������s���܂�:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SETREG);

	// ���C�� ���b�Z�[�W ���[�v:
	while( GetMessage(&msg, NULL, 0, 0) ) 
	{
		if( !TranslateAccelerator (msg.hwnd, hAccelTable, &msg) ) 
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	return msg.wParam;
}



//
//  �֐�: MyRegisterClass()
//
//  �p�r: �E�B���h�E �N���X�̓o�^
//
//  �R�����g:
//
//    ���̊֐�����т��̎g�p�͂��̃R�[�h�� Windows 95 �Ő�ɒǉ����ꂽ
//    'RegisterClassEx' �֐��� Win32 �V�X�e���̌݊�����ێ��������ꍇ��
//    �̂ݕK�v�ƂȂ�܂��B�A�v���P�[�V�������A�A�v���P�[�V�����Ɋ֘A�t��
//    ��ꂽ�X���[�� �A�C�R�����擾�ł���悤�A���̊֐����Ăяo�����Ƃ�
//    �d�v�ł��B
//
ATOM MyRegisterClass( HINSTANCE hInstance )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SETREG);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_SETREG;
	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SETREG);
	wcex.hIconSm		= ( HICON)LoadImage( hInstance, MAKEINTRESOURCE( IDI_SETREG), IMAGE_ICON, 16, 16, LR_DEFAULTSIZE);

	return RegisterClassEx( &wcex );
}

//
//   �֐�: InitInstance(HANDLE, int)
//
//   �p�r: �C���X�^���X �n���h���̕ۑ��ƃ��C�� �E�B���h�E�̍쐬
//
//   �R�����g:
//
//        ���̊֐��ł́A�C���X�^���X �n���h�����O���[�o���ϐ��ɕۑ����A�v���O������
//        ���C�� �E�B���h�E���쐬���\�����܂��B
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X �n���h����ۑ����܂�

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if( !hWnd ) 
   {
      return FALSE;
   }

   ShowWindow( hWnd, SW_HIDE);//SW_SHOWMINIMIZED);//nCmdShow );
   UpdateWindow( hWnd );

   return TRUE;
}

UINT m_unStartMessage = RegisterWindowMessage( _T( "OBOEGAKI#SETTING"));


typedef int (*LPDOCUSTOMIZE)( BOOL blSpec, HWND hWnd);

//
//  �֐�: WndProc(HWND, unsigned, WORD, LONG)
//
//  �p�r: ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- �I�����b�Z�[�W�̒ʒm�ƃ��^�[��
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch( message ) 
	{
	case WM_CREATE:
		PostMessage( hWnd, m_unStartMessage, 0, 0L);
		return 0;

	case WM_QUERYOPEN:
		return 0;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	default:
		break;
	}
	if( m_unStartMessage == message)
	{
		HINSTANCE hSetReg = LoadLibrary( _T( "SetReg.dll"));
		if( NULL == hSetReg)
		{
			LPVOID lpMsgBuf;
			FormatMessage(  FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

			MessageBox( hWnd, reinterpret_cast<LPCTSTR>( lpMsgBuf), "�ݒ�pDLL��������܂���", MB_OK | MB_ICONINFORMATION );

			LocalFree( lpMsgBuf );

		}
		else
		{
			LPDOCUSTOMIZE lpfnDoCustomize;

			lpfnDoCustomize = reinterpret_cast<LPDOCUSTOMIZE>( GetProcAddress( hSetReg, _T( "DoCustomize")));
			if( NULL == lpfnDoCustomize)
			{
				MessageBox( hWnd, _T( "�ݒ�pDLL���s���ł��B"), _T( "SetReg"), MB_OK);
			}
			else
			{
				lpfnDoCustomize( TRUE, hWnd);
			}
		}
		PostMessage( hWnd, WM_CLOSE, 0, 0L);
		return 0;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}
