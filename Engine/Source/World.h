#pragma once

#include "Includes.h"
#include "Object.h"
#include "Environment.h"
#include "Geometry.h"
#include "Pool.h"
#include "Rotator.h"
#include "Sprite.h"
#include "Translator.h"
#include "Vehicle.h"
#include "Waypoint.h"
#include "Terrain.h"

class World {
public:
	World() : skybox(NULL) {
		startPosition = Point(0, 0, 0);
	}

	~World() {
		unsigned int i;
		if(skybox != NULL) { delete skybox; }
		delete [] sortedObjects;
		delete [] sortedWater;
		delete [] sortedSprites;
		for(i=0;i<objects.size();i++) {
			delete objects.at(i);
		}
		for(i=0;i<water.size();i++) {
			delete water.at(i);
		}
		for(i=0;i<sprites.size();i++) {
			delete sprites.at(i);
		}
		for(i=0;i<waypoints.size();i++) {
			delete waypoints.at(i);
		}
	}
	
	void tick();
	void draw();
	
	void import(char * fileName, vector<Texture *> & textures, vector<char *> & heightMaps, vector<AnimatedTexture *> & animatedTextures);

private:
	bool compareDistance(const Object & x, const Object & y);
	void sortObjects();
	void sortWater();
	void sortSprites();
	bool checkUnderWater();
	bool underWater;

public:
	Point playerPosition;
	Point startPosition;
	Environment * skybox;
	vector<Object *> objects;
	vector<Pool *> water;
	vector<Sprite *> sprites;
	vector<Waypoint *> waypoints;

private:
	Object ** sortedObjects;
	Pool ** sortedWater;
	Sprite ** sortedSprites;
};
