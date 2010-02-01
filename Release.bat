@ECHO OFF
CLS

SET RELEASE=Game

ECHO Creating new release of Game in folder "%RELEASE%"...

REM ECHO.
REM ECHO Building Maps...
REM Builder\Release\builder.exe -dMaps
REM ECHO Done Building!

ECHO.
ECHO Copying Maps...
XCOPY "Maps\*.wrl" "%RELEASE%\Maps" /I /Y

ECHO.
ECHO Copying Textures...
XCOPY "Textures" "%RELEASE%\Textures" /I /Y

ECHO.
ECHO Copying Data Files...
XCOPY "Data" %RELEASE% /I /Y /S

ECHO.
ECHO Copying Game Executable...
XCOPY "Engine\Release\game.exe"  %RELEASE% /I /Y
REN "%RELEASE%\game.exe" "Game.exe"

DEL log

ECHO.
ECHO Done!
ECHO.

PAUSE
