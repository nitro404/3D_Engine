#ifndef _VEHICLE_H
#define _VEHICLE_H 

class Vehicle : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Vehicle() { }

	~Vehicle() {
		delete [] name;
		deleteFaceCollectionEntries(faces);
	}

	double distanceFrom(Point & p) const;

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);
private:
	char * name;
	int style; // 0 = car, 1 = bike, 2 = tank
};

declareCollection(Vehicle);

#endif
