# Microsoft Developer Studio Project File - Name="SOboe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SOboe - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SOboe.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SOboe.mak" CFG="SOboe - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SOboe - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "SOboe - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "SOboe - Win32 Profile" ("Win32 (x86) Application" 用)
!MESSAGE "SOboe - Win32 AsmCode" ("Win32 (x86) Application" 用)
!MESSAGE "SOboe - Win32 AlphaBlend" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SOboe", BAAAAAAA"
# PROP Scc_LocalPath "Desktop"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SOboe - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SOboe - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "SOboe - Win32 Profile"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "SOboe___Win32_Profile"
# PROP BASE Intermediate_Dir "SOboe___Win32_Profile"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Profile"
# PROP Intermediate_Dir "Profile"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /profile /debug /machine:I386

!ELSEIF  "$(CFG)" == "SOboe - Win32 AsmCode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SOboe___Win32_AsmCode"
# PROP BASE Intermediate_Dir "SOboe___Win32_AsmCode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AsmCode"
# PROP Intermediate_Dir "AsmCode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /FAs /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SOboe - Win32 AlphaBlend"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "SOboe___Win32_AlphaBlend"
# PROP BASE Intermediate_Dir "SOboe___Win32_AlphaBlend"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AlphaBlend"
# PROP Intermediate_Dir "AlphaBlend"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /D "_ALPHABLEND" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _DXLVER=0x200 /D "_ALPHABLEND" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d WINVER=0x0500
# ADD RSC /l 0x411 /d "NDEBUG" /d WINVER=0x0500
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ENDIF 

# Begin Target

# Name "SOboe - Win32 Release"
# Name "SOboe - Win32 Debug"
# Name "SOboe - Win32 Profile"
# Name "SOboe - Win32 AsmCode"
# Name "SOboe - Win32 AlphaBlend"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AlphaBlendDialog.cpp

!IF  "$(CFG)" == "SOboe - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 Profile"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 AsmCode"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 AlphaBlend"

# PROP BASE Intermediate_Dir "Release4W2K"
# PROP Intermediate_Dir "Release4W2K"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base64.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\BaseEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CatalogList.cpp
# End Source File
# Begin Source File

SOURCE=.\CelControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ChipExLib.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DelListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawExLib.cpp
# End Source File
# Begin Source File

SOURCE=.\DxlControl.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtSettingDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GetMemoData.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\IconImgList.cpp
# End Source File
# Begin Source File

SOURCE=.\LinkEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoChip.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoData.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoEditSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoListNode.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoSendSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\NetExLib.cpp
# End Source File
# Begin Source File

SOURCE=.\NotifyWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDrop.cpp
# End Source File
# Begin Source File

SOURCE=.\PassWordCheckDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintChipDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinterDC.cpp
# End Source File
# Begin Source File

SOURCE=.\PWEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\RcvMemoData.cpp
# End Source File
# Begin Source File

SOURCE=.\ResizableMemoEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultCheckDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SendEditPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SenderInfoDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SendWorkerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SOboe.cpp
# End Source File
# Begin Source File

SOURCE=.\SOboe.rc
# End Source File
# Begin Source File

SOURCE=.\SpEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TimerEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\TimerEditPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\AlphaBlendDialog.h

!IF  "$(CFG)" == "SOboe - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 Profile"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 AsmCode"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "SOboe - Win32 AlphaBlend"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Base64.h
# End Source File
# Begin Source File

SOURCE=.\BaseEditPage.h
# End Source File
# Begin Source File

SOURCE=.\CatalogList.h
# End Source File
# Begin Source File

SOURCE=.\CelControl.h
# End Source File
# Begin Source File

SOURCE=.\ChipArg_IF.h
# End Source File
# Begin Source File

SOURCE=.\ChipEx_IF.h
# End Source File
# Begin Source File

SOURCE=.\ChipExLib.h
# End Source File
# Begin Source File

SOURCE=.\ChipExLib.inl
# End Source File
# Begin Source File

SOURCE=.\DataListDialog.h
# End Source File
# Begin Source File

