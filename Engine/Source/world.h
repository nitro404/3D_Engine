#ifndef _WORLD_H
#define _WORLD_H 

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
	TextureCollection textures;
	AnimatedTextureCollection animatedTextures;

	World() {
		skybox = NULL;
	}

	~World() {
		if(skybox != NULL) { delete skybox; }
		delete [] sortedObjects;
		delete [] sortedWater;
		delete [] sortedSprites;
		deleteObjectCollectionEntries(objects);
		deletePoolCollectionEntries(water);
		deleteSpriteCollectionEntries(sprites);
		deleteWaypointCollectionEntries(waypoints);
		deleteTextureCollectionEntries(textures);
		deleteAnimatedTextureCollectionEntries(animatedTextures);


	}
	
	void tick();
	void draw();
	
	void import(char * fileName);
};

#endif
