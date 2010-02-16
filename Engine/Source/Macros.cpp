#include "Includes.h"

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
