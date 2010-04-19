#pragma once

#define BOTTOM_LEFT_BACK		0
#define BOTTOM_LEFT_FRONT		1
#define BOTTOM_RIGHT_BACK		2
#define BOTTOM_RIGHT_FRONT		3
#define TOP_LEFT_BACK			4
#define TOP_LEFT_FRONT			5
#define TOP_RIGHT_BACK			6
#define TOP_RIGHT_FRONT			7

#include "Point.h"

class BoundingBox
{
private:
	Point min;
	Point max;
public:
	BoundingBox();
	BoundingBox(Point min, Point max);
	~BoundingBox(void);

	Point getCorner(int index);
	Point getCenter();
	
	void offsetBy(Point offset);
	void setCenter(Point center);
	void rotateBy(double degrees);

	static BoundingBox* import(ifstream & input);

};
