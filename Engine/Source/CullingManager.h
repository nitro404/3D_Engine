#pragma once

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
	int* closestCorner;
	BoundingBox unitBox;
};
