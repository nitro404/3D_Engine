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
private:
	
};

declareCollection(Geometry);

#endif //geometryModule