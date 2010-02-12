#include "logging.h"

double timeNow() {
	//Returns how much time has elapsed since the first call of this function... Accurate to a microsecond...
	static INT64 countsPerSecond; static INT64 oldTime; static bool firstTime = true;
	if (firstTime) {firstTime = false; QueryPerformanceCounter ((LARGE_INTEGER *) &oldTime); QueryPerformanceFrequency ((LARGE_INTEGER *) &countsPerSecond);}
	
	INT64 newTime; QueryPerformanceCounter ((LARGE_INTEGER *) &newTime);
	INT64 elapsedCounts = newTime - oldTime; 
	
	double seconds = (double) elapsedCounts / (double) countsPerSecond; //count / (counts / second) = seconds
	return seconds;
}

#define setupStaticBuffer() \
	static char buffer [500]; va_list parameters; \
	va_start (parameters, message); \
	vsprintf (buffer, message, parameters); \
	va_end (parameters) 

void prompt(char * message, ...) {
	setupStaticBuffer();
	MessageBox(NULL, buffer, "Message", MB_OK);
}

void quit(char * message, ...) {
	if(message != NULL) {
		setupStaticBuffer();
		MessageBox(NULL, buffer, "Terminating", MB_OK);
	}
	exit(0);
}
