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
	Point & Point::operator = (const Point & p) { x = p.x; y = p.y; z = p.z; return *this; }
	~Point() { }
	
	void zero() { x = y = z = 0.0; }
	Point operator * (const double & d) const { return Point (x * d, y * d, z * d); }
    void operator *= (const double & d) { x *= d; y *= d; z *= d; }
	Point operator / (const double & d) const { double inverse = 1.0/d; return Point(x * inverse, y * inverse, z * inverse); }
    void operator /= (const double & d) {x /= d; y /= d; z /= d; }
	Point operator + (const Point & p) const { return Point (x + p.x, y + p.y, z + p.z); }
    void operator += (const Point & p) { x += p.x; y += p.y; z += p.z; }
    void operator += (const double & d) { x += d; y += d; z += d; }
	Point operator - (const Point & p) const { return Point (x - p.x, y - p.y, z - p.z); }
    void operator -= (const Point & p) { x -= p.x; y -= p.y; z -= p.z; }
    void operator -= (const double & d) { x -= d; y -=d ; z -= d; }
	Point operator - () const { return Point(-x, -y, -z); }
	bool operator == (const Point & p) { return x == p.x && y == p.y && z == p.z; }
	bool operator != (const Point & p) { return *this != p; }
	double squaredLength() { return x * x + y * y + z * z; }
	double length() const { return sqrt((x * x) + (y * y) + (z * z)); }
	void normalize() { double l = length(); if(l == 0) { x = y = z = 0.0; } else { x /= l; y /= l; z /= l; } }
	Point normalized() const { double l = length(); if(l == 0) { return Point(0.0, 0.0, 0.0); } else { return Point(x / l, y / l, z / l); } }
	double dot(Point & p) { return x * p.x + y * p.y + z * p.z; }
	Point cross(const Point & v) const { return Point (y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x); }
	Point transformBy(const Transformation & t) const;

public:
	double x, y, z;
};

#endif // POINT_H
