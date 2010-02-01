
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                    PropertyDictionary                                   //
//*****************************************************************************************//

inline double asDouble (char *string) {
	if (string == NULL || *string == '\0') return 0.0; //Since sscanf fails on empty string...
	if (*string == '-' && *(string+1) == '\0') return 0.0;
	double answer; sscanf (string, "%lf", &answer); return answer;
}

inline long asLong (char *string) {
	if (string == NULL || *string == '\0') return 0.0; //Since sscanf fails on empty string...
	if (*string == '-' && *(string+1) == '\0') return 0.0;
	long answer; sscanf (string, "%d", &answer); return answer;
}

char *PropertyDictionary::valueAt (char *key) {CString *value = get (key); return value == NULL ? "" : (char *) (LPCTSTR) *value;}
double PropertyDictionary::doubleValueAt (char *key) {return asDouble (valueAt (key));}
long PropertyDictionary::longValueAt (char *key) {return asLong (valueAt (key));}

void PropertyDictionary::valueAtPut (char *key, char *value) {
//if (stricmp (key, "_light") == 0 || stricmp (key, "color") == 0) {
//	::log ("");
//}
	CString *oldValue = ::get (properties, key);
	if (oldValue != NULL) delete oldValue;
	::put (properties, key, new CString (value));
}
