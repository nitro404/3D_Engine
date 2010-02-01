//*****************************************************************************************//
//                                     Utilities                                           //
//*****************************************************************************************//

#ifndef utilitiesModule
#define utilitiesModule 

//Macro "declareCollection" below declares a type of vector, e.g., declareCollection (Face);
//creates a vector of Face pointers.

//There are two ways to use such a collection

//	1. Typical way of using a collection:
//
//			FaceCollection faces; 
//			Face *face1 = ...; faces.push_back (face1); 
//			Face *face2 = ...; faces.push_back (face2);
//				...
//			loopVector (index, faces)
//				... faces [index] .... //provides an element of type Face *.
//			endloop
//			deleteFaceCollectionEntries (faces); //faces is deleted when the object containing it is deleted...

//	2. More unusual way (using a pointer to a collection):
//
//			FaceCollection *faces = new FaceCollection; 
//			Face *face1 = ...; faces->push_back (face1); 
//			Face *face2 = ...; faces->push_back (face2);
//				...
//			loopVector (index, *faces)
//				... (*faces) [index] .... //provides an element of type Face *.
//			endloop
//			deleteFaceCollection (faces); //deletes the entries and also faces itself...

#define declareCollection(type) \
	typedef vector <type *> type##Collection; \
	typedef vector <type *>::iterator type##CollectionIterator; \
		\
	inline void delete##type##CollectionEntries (type##Collection &collection) { \
		for (long index = 0; index < collection.size (); index++) { \
			delete collection [index]; \
		} \
	} \
	inline void delete##type##Collection (type##Collection *collection) { \
		delete##type##CollectionEntries (*collection); \
		delete collection; \
	}

#define loopVector(variable,vector) {for (long variable = 0; variable < (vector).size (); variable++) {
#define endloop }}



//Create a special kind for dealing with strings...
declareCollection (char); 
typedef charCollection StringCollection;
inline void deleteStringCollectionEntries (StringCollection &collection) {
	loopVector (index, collection)
		delete [] collection [index]; //What is different from the version above...
	endloop
}

inline void deleteStringCollection (StringCollection *collection) { 
	deleteStringCollectionEntries (*collection); 
	delete collection; 
} 

//Example use:
//	StringCollection test;
//	test.push_back("hello");
//	test.push_back("there");
//	test.push_back("you");
//	loopVector (index, test)
//		if (stricmp (test [index], "brick") == 0) ::log ("\nBrick is at index %d.", index);
//		::log ("\nString %d is \"%s\"...", index, test [index]);
//	endloop





//Macro "declareDictionary" below declares a type of map, e.g., declareDictionary (Face)
//creates a map that associates strings with Face pointers.

//There are two ways to use such a collection

//	1. Typical way of using a dictionary:
//
//			FaceDictionary faces; 
//			Face *face1 = ...; put (faces, "name1", face1); //Storing
//			Face *face2 = ...; put (faces, "name2", face2); 
//				...
//			Face *face = get (faces, "name1"); //Retrieving: get NULL if not found...
//			put (faces, "name1", face2); //Changing...
//			loopDictionary (faceKey, faceValue, faces, Face)
//				... faceValue .... //faceValue is locally declared by the loop macro to be type Face * and initialized...
//			endloop
//			deleteFaceDictionaryEntries (faces); //faces is deleted when the object containing it is deleted...

//	2. More unusual way (using a pointer to a collection):
//
//			FaceDictionary *faces = new FaceDictionary; 
//			As above but use "faces->" instead of "faces." or *faces instead of faces.
//			deleteFaceDictionary (faces); //deletes the entries and also faces itself...

#define loopDictionary(key,value,map,type) \
	{for (type##DictionaryIterator iterator = (map).begin (); iterator != (map).end (); ++iterator) { \
		const char *key = iterator->first.c_str (); \
		type *value = iterator->second;

#define declareDictionary(type) \
	typedef map <string, type *> type##Dictionary; \
	typedef type##Dictionary::iterator type##DictionaryIterator; \
		\
	inline void delete##type##DictionaryEntries (type##Dictionary &collection) { \
		loopDictionary (key, value, collection, type) \
			delete value; \
		endloop \
	} \
	inline void delete##type##Dictionary (type##Dictionary *collection) { \
		delete##type##DictionaryEntries (*collection); \
		delete collection; \
	} \
		\
	inline type *get (type##Dictionary &collection, char *key) { \
		type##DictionaryIterator result = collection.find (key); \
		if (result == collection.end ()) return NULL; /*Key not there.*/ \
		return result->second; \
	} \
	inline void put (type##Dictionary &collection, char *key, type *value) { \
		collection.insert (type##Dictionary::value_type (key, value)); \
	}

//Create a special kind for dealing with properties...
declareDictionary (char); 
typedef charDictionary StringDictionary;
inline void deleteStringDictionaryEntries (StringDictionary &collection) {
	loopDictionary (key, value, collection, char)
		delete [] value; //What is different from the version above...
	endloop
}

inline void deleteStringDictionary (StringDictionary *collection) { 
	deleteStringDictionaryEntries (*collection); 
	delete collection; 
} 

//If you need a forward reference to a collection, you will need the following macros.
//An example would be if class Object itself needs to keep track of a collection of sub-objects.
//In that case, you would need to declare a collection of objects but you will have difficulty
//doing this before the class definition...

#define preDeclareCollection(classOrStruct,type) \
	classOrStruct type; \
	typedef vector <type *> type##Collection;

#define preDeclareDictionary(classOrStruct,type) \
	classOrStruct type; \
	typedef vector <type *> type##Collection;


//Generic macros for reading universal files...
//Note: input.getline (line, 256, c) => picks up everything up to c exclusive and discards c.

#define SKIP_TO(character) input.getline (line, 256, character)
#define SKIP_TO_ENDLINE SKIP_TO ('\n')
#define SKIP_TO_COMMA SKIP_TO (',')
#define SKIP_TO_COLON SKIP_TO (':')
#define SKIP_TO_SEMICOLON SKIP_TO (';')
#define CLEAR_THE_LINE SKIP_TO_ENDLINE

//Useful utility...
void convertToLowercase (char *name);

#endif //utilitiesModule