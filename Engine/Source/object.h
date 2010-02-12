#ifndef _OBJECT_H
#define _OBJECT_H

class Object {
public:
	Object() { }
	~Object() { }
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void tick () { }
	virtual void draw () { }
	virtual void import (ifstream &input, TextureCollection & textures) { }
};

declareCollection (Object);

#endif
