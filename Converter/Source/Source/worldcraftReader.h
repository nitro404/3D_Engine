//95.402 Wilf LaLonde: Student development Worldcraft to Student world converter.
//This program creates world (or level) files for use by the game engine.

//*****************************************************************************************//
//                                  Worldcraft Reader                                      //
//*****************************************************************************************//

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#ifndef worldcraftReaderModule
#define worldcraftReaderModule 

//*****************************************************************************************//
//                                    Worldcraft Reader                                    //
//*****************************************************************************************//

//The reader reads in raw worldcraft data that describes objects (brushes they call them) in 
//terms of planes (that we'll call a worldcraft side plane since it is not at all what a 
//standard plane looks like). The reader reads all this information in, converts the side 
//plane data into coordinates for faces and returns a vector of worldcraft objects (a map 
//that contains  everything worldcraft provided as key/value pairs (both keys and values are
//strings) except for one additional key not provided by worldcraft:
//
//		"objects" with an ObjectCollection as value (each object contains a FaceCollection).
//
//It is the user's responsibility to delete all the keys and values when a worldcraft
//object is deleted. Use deleteWorldcraftGroup (aWorldCraftObject) to do this properly 
//for you.

//*****************************************************************************************//
//                                    WorldcraftGroup                                      //
//*****************************************************************************************//

//A worldcraft group is a dictionary/map. The key is a string and the value is a pointer (usually
//"char *" but sometimes (Object *"). The map owns the keys and values so that deleting the map
// will delete the strings and the pointers (but not what the pointers points at).

typedef map <string, void *> WorldcraftGroup;
typedef WorldcraftGroup::iterator WorldcraftGroupIterator;
//inline void deleteWorldcraftGroupEntries (WorldcraftGroup *collection) {..} //Moved down to avoid a forward reference.
inline char *get (WorldcraftGroup *group, char *key) {
	WorldcraftGroupIterator result = group->find (key);
	if (result == group->end ()) return NULL; //Key not there.
	return (char *) result->second;
}

//*****************************************************************************************//
//                               WorldcraftGroupCollection                               //
//*****************************************************************************************//

//The worldcraft group collection is a vector of worldcraft groups (each a dictionary/map).

declareCollection (WorldcraftGroup);

//*****************************************************************************************//
//                                    WorldcraftObject                                     //
//*****************************************************************************************//

class WorldcraftObject {
public:
	char *type;
	WorldcraftGroup *properties; //A shared pointer owned (and deleted) by a WorldcraftObjectCollection.
	FaceCollection *faces;
	WorldcraftObject () {type = NULL; properties = NULL; faces = NULL;};
	WorldcraftObject (FaceCollection *faces) {type = NULL; properties = NULL; this->faces = faces;};
	~WorldcraftObject () {if (faces != NULL) deleteFaceCollection (faces);}
	void log ();

	//The following routines extract a property from a property dictionary. Property keys and
	//property values are strings. If a property is missing, NULL is returned in which case
	//the default is used instead.
	Point asPointProperty (char *propertyName, char *propertyDefault);
	char *asStringProperty (char *propertyName, char *propertyDefault);
	long asIntegerProperty (char *propertyName, char *propertyDefault);
	double asDoubleProperty (char *propertyName, char *propertyDefault);
};

declareCollection (WorldcraftObject);

//The following belongs in the WorldcraftGroup section but references WorldcraftObjectCollection defined in the line above.

inline void deleteWorldcraftGroup (WorldcraftGroup *collection) {
	for (WorldcraftGroupIterator next = collection->begin (); next != collection->end (); ++next) {
		const char *key = (*next).first.c_str (); void *value = (*next).second;
		//We don't need to delete the key (a string) or the value itself (a pointer)
		//but we do need to delete what the pointer points at.
		if (stricmp (key, "objects") == 0)
			deleteWorldcraftObjectCollection ((WorldcraftObjectCollection *) value); //It's a collection of objects.
		else 
			delete [] (char *) value; //It's a "char *"; i.e., a c string...
	} 
	delete collection;
}

//*****************************************************************************************//
//                                       The API                                           //
//*****************************************************************************************//

WorldcraftGroupCollection *worldcraftReader (char *filename);

#endif