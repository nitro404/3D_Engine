//*****************************************************************************************//
//                                        World                                            //
//*****************************************************************************************//

#ifndef worldModule
#define worldModule 

declareDictionary (Texture);

class World {
public:
	Point startPosition;
	ObjectCollection objects;
	SpriteCollection sprites;
	WaypointCollection waypoints;
	TextureCollection textures; //This is necessary if the world is to own the texture objects.

	World () {};
	~World () {
		deleteObjectCollectionEntries(objects); 
		deleteSpriteCollectionEntries(sprites);
		deleteWaypointCollectionEntries(waypoints);
		deleteTextureCollectionEntries(textures);
		
	}

	static void setup ();
	static void wrapup ();

	void tick ();
	void draw ();

	void read (); //Prompts for a file name to read a ".wrl" file from and reads it into a new world...
	void import (char * fileName);
	void import (ifstream &input);
	void unload (); //Makes sure all textures are unloaded prior to deleting the world.
};

#endif //worldModule