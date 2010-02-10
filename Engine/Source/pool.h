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

	}

	~Pool() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}
	
	double distanceFrom(Point & p) const;
	bool insideOf(Point & p) const;
	
	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
private:
	double maxX, maxY, maxZ;
	double minX, minY, minZ;
	char * name;
	double red;
	double green;
	double blue;
	double alpha;
};

declareCollection(Pool);

#endif //poolModule