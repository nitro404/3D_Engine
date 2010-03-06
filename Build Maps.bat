@ECHO OFF
CLS

SET TEXTURE_COMPILER_DIR=Texture Compiler
SET MAP_BUILDER_DIR=Builder 2.0

SET TEXTURE_COMPILER_ARGS=-t"../../textures/" -m"../../textures/height maps/" -a"../animations.ini" -h"../heightmaps.ini" -o"../../Data/textures.ini"
SET MAP_BUILDER_ARGS=-d../../Maps -iuni -owrl -m../../Maps -t../../Data/textures.ini -h"../../Textures/Height Maps/"

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
PAUSE
