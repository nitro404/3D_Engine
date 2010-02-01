//*****************************************************************************************//
//                                Point/Vector (synonyms)                                  //
//*****************************************************************************************//

#ifndef pointsModule
#define pointsModule 

inline double squared (double input) {return input * input;}
inline double asRadians (double degrees) {return degrees * (3.14159 / 180.0);}
inline double asDegrees (double radians) {return radians * (180.0 / 3.14159);}
inline double absolute (double number) {return fabs (number);} //Let the compiler pick
inline long absolute (long number) {return abs (number);} //the right absolute value function.

inline double epsilon () {return 1.0e-5;};
inline double squaredEpsilon () {return 1.0e-10;};
inline double inverseEpsilon () {return 1.0 / epsilon ();}

inline long maximum (long a, long b) {return (a < b ? b : a);}
inline double maximum (const double &a, const double &b) {return (a < b ? b : a);}
inline long minimum (long a, long b) {return (a > b ? b : a);}
inline double minimum (const double &a, const double &b) {return (a < b ? a : b);}

inline double clamp (double input, double min, double max) {return maximum (min, minimum (input, max));} //Bounds the input to values between min and max.

typedef class Point Vector; //Make Vector and Point synonyms.
class Transformation; //Forward reference.

class Point {
public:
	double x, y, z;

	Point () {}
	Point (double x, double y, double z) {this->x = x; this->y = y; this->z = z;}
	Point (const Point &point) {x = point.x; y = point.y; z = point.z;}
	~Point () {}
	
	inline void assign (const double value) {x=y=z=value;}
	inline void assign (const double X, const double Y, const double Z) {x=X; y=Y; z=Z;}
	
	inline Point *newCopy () {Point *copy = new Point (x, y, z); return copy;}
	inline void zero () {x = y = z = 0.0;}
	inline Vector operator * (const double &d) const {Vector result (x*d, y*d, z*d); return result;}
    inline void operator *= (const double &d) {x*=d; y*=d; z*=d;}
	inline Vector operator / (const double &d) const {Vector result (x/d, y/d, z/d); return result;}
    inline void operator /= (const double &d) {x/=d; y/=d; z/=d;}
	inline Vector operator + (const Vector &p) const {Vector result (x+p.x, y+p.y, z+p.z); return result;}
    inline void operator += (const Vector &p) {x+=p.x; y+=p.y; z+=p.z;}
    inline void operator += (const double &d) {x+=d; y+=d; z+=d;}
	inline Vector operator - (const Vector &p) const {Vector result (x-p.x, y-p.y, z-p.z); return result;}
    inline void operator -= (const Vector &p) {x-=p.x; y-=p.y; z-=p.z;}
    inline void operator -= (const double &d) {x-=d; y-=d; z-=d;}
	inline Vector operator - () const {Point result (-x, -y, -z); return result;}; //unary
	inline double squaredLength () {return x*x + y*y + z*z;}
	inline double length () {return sqrt (squaredLength ());}
	inline double dot (Vector &p) {return x*p.x + y*p.y + z*p.z;}

	inline double dot (const Vector &v) const {
		//Vector dot product (this dot v).
		return x*v.x + y*v.y + z*v.z;
	}

	inline Vector cross (const Vector &v) const {
		//Vector cross product (this cross v).
		Point result (y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
		return result;
	}

	inline Point pointCross (Point &p2, Point &p3) {
		//Point cross product assuming p1,p2,p3 are counter-clockwise.
		//Create vectors from p1 (this) to p2, and p1 to p3.
		Vector vector1 (p2.x - x, p2.y - y, p2.z - z);
		Vector vector2 (p3.x - x, p3.y - y, p3.z - z);
		return vector1.cross (vector2);
	}
	
	inline void normalize () {//destructively
		double length = sqrt (squaredLength ());
		if (absolute (length) > epsilon ()) {
			double oneOverLength = 1.0 / length;
			x *= oneOverLength; y *= oneOverLength; z *= oneOverLength;
		}
	}	
	
	inline Vector normalized () const {
		Vector result = *this; result.normalize (); return result;
	}
	inline Vector normalized () {
		Vector result = *this; result.normalize (); return result;
	}

	inline double squaredDistanceTo (Point &point) {
		Point difference = point - *this; return difference.dot (difference);
	}

	inline double distanceTo (Point &point) {return sqrt (squaredDistanceTo (point));}

	Point operator * (const Transformation &transformation) const;

	void log () {
		::log ("[%3.3f, %3.3f, %3.3f]", x, y, z);
	}

	inline double angleInDegreesBetweenNormalizedVectors (const Vector &vector) const {
		return ::asDegrees (acos (::clamp (this->dot (vector), -1.0, +1.0)));} //|A| |B| cos @ = A.B

	inline double angleInDegreesBetweenVectors (const Vector &vector) const {
		return this->normalized ().angleInDegreesBetweenNormalizedVectors (vector.normalized ());}
};

extern Point Zero;
extern Vector Up;
extern Vector Down;
extern Vector Left;
extern Vector Right;
extern Vector Ahead;
extern Vector Back;

declareCollection (Point);

#endif
