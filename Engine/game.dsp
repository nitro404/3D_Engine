# Microsoft Developer Studio Project File - Name="game" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=game - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "game.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "game.mak" CFG="game - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "game - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "game - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "game - Win32 Release"

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
# ADD BASE RSC /l 0x1009 /d "NDEBUG"
# ADD RSC /l 0x1009 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "game - Win32 Debug"

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
# ADD CPP /nologo /w /W0 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x1009 /d "_DEBUG"
# ADD RSC /l 0x1009 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 glut32.lib glu32.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "game - Win32 Release"
# Name "game - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Source\AnimatedFace.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\AnimatedTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Colour.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Face.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\InputManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Macros.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Plane.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Point.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Variable.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Variables.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\World.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Source\AnimatedFace.h
# End Source File
# Begin Source File

SOURCE=.\Source\AnimatedTexture.h
# End Source File
# Begin Source File

SOURCE=.\Source\Camera.h
# End Source File
# Begin Source File

SOURCE=.\Source\Colour.h
# End Source File
# Begin Source File

SOURCE=.\Source\Face.h
# End Source File
# Begin Source File

SOURCE=.\Source\Game.h
# End Source File
# Begin Source File

SOURCE=.\Source\Includes.h
# End Source File
# Begin Source File

SOURCE=.\Source\InputManager.h
# End Source File
# Begin Source File

SOURCE=.\Source\Plane.h
# End Source File
# Begin Source File

SOURCE=.\Source\Player.h
# End Source File
# Begin Source File

SOURCE=.\Source\Point.h
# End Source File
# Begin Source File

SOURCE=.\Source\Sound.h
# End Source File
# Begin Source File

SOURCE=.\Source\Texture.h
# End Source File
# Begin Source File

SOURCE=.\Source\Transformation.h
# End Source File
# Begin Source File

SOURCE=.\Source\Variable.h
# End Source File
# Begin Source File

SOURCE=.\Source\Variables.h
# End Source File
# Begin Source File

SOURCE=.\Source\World.h
# End Source File
# End Group
# Begin Group "GLUT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\GLUT\GL.H
# End Source File
# Begin Source File

SOURCE=.\Source\GLUT\GLU.H
# End Source File
# Begin Source File

SOURCE=.\Source\GLUT\glut.h
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Source\Environment.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Environment.h
# End Source File
# Begin Source File

SOURCE=.\Source\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\Source\Object.h
# End Source File
# Begin Source File

SOURCE=.\Source\Pool.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Pool.h
# End Source File
# Begin Source File

SOURCE=.\Source\Rotator.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Rotator.h
# End Source File
# Begin Source File

SOURCE=.\Source\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Sprite.h
# End Source File
# Begin Source File

SOURCE=.\Source\Translator.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Translator.h
# End Source File
# Begin Source File

SOURCE=.\Source\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Vehicle.h
# End Source File
# Begin Source File

SOURCE=.\Source\Waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\Source\Waypoint.h
# End Source File
# End Group
# End Target
# End Project
