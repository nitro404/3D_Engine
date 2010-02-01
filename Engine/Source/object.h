//*****************************************************************************************//
//                                        Object                                           //
//*****************************************************************************************//

#ifndef objectModule
#define objectModule 

class Object {
public:
	DualTransformation transformation;
	FaceCollection faces;

	Object () { };
	~Object () { deleteFaceCollectionEntries (faces); }

	virtual void tick () {};
	virtual void draw () {};
	virtual void import (ifstream &input, TextureCollection & textures) {};

	virtual void printOn(ostream & o) const {};
};

declareCollection (Object);

ostream & operator << (ostream & o, const Object & x);

#endif //objectModule