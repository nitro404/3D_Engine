#include "Methods.h"

char * strsub(const char * str, int start, int end) {
	char * sub = NULL;
	if(str == NULL || strlen(str) == 0) {
		sub = new char[1];
		sub[0] = '\0';
		return sub;
	}
	if(start < 0) { start = 0; }
	if(end > (int)(strlen(str))-1) { end = (int)(strlen(str))-1; }
	
	int j=0;
	
	sub = new char[end-start+2];

	for(int i=start;i<=end;i++) {
		sub[j++] = str[i];
	}
	
	sub[j] = '\0';
	
	return sub;
}
