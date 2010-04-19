#include "CullingManager.h"

CullingManager::CullingManager()
{
	cameraPlanes = new Plane[4];
	maxCorners = new int[4];
	minCorners = new int[4];
}

CullingManager::~CullingManager()
{
	delete [] cameraPlanes;
	delete [] maxCorners;
	delete [] minCorners;
}


void CullingManager::prepareForDraw() {
	Transformation cameraInverse;
	glGetMatrixd(cameraInverse);

	Transformation projectionMatrix;
	glGetMatrixd(GL_PROJECTION_MATRIX, projectionMatrix);

	Transformation mul;
	mul.multiply(cameraInverse,projectionMatrix);
	
	//left
	Point leftP = Point(mul.m14 + mul.m11, mul.m24 + mul.m21, mul.m34 + mul.m31);
	double leftOffset = (mul.m44 + mul.m41) / leftP.length();
	leftP /= leftP.length();
	cameraPlanes[0] = Plane(leftP, leftOffset );

	//right
	Point rightP = Point(mul.m14 - mul.m11, mul.m24 - mul.m21, mul.m34 - mul.m31);
	double rightOffset = (mul.m44 - mul.m41) / rightP.length();
	rightP /= rightP.length();
	cameraPlanes[1] = Plane(rightP, rightOffset);

	//up
	Point upP = Point(mul.m14 - mul.m12, mul.m24 - mul.m22, mul.m34 - mul.m32);
	double upOffset = (mul.m44 - mul.m42) / upP.length();
	upP /= upP.length();
	cameraPlanes[2] = Plane(upP, upOffset);

	//down
	Point downP = Point(mul.m14 + mul.m12, mul.m24 + mul.m22, mul.m34 + mul.m32);
	double downOffset = (mul.m44 + mul.m42) / downP.length();
	downP /= downP.length();
	cameraPlanes[3] = Plane(downP, downOffset);

	//calculate the closest corner for each plane
	for (int i = 0;i < 4;i++) {
		int maxcorner = 0;
		int mincorner = 0;
		int maxdistance = cameraPlanes[i].distanceToPoint(unitBox.getCorner(0));
		int mindistance = maxdistance;
		for (int j = 1; j < 8;j++) {
			int d = cameraPlanes[i].distanceToPoint(unitBox.getCorner(j));
			if (d > maxdistance) {
				maxdistance = d;
				maxcorner = j;
			}
			if (d < mindistance) {
				mindistance = d;
				mincorner = j;
			}
		}
		maxCorners[i] = maxcorner;
		minCorners[i] = mincorner;
	}

}

bool CullingManager::shouldCull(BoundingBox box) {
	for (int i = 0; i < 4;i++) {
		Point max = box.getCorner(maxCorners[i]);
		Point min = box.getCorner(minCorners[i]);
		//int sign = cameraPlanes[i].whereIsLine(max, min);
		//if (sign == backSign) {
		//	return true;
		//}
		if (cameraPlanes[i].whereIsPoint(max) == negativeSign) {
			return true;
		}
	}
	return false;
}