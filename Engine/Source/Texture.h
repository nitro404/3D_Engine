#ifndef TEXTURE_H
#define TEXTURE_H

#include "Includes.h"

enum TextureType {RGBAType, RGBType};

class Texture {
public:
	Texture(long width, long height, TextureType type);
	~Texture();
	
	static long pack(const long red, const long green, const long blue, const long alpha);
	
	void activate();
	
	static Texture * readTexture(char * fileName);
	static Texture * readBMPTexture(char * fullPathName);
	static Texture * readTGATexture(char * fullPathName);

private:
	void load(bool mipmapping = true, bool forceClamp = false);

public:
	TextureType type;
	long width, height;

private:
	GLuint textureHandle;
	bool textureLoaded;
	char * textureName;
	long * bytes;
};

#endif // TEXTURE_H
