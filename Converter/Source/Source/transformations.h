
//*****************************************************************************************//
//                                   Transformations                                       //
//*****************************************************************************************//

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#ifndef transformationsModule
#define transformationsModule 

#include "points.h"

//*****************************************************************************************//
//                           OpenGL Stack Manipulation Extensions                          //
//*****************************************************************************************//

inline void glGetMatrixd (GLenum whichMatrix, Transformation &matrix) {
	glGetDoublev (whichMatrix, (GLdouble *) &matrix);
}

inline void glLoadMatrixd (Transformation &matrix) {
	glLoadMatrixd ((const GLdouble *) &matrix);
}

inline void glMultMatrixd (Transformation &matrix) {
	glMultMatrixd ((const GLdouble *) &matrix);
}

inline void glPushMatrixd (Transformation &matrix) {
	glPushMatrix ();
	glLoadMatrixd (matrix);
}

inline void glPopMatrixd (GLenum whichMatrix, Transformation &matrix) {
	glGetMatrixd (whichMatrix, matrix);
	glPopMatrix ();
}

inline void glPushIdentity () {
	glPushMatrix ();
	glLoadIdentity ();
}

//*****************************************************************************************//
//                              Transformation Implementation                              //
//*****************************************************************************************//

class Transformation {
public:
	double m11, m12, m13, m14; //avoid arrays and 
	double m21, m22, m23, m24; //subscripting for more
	double m31, m32, m33, m34; //efficient compiler-generated
	double m41, m42, m43, m44; //code.

	inline bool isIdentity () {
		const long small = epsilon ();
		if (absolute (m11 - 1.0) > small) return false;
		if (absolute (m12) > small) return false;
		if (absolute (m13) > small) return false;
		if (absolute (m14) > small) return false;
		if (absolute (m21) > small) return false;
		if (absolute (m22 - 1.0) > small) return false;
		if (absolute (m23) > small) return false;
		if (absolute (m24) > small) return false;
		if (absolute (m31) > small) return false;
		if (absolute (m32) > small) return false;
		if (absolute (m33 - 1.0) > small) return false;
		if (absolute (m34) > small) return false;
		if (absolute (m41) > small) return false;
		if (absolute (m42) > small) return false;
		if (absolute (m43) > small) return false;
		if (absolute (m44 - 1.0) > small) return false;
		return true;
	}

	inline Transformation () {setToIdentity ();}
	inline Transformation (double a11, double a12, double a13, double a14, 
		double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, 
		double a41, double a42, double a43, double a44) {
		m11 = a11; m12 = a12; m13 = a13; m14 = a14;
		m21 = a21; m22 = a22; m23 = a23; m24 = a24;
		m31 = a31; m32 = a32; m33 = a33; m34 = a34;
		m41 = a41; m42 = a42; m43 = a43; m44 = a44;
	}
	inline ~Transformation () {};

	inline void set (double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44) {
		m11 = a11; m12 = a12; m13 = a13; m14 = a14;
		m21 = a21; m22 = a22; m23 = a23; m24 = a24;
		m31 = a31; m32 = a32; m33 = a33; m34 = a34;
		m41 = a41; m42 = a42; m43 = a43; m44 = a44;
	}

	inline void setToIdentity () {
		m11 = 1.0; m12 = 0.0; m13 = 0.0; m14 = 0.0;
		m21 = 0.0; m22 = 1.0; m23 = 0.0; m24 = 0.0;
		m31 = 0.0; m32 = 0.0; m33 = 1.0; m34 = 0.0;
		m41 = 0.0; m42 = 0.0; m43 = 0.0; m44 = 1.0;
	}

	inline Point position () const {return Point (m41, m42, m43);}

	Point vectorTransform (const Point &point) const {
		Point result (
			point.x*m11 + point.y*m21 + point.z*m31, 
			point.x*m12 + point.y*m22 + point.z*m32, 
			point.x*m13 + point.y*m23 + point.z*m33); 
		double w = point.x*m14 + point.y*m24 + point.z*m34 + m44;
		double absoluteW = absolute (w);
		if ((absoluteW - 1.0) < epsilon ()) return result; //usually, w is 1.0
		if (absoluteW < epsilon ()) return result * inverseEpsilon (); //w is 0.0 (use epsilon).
		return result * (1.0 / w); //the complicated case.
	}

