#include "Point.h"
#include "Transformation.h"

Point Point::operator * (const Transformation & t) const {
	Point result(x * t.m11 + y * t.m21 + z * t.m31 + t.m41, 
				 x * t.m12 + y * t.m22 + z * t.m32 + t.m42, 
				 x * t.m13 + y * t.m23 + z * t.m33 + t.m43); 
	double w = x * t.m14 + y * t.m24 + z * t.m34 + t.m44;
	double w2 = fabs(w);
	if((w2 - 1.0) < 1.0e-5) {
		return result;
	}
	if(w2 < 1.0e-5) {
		return result * (1.0 / 1.0e-5);
	}
	return result * (1.0 / w);
}

Point Point::transformBy(const Transformation & t) const {
	return Point(x * t.m11 + y * t.m21 + z * t.m31, x * t.m12 + y * t.m22 + z * t.m32, x * t.m13 + y * t.m23 + z * t.m33);
}