SOURCE=.\DDData.h
# End Source File
# Begin Source File

SOURCE=.\DelListDialog.h
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

SOURCE=.\DxlControl.h
# End Source File
# Begin Source File

SOURCE=.\DxlControl.inl
# End Source File
# Begin Source File

SOURCE=.\ExtendEditPage.h
# End Source File
# Begin Source File

SOURCE=.\ExtSettingDialog.h
# End Source File
# Begin Source File

SOURCE=.\GetMemoData.h
# End Source File
# Begin Source File

SOURCE=.\GroupEntry.h
# End Source File
# Begin Source File

SOURCE=.\GroupEntry.inl
# End Source File
# Begin Source File

SOURCE=.\IconImgList.h
# End Source File
# Begin Source File

SOURCE=.\LinkEdit.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemoChip.h
# End Source File
# Begin Source File

SOURCE=.\MemoChip.inl
# End Source File
# Begin Source File

SOURCE=.\MemoChipList.h
# End Source File
# Begin Source File

SOURCE=.\MemoData.h
# End Source File
# Begin Source File

SOURCE=.\MemoData.inl
# End Source File
# Begin Source File

SOURCE=.\MemoEditSheet.h
# End Source File
# Begin Source File

SOURCE=.\MemoEditSheet.inl
# End Source File
# Begin Source File

SOURCE=.\MemoListBox.h
# End Source File
# Begin Source File

SOURCE=.\MemoListNode.h
# End Source File
# Begin Source File

SOURCE=.\MemoListNode.inl
# End Source File
# Begin Source File

SOURCE=.\MemoSendSheet.h
# End Source File
# Begin Source File

SOURCE=.\MemoSendSheet.inl
# End Source File
# Begin Source File

SOURCE=.\NetEx_IF.h
# End Source File
# Begin Source File

SOURCE=.\NetExLib.h
# End Source File
# Begin Source File

SOURCE=.\NotifyWnd.h
# End Source File
# Begin Source File

SOURCE=.\OleDrop.h
# End Source File
# Begin Source File

SOURCE=.\PassWordCheckDialog.h
# End Source File
# Begin Source File

SOURCE=.\PrintChipDialog.h
# End Source File
# Begin Source File

SOURCE=.\PrinterDC.h
# End Source File
# Begin Source File

SOURCE=.\PWEdit.h
# End Source File
# Begin Source File

SOURCE=.\RcvMemoData.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMemoEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResizableMemoEditDialog.inl
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ResultCheckDialog.h
# End Source File
# Begin Source File

SOURCE=.\ResultListBox.h
# End Source File
# Begin Source File

SOURCE=.\SendEditPage.h
# End Source File
# Begin Source File

SOURCE=.\SenderInfoDialog.h
# End Source File
# Begin Source File

SOURCE=.\SendWorkerDialog.h
# End Source File
# Begin Source File

SOURCE=.\SOboe.h
# End Source File
# Begin Source File

SOURCE=.\SOboe.inl
# End Source File
# Begin Source File

SOURCE=.\SpEdit.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TimerEdit.h
# End Source File
# Begin Source File

SOURCE=.\TimerEditPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\absence.ico
# End Source File
# Begin Source File

SOURCE=.\res\hide.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icons.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\og1.ico
# End Source File
# Begin Source File

SOURCE=.\RES\og2.ico
# End Source File
# Begin Source File

SOURCE=.\res\receive.ico
# End Source File
# Begin Source File

SOURCE=.\res\SOboe.ico
# End Source File
# Begin Source File

SOURCE=.\res\SOboe.rc2
# End Source File
# Begin Source File

SOURCE=.\RES\stop.ico
# End Source File
# Begin Source File

SOURCE=.\RES\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\treeimg.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\DxlMemo.txt
# End Source File
# Begin Source File

SOURCE=.\EtcMemo.txt
# End Source File
# Begin Source File

SOURCE=.\History.txt
# End Source File
# Begin Source File

SOURCE=.\NetMemo.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\RES\SOboe.manifest
# End Source File
# End Target
# End Project
