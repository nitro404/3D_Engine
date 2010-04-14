#pragma once

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

#include "GL/glew.h"
#include "GL/glut.h"

#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glew32.lib")

bool isTrue(const char * str);
void prompt(const char *message, ...);
void quit(const char *message, ...);
