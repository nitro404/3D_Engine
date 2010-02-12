#include "includes.all"

void convertToLowercase (char *name) {
	for (long index = strlen (name) - 1; index >= 0; index--) {
		name [index] = tolower (name [index]);
	}
}