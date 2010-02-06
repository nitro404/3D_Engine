//*****************************************************************************************//
//                                        Rotator                                          //
//*****************************************************************************************//

#ifndef rotatorModule
#define rotatorModule 

class Rotator : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;

	Rotator() {
		angleInDegrees = 0;
	}
	
	~Rotator() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
private:
	char * name;
	Point axis;
	double rateInDegreesPersecond;
	double angleInDegrees;
};

declareCollection(Rotator);

#endif //rotatorModule