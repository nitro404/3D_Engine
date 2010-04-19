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

	Point min2 = min;
	Point max2 = max;

	min.x = min(min2.x, max2.x);
	max.x = max(min2.x, max2.x);
	min.y = min(min2.y, max2.y);
	max.y = max(min2.y, max2.y);
	min.z = min(min2.z, max2.z);
	max.z = max(min2.z, max2.z);

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
	Point offset = center - getCenter();
	offsetBy(offset);
}

void BoundingBox::rotateBy(double degrees, Point axis) {
	int iDeg = (int) degrees;
	double afterd = degrees - iDeg;
	double radians = (iDeg % 90 + afterd) / 180 * pi + pi / 4.0;
	double ratio = sin(radians) - sin(pi / 4.0);
	axis /= axis.length();
	Point distance = (max - min) / 2;
	Point offset = axis * distance.length() * ratio;
	max += offset;
	min -= offset;
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
