#ifndef CUBE_H
#define CUBE_H

#include <PxRigidDynamic.h>
#include "Colour.h"
#include "Object.h"

using namespace physx;

class Cube : public Object {
public:
	Cube(const DualTransformation & cubeTransformation, int cubeSize, const Point & cubeVelocity, int cubeMass, const Colour & cubeColour = Colour(255, 255, 255, 255), Texture ** cubeTextures = NULL);
	~Cube();
	
	bool load();
	bool unload();
	bool isLoaded() const;

	void update(double timeElapsed);
	void draw();

private:
	int size;
	int mass;
	Point velocity;
	PxRigidDynamic * collisionMesh;
	GLuint cubeList;
	Colour colour;
	Texture ** textures;
	bool loaded;
};

#endif // CUBE_H
