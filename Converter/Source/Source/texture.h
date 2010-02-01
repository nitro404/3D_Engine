//*****************************************************************************************//
//                                      Texture                                           //
//*****************************************************************************************//

#ifndef textureModule
#define textureModule 

enum TextureType {RGBAType, RGBType};

class Texture {
public:
	TextureType type;
	long width, height;
	GLuint textureHandle;
	char *textureName;
	long *bytes;

	Texture (char *name); //Used only for textures that cannot be located...
	Texture (long width, long height, TextureType type);
	~Texture ();

	void activate (); //Do this before drawing a polygon.
	void load (bool mipmapping = true, bool forceClamp = false); //Give it to the game card (use defaults).
	void unload (); //Let the game card lose it.
	
	static Texture *readTexture (char *fileName);
	static bool readTextureExtent (char *fileName, long &width, long &height);

	static void setup () {
		//Not yet finished, need isExtensionSupported...
		
		//static PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;
		/*
		if (isExtensionSupported ("GL_ARB_texture_compression", extensions)) {
			glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) wglGetProcAddress ("glCompressedTexImage2DARB");
		} else {
			halt ("\nCompressed DDS texture compression NEEDED to run...");
		}
		*/
	}
};

declareCollection (Texture);

#endif