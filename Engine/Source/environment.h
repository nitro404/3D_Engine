#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

class Environment : public Object {
public:
	Environment() {
		skyboxTextures = new Texture*[6];
	}

	~Environment() {
		delete [] name;
		delete [] ambientLight;
		delete [] skyboxTextures;
		delete [] skyColour;
	}

	double distanceFrom(Point & p) const { return 0; }

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);

private:
	char * name;
	double * ambientLight;
	int cloudLayers;
	Texture ** skyboxTextures;
	double * skyColour;
	int surrounds;
};

declareCollection(Environment);

#endif
