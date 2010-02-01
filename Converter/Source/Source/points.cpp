
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                   Points/Vectors                                       //
//*****************************************************************************************//

Point Point::operator * (const Transformation &t) const {
	//or Vector = Vector * transformation.
	Point result (
		x*t.m11 + y*t.m21 + z*t.m31 + t.m41, 
		x*t.m12 + y*t.m22 + z*t.m32 + t.m42, 
		x*t.m13 + y*t.m23 + z*t.m33 + t.m43); 
	double w = x*t.m14 + y*t.m24 + z*t.m34 + t.m44;
	double absoluteW = absolute (w);
	if ((absoluteW - 1.0) < epsilon ()) return result; //usually, w is 1.0
	if (absoluteW < epsilon ()) return result * inverseEpsilon (); //w is 0.0 (use epsilon).
	return result * (1.0 / w); //the complicated case.
}


Point Zero (0.0, 0.0, 0.0);
Vector Up (0.0, 1.0, 0.0);
Vector Down (0.0, -1.0, 0.0);
Vector Left (-1.0, 0.0, 0.0);
Vector Right (1.0, 0.0, 0.0);
Vector Ahead (0.0, 0.0, -1.0);
Vector Back (0.0, 0.0, 1.0);
