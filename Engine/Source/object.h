//*****************************************************************************************//
//                                        Object                                           //
//*****************************************************************************************//

#ifndef objectModule
#define objectModule 

class Object {
public:
	Object () { }
	~Object () { }

	virtual void tick () {}
	virtual void draw () {}
	virtual void import (ifstream &input, TextureCollection & textures) {}

	virtual void printOn(ostream & o) const {}
};

declareCollection (Object);

ostream & operator << (ostream & o, const Object & x);

#endif //objectModule