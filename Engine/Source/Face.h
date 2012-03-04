#ifndef FACE_H
#define FACE_H

#include "Point.h"
#include "Colour.h"
#include "Texture.h"
#include "Shader.h"

class Face {
public:
	Face() : texture(NULL), translationLocation(0), translationSpeed(0) { }

	virtual ~Face() {
		for(unsigned int i=0;i<points.size();i++) {
			delete points.at(i);
		}
	}

	virtual void tick() { }
	virtual void draw();
	virtual void draw(Colour & Colour);
	virtual void draw(Colour & Colour, bool drawBothSides);

	virtual void import(ifstream & input, vector<Texture *> & textures);

public:
	Texture * texture;
	vector<GamePoint *> points;
	double translationLocation;
	double translationSpeed;
};

#endif // FACE_H
