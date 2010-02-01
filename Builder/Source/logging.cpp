
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "logging.h"

//*****************************************************************************************//
//                                       Timing                                            //
//*****************************************************************************************//

long timeNow () {return timeGetTime ();} //Time in milliseconds.


//*****************************************************************************************//
//                                Debugging Facilities                                     //
//*****************************************************************************************//

void clearLog () { 
	static char logFileName [255] = "";
	if (*((char *) &logFileName) == '\0') _fullpath ((char *) &logFileName, ".\\log", sizeof (logFileName));
	FILE *file = fopen ((char *) &logFileName, "w"); if (file == NULL) return;
	fclose (file);					 
}

#define setupStaticBuffer() \
	static char buffer [500]; va_list parameters; \
	va_start (parameters, message); \
	vsprintf (buffer, message, parameters); \
	va_end (parameters) 

void log (char *message, ...) { 
	//Example use: log ("\nInteger %d float %4.2f hex %8.8x.", 10, 1.2, 16);
	setupStaticBuffer ();
	static char logFileName [255] = "";
	if (*((char *) &logFileName) == '\0') 
		_fullpath ((char *) &logFileName, ".\\log", sizeof (logFileName));
	//MessageBox (GetActiveWindow (), buffer, "      Message      ", MB_OK);
	FILE *file = fopen ((char *) &logFileName, "a");
	if (file == NULL) return;
	fprintf (file, "%s", buffer);
	fclose (file);					 
}

void prompt (char *message, ...) { 
	//Use like log.
	setupStaticBuffer ();
	MessageBox (/*GetActiveWindow ()*/ NULL, buffer, "      Message      ", MB_OK);
}

void quit (char *message, ...) { 
	//Use like log.
	setupStaticBuffer ();
	::log ("\n%s", buffer);
	MessageBox (/*GetActiveWindow ()*/ NULL, buffer, "      Message      ", MB_OK);
	exit (0);
}
 
char *asString (char *message, ...) { 
	//Use like log.
	setupStaticBuffer ();
	return &buffer [0]; //Careful: Two asStrings in a row use the same buffer.
}
