#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include "Colour.h"

class Environment : public Object {
public:
	Environment() {
		skyboxTextures = new Texture*[6];
	}

	~Environment() {
		delete [] name;
		delete [] skyboxTextures;
	}

	double distanceFrom(Point & p) const { return 0; }

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);

private:
	char * name;
	Colour ambientLight;
	int cloudLayers;
	Texture ** skyboxTextures;
	Colour skyColour;
	int surrounds;
};

declareCollection(Environment);

#endif
