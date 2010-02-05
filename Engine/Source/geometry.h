//*****************************************************************************************//
//                                        Static Geometry                                  //
//*****************************************************************************************//

#ifndef geometryModule
#define geometryModule 

class Geometry : public Object {
public:
	DualTransformation transformation;
	FaceCollection faces;
	
	Geometry() { }
	~Geometry() {
		deleteFaceCollectionEntries(faces);
	}

	void tick();
	void draw();

	void import (ifstream &input, TextureCollection & textures);
	
	void printOn(ostream & o) const;
	
private:
	
};

declareCollection(Geometry);

ostream & operator << (ostream & o, const Geometry & x);

#endif //geometryModule