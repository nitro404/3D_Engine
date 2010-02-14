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
java TextureCompiler -d"../../textures/" -i"../animations.ini" -o"../textures.ini"
ECHO Done Compiling Texture List!
CD "../../"
ECHO.
ECHO Copying Texture List...
XCOPY "Texture Compiler\textures.ini" "Builder 2.0" /I /Y
XCOPY "Texture Compiler\textures.ini" "Data" /I /Y

ECHO.
PAUSE
