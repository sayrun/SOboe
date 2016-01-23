#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <prsht.h>
#include <commdlg.h>

#if(_WIN32_WINNT >= 0x0400)
#include <winsock2.h>
#include <Wincrypt.h>
#pragma comment( lib, "Advapi32.lib")
#else
#include <winsock.h>
#endif


#include <stdlib.h>
#include <process.h>
#include <mbstring.h>
#include <lmcons.h>

#include <string>
