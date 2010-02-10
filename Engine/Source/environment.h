//*****************************************************************************************//
//                                          Environment                                    //
//*****************************************************************************************//

#ifndef environmentModule
#define environmentModule 

class Environment : public Object {
public:
	char ** skyboxTextureExtensions;

	Environment() {
		skyboxTextureExtensions = new char*[6];
		for(int i=0;i<6;i++) {
			skyboxTextureExtensions[i] = new char[7];
		}
		strcpy(skyboxTextureExtensions[0], "-left");
		strcpy(skyboxTextureExtensions[1], "-right");
		strcpy(skyboxTextureExtensions[2], "-front");
		strcpy(skyboxTextureExtensions[3], "-back");
		strcpy(skyboxTextureExtensions[4], "-up");
		strcpy(skyboxTextureExtensions[5], "-down");
	}
	~Environment() {
for(int i=0;i<skyboxTextures.size();i++) {
skyboxTextures.at(i)->unload();
delete skyboxTextures.at(i);
}
	}

	double distanceFrom(Point & p) const { return 0; }

	void tick();
	void draw();

	void import(ifstream &input, TextureCollection & textures);
private:
	char * name;
	double * ambientLight;
	int cloudLayers;
	vector<Texture *> skyboxTextures;
	double * skyColour;
	int surrounds;
};

declareCollection(Environment);

#endif //environmentModule