	Point pointInverseTransform (const Point &point) const {
		//Point * M-1 where if M=RT (no scaling), then M-1 = (RT)-1 = T-1*R-1 = T-1*Rt (t is transpose). 
		const double tx = point.x - m41; const double ty = point.y - m42; const double tz = point.z - m43; //T-1
		return Point (tx*m11 + ty*m12 + tz*m13, tx*m21 + ty*m22 + tz*m23, tx*m31 + ty*m32 + tz*m33); //*Rt
	}

	inline void rotateToAxes (Point &xAxis, Point &yAxis, Point &zAxis) {
		set (xAxis.x, xAxis.y, xAxis.z, 0.0,
			yAxis.x, yAxis.y, yAxis.z, 0.0,
			zAxis.x, zAxis.y, zAxis.z, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	//Given a matrix M and a transformation T, we will say that we
	//	pre-multiply M if we compute T * M
	//  post-multiply M if we compute M * T

	inline void preTranslateBy (Point &translation) {//Translation * matrix;
		glPushMatrixd (*this);
			glTranslated (translation.x, translation.y, translation.z);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void postTranslateBy (Point &translation) {//matrix * Translation.
		glPushIdentity ();
			glTranslated (translation.x, translation.y, translation.z);
			glMultMatrixd (*this);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void translateBy (Point &translation) {preTranslateBy (translation);} //translate means pre-translate

	inline void preRotateBy (Point &rotation) {//rotation * matrix;
		//This rotation denotes [degreesAroundXAxis, degreesAroundYAxis, degreesAroundZAxis].
		//Actually, Ry*Rx*Rz * matrix.
		glPushMatrixd (*this);
			if (rotation.z != 0.0) glRotated (rotation.z, 0.0, 0.0, 1.0);
			if (rotation.x != 0.0) glRotated (rotation.x, 1.0, 0.0, 0.0);
			if (rotation.y != 0.0) glRotated (rotation.y, 0.0, 1.0, 0.0);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void postRotateBy (Point &rotation) {//matrix * rotation
		//This rotation denotes [degreesAroundXAxis, degreesAroundYAxis, degreesAroundZAxis].
		//Actually matrix-1 * Rz-1 * Rx-1 * Ry-1
		glPushIdentity ();
			if (rotation.y != 0.0) glRotated (rotation.y, 0.0, 1.0, 0.0); 
			if (rotation.x != 0.0) glRotated (rotation.x, 1.0, 0.0, 0.0); 
			if (rotation.z != 0.0) glRotated (rotation.z, 0.0, 0.0, 1.0);
			glMultMatrixd (*this);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void rotateBy (Point &rotation) {preRotateBy (rotation);} //rotateBy means pre-rotateBy

	inline void preRotateBy (double degrees, Point &axis) {//rotation * matrix
		glPushMatrixd (*this);
			glRotated (degrees, axis.x, axis.y, axis.z);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void postRotateBy (double degrees, Point &axis) {//matrix * rotation
		glPushIdentity ();
			glRotated (degrees, axis.x, axis.y, axis.z);
			glMultMatrixd (*this);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void rotateBy (double degrees, Point &axis) {preRotateBy (degrees, axis);} //rotateBy means pre-rotateBy

	inline void preScaleBy (Point &scale) {//scale * matrix
		glPushMatrixd (*this);
			glScaled (scale.x, scale.y, scale.z);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void postScaleBy (Point &scale) {//matrix * scale
		glPushIdentity ();
			glScaled (scale.x, scale.y, scale.z);
			glMultMatrixd (*this);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void scaleBy (Point &scale) {preScaleBy (scale);} //scaleBy means pre-scaleBy

	void multiply (Transformation &a, Transformation &b) {
		//this = a * b
		m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
		m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
		m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
		m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

		m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
		m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
		m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
		m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

		m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
		m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
		m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
		m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

		m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
		m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
		m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
		m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;
	}

	void multiply (Transformation &a) {
		//this = this * a
		double r11 = m11 * a.m11 + m12 * a.m21 + m13 * a.m31 + m14 * a.m41;
		double r12 = m11 * a.m12 + m12 * a.m22 + m13 * a.m32 + m14 * a.m42;
		double r13 = m11 * a.m13 + m12 * a.m23 + m13 * a.m33 + m14 * a.m43;
		double r14 = m11 * a.m14 + m12 * a.m24 + m13 * a.m34 + m14 * a.m44;

		double r21 = m21 * a.m11 + m22 * a.m21 + m23 * a.m31 + m24 * a.m41;
		double r22 = m21 * a.m12 + m22 * a.m22 + m23 * a.m32 + m24 * a.m42;
		double r23 = m21 * a.m13 + m22 * a.m23 + m23 * a.m33 + m24 * a.m43;
		double r24 = m21 * a.m14 + m22 * a.m24 + m23 * a.m34 + m24 * a.m44;

		double r31 = m31 * a.m11 + m32 * a.m21 + m33 * a.m31 + m34 * a.m41;
		double r32 = m31 * a.m12 + m32 * a.m22 + m33 * a.m32 + m34 * a.m42;
		double r33 = m31 * a.m13 + m32 * a.m23 + m33 * a.m33 + m34 * a.m43;
		double r34 = m31 * a.m14 + m32 * a.m24 + m33 * a.m34 + m34 * a.m44;

		double r41 = m41 * a.m11 + m42 * a.m21 + m43 * a.m31 + m44 * a.m41;
		double r42 = m41 * a.m12 + m42 * a.m22 + m43 * a.m32 + m44 * a.m42;
		double r43 = m41 * a.m13 + m42 * a.m23 + m43 * a.m33 + m44 * a.m43;
		double r44 = m41 * a.m14 + m42 * a.m24 + m43 * a.m34 + m44 * a.m44;
		set (r11,r12,r13,r14,r21,r22,r23,r24,r31,r32,r33,r34,r41,r42,r43,r44);
	}
	
	void log (long tabs = 0) {
		char *tabString = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"; tabString += strlen (tabString) - tabs;
		::log ("\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f"
			"\n%s%3.3f, %3.3f, %3.3f, %3.3f", tabString, m11, m12, m13, m14, tabString, m21, m22, m23, m24, 
			tabString, m31, m32, m33, m34, tabString, m41, m42, m43, m44);
	}

	/*
	void log () {
		::log ("\n%3.3f, %3.3f, %3.3f, %3.3f" "\n%3.3f, %3.3f, %3.3f, %3.3f" "\n%3.3f, %3.3f, %3.3f, %3.3f"
			"\n%3.3f, %3.3f, %3.3f, %3.3f", m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34,
			m41, m42, m43, m44);
	}
	*/

	//Additions needed by UniversalObject...
	inline void rotateToAxes (const Vector &xAxis, const Vector &yAxis, const Vector &zAxis, const Point &origin) {
		//Derivation (ignoring tranlation): [1,0,0] maps to xAxis, [0,1,0] maps to yAxis, [0,0,1] maps to zAxis. Try it...
		//The translation is added as a postTranslation which does not affect the rotation part...
		this->set (
			xAxis.x, xAxis.y, xAxis.z, 0.0,
			yAxis.x, yAxis.y, yAxis.z, 0.0,
			zAxis.x, zAxis.y, zAxis.z, 0.0,
			origin.x, origin.y, origin.z, 1.0);
	}

	static Transformation lookAtForCamera (const Vector &from, const Vector &direction, const Vector &up) {
		//This version returns the same thing as gluLookAt but computes it without using the stack...
		//Note: up is an approximate yAxis. If it is erroneously directed into the z-direction, use alternateUp instead.
		Vector zAxis = -direction.normalized ();
		Vector xAxis = (up.cross (zAxis)).normalized ();
		Vector yAxis = zAxis.cross (xAxis);
		Transformation result; 
		//Compute the inverse of rotateToAxes (xAxis, yAxis, zAxis, from) which interpreted as RT means computing T-1R-1.
		result.set (
			xAxis.x, yAxis.x, zAxis.x, 0.0,
			xAxis.y, yAxis.y, zAxis.y, 0.0,
			xAxis.z, yAxis.z, zAxis.z, 0.0,
			-from.x*xAxis.x - from.y*xAxis.y - from.z*xAxis.z,
			-from.x*yAxis.x - from.y*yAxis.y - from.z*yAxis.z,
			-from.x*zAxis.x - from.y*zAxis.y - from.z*zAxis.z,
			1.0);
		return result;
	}
	
	static Transformation Transformation::lookAtForObject (const Vector &from, const Vector &direction, const Vector &up) {
		//This version returns the inverse of gluLookAt without modifying the stack. Useful when you want an OBJECT to look at something RATHER THAN THE CAMERA.
		//Note: up is an approximate yAxis. If it is erroneously directed into the z-direction, use alternateUp instead.
		Vector zAxis = -direction.normalized ();
		Vector xAxis = (up.cross (zAxis)).normalized ();
		Vector yAxis = zAxis.cross (xAxis);
		Transformation result; result.rotateToAxes (xAxis, yAxis, zAxis, from);
		return result;
	}

	void log (char *title, long tabs = 0) {
		char *tabString = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t"; tabString += strlen (tabString) - tabs;
		::log ("%s\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f"
			"\n%s%3.3f, %3.3f, %3.3f, %3.3f", title, tabString, m11, m12, m13, m14, tabString, m21, m22, m23, m24, 
			tabString, m31, m32, m33, m34, tabString, m41, m42, m43, m44);
	}

	void Transformation::preTranslateTo (const Vector &globalPosition) {
		//For a full explanation of the implemenation of 'preTranslateTo' and 'postTranslateTo',
		//see the documentation 'Design Issue 1' in transformations.ppt
		m41 = globalPosition.x; m42 = globalPosition.y; m43 = globalPosition.z; //Move to the new position
	}

	void Transformation::postTranslateTo (const Vector &globalPosition) {
		//For a full explanation of the implemenation of 'postTranslateTo' and 'postTranslateTo',
		//see the documentation 'Design Issue 1' in transformations.ppt

		m41 = 0.0; m42 = 0.0; m43 = 0.0; //Move to the origin.
		//Now, pre-translate (global translation) by globalPosition
		glPushMatrixd (*this);
		glTranslated (globalPosition.x, globalPosition.y, globalPosition.z);
		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	
	inline double determinantOf3By3 () const {
		return m11*(m22*m33-m23*m32) - m12*(m21*m33-m23*m31) + m13*(m21*m32-m22*m31);
	}

	void Transformation::invert (const Transformation &t) {
		//If M=(SR)T=NT, then M-1 = T-1*N-1...

		//See page 41 of Math for 3D Game Programming & Computer Graphics for the inverse of a 3 by 3 transformation.
		//Part 1: Let N be the 3 by 3 portion of t. Place N-1 into receiver... 
		double factor = 1.0 / t.determinantOf3By3 ();
		m11 = factor * (t.m22*t.m33 - t.m23*t.m32);
		m12 = factor * (t.m13*t.m32 - t.m12*t.m33);
		m13 = factor * (t.m12*t.m23 - t.m13*t.m22);
		
		m21 = factor * (t.m23*t.m31 - t.m21*t.m33);
		m22 = factor * (t.m11*t.m33 - t.m13*t.m31);
		m23 = factor * (t.m13*t.m21 - t.m11*t.m23);
		
		m31 = factor * (t.m21*t.m32 - t.m22*t.m31);
		m32 = factor * (t.m12*t.m31 - t.m11*t.m32);
		m33 = factor * (t.m11*t.m22 - t.m12*t.m21);

		//Part 2: Premultiply N-1 by T-1 in place...
		Vector T = t.position ();
		m41 = -(T.x*m11 + T.y*m21 + T.z*m31);
		m42 = -(T.x*m12 + T.y*m22 + T.z*m32);
		m43 = -(T.x*m13 + T.y*m23 + T.z*m33);
		m44 = 1.0;
	}
	
	void scaleFreeInverse (const Transformation &input) {
		const bool alwaysComputeCorrectInverse = true;
		if (alwaysComputeCorrectInverse) {this->invert (input); return;}
		//This assumes no scaling. If M=RT, M-1 = T-1*R-1...
		//Part 1: R-1 = transpose of R.
		m11 = input.m11; m12 = input.m21; m13 = input.m31; m14 = 0.0;
		m21 = input.m12; m22 = input.m22; m23 = input.m32; m24 = 0.0;
		m31 = input.m13; m32 = input.m23; m33 = input.m33; m34 = 0.0;
		
		//m41 = -row4 of T dot (column 1 of R-1 OR row 1 of R)
		m41 = -(input.m41 * input.m11 + input.m42 * input.m12 + input.m43 * input.m13 + input.m44 * input.m14);
		//Similarly, m42 = -row4 of T dot (column 2 of R-1 OR row 2 of R)
		m42 = -(input.m41 * input.m21 + input.m42 * input.m22 + input.m43 * input.m23 + input.m44 * input.m24);
		m43 = -(input.m41 * input.m31 + input.m42 * input.m32 + input.m43 * input.m33 + input.m44 * input.m34);
		m44 = 1.0;
	}
};

declareCollection (Transformation);

//*****************************************************************************************//
//                      Dual Transformations (Have Their Own Inverses)                     //
//*****************************************************************************************//

/*
class DualTransformation : public Transformation {
public:
	//Has both a normal transformation and an inverse; maintains the identith "this * this->inverse = I".
	Transformation inverse;

	Transformation &normal () {return *((Transformation *) this);}

	//Important fact: if A-1 denotes the inverse of A, then (A*B)-1 = B-1*A-1 (the order is reversed).
	//Proof: If B-1*A-1 is the inverse of A*B, their product will turn out to be the identity.
	//Is it? B-1*A-1 * A*B = B-1 * B (since A-1 * A = I) = I
	//Given a matrix M and a transformation T, we will say that we
	//	pre-multiply M if we compute T * M
	//  post-multiply M if we compute M * T
	//Important fact: when we premultiply M by T, we must post-multiply the inverse by T-1.
	//Proof: (T*M)-1 = M-1*T-1; i.e., the inverse must be post-multiplied by the inverse of T.
	
	inline DualTransformation () {setToIdentity ();}
	inline DualTransformation (double a11, double a12, double a13, double a14, 
		double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, 
		double a41, double a42, double a43, double a44) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}
	inline ~DualTransformation () {};

	inline void set (double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void setToIdentity () {
		Transformation::setToIdentity (); inverse.setToIdentity ();
	}

	inline void preTranslateBy (Point &translation) {
		Transformation::preTranslateBy (translation);
		inverse.postTranslateBy (-translation);
	}
	inline void postTranslateBy (Point &translation) {
		Transformation::postTranslateBy (translation);
		inverse.preTranslateBy (-translation);
	}
	inline void translateBy (Point &translation) {preTranslateBy (translation);}


	inline void preRotateBy (Point &rotation) {
		Transformation::preRotateBy (rotation);
		inverse.postRotateBy (-rotation);
	}
	inline void postRotateBy (Point &rotation) {
		Transformation::postRotateBy (rotation);
		inverse.preRotateBy (-rotation);
	}
	inline void rotateBy (Point &rotation) {preRotateBy (rotation);}


	inline void preRotateBy (double degrees, Point &axis) {
		Transformation::preRotateBy (degrees, axis);
		inverse.postRotateBy (-degrees, axis);
	}
	inline void postRotateBy (double degrees, Point &axis) {
		Transformation::postRotateBy (degrees, axis);
		inverse.preRotateBy (-degrees, axis);
	}
	inline void rotateBy (double degrees, Point &axis) {preRotateBy (degrees, axis);}

	void multiply (Transformation &a, Transformation &b) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void multiply (DualTransformation &a, DualTransformation &b) {
		this->Transformation::multiply (a.normal (), b.normal ());
		this->inverse.Transformation::multiply (a.inverse, b.inverse);
	}

	void multiply (Transformation &a) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void multiply (DualTransformation &a) {
		this->Transformation::multiply (a.normal ());
		this->inverse.Transformation::multiply (a.inverse);
	}
	
	void log () {::log ("\nNormal:"); normal ().log (); ::log ("\nInverse:"); inverse.log ();}
};
*/

//*****************************************************************************************//
//                      Dual Transformations (Have Their Own Inverses)                     //
//*****************************************************************************************//

class DualTransformation : public Transformation {
public:
	//Has both a normal transformation and an inverse; maintains the identity "this * this->inverse = I".
	Transformation inverse;

	Transformation &normal () {return *((Transformation *) this);}

	//Important fact: if A-1 denotes the inverse of A, then (A*B)-1 = B-1*A-1 (the order is reversed).
	//Proof: If B-1*A-1 is the inverse of A*B, their product will turn out to be the identity.
	//Is it? B-1*A-1 * A*B = B-1 * B (since A-1 * A = I) = I
	//Given a matrix M and a transformation T, we will say that we
	//	pre-multiply M if we compute T * M
	//  post-multiply M if we compute M * T
	//Important fact: when we premultiply M by T, we must post-multiply the inverse by T-1.
	//Proof: (T*M)-1 = M-1*T-1; i.e., the inverse must be post-multiplied by the inverse of T.
	
	inline DualTransformation () {} //setToIdentity ();} Already done by Transformation...
	inline DualTransformation (double a11, double a12, double a13, double a14, 
		double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, 
		double a41, double a42, double a43, double a44) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}
	inline ~DualTransformation () {};

	inline void set (double a11, double a12, double a13, double a14, double a21, double a22, double a23, double a24, 
		double a31, double a32, double a33, double a34, double a41, double a42, double a43, double a44) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void setToIdentity () {
		Transformation::setToIdentity (); inverse.setToIdentity ();
	}


	inline void translateTo (const Point &globalPosition) {
		Transformation::preTranslateTo (globalPosition); 
		inverse.postTranslateTo (-globalPosition);
	}
	inline void preTranslateBy (Point &translation) {
		Transformation::preTranslateBy (translation);
		inverse.postTranslateBy (-translation);
	}
	inline void postTranslateBy (Point &translation) {
		Transformation::postTranslateBy (translation);
		inverse.preTranslateBy (-translation);
	}
	inline void translateBy (Point &translation) {preTranslateBy (translation);}


	inline void preRotateBy (Point &rotation) {
		Transformation::preRotateBy (rotation);
		inverse.postRotateBy (-rotation);
	}
	inline void postRotateBy (Point &rotation) {
		Transformation::postRotateBy (rotation);
		inverse.preRotateBy (-rotation);
	}
	inline void rotateBy (Point &rotation) {preRotateBy (rotation);}


	inline void preRotateBy (double degrees, Point &axis) {
		Transformation::preRotateBy (degrees, axis);
		inverse.postRotateBy (-degrees, axis);
	}
	inline void postRotateBy (double degrees, Point &axis) {
		Transformation::postRotateBy (degrees, axis);
		inverse.preRotateBy (-degrees, axis);
	}
	inline void rotateBy (double degrees, Point &axis) {preRotateBy (degrees, axis);}

	void multiply (Transformation &a, Transformation &b) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void multiply (DualTransformation &a, DualTransformation &b) {
		this->Transformation::multiply (a.normal (), b.normal ());
		this->inverse.Transformation::multiply (b.inverse, a.inverse);
	}

	inline void multiplyWithInverse (DualTransformation &a, DualTransformation &b) {
		this->Transformation::multiply (a.normal (), b.inverse);
		this->inverse.Transformation::multiply (b.normal (), a.inverse);
	}

	void multiply (Transformation &a) {
		halt ("Illegal since inverse elements too expensive to compute.");
	}

	inline void multiply (DualTransformation &a) {
		this->Transformation::multiply (a.normal ());
		this->inverse.Transformation::multiply (a.inverse);
	}
	
	void log () {normal ().log ("\nNormal:"); inverse.log ("\nInverse:");}

	inline void lookAt (const Vector &from, const Vector &direction, const Vector &up) {
		* ((Transformation *) this) = lookAtForObject (from, direction, up);
		inverse = Transformation::lookAtForCamera (from, direction, up);
	}
};

#endif