
#ifndef loggingModule
#define loggingModule 

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include <stdio.h> //For file manipulation.
#include <windows.h> //For va_start, ShowCursor.
#include <stdlib.h> //For _fullpath.
#include <mmsystem.h> //For time

//*****************************************************************************************//
//                                      Externals                                          //
//*****************************************************************************************//

void clearLog ();
double timeNow ();
void log (char *message, ...);
void prompt (char *message, ...);
void quit (char *message, ...);
#define halt quit
char *asString (char *message, ...);

#endif //loggingModule