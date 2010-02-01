//*****************************************************************************************//
//                                        Rotator                                          //
//*****************************************************************************************//

#ifndef rotatorModule
#define rotatorModule 

class Rotator : public Object {
public:
	Rotator();
	~Rotator();

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	char * name;
	Point axis;
	double rateInDegreesPersecond;
	double angleInDegrees;
};

declareCollection(Rotator);

ostream & operator << (ostream & o, const Rotator & x);

#endif //rotatorModule