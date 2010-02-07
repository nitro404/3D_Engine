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
private:
	char * name;
	double red;
	double green;
	double blue;
	double alpha;
};

declareCollection(Pool);

#endif //poolModule