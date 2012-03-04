#ifndef CULLING_MANAGER_H
#define CULLING_MANAGER_H

#include "Plane.h"
#include "BoundingBox.h"
#include "Transformation.h"

class CullingManager {
public:
	CullingManager();
	~CullingManager();

	void prepareForDraw();
	bool shouldCull(BoundingBox box);

private:
	Plane* cameraPlanes;
	int* maxCorners;
	//int* minCorners;
	BoundingBox unitBox;
};

#endif // CULLING_MANAGER_H
