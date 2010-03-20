#pragma once

#include "Point.h"

#define USE_QUADS							0 // works
#define USE_STRIPS							1 // works
#define USE_FACE_GROUPS						2 // doesn't work yet
#define USE_FACE_GROUPS_AND_FRUSTUM_CULLING 3 // doesn't work yet
#define TERRAIN_IMPLEMENTATION USE_STRIPS

class FaceGroup
{
private:
	GamePoint* vertices;
	int width;
	int height;
public:
	FaceGroup(GamePoint * vertices, int width, int height);
	~FaceGroup(void);

	void draw();

	void drawQuads();
	void drawStrips();
	void drawFrustrums();
	void drawFull();
};
