@ECHO OFF
CLS

SET RELEASE=Game


CD "Texture Compiler"
ECHO Compiling Texture Compiler...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Compiling Texture List...
java TextureCompiler -d"../../textures/" -i"../animations.ini" -o"../../Data/textures.ini"
ECHO Done Compiling Texture List!
CD "../../"
ECHO.
ECHO Copying Texture List...

ECHO.
CD "Builder 2.0"
ECHO Compiling Map Builder...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Building Maps...
java Builder -d../../Maps -iuni -owrl -m../../Maps -t../../Data/textures.ini -h"../../Textures/Height Maps/"
ECHO Done Building Maps!
CD "../../"

ECHO.
PAUSE
