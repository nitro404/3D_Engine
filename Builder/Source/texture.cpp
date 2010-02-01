
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                      Texture                                            //
//*****************************************************************************************//

//*****************************************************************************************//
//                          Private Packing/Unpacking Facilities                           //
//*****************************************************************************************//

#define redMask 0X000000FF
#define greenMask 0X0000FF00
#define blueMask 0X00FF0000
#define alphaMask 0XFF000000

#define redShift 0
#define greenShift 8
#define blueShift 16
#define alphaShift 24

inline long pack (const long red, const long green, const long blue, const long alpha) {
	return 
		((red << redShift) & redMask) | 
		((green << greenShift) & greenMask) | 
		((blue << blueShift) & blueMask)| 
		((alpha << alphaShift) & alphaMask);
}

inline long unpackRed (const long pixel) {
	return (pixel >> redShift) & 0X000000FF;
}

inline long unpackGreen (const long pixel) {
	return (pixel >> greenShift) & 0X000000FF;
}

inline long unpackBlue (const long pixel) {
	return (pixel >> blueShift) & 0X000000FF;
}

inline long unpackAlpha (const long pixel) {
	return (pixel >> alphaShift) & 0X000000FF;
}

inline bool isPowerOf2 (long value) {
	return (value & ~(value - 1)) == value;
}	

//*****************************************************************************************//
//                                Texture Implementation                                   //
//*****************************************************************************************//

Texture::Texture (long width, long height, TextureType type) {
	//Fill in all attributes.
	this->type = type;
	this->width = width;
	this->height = height;
	this->bytes = new long [width * height]; 
	this->textureHandle = -1; //If we don't get a handle, it will still be -1.
	this->textureName = NULL; //So far.
	glGenTextures (1, &this->textureHandle);
}

Texture::~Texture () {
	delete [] bytes; delete [] textureName;
}

//Private utility...
inline char *fileSuffix (const char *fileName) {
	//Returns a pointer to an ALL UPPERCASE string in a static area containing the 3 CHARACTER SUFFIX SUCH AS
	//"BMP" OR "TGA". Don't call twice on 2 different file names and then process (call once, process, call 
	//again, process, ...).
	long size = strlen (fileName); static char uppercase [5];
	for (char *s = (char *) &fileName [size - 4], *d = &uppercase [0]; *s; s++, d++) {
		*d = toupper (*s);
	}
	uppercase [4] = '\0'; //Make sure the file extension ends with a NULL
	return uppercase;
}

Texture *Texture::readTexture (char *fileName) {
	//Special case 3 possibilities...
	char *suffix = fileSuffix (fileName);
	Texture *readBMPTexture (char *fileName); //Forward reference.
	Texture *readTGATexture (char *fileName); //Forward reference.
	if (strcmp (suffix, ".BMP") == 0) return readBMPTexture (fileName);
	if (strcmp (suffix, ".TGA") == 0) return readTGATexture (fileName);
	halt ("\nUnknown texture type requested for \"%s\"...", fileName); 
	return NULL;
}	

