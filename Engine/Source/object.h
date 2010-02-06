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
};

declareCollection (Object);

#endif //objectModule