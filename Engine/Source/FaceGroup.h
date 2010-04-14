#pragma once

#include "Point.h"

#define USE_QUADS							0 // works
#define USE_STRIPS							1 // works
#define USE_BUFFERS							2 // doesn't work yet
#define TERRAIN_IMPLEMENTATION USE_STRIPS

class SubGroup;

class FaceGroup
{
private:
	GamePoint* vertices;
	GLuint	   verticesBuffer;

	vector<SubGroup*> groups;

	int width;
	int height;
public:
	FaceGroup(GamePoint * vertices, int width, int height, int squareSections);
	~FaceGroup(void);

	void updateBuffers();

	void draw();
};

class SubGroup
{
private:
	GamePoint* vertices;

	int* indices;
	GLuint indicesBuffer;
	
	int width;
	int height;

public:

	SubGroup(GamePoint * vertices, int verticesWidth, int verticesHeight, int xStart, int yStart, int xEnd, int yEnd);
	SubGroup::~SubGroup(void);
	
	void draw(GLuint verticesBuffer);

	void drawQuads();
	void drawStrips();
	void drawBuffers(GLuint verticesBuffer);

};