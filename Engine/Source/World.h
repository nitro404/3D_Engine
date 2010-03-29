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
#include "Waterfall.h"

class World {
public:
	World();
	~World();
	
	void tick();
	void draw();
	void drawSkybox();
	
	void import(char * fileName, vector<Texture *> & textures, vector<char *> & heightMaps, vector<AnimatedTexture *> & animatedTextures);

private:
	bool compareDistance(const Object & x, const Object & y);

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
	int * spriteWater;
	int underWater;
};
