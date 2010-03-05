#pragma once
#include "object.h"

class Terrain :
	public Object
{
private:
	char* name;
	Texture* textureMap;
	char* heightMapFile;
	int width;
	int height;
	Point minPoint;
	Point maxPoint;
	GamePoint* points; // GamePoint[][]
	//int drawType;

	void drawQuads();
	void drawFrustrums();
	void drawFull();

public:
	Terrain(void);
	~Terrain(void);

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures);
};
