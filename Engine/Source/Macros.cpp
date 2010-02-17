#include "Includes.h"

void prompt(char * message, ...) {
	if(message != NULL) {
		char * buffer;
		buffer = new char[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf(buffer, message, parameters);
		va_end(parameters);
		MessageBox(NULL, buffer, "Message", MB_OK);
		delete [] buffer;
	}
}

void quit(char * message, ...) {
	if(message != NULL) {
		char * buffer;
		buffer = new char[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf(buffer, message, parameters);
		va_end(parameters);
		MessageBox(NULL, buffer, "Terminating", MB_OK);
		delete [] buffer;
	}
	exit(0);
}
