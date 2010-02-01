//*****************************************************************************************//
//                                       Plane                                             //
//*****************************************************************************************//

#ifndef planeModule
#define planeModule 

enum PointSign {positiveSign, zeroSign, negativeSign};
enum LineSign {frontSign, backSign, onSign, straddlesSign};

class Plane {
public:
	Point normal; double minusP0DotNormal;

	Plane () {};
	Plane (Vector &normal, double minusP0DotNormal) {this->normal = normal; this->minusP0DotNormal = minusP0DotNormal;}
	Plane (Vector &normal, Point &point) {this->normal = normal; minusP0DotNormal = -(point.dot (normal));}
	Plane (Point &point1, Point &point2, Point &point3) {normal = (point2-point1).cross (point3-point1).normalized (); minusP0DotNormal = -(point1.dot (normal));}
	~Plane () {};

	//class methods
	static bool normalIsValid (Point &point1, Point &point2, Point &point3, Vector &normal, double &squaredLength);
	static Plane *fromDangerousPoints (Point &point1, Point &point2, Point &point3);

	//instance methods
	double distanceToPoint (Point &point) {return normal.dot (point) + minusP0DotNormal;}

	PointSign whereIsPoint (Point &point) {
		//Returns positiveSign, zeroSign, negativeSign.
		double distance = distanceToPoint (point);
		if (distance < -epsilon ()) return negativeSign;
		if (distance > epsilon ()) return positiveSign;
		return zeroSign;
	}

	LineSign whereIsLine (Point &fromPoint, Point &toPoint) {
		//Returns frontSign, backSign, onSign, or straddlesSign.
		PointSign sign1 = whereIsPoint (fromPoint);
		PointSign sign2 = whereIsPoint (toPoint);
		if (sign1 == sign2) {
			if (sign1 == positiveSign) return frontSign;
			if (sign1 == negativeSign) return backSign;
			return onSign;
		}
		return straddlesSign;
	}

	Point Plane::projectionOfPoint (Point &point) {
		//Let the projection of p on this plane be q (q is the nearest point on the plane). 
		//If d is the distance from q to the plane in the direction of the normal N, then 
		//p = q + d*N. So q = p - d*N."
		return point - normal * distanceToPoint (point);
	}
	
	inline bool normalNearlyPerpendicular (Vector &unitDirection) {
		return absolute (normal.dot (unitDirection)) < epsilon ();
	}
	inline bool normalNearlyParallel (Vector &unitDirection) {
		return absolute (normal.dot (unitDirection)) - 1.0 < epsilon ();
	}
	inline displaceByDistance (double distance) {
		//If P0 is replaced by P0 + N*d, then P0.N is replaced by (P0 + N*d).N = P0.N + N.N*d = P0.N + d if N is unit length...
		minusP0DotNormal += distance;
	}
	
	bool intersectsInfiniteLine (const Point &start, const Point &end, Vector &direction, double &t) const {
		//Same as intersectionTValue (double &t, const Point &p1, const Point &p2, double radius) where radius = 0.0
		//		which contains derivation for		t = N.(P0 - P1) / N.(P2 - P1)
		//but returns true for any t value.
		direction = end - start; double denominator = direction.dot (normal);
		if (absolute (denominator) < epsilon ()) return false;
		t = (-minusP0DotNormal - start.dot (normal)) / denominator;
		return true;
	}
};

declareCollection (Plane);

#endif