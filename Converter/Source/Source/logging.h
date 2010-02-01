
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
long timeNow ();
void log (char *message, ...);
void prompt (char *message, ...);
void quit (char *message, ...);
void halt (char *message, ...);
enum QueryResult {Yes, No, Cancel};
QueryResult yesNoCancelQuery (char *message, ...);
bool booleanQuery (char *message, ...);
char *asString (char *message, ...);

#endif
