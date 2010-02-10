//*****************************************************************************************//
//                                        Translator                                       //
//*****************************************************************************************//

#ifndef translatorModule
#define translatorModule 

class Translator : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;

	Translator() {
		forward = true;
	}

	~Translator() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}

	double distanceFrom(Point & p) const;
	
	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
private:
	char * name;
	Point origin;
	Point position;
	Point offset;
	Point destination;
	Vector unitVector;
	double rateInMetersPerSecond;
	bool forward;
	double distanceTravelled;
	double distanceToTravel;
};

declareCollection(Translator);

#endif //translatorModule