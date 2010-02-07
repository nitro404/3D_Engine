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

typedef class Point Vector; //Make Vector and Point synonyms.
class Transformation; //Forward reference.

class Point {
public:
	double x, y, z;

	Point () {x = 0.0; y = 0.0; z = 0.0;}
	Point (double x, double y, double z) {this->x = x; this->y = y; this->z = z;}
	Point (Point &point) {x = point.x; y = point.y; z = point.z;}
	~Point () {}
	
	inline void zero () {x = y = z = 0.0;}
	inline bool isZero () const {return fabs (x) < epsilon () && fabs (y) < epsilon () && fabs (z) < epsilon ();}
	inline Vector operator * (const double &d) const {return Vector (x*d, y*d, z*d);}
    inline void operator *= (const double &d) {x*=d; y*=d; z*=d;}
	inline Vector operator / (const double &d) const {double inverse = 1.0/d; return Vector (x*inverse, y*inverse, z*inverse);}
    inline void operator /= (const double &d) {x/=d; y/=d; z/=d;}
	inline Vector operator + (const Vector &p) const {return Vector (x+p.x, y+p.y, z+p.z);}
    inline void operator += (const Vector &p) {x+=p.x; y+=p.y; z+=p.z;}
    inline void operator += (const double &d) {x+=d; y+=d; z+=d;}
	inline Vector operator - (const Vector &p) const {return Vector (x-p.x, y-p.y, z-p.z);}
    inline void operator -= (const Vector &p) {x-=p.x; y-=p.y; z-=p.z;}
    inline void operator -= (const double &d) {x-=d; y-=d; z-=d;}
	inline Vector operator - () const {return Point (-x, -y, -z);} //unary
	inline bool operator == (const Vector &p) { return x == p.x && y == p.y && z == p.z; }
	inline bool operator != (const Vector &p) { return *this != p; }
	inline double squaredLength () {return x*x + y*y + z*z;}
	inline double length () {return sqrt (squaredLength ());}
	inline double dot (Vector &p) {return x*p.x + y*p.y + z*p.z;}

	inline Vector cross (const Vector &v) const {
		//Vector cross product (this cross v).
		return Point (y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x);
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

	inline double squaredDistanceTo (Point &point) {
		Point difference = point - *this; 
		return difference.dot (difference);
	}

	Point operator * (const Transformation &transformation) const;

	//void log () {::log ("[%3.2f,%3.2f,%3.2f]", x, y, z);}
};

extern Point Zero;

declareCollection (Point);

#endif //pointsModule
