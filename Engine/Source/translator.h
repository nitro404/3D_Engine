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
		forwardDirectionX = true;
		forwardDirectionY = true;
		forwardDirectionZ = true;
	}

	~Translator() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}
	
	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
private:
	char * name;
	Point origin;
	Point currentLocation;
	Point offset;
	double rateInMetersPerSecond;
	bool forwardDirectionX;
	bool forwardDirectionY;
	bool forwardDirectionZ;
};

declareCollection(Translator);

#endif //translatorModule