#ifndef TERRAIN_H
#define TERRAIN_H

#include <PxRigidStatic.h>
#include <PxHeightFieldGeometry.h> 
#include <PxGeometry.h>
#include <PxShapeExt.h>
#include <PxShape.h>
#include <PxMat33.h> 
#include "Object.h"
#include "FaceGroup.h"

using namespace physx;

class Terrain : public Object {
public:
	Terrain();
	~Terrain();

	int getWidth() const;
	int getHeight() const;
	double getTileSizeX() const;
	double getTileSizeZ() const;
	const GamePoint * getPoint(int x, int y) const;
	const BoundingBox * getBoundingBox() const;

	void setCollisionMesh(PxRigidStatic * mesh);

	virtual void draw();
	virtual void import(ifstream & input, vector<Texture *> & textures, vector<char *> & heightMaps, vector<Shader *> shaders);

public:
	Shader * shader;

private:
	double scaleHeight(int x, int y, int * heightMapData);

	char * name;
	Texture * textureMap;
	char * heightMap;
	int width;
	int height;
	int tiled;
	double tileSizeX;
	double tileSizeZ;
	GamePoint * points;
	FaceGroup * group;
	PxRigidStatic * collisionMesh;
};

#endif // TERRAIN_H
