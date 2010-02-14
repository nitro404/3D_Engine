#ifndef _FACE_H
#define _FACE_H 

#include "Colour.h"

struct GamePoint {
	double x, y, z;
	double tx, ty;
};

declareCollection(GamePoint);

class Face {
public:
	Texture * texture;
	GamePointCollection points;

	Face() {
		texture = NULL;
	}

	virtual ~Face() {
		deleteGamePointCollectionEntries(points);
	}

	virtual void tick() { }
	virtual void draw();
	virtual void draw(Colour & Colour);

	virtual void import(ifstream & input, TextureCollection & textures);
};

declareCollection (Face);

#endif
