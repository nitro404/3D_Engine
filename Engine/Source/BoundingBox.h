#pragma once

#include "Point.h"

class BoundingBox {
public:
	BoundingBox();
	BoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);
	BoundingBox(Point min, Point max);
	~BoundingBox();

	Point getMax() const;
	Point getMin() const;
	Point getCorner(int index) const;
	Point getCenter() const;
	Point getBottomCenter() const;
	Point getExtent() const;
	
	void offsetBy(Point offset);
	void setCenter(Point center);
	void expandForRotation();

	static BoundingBox * import(ifstream & input);

private:
	void init();

public:
	const static int BOTTOM_LEFT_BACK;
	const static int BOTTOM_LEFT_FRONT;
	const static int BOTTOM_RIGHT_BACK;
	const static int BOTTOM_RIGHT_FRONT;
	const static int TOP_LEFT_BACK;
	const static int TOP_LEFT_FRONT;
	const static int TOP_RIGHT_BACK;
	const static int TOP_RIGHT_FRONT;

private:
	Point min;
	Point max;
	Point center;
	Point bottomCenter;
	Point extent;
};
