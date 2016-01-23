// SetReg.cpp : アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;					// 現在のインスタンス
TCHAR szTitle[MAX_LOADSTRING];				// タイトル バー テキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// タイトル バー テキスト

// このコード モジュールに含まれる関数の前宣言:
ATOM			MyRegisterClass( HINSTANCE hInstance );
BOOL			InitInstance( HINSTANCE, int );
LRESULT CALLBACK	WndProc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK	About( HWND, UINT, WPARAM, LPARAM );

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow )
{
	// 二重起動の抑制
	HANDLE	hPrevMutex = CreateMutex( NULL, TRUE, "hPrevInstance#Memo|Shin");
	// 重複？ンじゃ、終了ね。
	if( ERROR_ALREADY_EXISTS == GetLastError())
	{
		MessageBox( NULL, "覚え書が起動されています\n\n先に覚え書を終了して下さい。", "覚え書レジストリ設定", MB_OK | MB_ICONHAND | MB_SYSTEMMODAL);
		return FALSE;
	}

	HRESULT hResult = CoInitialize( NULL);

	// TODO: この位置にコードを記述してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル ストリングを初期化します
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SETREG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass( hInstance );

	// アプリケーションの初期化を行います:
	if( !InitInstance( hInstance, nCmdShow ) ) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SETREG);

	// メイン メッセージ ループ:
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
//  関数: MyRegisterClass()
//
//  用途: ウィンドウ クラスの登録
//
//  コメント:
//
//    この関数およびその使用はこのコードを Windows 95 で先に追加された
//    'RegisterClassEx' 関数と Win32 システムの互換性を保持したい場合に
//    のみ必要となります。アプリケーションが、アプリケーションに関連付け
//    られたスモール アイコンを取得できるよう、この関数を呼び出すことは
//    重要です。
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
//   関数: InitInstance(HANDLE, int)
//
//   用途: インスタンス ハンドルの保存とメイン ウィンドウの作成
//
//   コメント:
//
//        この関数では、インスタンス ハンドルをグローバル変数に保存し、プログラムの
//        メイン ウィンドウを作成し表示します。
//
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス ハンドルを保存します

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
//  関数: WndProc(HWND, unsigned, WORD, LONG)
//
//  用途: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 終了メッセージの通知とリターン
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

			MessageBox( hWnd, reinterpret_cast<LPCTSTR>( lpMsgBuf), "設定用DLLが見つかりません", MB_OK | MB_ICONINFORMATION );

			LocalFree( lpMsgBuf );

		}
		else
		{
			LPDOCUSTOMIZE lpfnDoCustomize;

			lpfnDoCustomize = reinterpret_cast<LPDOCUSTOMIZE>( GetProcAddress( hSetReg, _T( "DoCustomize")));
			if( NULL == lpfnDoCustomize)
			{
				MessageBox( hWnd, _T( "設定用DLLが不正です。"), _T( "SetReg"), MB_OK);
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
