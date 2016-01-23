// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__62DC9F33_97AD_4EA0_8E2E_8C7FC0F02DC4__INCLUDED_)
#define AFX_STDAFX_H__62DC9F33_97AD_4EA0_8E2E_8C7FC0F02DC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// この位置にヘッダーを挿入してください
#define WIN32_LEAN_AND_MEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します

#include <windows.h>
#include <process.h>
#pragma comment(lib, "wininet.lib")
#include <ShellApi.h>
#pragma comment(lib, "shell32.lib")
#include "CommCtrl.h"
#pragma comment(lib, "comctl32.lib")

#include <stdlib.h>
#include <limits.h>
#include <tchar.h>

#include <comdef.h>
#include <tchar.h>

#import "c:\winnt\system32\msxml3.dll"

#define	_ICON_WIDTH	16
#define _ICON_HEIGHT	16

// TODO: プログラムで必要なヘッダー参照を追加してください。

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__62DC9F33_97AD_4EA0_8E2E_8C7FC0F02DC4__INCLUDED_)
