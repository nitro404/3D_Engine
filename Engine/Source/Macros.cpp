#include "Includes.h"

bool isTrue(const char * str) {
	return	(str != NULL && strlen(str) > 0) &&
			(str[0] == '1' ||
			 str[0] == 'y' ||
			 str[0] == 'Y' ||
			 str[0] == 't' ||
			 str[0] == 'T' ||
			 (strlen(str) > 1 && _stricmp(str, "on") == 0));
}

void prompt(const char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer, 256, message, parameters);
		va_end(parameters);
		MessageBox(NULL, buffer, "Message", MB_OK);
	}
}

void quit(const char * message, ...) {
	if(message != NULL) {
		char buffer[256];
		va_list parameters;
		va_start(parameters, message);
		vsprintf_s(buffer,256, message, parameters);
		va_end(parameters);
		MessageBox(NULL, buffer, "Terminating", MB_OK);
	}
	exit(0);
}
