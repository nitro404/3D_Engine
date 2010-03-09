#pragma once

#include "Object.h"

#define USE_QUADS							0
#define USE_STRIPS							1
#define USE_FACE_GROUPS						2
#define USE_FACE_GROUPS_AND_FRUSTUM_CULLING 3 
#define TERRAIN_IMPLEMENTATION USE_QUADS
//#define TERRAIN_IMPLEMENTATION -1

class Terrain : public Object {
	
public:
	Terrain();
	~Terrain();

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps);

private:
	double scaleHeight(int x, int y, int * heightMapData);

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
