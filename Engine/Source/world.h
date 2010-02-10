//*****************************************************************************************//
//                                        World                                            //
//*****************************************************************************************//

#ifndef worldModule
#define worldModule 

declareDictionary (Texture);

class World {
private:
	static bool compareDistance(const Object & x, const Object & y);
	void sortObjects();
	void sortWater();
	void sortSprites();
	Object ** sortedObjects;
	Pool ** sortedWater;
	Sprite ** sortedSprites;
	bool checkUnderWater() const;
	bool underWater;
public:
	static Point playerPosition;
	Point startPosition;
	Environment * skybox;
	ObjectCollection objects;
	PoolCollection water;
	SpriteCollection sprites;
	WaypointCollection waypoints;
	TextureCollection textures; //This is necessary if the world is to own the texture objects.

	World () {};
	~World () {
		skybox = NULL;
		deleteObjectCollectionEntries(objects);
		deletePoolCollectionEntries(water);
		deleteSpriteCollectionEntries(sprites);
		deleteWaypointCollectionEntries(waypoints);
		deleteTextureCollectionEntries(textures);
		delete [] sortedObjects;
		delete [] sortedWater;
		delete [] sortedSprites;
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