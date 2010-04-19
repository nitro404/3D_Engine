#pragma once

#include "Plane.h"
#include "BoundingBox.h"
#include "Transformation.h"

class CullingManager
{
private:
	Plane* cameraPlanes;
	int* closestCorner;
	BoundingBox unitBox;

public:
	CullingManager(void);
	~CullingManager(void);

	void prepareForDraw();
	bool shouldCull(BoundingBox box);

};
