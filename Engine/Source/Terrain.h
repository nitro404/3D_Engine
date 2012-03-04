#ifndef TERRAIN_H
#define TERRAIN_H

#include "Object.h"
#include "FaceGroup.h"

class Terrain : public Object {
public:
	Terrain();
	~Terrain();

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps, vector<Shader *> shaders);

public:
	Shader * shader;

private:
	double scaleHeight(int x, int y, int * heightMapData);
	double getHeightAt(int x, int y, int * heightMapData);

	char * name;
	Texture * textureMap;
	char * heightMap;
	int width;
	int height;
	int tiled;
	GamePoint * points;
	FaceGroup * group;
};

#endif // TERRAIN_H
