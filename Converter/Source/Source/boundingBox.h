//*****************************************************************************************//
//                                       BoundingBox                                       //
//*****************************************************************************************//

#ifndef boundingBoxModule
#define boundingBoxModule 

class Object;

class BoundingBox {
public:
	static BoundingBox UnitBox;
	Point minimum; Point maximum;

	BoundingBox () {};
	inline BoundingBox (const Point &point) {minimum = maximum = point;};
	inline BoundingBox (const Point &point1, const Point &point2) {minimum = maximum = point1; add (point2);};
	inline BoundingBox (const Point &position, const double radius) {minimum = maximum = position; minimum -= radius; maximum += radius;};
	BoundingBox (Object *object);
	~BoundingBox () {}

	void reset () {minimum.assign (DBL_MAX); maximum.assign (-DBL_MAX);}
	void add (const BoundingBox &box) {add (box.maximum); add (box.minimum);}
	Point center () const {return (minimum + maximum) * 0.5;}
	void growBy (double positiveDelta) {minimum -= positiveDelta; maximum += positiveDelta;}
	void growEverSoSlightly () {growBy (epsilon ());}
	void resize (BoundingBox box) {minimum = box.minimum; maximum = box.maximum;}
	void resize (const Point &point1, const Point &point2) {minimum = maximum = point1; add (point2);}
	void resize (Vector &extent) {resize (extent * -0.5, extent * 0.5);}

	void add (const Point &point) {
		minimum.x = ::minimum (minimum.x, point.x);
		minimum.y = ::minimum (minimum.y, point.y);
		minimum.z = ::minimum (minimum.z, point.z);
		maximum.x = ::maximum (maximum.x, point.x);
		maximum.y = ::maximum (maximum.y, point.y);
		maximum.z = ::maximum (maximum.z, point.z);		
	}

	BoundingBox baseBox () const {
		return BoundingBox (minimum, Point (maximum.x, minimum.y, maximum.z));
	}

	BoundingBox unionWith (const BoundingBox &box) const {
		return BoundingBox (
			Point (
				minimum.x < box.minimum.x ? minimum.x : box.minimum.x,
				minimum.y < box.minimum.y ? minimum.y : box.minimum.y,
				minimum.z < box.minimum.z ? minimum.z : box.minimum.z),
			Point (
				maximum.x > box.maximum.x ? maximum.x : box.maximum.x,
				maximum.y > box.maximum.y ? maximum.y : box.maximum.y,
				maximum.z > box.maximum.z ? maximum.z : box.maximum.z)
			);
	}

	BoundingBox intersectionWith (const BoundingBox &box) const {
		return BoundingBox (
			Point (
				minimum.x > box.minimum.x ? minimum.x : box.minimum.x,
				minimum.y > box.minimum.y ? minimum.y : box.minimum.y,
				minimum.z > box.minimum.z ? minimum.z : box.minimum.z),
			Point (
				maximum.x < box.maximum.x ? maximum.x : box.maximum.x,
				maximum.y < box.maximum.y ? maximum.y : box.maximum.y,
				maximum.z < box.maximum.z ? maximum.z : box.maximum.z)
			);
	}

	bool isZeroSize () {
		return (minimum.x >= maximum.x || minimum.y >= maximum.y || minimum.z >= maximum.z);
	}

	/*
	bool isEqualTo (const BoundingBox &box) const {
		return minimum.isEqualTo (box.minimum) && maximum.isEqualTo (box.maximum);
	}
	*/

	bool intersects (const Point &sphereCenter, const double sphereRadius) const {
		//Intersects the sphere's bounding box...
		if (sphereCenter.x + sphereRadius < minimum.x ) return false;
		if (sphereCenter.x - sphereRadius > maximum.x ) return false;
		if (sphereCenter.y + sphereRadius < minimum.y ) return false;
		if (sphereCenter.y - sphereRadius > maximum.y ) return false;
		if (sphereCenter.z + sphereRadius < minimum.z ) return false;
		if (sphereCenter.z - sphereRadius > maximum.z ) return false;
		return true;
	}

