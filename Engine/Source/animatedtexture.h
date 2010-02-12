#ifndef _ANIMATEDTEXTURE_H
#define _ANIMATEDTEXTURE_H

class AnimatedTexture {
public:
	char * name;
	int frames;
	int speed;
	vector<Texture *> textures;

	AnimatedTexture() { }

	~AnimatedTexture() {
		delete [] name;
	}
	
	void import(ifstream &input, TextureCollection & worldTextures);
};


declareCollection(AnimatedTexture);

#endif
