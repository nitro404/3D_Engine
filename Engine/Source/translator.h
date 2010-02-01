//*****************************************************************************************//
//                                        Translator                                       //
//*****************************************************************************************//

#ifndef translatorModule
#define translatorModule 

class Translator : public Object {
public:
	Translator();
	~Translator();

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
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

ostream & operator << (ostream & o, const Translator & x);

#endif //translatorModule