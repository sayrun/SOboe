# Microsoft Developer Studio Project File - Name="SetReg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=SetReg - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SetReg.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SetReg.mak" CFG="SetReg - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SetReg - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "SetReg - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SetReg - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386

!ELSEIF  "$(CFG)" == "SetReg - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug/SetReg.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SetReg - Win32 Release"
# Name "SetReg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Base64.cpp
# End Source File
# Begin Source File

SOURCE=.\ChipExDllSettingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomExtPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DesignPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Dll_IF.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawExLib.cpp
# End Source File
# Begin Source File

SOURCE=.\DxlEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtDllPage.cpp
# End Source File
# Begin Source File

SOURCE=.\IconImgList.cpp
# End Source File
# Begin Source File

SOURCE=.\InterfacePage.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoChip.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoData.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkExtPage.cpp
# End Source File
# Begin Source File

SOURCE=.\NetworkPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\ProfileData.cpp
# End Source File
# Begin Source File

SOURCE=.\SecurityPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SetReg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetReg.def
# End Source File
# Begin Source File

SOURCE=.\SetReg.rc
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TempletPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Base64.h
# End Source File
# Begin Source File

SOURCE=.\ChipArg_IF.h
# End Source File
# Begin Source File

SOURCE=.\ChipExDllSettingDialog.h
# End Source File
# Begin Source File

SOURCE=.\CustomExtPage.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeSheet.h
# End Source File
# Begin Source File

SOURCE=.\DDData.h
# End Source File
# Begin Source File

SOURCE=.\DesignPage.h
# End Source File
# Begin Source File

SOURCE=.\Dll_IF.h
# End Source File
# Begin Source File

SOURCE=.\DrawEx_IF.h
# End Source File
# Begin Source File

SOURCE=.\DrawExLib.h
# End Source File
# Begin Source File

SOURCE=.\DrawExLib.inl
# End Source File
# Begin Source File

SOURCE=.\DxlEdit.h
# End Source File
# Begin Source File

SOURCE=.\ExtDllPage.h
# End Source File
# Begin Source File

SOURCE=.\IconImgList.h
# End Source File
# Begin Source File

SOURCE=.\InterfacePage.h
# End Source File
# Begin Source File

SOURCE=.\MemoChip.h
# End Source File
# Begin Source File

SOURCE=.\MemoChip.inl
# End Source File
# Begin Source File

SOURCE=.\MemoData.h
# End Source File
# Begin Source File

SOURCE=.\MemoData.inl
# End Source File
# Begin Source File

SOURCE=.\MemoListBox.h
# End Source File
# Begin Source File

SOURCE=.\NetworkExtPage.h
# End Source File
# Begin Source File

SOURCE=.\NetworkPage.h
# End Source File
# Begin Source File

SOURCE=.\OleDrop.h
# End Source File
# Begin Source File

SOURCE=.\ProfileData.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SecurityPage.h
# End Source File
# Begin Source File

SOURCE=.\SetReg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TempletPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\postit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SetReg.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
