#ifndef POINT_H
#define POINT_H

#include "Includes.h"

struct GamePoint {
	double x, y, z;
	double nx, ny, nz;
	double tx, ty;
};

class Transformation;

class Point {
public:
	Point() : x(0), y(0), z(0) { }
	Point(double xPos, double yPos, double zPos) : x(xPos), y(yPos), z(zPos) { }
	Point(const Point & point) : x(point.x), y(point.y), z(point.z) { }
	~Point() { }
	
	inline void zero() { x = y = z = 0.0; }
	inline Point operator * (const double & d) const { return Point (x * d, y * d, z * d); }
    inline void operator *= (const double & d) { x *= d; y *= d; z *= d; }
	inline Point operator / (const double & d) const { double inverse = 1.0/d; return Point(x * inverse, y * inverse, z * inverse); }
    inline void operator /= (const double & d) {x /= d; y /= d; z /= d; }
	inline Point operator + (const Point & p) const { return Point (x + p.x, y + p.y, z + p.z); }
    inline void operator += (const Point & p) { x += p.x; y += p.y; z += p.z; }
    inline void operator += (const double & d) { x += d; y += d; z += d; }
	inline Point operator - (const Point & p) const { return Point (x - p.x, y - p.y, z - p.z); }
    inline void operator -= (const Point & p) { x -= p.x; y -= p.y; z -= p.z; }
    inline void operator -= (const double & d) { x -= d; y -=d ; z -= d; }
	inline Point operator - () const { return Point(-x, -y, -z); }
	inline bool operator == (const Point & p) { return x == p.x && y == p.y && z == p.z; }
	inline bool operator != (const Point & p) { return *this != p; }
	inline double squaredLength() { return x * x + y * y + z * z; }
	inline double length() { return sqrt(squaredLength()); }
	inline double dot(Point & p) { return x * p.x + y * p.y + z * p.z; }
	inline Point cross(const Point & v) const { return Point (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
	Point operator * (const Transformation & transformation) const;

public:
	double x, y, z;
};

#endif // POINT_H
