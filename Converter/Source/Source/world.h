//*****************************************************************************************//
//                                        World                                            //
//*****************************************************************************************//

#ifndef worldModule
#define worldModule 

class Object {
public:
	FaceCollection faces;
	Object () {};
	~Object () {deleteFaceCollectionEntries (faces);}
	virtual void log ();
	virtual void export (::ofstream &output);

	void copyFaces (FaceCollection &faces);
};

declareCollection (Object);


class WorldcraftObject; //forward declaration

class World {
public:
	World ();
	~World ();

	ObjectCollection objects;
	Point startPosition;

	static void setup ();
	static void wrapup ();

	static World *read ();
	static World *read (char *filename);
	void finalize ();
	void export (char *filename);
	
	void OLDadd (WorldcraftObject &worldcraftObject);
	void add (WorldcraftObject &worldcraftObject);
	void log ();
};

#endif