	bool intersects (const BoundingBox &box) const {
		if (box.maximum.x < minimum.x ) return false;
		if (box.minimum.x > maximum.x ) return false;
		if (box.maximum.y < minimum.y ) return false;
		if (box.minimum.y > maximum.y ) return false;
		if (box.maximum.z < minimum.z ) return false;
		if (box.minimum.z > maximum.z ) return false;
		return true;
	}

	bool contains (const BoundingBox &box) const {
		if (!includesPoint (box.minimum)) {return false;}
		if (!includesPoint (box.maximum)) {return false;}
		return true;
	}

	const bool intersectsLine (const Vector &p1, const Vector &p2) const;
	const bool intersectsLine (double &t, Plane  &p, const Vector &p1, const Vector &p2);

	inline Vector extent () const {return maximum - minimum;}
	inline double width () const {return maximum.x - minimum.x;}
	inline double height () const {return maximum.y - minimum.y;}
	inline double depth () const {return maximum.z - minimum.z;}

	BoundingBox transformedBy (const Transformation &transformation) {
		static Point points [8];
		for (long i = 0; i < 8; i ++) {
			pointForIndex (points [i], i);
			points [i] = points [i] * transformation;
		}
		BoundingBox newBox;
		newBox.minimum = newBox.maximum = points [0];
		for (i = 1; i < 8; i ++) {
			newBox.add (points [i]);
		}
		return newBox;
	}

	BoundingBox translatedBy (const Point &translation) {
		BoundingBox newBox;
		newBox.minimum = minimum + translation;
		newBox.maximum = maximum + translation;
		return newBox;
	}

	void transformBy (const Transformation &transformation) {
		static Point points [8];
		for (long i = 0; i < 8; i ++) {
			pointForIndex (points [i], i);
			points [i] = points [i] * transformation;
		}
		minimum = maximum = points [0];
		for (i = 1; i < 8; i ++) {
			add (points [i]);
		}
	}

	inline void pointForIndex (Point &point, long index) const {
		//Index = 0..7
		#define a minimum
		#define b maximum
		switch (index) {
			case 0: point.x = a.x; point.y = a.y; point.z = a.z; return; //left back bot
			case 1: point.x = a.x; point.y = a.y; point.z = b.z; return; //left front bot
			case 2: point.x = b.x; point.y = a.y; point.z = b.z; return; //right front bot
			case 3: point.x = b.x; point.y = a.y; point.z = a.z; return; //right back bot
			case 4: point.x = a.x; point.y = b.y; point.z = a.z; return; //left back top
			case 5: point.x = a.x; point.y = b.y; point.z = b.z; return; //left front top
			case 6: point.x = b.x; point.y = b.y; point.z = b.z; return; //right front top
			default: point.x = b.x; point.y = b.y; point.z = a.z; return; //right back top
		}
		#undef a
		#undef b
	}
	
	inline void faceForIndex (Point &leftBottom, Point &RightBottom, Point &rightTop, Point &leftTop, long index) const {
		#define leftBackBottom 0
		#define leftFrontBottom 1
		#define rightFrontBottom 2
		#define rightBackBottom 3
		#define leftBackTop 4
		#define leftFrontTop 5
		#define rightFrontTop 6
		#define rightBackTop 7	
		switch (index) {
			case 0: //front
				pointForIndex (leftBottom, leftFrontBottom);
				pointForIndex (RightBottom, rightFrontBottom);
				pointForIndex (rightTop, rightFrontTop);
				pointForIndex (leftTop, leftFrontTop);
				return;
			case 1: //back
				pointForIndex (leftBottom, rightBackBottom);
				pointForIndex (RightBottom, leftBackBottom);
				pointForIndex (rightTop, leftBackTop);
				pointForIndex (leftTop, rightBackTop);
				return;
			case 2: //left
				pointForIndex (leftBottom, leftBackBottom);
				pointForIndex (RightBottom, leftFrontBottom);
				pointForIndex (rightTop, leftFrontTop);
				pointForIndex (leftTop, leftBackTop);
				return;
			case 3: //right
				pointForIndex (leftBottom, rightFrontBottom);
				pointForIndex (RightBottom, rightBackBottom);
				pointForIndex (rightTop, rightBackTop);
				pointForIndex (leftTop, rightFrontTop);
				return;
			case 4: //top
				pointForIndex (leftBottom, leftFrontTop);
				pointForIndex (RightBottom, rightFrontTop);
				pointForIndex (rightTop, rightBackTop);
				pointForIndex (leftTop, leftBackTop);
				return;
			case 5: //bottom
				pointForIndex (leftBottom, leftBackBottom);
				pointForIndex (RightBottom, rightBackBottom);
				pointForIndex (rightTop, rightFrontBottom);
				pointForIndex (leftTop, leftFrontBottom);
				return;
		}
		#undef leftBackBottom
		#undef leftFrontBottom
		#undef rightFrontBottom
		#undef rightBackBottom
		#undef leftBackTop
		#undef leftFrontTop
		#undef rightFrontTop
		#undef rightBackTop	
	}

