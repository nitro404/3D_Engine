
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                    BoundingBox                                          //
//*****************************************************************************************//

BoundingBox BoundingBox::UnitBox;

BoundingBox::BoundingBox (Object *object) {
	reset ();
	Transformation &t = object->transformation.normal ();

	if (t.isIdentity ()) {
		loopVector (faceIndex, object->faces)
			Face &face = *(object->faces [faceIndex]);
			loopVector (pointIndex, face.points)
				GamePoint &point = *(face.points [pointIndex]);
				add (point.asPoint ());
			endloop
		endloop

	} else {
		loopVector (faceIndex, object->faces)
			Face &face = *(object->faces [faceIndex]);
			loopVector (pointIndex, face.points)
				GamePoint &point = *(face.points [pointIndex]);
				Point transformedPoint = point.asPoint () * t;
				add (transformedPoint);
			endloop
		endloop
	}
};

const bool BoundingBox::intersectsLine (const Vector &p1, const Vector &p2) const {
	//Returns true if there is an intersection. Approach computes a value for 
	//t in range [0.0, 1.0). Recall: t = - N.(P1 - P0) / N.(P2 - P1) is where 
	//line from P1 to P2 intersects plane with normal N and point P0. Notice 
	//that N's sign cancels, so 
	//	t = unsigned(N).(P1 - P0) / unsigned(N).(P1 - P2).

	//Use n1 and n2 for two numerators; d for denominator."
	Vector d = p1 - p2; Vector n1 = p1 - minimum; Vector n2 = p1 - maximum;
	double t, newT = 1.0;

	//Planes left, bottom, front contain the minimum point.
	//Planes right, top, back contain the maximum point."
	#define test(a1,a2,b,which) \
		if (fabs (b) > epsilon) { \
			double oneOverD = 1.0 / b; \
			t = a1 * oneOverD; \
			if (t >= 0.0 && t < newT && includes##which##ofPoint (p1 - (d * t))) \
				{newT = t;}; \
			t = a2 * oneOverD; \
			if (t >= 0.0 && t < newT && includes##which##ofPoint (p1 - (d * t))) \
				{newT = t;}; \
		}

	double epsilon = ::epsilon ();
	//left has N = [-1,0,0] and right has N = [1,0,0].
	test (n1.x, n2.x, d.x, YZ); 
	//bottom has N = [0,-1,0] and top has N = [0,1,0].
	test (n1.y, n2.y, d.y, XZ);
	//front has N = [0,0,-1] and back has N = [0,0,1].
	test (n1.z, n2.z, d.z, XY); 
	if (newT < 1.0) {t = newT; return true;} else return false;
	#undef test
}

void BoundingBox::draw () {
	glDisable (GL_TEXTURE_2D);
	glColor4d (0.0, 1.0, 0.0, 1.0);
	//blend (None);
	glLineWidth (4.0); 
	Point boxCenter = minimum + (extent () * 0.5);
	Point boxExtent = extent ();
	glPushMatrix ();
		glTranslated (boxCenter.x, boxCenter.y, boxCenter.z);
		glScaled (boxExtent.x, boxExtent.y, boxExtent.z);
		glutWireCube (1.0);
	glPopMatrix ();
	glColor4d (1.0, 1.0, 1.0, 1.0);
	glLineWidth (1.0); 
	glEnable (GL_TEXTURE_2D);
}
