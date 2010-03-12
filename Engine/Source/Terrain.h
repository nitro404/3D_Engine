#pragma once

#include "Object.h"

#define USE_QUADS							0 // works
#define USE_STRIPS							1 // works
#define USE_FACE_GROUPS						2 // doesn't work yet
#define USE_FACE_GROUPS_AND_FRUSTUM_CULLING 3 // doesn't work yet
#define TERRAIN_IMPLEMENTATION USE_QUADS

class Terrain : public Object {
public:
	Terrain();
	~Terrain();

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps);

public:
	DualTransformation * transformation;

private:
	double scaleHeight(int x, int y, int * heightMapData);
	double getHeightAt(int x, int y, int * heightMapData);

	char * name;
	Texture * textureMap;
	char * heightMap;
	int width;
	int height;
	int tiled;
	Point minPoint;
	Point maxPoint;
	GamePoint * points;

	void drawQuads();
	void drawStrips();
	void drawFrustrums();
	void drawFull();
};
