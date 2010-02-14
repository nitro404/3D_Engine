#ifndef _ANIMATEDFACE_H
#define _ANIMATEDFACE_H 

class AnimatedFace : public Face {
public:
	AnimatedTexture * animatedTexture;
	double animationSpeed;

	AnimatedFace() {
		textureIndex = 0;
		animatedTexture = NULL;
		animationSpeed = -1;
	}

	~AnimatedFace() { }

	void tick();
	void draw();
	void draw(Colour & colour);
	void import(ifstream & input, AnimatedTextureCollection & animatedTextures);
	
private:
	double textureIndex;
};

declareCollection(AnimatedFace);

#endif