Texture *readBMPTexture (char *fileName) {
	//Read a texture and fill in all attributes; returns NULL if unsuccessful. 
	//Creates a full RGBA texture rather than optimize with just a RGB texture...

	//Get Microsoft to read it (they must know how).
    HBITMAP bitmapHandle = (HBITMAP) LoadImage (NULL, fileName, 
    	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	if (bitmapHandle == NULL) {log ("\nFile \"%s\" not found...", fileName); return NULL;}

    //Find out how big it is.
	BITMAP bitmap; GetObject (bitmapHandle, sizeof (bitmap), &bitmap);
	long width = bitmap.bmWidth;	
	long byteWidth = bitmap.bmWidthBytes; 
	long height = bitmap.bmHeight;
	long pixelSize = bitmap.bmBitsPixel;
	long planes = bitmap.bmPlanes;
	char *bits = (char *) bitmap.bmBits;

	if (!isPowerOf2 (width) || !isPowerOf2 (height)) {
		//OpenGL needs a power of 2 (it is possible to force OpenGL to resize it but that's slow).
		prompt ("\nBitmap \"%s\" is not a power of 2; width %d, height %d.", fileName, width, height);
		log ("\nBitmap \"%s\" is not a power of 2; width %d, height %d.", fileName, width, height);
	}

	//Allocate space for RGBA format.
	Texture *texture = new Texture (width, height, RGBAType);
	if (texture == NULL) {DeleteObject (bitmapHandle); return NULL;}

	//Prepare to transfer the bits.
	long *destination = texture->bytes; //REVERSES: destination += height * width; //past the end
    BYTE *source = (BYTE *) bits; //at the beginning

	//Move the bits from the bitmap handle to the texture.	 
	long red, green, blue; long alpha = 255; //0=>transparent, 255=>opaque
	if (pixelSize == 24) {
		//Successively read 3 RGB bytes at a time, write RGBA bytes. 
    	for	(long j = height; j > 0; j--) {
			//REVERSES: destination -= width; 
			long *to = destination; BYTE *from = source;
    		for (int i = 0; i < width; i++) { 
				blue = *from++;	green = *from++; red = *from++;
				*to++ = pack (red, green, blue, alpha);
    		}   
			destination += width; 											   
			source += byteWidth;
		} 
	} else if (pixelSize == 8) { 
		//Obtains the palette information from the handle.
		typedef PALETTEENTRY PaletteEntries [256];
		HDC deviceContextHandle = CreateCompatibleDC (NULL);
		SelectObject (deviceContextHandle, bitmapHandle);
		PaletteEntries paletteEntries; 
		ZeroMemory (&paletteEntries, sizeof (PaletteEntries));
		GetDIBColorTable (deviceContextHandle, 0, 256, (RGBQUAD *) &paletteEntries);
		DeleteDC (deviceContextHandle);	
		long packedPaletteEntries [256];

		//Pack it into a nice RGBA array.
		long *palette = (long *) &paletteEntries; 
		for (int i = 0; i < 256; i++) {
			//DIB color tables have their colors stored BGR not RGB (so flip).
			BYTE red = paletteEntries [i].peBlue;
			BYTE green = paletteEntries [i].peGreen;
			BYTE blue = paletteEntries [i].peRed;
			packedPaletteEntries [i] = pack (red, green, blue, alpha);
		} 

		//Move the bits.
    	for	(long j = height; j > 0; j--) {
			//REVERSES: destination -= width; 
			long *to = destination; BYTE *from = source; 
			for (int i = width; i > 0; i--) { 
				*to++ = packedPaletteEntries [*from++];
			}
			destination += width; 
      		source += byteWidth; 
    	} 
	} else {
		//We only handle 8 and 24 bits so far...
		log ("\nRead texture can only handle 8 and 24 bit textures, not %d.", pixelSize); 
		delete texture; DeleteObject (bitmapHandle); return NULL;
	}

	texture->textureName = new char [strlen (fileName) + 1];
	strcpy (texture->textureName, fileName);
    DeleteObject (bitmapHandle); return texture;
}

Texture *readTGATexture (char *fileName) {
	//Creates either an RGBA texture or an RGB texture depending on whether or not the file
	//contains alpha bits... 24 bit TGA textures have not been tested...

	FILE *file = fopen (fileName, "rb");
	if (file == NULL) {::log ("\nUnable to open texture %s", fileName); return NULL;}

	#define logError(message) {log (message, fileName); fclose (file); return NULL;}
	struct TGAHeader {
		byte idLength, colorMapType, imageType, colorMapSpecification [5];
		short xOrigin, yOrigin, imageWidth, imageHeight;
		byte pixelDepth, imageDescriptor;
	};

	TGAHeader header;
	if (fread (&header, 1, sizeof (header), file) != sizeof (header))
		logError ("\nTGA file \"%s\" appears to be truncated.");
	if (header.colorMapType != 0) //1=>has color map, 0=>does not have color map.
		logError ("\nCan't read \"%s\" since it's paletted.");
	if (header.imageType != 2) //0..11; 2=>uncompressed true-color
		logError ("\nCan't read \"%s\" since it's compressed or not true color.");
	if (header.pixelDepth != 32 && header.pixelDepth != 24) {
		log ("\nFile \"%s\" is a %d bit .TGA file, need 24 or 32.", fileName, header.pixelDepth); 
		fclose (file); return NULL;
	}

	bool hasAlpha = header.pixelDepth == 32;
	bool useAlpha = header.pixelDepth == 32; //If hardwired to true, will create a full RGBA texture.

	//Allocate space for RGBA format.
	const long width = header.imageWidth; const long height = header.imageHeight; 
	Texture *texture = new Texture (width, height, useAlpha ? RGBAType : RGBType);
	if (texture == NULL) logError ("\nFailed to create textures for \"%s\".");

	//Prepare to copy the bits from the file.
	long numberOfPixels = width * height;
	long bytesSize = numberOfPixels * (useAlpha ? 4 : 3);
	BYTE *localBytes = (BYTE *) texture->bytes; BYTE *destination = (BYTE *) localBytes;
 
	long bytesRead = fread (localBytes, sizeof (BYTE), bytesSize, file);
	if (bytesRead == 0) {delete texture; logError ("\nUnable to read all the bytes in file \"%s\".");}
	fclose (file); //From here on, no longer have "close the file" as a pending action...

	//TGA is stored as BGR(A). Swizzle bits into RGB(A) format
	if (hasAlpha) {
		struct _RGBA {BYTE r, g, b, a;}; _RGBA *pixel = (_RGBA*) localBytes;
		for (long i = 0; i < numberOfPixels; i++, pixel++) {
			BYTE oldR = pixel->r; pixel->r = pixel->b; pixel->b = oldR;
		}
	} else {
		if (useAlpha) {//Need to shift in addition to swizzling.
			struct _RGB {BYTE r, g, b;}; _RGB *RGBpixel = (_RGB*) localBytes; RGBpixel += numberOfPixels - 1;
			struct _RGBA {BYTE r, g, b, a;}; _RGBA *RGBApixel = (_RGBA*) localBytes; RGBApixel += numberOfPixels - 1;
			long alpha = 255;
			for (long i = 0; i < numberOfPixels; i++, RGBpixel--, RGBApixel--) {
				BYTE R = RGBpixel->r; BYTE G = RGBpixel->g; BYTE B = RGBpixel->r;
				RGBApixel->r = B; RGBApixel->g = G; RGBApixel->b = R; RGBApixel->a = alpha;
			}
		} else {
			struct _RGB {BYTE r, g, b;}; _RGB *pixel = (_RGB*) localBytes;
			for (long i = 0; i < numberOfPixels; i++, pixel++) {
				BYTE oldR = pixel->r; pixel->r = pixel->b; pixel->b = oldR;
			}
		}
	}

	texture->textureName = new char [strlen (fileName) + 1];
	strcpy (texture->textureName, fileName);
    return texture;
	#undef logError
}

bool Texture::readTextureExtent (char *fileName, long &width, long &height) {
	//Read just enough of a texture to determine it's extent; i.e., width and height.
	//Returns true if successful; false otherwise. Special case 3 possibilities...
	char *suffix = fileSuffix (fileName);
	bool readRGBTextureExtent (char *fileName, long &width, long &height); //Forward reference.
	bool readTGATextureExtent (char *fileName, long &width, long &height); //Forward reference.
	if (strcmp (suffix, ".BMP") == 0) return readRGBTextureExtent (fileName, width, height);
	if (strcmp (suffix, ".TGA") == 0) return readTGATextureExtent (fileName, width, height);
	halt ("\nUnknown texture type requested for \"%s\"...", fileName); return false;
}

bool readRGBTextureExtent (char *fileName, long &width, long &height) {
	//Read just enough of a texture to determine it's extent; i.e., width and height.
	//Returns true if successful; false otherwise.

	//Get Microsoft to read it (they must know how).
    HBITMAP bitmapHandle = (HBITMAP) LoadImage (NULL, fileName, 
    	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	if (bitmapHandle == NULL) {log ("\nFile \"%s\" not found...", fileName); return false;}

    //Find out how big it is.
	BITMAP bitmap; GetObject (bitmapHandle, sizeof (bitmap), &bitmap);
	width = bitmap.bmWidth;	height = bitmap.bmHeight;
	DeleteObject (bitmapHandle); 
	return true;
}

bool readTGATextureExtent (char *fileName, long &width, long &height) {
	//Read just enough of a texture to determine it's extent; i.e., width and height.
	//Returns true if successful; false otherwise.

	FILE *file = fopen (fileName, "rb");
	if (file == NULL) {::log ("\nUnable to open texture %s", fileName); return false;}

	struct TGAHeader {
		byte idLength, colorMapType, imageType, colorMapSpecification [5];
		short xOrigin, yOrigin, imageWidth, imageHeight;
		byte pixelDepth, imageDescriptor;
	};

	TGAHeader header;
	if (fread (&header, 1, sizeof (header), file) != sizeof (header)) {
		log ("\nTGA file \"%s\" appears to be truncated."); return false;
	}
	fclose (file); width = header.imageWidth; height = header.imageHeight; 
	return true;
}

void Texture::activate () {
	if (textureHandle == -1) {glDisable (GL_TEXTURE_2D); return;}
	glEnable (GL_TEXTURE_2D);  //Turn on texturing.
	glBindTexture (GL_TEXTURE_2D, textureHandle); //Bind the current texture.
}

void Texture::load (bool mipmapping, bool forceClamp) {
	//Give the texture to the game card.
	if (textureHandle == -1 || bytes == NULL) {
		log ("\nTexture handle is %d (-1 means not set), bytes %x (null means not read)...", textureHandle, bytes);
		return;
	}
	static long alignment [2] = {4, 1};
	static GLint wrap [2] = {GL_REPEAT, GL_REPEAT}; //GL_CLAMP is currently not used
	static GLint components [2] = {GL_RGBA8, GL_RGB8};
	static GLenum format [2] = {GL_RGBA, GL_RGB};

	activate ();
	glPixelStorei (GL_PACK_ALIGNMENT, alignment [type]);
	GLint wrapping = forceClamp ? GL_CLAMP : wrap [type];
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint minFilter = mipmapping ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	if (mipmapping)
		gluBuild2DMipmaps (GL_TEXTURE_2D, components [type], width, height,
			format [type], GL_UNSIGNED_BYTE, bytes);
	else
		glTexImage2D (GL_TEXTURE_2D, 0, components [type], width, height, 0,
			format [type], GL_UNSIGNED_BYTE, bytes);
	//log ("\nLoad \"%s\", handle %d.", textureName, textureHandle);
}

void Texture::unload () {
	//log ("\nUnload \"%s\", handle %d.", textureName, textureHandle);
	if (textureHandle != -1) glDeleteTextures (1, &textureHandle);
	textureHandle = -1;
}
