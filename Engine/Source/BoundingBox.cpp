#include "BoundingBox.h"
#include <math.h>

double pi = 3.14159265;
const int BoundingBox::BOTTOM_LEFT_BACK = 0;
const int BoundingBox::BOTTOM_LEFT_FRONT = 1;
const int BoundingBox::BOTTOM_RIGHT_BACK = 2;
const int BoundingBox::BOTTOM_RIGHT_FRONT = 3;
const int BoundingBox::TOP_LEFT_BACK = 4;
const int BoundingBox::TOP_LEFT_FRONT = 5;
const int BoundingBox::TOP_RIGHT_BACK = 6;
const int BoundingBox::TOP_RIGHT_FRONT = 7;

BoundingBox::BoundingBox()
{
	min = Point(0,0,0);
	max = Point(1,1,1);
	init();
}

BoundingBox::BoundingBox(Point min, Point max)
{
	this->min = min;
	this->max = max;
	init();
}

BoundingBox::BoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
{
	this->min = Point(minX, minY, minZ);
	this->max = Point(maxX, maxY, maxZ);
	init();
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::init() {

	center = (min + max) / 2;

	bottomCenter = center;
	bottomCenter.y = min.y;

	extent = max - min;
}

Point BoundingBox::getMax() const {
	return max;
}

Point BoundingBox::getMin() const {
	return min;
}

Point BoundingBox::getCorner(int index) const {
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
		default:
			return Point(-INT_MAX,-INT_MAX,-INT_MAX);
	}
}

Point BoundingBox::getCenter() const {
	return center;
}

Point BoundingBox::getBottomCenter() const {
	return bottomCenter;
}

Point BoundingBox::getExtent() const {
	return extent;
}

void BoundingBox::offsetBy(Point offset) {
	min += offset;
	max += offset;
}

void BoundingBox::setCenter(Point center) {
	max = center + extent / 2;
	min = center - extent / 2;
	init();
}

void BoundingBox::expandForRotation() {
	max *= 1.65;
	min *= 1.65;

	//now make into cube

	double diff = max(max.x - min.x, max.y - min.y);
	diff = max(diff, max.z - min.z);
	diff /= 2;

	min = center;
	min -= diff;
	max = center;
	max += diff;
	init();

}

BoundingBox * BoundingBox::import(ifstream & input) {
	char line[256];
	double maxX, maxY, maxZ;
	double minX, minY, minZ;

	// input the maximum
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	maxX = atof(line);
	input.getline(line, 256, ',');
	maxY = atof(line);
	input.getline(line, 256, ';');
	maxZ = atof(line);
	input.getline(line, 256, '\n');
	// input the minimum
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	minX = atof(line);
	input.getline(line, 256, ',');
	minY = atof(line);
	input.getline(line, 256, ';');
	minZ = atof(line);
	input.getline(line, 256, '\n');

	return new BoundingBox(minX, minY, minZ, maxX, maxY, maxZ);
}
