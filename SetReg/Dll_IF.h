#ifndef	__DLL_IF_H__
#define	__DLL_IF_H__

#define	_SETREG_VERSION		0x0200

#ifdef	__cplusplus
extern "C" 
{
#endif
__declspec( dllexport ) UINT GetSetRegVersion( void);
__declspec( dllexport ) int DoCustomize( BOOL blSpec, HWND hWnd);
#ifdef	__cplusplus
}
#endif

#endif	// __DLL_IF_H__
