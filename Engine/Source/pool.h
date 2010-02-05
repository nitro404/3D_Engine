//*****************************************************************************************//
//                                        Pool                                             //
//*****************************************************************************************//

#ifndef poolModule
#define poolModule 

class Pool : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Pool() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}

	~Pool() {
		deleteFaceCollectionEntries(faces);
	}

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
};

declareCollection(Pool);

ostream & operator << (ostream & o, const Pool & x);

#endif //poolModule