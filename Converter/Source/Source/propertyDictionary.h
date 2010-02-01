//*****************************************************************************************//
//                                    PropertyDictionary                                   //
//*****************************************************************************************//

#ifndef propertyDictionaryModule
#define propertyDictionaryModule 

declareDictionary (CString);
 
#define loopPropertyDictionary(name,value,dictionary) \
	loopDictionary (name, value, CString, (dictionary).properties)

class PropertyDictionary {
public:
	CStringDictionary properties;

	PropertyDictionary () {}
	~PropertyDictionary () {deleteCStringDictionaryEntries (properties);}

	long size () {return properties.size ();}

	void log () {
		::log ("\nProperties");
		loopPropertyDictionary (name, value, *this)
			::log ("\n\t%s => \"%s\"", name, (LPCTSTR) *value);
		endloop
	}

	CString *get (char *key) {return ::get (properties, key);} //Returns NULL if there is no key...

	void put (char *key, char *value) {put (key, new CString (value));}
	void put (char *key, const char *value) {put (key, new CString (value));}
	void put (char *key, CString *value) {
		CString *oldValue = ::get (properties, key);
		if (oldValue != NULL) delete oldValue;
		::put (properties, key, value);
	}
	void removeKey (char *key) {::removeKey (properties, key);}

	char *valueAt (char *key); //Returns "" if there is no key...
	double doubleValueAt (char *key); //Returns 0.0 if there is no key...
	long longValueAt (char *key); //Returns 0 if there is no key...
	void valueAtPut (char *key, char *value);	
};

inline char *asString (CString *string) {return string == NULL ? "NULL" : (char *) (LPCTSTR) *string;}

#endif 
