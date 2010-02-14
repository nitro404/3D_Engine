#ifndef _ROTATOR_H
#define _ROTATOR_H 

class Rotator : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Rotator() {
		angleInDegrees = 0;
		rateInDegreesPersecond = 0;
	}
	
	~Rotator() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}
	
	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);
	
private:
	char * name;
	Point axis;
	double rateInDegreesPersecond;
	double angleInDegrees;
};

declareCollection(Rotator);

#endif
