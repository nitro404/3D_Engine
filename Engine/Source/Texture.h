#pragma once

#include "Includes.h"

enum TextureType {RGBAType, RGBType};

class Texture {
public:
	TextureType type;
	long width, height;
	GLuint textureHandle;
	bool textureLoaded;
	char * textureName;
	long * bytes;

	Texture(long width, long height, TextureType type);
	~Texture();
	
	static long pack(const long red, const long green, const long blue, const long alpha);
	
	void activate();
	
	static Texture * readTexture(char * fileName);
	static Texture * readBMPTexture(char * fullPathName);
	static Texture * readTGATexture(char * fullPathName);

private:
	void load(bool mipmapping = true, bool forceClamp = false); //Give it to the game card (use defaults).
};
