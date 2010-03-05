#pragma once
#include "object.h"

class TerrainMap :
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
	TerrainMap(void);
	~TerrainMap(void);

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures);
};
