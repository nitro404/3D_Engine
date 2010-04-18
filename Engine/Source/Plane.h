#pragma once

#include "Includes.h"
#include "Point.h"

enum PointSign {positiveSign, zeroSign, negativeSign};
enum LineSign {frontSign, backSign, onSign, straddlesSign};

class Plane {
public:
	Point normal;
	double minusP0DotNormal;

	Plane() { }

	Plane(Point & normal, double minusP0DotNormal) {
		this->normal = normal;
		this->minusP0DotNormal = minusP0DotNormal;
	}

	Plane(Point & normal, Point & point) {
		this->normal = normal;
		minusP0DotNormal = -(point.dot(normal));
	}

	~Plane() { }

	//class methods
	static bool normalIsValid(Point & point1, Point & point2, Point & point3, Point & normal, double & squaredLength);
	static Plane * fromDangerousPoints(Point & point1, Point & point2, Point & point3);
	
	//instance methods
	double distanceToPoint(Point & point) {
		return normal.dot(point) + minusP0DotNormal;
	}

	PointSign whereIsPoint(Point & point) {
		//Returns positiveSign, zeroSign, negativeSign.
		double distance = distanceToPoint (point);
		if(distance < -(1.0e-5)) return negativeSign;
		if(distance > 1.0e-5) return positiveSign;
		return zeroSign;
	}

	LineSign whereIsLine(Point & fromPoint, Point & toPoint) {
		//Returns frontSign, backSign, onSign, or straddlesSign.
		PointSign sign1 = whereIsPoint(fromPoint);
		PointSign sign2 = whereIsPoint(toPoint);
		if(sign1 == sign2) {
			if(sign1 == positiveSign) return frontSign;
			if(sign1 == negativeSign) return backSign;
			return onSign;
		}
		return straddlesSign;
	}

	Point Plane::projectionOfPoint (Point &point) {
		//Let the projection of p on this plane be q (q is the nearest point on the plane). 
		//If d is the distance from q to the plane in the direction of the normal N, then 
		//p = q + d*N. So q = p - d*N."
		return point - normal * distanceToPoint(point);
	}
	
	inline bool normalNearlyPerpendicular(Point & unitDirection) {
		return fabs(normal.dot(unitDirection)) < 1.0e-5;
	}
	inline bool normalNearlyParallel(Point & unitDirection) {
		return fabs(normal.dot(unitDirection)) - 1.0 < 1.0e-5;
	}
};
