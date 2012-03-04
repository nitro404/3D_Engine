#ifndef INCLUDES_H
#define INCLUDES_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>    
#include <iostream> 
#include <fstream>
#include <time.h>
#include <vector>
#include <map>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

#define GLEW_STATIC

#include "GL/glew.h"
#include "GL/glut.h"

bool isTrue(const char * str);
void prompt(const char *message, ...);
void quit(const char *message, ...);

#endif // INCLUDES_H
