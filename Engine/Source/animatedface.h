#ifndef _ANIMATEDFACE_H
#define _ANIMATEDFACE_H 

class AnimatedFace : public Face {
private:
	double textureIndex;
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
	void draw(double red, double green, double blue, double alpha);
	void import(ifstream &input, AnimatedTextureCollection & animatedTextures);
};


declareCollection(AnimatedFace);

#endif