	static long minimalPointIndex (Plane &plane) {
		//Find the index of the point on the sample box that is most negative with respect to the plane.

		//Process the first point.
		long minimalIndex = 0; Point point; UnitBox.pointForIndex (point, 0);
		double minimalDistance = plane.distanceToPoint (point);

		//Process all the others.
		for (long index = 1; index < 8; index++) {
			UnitBox.pointForIndex (point, index); double distance = plane.distanceToPoint (point);
			if (distance < minimalDistance) {minimalDistance = distance; minimalIndex = index;}
		}
		return minimalIndex;
	}

	static long maximalPointIndex (Plane &plane) {
		//Find the index of the point on the sample box that is most positive with respect to the plane.

		//Process the first point.
		long maximalIndex = 0; Point point; UnitBox.pointForIndex (point, 0);
		double maximalDistance = plane.distanceToPoint (point);

		//Process all the others.
		for (long index = 1; index < 8; index++) {
			UnitBox.pointForIndex (point, index);
			double distance = plane.distanceToPoint (point);
			if (distance > maximalDistance) {maximalDistance = distance; maximalIndex = index;}
		}
		return maximalIndex;
	}

	/*
	inline bool isVisible (long pointIndex, Plane &plane) {	
		//Is the indexed point on the front side of the plane.
		Point point; pointForIndex (point, pointIndex);
		return plane.isInFront (point);
	}
	*/

	void log () {::log ("BoundingBox ["); minimum.log (); ::log ("->"); maximum.log (); ::log ("]");}

	inline const bool includesPoint (const Vector &aPoint) const {
		if (minimum.x > aPoint.x) return false;
		if (aPoint.x > maximum.x) return false;
		if (minimum.y > aPoint.y) return false;
		if (aPoint.y > maximum.y) return false;
		if (minimum.z > aPoint.z) return false;
		if (aPoint.z > maximum.z) return false;
		return true;
	}
	inline const bool includesXYofPoint (const Vector &aPoint) const {
		if (minimum.x > aPoint.x) return false;
		if (aPoint.x > maximum.x) return false;
		if (minimum.y > aPoint.y) return false;
		if (aPoint.y > maximum.y) return false;
		return true;
	}
	inline const bool includesXZofPoint (const Vector &aPoint) const {
		if (minimum.x > aPoint.x) return false;
		if (aPoint.x > maximum.x) return false;
		if (minimum.z > aPoint.z) return false;
		if (aPoint.z > maximum.z) return false;
		return true;
	}
	inline const bool includesYZofPoint (const Vector &aPoint) const {
		if (minimum.y > aPoint.y) return false;
		if (aPoint.y > maximum.y) return false;
		if (minimum.z > aPoint.z) return false;
		if (aPoint.z > maximum.z) return false;
		return true;
	}

	void draw ();
	
//	const bool intersectsRay (const Line &ray, double &t);
//	const bool intersectsRay (const Line &ray) {double unusedT; return intersectsRay (ray, unusedT);}
};

#endif