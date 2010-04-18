#include "Plane.h"

bool Plane::normalIsValid(Point & point1, Point & point2, Point & point3, Point & normal, double & squaredLength) {
    //Where point1, point2, point3 are counter-clockwise. Returns true if a normal
	//can be computed and also returns the UNNORMALIZED normal and it's squared length; otherwise, 
	//returns false.
    normal = (point2 - point1).cross(point3 - point2);
    squaredLength = normal.squaredLength();
    return squaredLength > 1.0e-10;
}

Plane * Plane::fromDangerousPoints(Point & point1, Point & point2, Point & point3) {
    //Where point1, point2, point3 are counter-clockwise. Returns NULL if
    //the result is not a valid plane; otherwise, allocates and returns a new plane.
	Point normal;
	double squaredLength;
	if(!normalIsValid (point1, point2, point3, normal, squaredLength)) {
		return NULL;
	}
    return new Plane(normal * (1.0 / sqrt (squaredLength)), point1);
}

