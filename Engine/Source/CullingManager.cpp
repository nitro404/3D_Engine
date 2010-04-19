#include "CullingManager.h"

CullingManager::CullingManager(void)
{
	cameraPlanes = new Plane[4];
	closestCorner = new int[4];
}

CullingManager::~CullingManager(void)
{
	delete[] cameraPlanes;
	delete[] closestCorner;
}


void CullingManager::prepareForDraw() {
	Transformation cameraInverse;
	glGetMatrixd(cameraInverse);

	Transformation projectionMatrix;
	glGetMatrixd(GL_PROJECTION_MATRIX, projectionMatrix);

	Transformation mul;
	mul.multiply(cameraInverse,projectionMatrix);
	
	//left
	cameraPlanes[0] = Plane(Point(mul.m14 + mul.m11, mul.m24 + mul.m21, mul.m34 + mul.m31), mul.m44 + mul.m41);

	//right
	cameraPlanes[1] = Plane(Point(mul.m14 - mul.m11, mul.m24 - mul.m21, mul.m34 - mul.m31), mul.m44 - mul.m41);

	//up
	cameraPlanes[2] = Plane(Point(mul.m14 - mul.m12, mul.m24 - mul.m22, mul.m34 - mul.m32), mul.m44 - mul.m42);

	//down
	cameraPlanes[3] = Plane(Point(mul.m14 + mul.m12, mul.m24 + mul.m22, mul.m34 + mul.m32), mul.m44 + mul.m42);

	//calculate the closest corner for each plane
	for (int i = 0;i < 4;i++) {
		int corner = 0;
		int distance = cameraPlanes[i].distanceToPoint(unitBox.getCorner(0));
		for (int j = 1; j < 8;j++) {
			int d = cameraPlanes[i].distanceToPoint(unitBox.getCorner(j));
			if (d > distance) {
				distance = d;
				corner = j;
			}
		}
		closestCorner[i] = corner;
	}

}

bool CullingManager::shouldCull(BoundingBox box) {
	for (int i = 0; i < 4;i++) {
		Point corner = box.getCorner(closestCorner[i]);
		if (cameraPlanes[i].whereIsPoint(corner) == negativeSign) {
			return false;
		}
	}
	return true;
}