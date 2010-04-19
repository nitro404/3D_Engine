#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	min = Point(0,0,0);
	max = Point(1,1,1);
}

BoundingBox::BoundingBox(Point min, Point max)
{
	this->min = min;
	this->max = max;
}

BoundingBox::~BoundingBox(void)
{
}

Point BoundingBox::getCorner(int index) {
	switch (index) {
		case BOTTOM_LEFT_BACK:
			return min;
		case BOTTOM_LEFT_FRONT:
			return Point( min.x, min.y, max.z);
		case BOTTOM_RIGHT_BACK:
			return Point( max.x, min.y, min.z);
		case BOTTOM_RIGHT_FRONT:
			return Point( max.x, min.y, max.z);
		case TOP_LEFT_BACK:
			return Point( min.x, max.y, min.z);
		case TOP_LEFT_FRONT:
			return Point( min.x, max.y, max.z);
		case TOP_RIGHT_BACK:
			return Point( max.x, max.y, min.z);
		case TOP_RIGHT_FRONT:
			return max;
	}
	return Point(-9999999999,-999999999,-999999999);
}

Point BoundingBox::getCenter() {
	return (min + max) / 2;
}

void BoundingBox::offsetBy(Point offset) {
	min += offset;
	max += offset;
}

void BoundingBox::setCenter(Point center) {
	Point offset = center - getCenter();
	offsetBy(offset);
}

void BoundingBox::rotateBy(double degrees) {
	//need to make this expand the box
}

BoundingBox* BoundingBox::import(ifstream & input) {
	//needs to be implemented
	return NULL;
}
