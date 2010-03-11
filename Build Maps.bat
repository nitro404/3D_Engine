@ECHO OFF
CLS

SET CONTENT_DIR=Content
SET DATA_DIR=Data
SET MAP_DIR=Maps

SET TEXTURE_COMPILER_DIR=Texture Compiler
SET MAP_BUILDER_DIR=Builder 2.0

SET TEXTURE_COMPILER_ARGS=-t"../../Content/Data/Textures/" -m"../../Content/Data/Height Maps/" -a"../animations.ini" -h"../heightmaps.ini" -o"../../Content/Data/textures.ini"
SET MAP_BUILDER_ARGS=-d"../../Maps/" -iuni -owrl -m"../../Maps/" -t"../../Content/Data/textures.ini" -h"../../Content/Data/Height Maps/"

CD "%TEXTURE_COMPILER_DIR%"
ECHO Compiling Texture Compiler...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Compiling Texture List...
java TextureCompiler %TEXTURE_COMPILER_ARGS%
ECHO Done Compiling Texture List!
CD "../../"
ECHO.
ECHO Copying Texture List...

ECHO.
CD "%MAP_BUILDER_DIR%"
ECHO Compiling Map Builder...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Building Maps...
java Builder %MAP_BUILDER_ARGS%
ECHO Done Building Maps!
CD "../../"

ECHO.
ECHO Copying Maps...
XCOPY "%MAP_DIR%\*.wrl" "%CONTENT_DIR%\%DATA_DIR%\%MAP_DIR%" /I /Y

ECHO.
PAUSE
