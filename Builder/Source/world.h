//*****************************************************************************************//
//                                        World                                            //
//*****************************************************************************************//

#ifndef worldModule
#define worldModule 

class WorldcraftObject; //forward declaration

class World {
public:
	World ();
	~World ();

	ObjectCollection objects;
	Point startPosition;

	static void setup ();
	static void wrapup ();

	static World *read (); //Prompts for a file name to read a ".uni" file from and reads it into new world.
	World *World::write (); //Prompts for a file name to write a ".wrl" file into and writes the world into that file.
	World *World::write (char * outputFilename);

	void log ();
public:
	static World *import (char *filename);
	void import (ifstream &input);

	void export (char *filename);
	void export (ofstream &output);
};

#endif //worldModule