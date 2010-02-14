@ECHO OFF
CLS

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

ECHO.
PAUSE
