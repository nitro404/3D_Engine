#include "Point.h"
#include "Transformation.h"

Point Point::transformBy(const Transformation & t) const {
	return Point(x * t.m11 + y * t.m21 + z * t.m31, x * t.m12 + y * t.m22 + z * t.m32, x * t.m13 + y * t.m23 + z * t.m33);
}
