#ifndef _FACE_H
#define _FACE_H 

#include "Colour.h"
#include "Texture.h"

struct GamePoint {
	double x, y, z;
	double tx, ty;
};

class Face {
public:
	Texture * texture;
	vector<GamePoint *> points;

	Face() {
		texture = NULL;
	}

	virtual ~Face() {
		for(unsigned int i=0;i<points.size();i++) {
			delete points.at(i);
		}
	}

	virtual void tick() { }
	virtual void draw();
	virtual void draw(Colour & Colour);

	virtual void import(ifstream & input, vector<Texture *> & textures);
};

#endif
