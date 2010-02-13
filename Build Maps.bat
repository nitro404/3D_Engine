@ECHO OFF
CLS

CD "Builder 2.0"
ECHO Compiling Map Builder...
javac src\*.java -d bin
ECHO Done Compiling!
ECHO.
CD bin
ECHO Building Maps...
java Builder -d../../Maps -iuni -owrl -m../../Maps -t../textures.ini
ECHO Done Building Maps!
CD "../../"

ECHO.
PAUSE
