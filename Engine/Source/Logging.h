#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h> //For file manipulation.
#include <windows.h> //For va_start, ShowCursor.
#include <stdlib.h> //For _fullpath.
#include <mmsystem.h> //For time

double timeNow();
void prompt(char *message, ...);
void quit(char *message, ...);

#endif
