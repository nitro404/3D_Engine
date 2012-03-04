#ifndef WORLD_H
#define WORLD_H

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
	
	void update(double timeElapsed);
	void draw();
	void drawSkybox();
	
	void import(const char * fileName, vector<Texture *> & textures, vector<char *> & heightMaps, vector<AnimatedTexture *> & animatedTextures, vector<Shader *> shaders);

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

	bool cullingEnabled;

private:
	Pool ** sortedWater;
	Sprite ** sortedSprites;
	int * spriteWater;
	int underWater;
};

#endif // WORLD_H
