//*****************************************************************************************//
//                                   AnimatedFace                                          //
//*****************************************************************************************//

#ifndef animatedFaceModule
#define animatedFaceModule 

class AnimatedFace : public Face {
private:
	double textureIndex;
	static double animationSpeed;
public:
	//A face owns it's points and therefore deletes them on its own.
	//It does not own it's texture because other faces will refer to the same texture...
	//The world should own the texture...
//	Texture * texture;
	Texture ** textures;
//	GamePointCollection points;

	AnimatedFace() {
		textureIndex = 0;
//		texture = NULL;
		textures = NULL;
	}

	~AnimatedFace() {
for(int i=0;i<32;i++){
delete textures[i];
}
delete [] textures;
//		deleteGamePointCollectionEntries(points);
	}

	void tick();
	void draw();
	void draw (double red, double green, double blue, double alpha);
	void import(ifstream &input, TextureCollection & textures);
	void import(ifstream &input);
};


declareCollection(AnimatedFace);

#endif //animatedFaceModule