
//*****************************************************************************************//
//                                   Transformations                                       //
//*****************************************************************************************//

//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#ifndef transformationsModule
#define transformationsModule 

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

inline GLint currentMatrixStack () {
	GLint mode; glGetIntegerv (GL_MATRIX_MODE, &mode);
	return 
		mode == GL_MODELVIEW ? GL_MODELVIEW_MATRIX :
		mode == GL_PROJECTION ? GL_PROJECTION_MATRIX :
		GL_TEXTURE_MATRIX;
}

inline void glGetMatrixd (Transformation &matrix) {
	glGetDoublev (currentMatrixStack (), (double *) &matrix);
}

inline void glPopMatrixd (Transformation &matrix) {
	glGetMatrixd (matrix);
	glPopMatrix ();
}

//And one useful utility for tabbing...
inline char *indentation (long tabs) {
	static char *string = "\t\t\t\t\t\t\t\t\t\t"; //Exactly 10 tabs,,,
	return &string [10 - tabs];
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
		//Actually, Rx*Ry*Rz * matrix.
		glPushMatrixd (*this);
			if (rotation.z != 0.0) glRotated (rotation.z, 0.0, 0.0, 1.0);
			if (rotation.y != 0.0) glRotated (rotation.y, 0.0, 1.0, 0.0);
			if (rotation.x != 0.0) glRotated (rotation.x, 1.0, 0.0, 0.0);

		glPopMatrixd (GL_MODELVIEW_MATRIX, *this);
	}
	inline void postRotateBy (Point &rotation) {//matrix * rotation
		//This rotation denotes [degreesAroundXAxis, degreesAroundYAxis, degreesAroundZAxis].
		//Actually (Rx*Ry*Rz * matrix)-1 = matrix-1 * Rz-1 * Ry-1 * Rx-1
		glPushIdentity ();
			if (rotation.x != 0.0) glRotated (rotation.x, 1.0, 0.0, 0.0); 
			if (rotation.y != 0.0) glRotated (rotation.y, 0.0, 1.0, 0.0); 
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
	
	Point position () {return Point (m41, m42, m43);}

	void log (long tabs = 0) {
		char *tabbing = indentation (tabs);
		::log ("\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f" 
			"\n%s%3.3f, %3.3f, %3.3f, %3.3f" "\n%s%3.3f, %3.3f, %3.3f, %3.3f", 
			tabbing, m11, m12, m13, m14, tabbing, m21, m22, m23, m24, 
			tabbing, m31, m32, m33, m34, tabbing, m41, m42, m43, m44);
	}
};

declareCollection (Transformation);

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
	
	Point position () {return this->normal ().position ();}

	void log (long tabs = 0) {
		char *tabbing = indentation (tabs);
		::log ("\n%sNormal:", tabbing); normal ().log (tabs+1); 
		::log ("\n%sInverse:", tabbing); inverse.log (tabs+1);
	}
};

#endif //transformationsModule