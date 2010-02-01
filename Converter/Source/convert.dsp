# Microsoft Developer Studio Project File - Name="convert" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=convert - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "convert.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "convert.mak" CFG="convert - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "convert - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "convert - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "convert - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "convert - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /Gm /GX /ZI /Od /I "XSIFTK_3.6.2\export\h\FTK" /I "XSIFTK_3.6.2\export\h\Core" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__XSI_APPLICATION_" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 "XSIFTK_3.6.2\export\lib\XSIFtkd.lib" opennurbs_vc60.lib vfw32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"libc.lib" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "convert - Win32 Release"
# Name "convert - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\boundingBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\extensionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\fileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\game.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\logging.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\main.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\points.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\propertyDictionary.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\rhinoReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\softimageReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\universalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\worldcraftFGDReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\worldcraftReader.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Source\boundingBox.h
# End Source File
# Begin Source File

SOURCE=.\Source\camera.h
# End Source File
# Begin Source File

SOURCE=.\Source\extensionManager.h
# End Source File
# Begin Source File

SOURCE=.\Source\fileDialog.h
# End Source File
# Begin Source File

SOURCE=.\Source\game.h
# End Source File
# Begin Source File

SOURCE=.\Source\includes.all
# End Source File
# Begin Source File

SOURCE=.\Source\logging.h
# End Source File
# Begin Source File

SOURCE=.\Source\plane.h
# End Source File
# Begin Source File

SOURCE=.\Source\points.h
# End Source File
# Begin Source File

SOURCE=.\Source\propertyDictionary.h
# End Source File
# Begin Source File

SOURCE=.\Source\rhinoReader.h
# End Source File
# Begin Source File

SOURCE=.\Source\softimageReader.h
# End Source File
# Begin Source File

SOURCE=.\Source\texture.h
# End Source File
# Begin Source File

SOURCE=.\Source\transformations.h
# End Source File
# Begin Source File

SOURCE=.\Source\universalObject.h
# End Source File
# Begin Source File

SOURCE=.\Source\worldcraftFGDReader.h
# End Source File
# Begin Source File

SOURCE=.\Source\worldcraftReader.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Debugging"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FGDlog
# End Source File
# Begin Source File

SOURCE=.\out.txt
# End Source File
# End Group
# End Target
# End Project
