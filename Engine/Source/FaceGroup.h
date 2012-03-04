#ifndef FACE_GROUP_H
#define FACE_GROUP_H

#include "Point.h"

#define USE_QUADS							0
#define USE_STRIPS							1
#define USE_BUFFERS							2
#define TERRAIN_IMPLEMENTATION USE_BUFFERS

class SubGroup;

class FaceGroup
{
private:
	GLuint	   verticesBuffer;

	vector<SubGroup*> groups;

	int width;
	int height;
public:
	FaceGroup(GamePoint * vertices, int width, int height, int squareSections);
	~FaceGroup(void);

	void updateBuffers(GamePoint * vertices);

	void draw();
};

class SubGroup
{
private:
	GamePoint* vertices;

	unsigned int indicesSize;
	unsigned int* indices;
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

#endif // FACE_GROUP_H
