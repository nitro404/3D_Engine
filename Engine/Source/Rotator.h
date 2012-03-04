#ifndef ROTATOR_H
#define ROTATOR_H

#include "Includes.h"
#include "Object.h"

class Rotator : public Object {
public:
	Rotator();
	
	~Rotator();
	
	double distanceFrom(Point & p) const;

	void update(double timeElapsed);
	void draw();

	void import(ifstream & input, vector<Texture *> & textures, vector<Shader *> shaders);

public:
	vector<Face *> faces;
	Shader * shader;
	
private:
	BoundingBox * origPointer;
	BoundingBox curBox;
	char * name;
	Point axis;
	double rateInDegreesPerSecond;
	double angleInDegrees;
};

#endif // ROTATOR_H
