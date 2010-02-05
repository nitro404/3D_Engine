//*****************************************************************************************//
//                                        Vehicle                                          //
//*****************************************************************************************//

#ifndef vehicleModule
#define vehicleModule 

class Vehicle : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Vehicle() { }

	~Vehicle() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);

	void printOn(ostream & o) const;
	
private:
	char * name;
	int style; // 0 = car, 1 = bike, 2 = tank
};

declareCollection(Vehicle);

ostream & operator << (ostream & o, const Vehicle & x);

#endif
