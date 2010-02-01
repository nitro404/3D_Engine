
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"
#include "glati.h" //For DDS compressed texture support
//#include <ddraw.h> //For DDS compressed texture support
#include "../Direct3D/ddraw.h" //For DDS compressed texture support

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

Texture::Texture (char *name) {
	//Used only if a texture does not exist (properly keep the name)...
	this->type = RGBType;
	this->width = 0;
	this->height = 0;
	this->bytes = NULL; 
	this->textureHandle = -1; //If we don't get a handle, it will still be -1.
	this->textureName = new char [strlen (name) + 1];
	strcpy (this->textureName, name);
}

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
#define first strchr
#define last strrchr

inline char *fileSuffix (const char *fileName) {
	//For "c:\test\more\yellow.tga", returns "tga".
	//For "..\more\yellow.tga", returns "tga".
	//For "..\more\yellow", returns "".

	char *start = last (fileName, '.'); 
	return start == NULL ? "" : (first (start, '\\') == NULL ? start + 1 : "");


	/*
	//Returns a pointer to an ALL UPPERCASE string in a static area containing the 3 CHARACTER SUFFIX SUCH AS
	//"BMP" OR "TGA". Don't call twice on 2 different file names and then process (call once, process, call 
	//again, process, ...).
	long size = strlen (fileName); static char uppercase [5];
	for (char *s = (char *) &fileName [size - 4], *d = &uppercase [0]; *s; s++, d++) {
		*d = toupper (*s);
	}
	uppercase [4] = '\0'; //Make sure the file extension ends with a NULL
	return uppercase;
	*/
}

inline char *bareFileName (const char *fileName) {
	//For "c:\test\more\yellow.tga", returns "yellow" in a static area...
	static char name [256]; 

	char *pastEnd = last (fileName, '.'); 
	pastEnd = pastEnd == NULL ? NULL : (first (pastEnd, '\\') == NULL ? pastEnd : NULL);
	if (pastEnd == NULL) pastEnd = (char *) &fileName [strlen (fileName)];

	char *beforeStart = last (fileName, '\\'); 
	if (beforeStart == NULL) beforeStart = first (fileName, ':');
	if (beforeStart == NULL) beforeStart = (char *) (fileName - 1);

	long size = pastEnd - beforeStart - 1;
	CopyMemory (name, beforeStart + 1, size);
	name [size] = '\0';
	return name;
}

#undef first 
#undef last 

Texture *Texture::readTexture (char *fileName) {
	//Special case 3 possibilities...
	char *suffix = fileSuffix (fileName);
	Texture *readBMPTexture (char *fileName); //Forward reference.
	Texture *readTGATexture (char *fileName); //Forward reference.
	Texture *readDDSTexture (char *fileName); //Forward reference.
	if (stricmp (suffix, "BMP") == 0) return readBMPTexture (fileName);
	if (stricmp (suffix, "TGA") == 0) return readTGATexture (fileName);
	if (stricmp (suffix, "DDS") == 0) return readDDSTexture (fileName);
	halt ("\nUnknown texture type requested for \"%s\"...", fileName); 
	return NULL;
}	

Texture *readBMPTexture (char *fileName) {
	//Read a texture and fill in all attributes; returns NULL if unsuccessful. 
	//Creates a full RGBA texture rather than optimize with just a RGB texture...

	//Get Microsoft to read it (they must know how).
    HBITMAP bitmapHandle = (HBITMAP) LoadImage (NULL, fileName, 
    	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	if (bitmapHandle == NULL) {/*::log ("\nFile \"%s\" not found...", fileName);*/ return NULL;}

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

	char *shortName = bareFileName (fileName);
	texture->textureName = new char [strlen (shortName) + 1];
	strcpy (texture->textureName, shortName);
    DeleteObject (bitmapHandle); return texture;
}

Texture *readTGATexture (char *fileName) {
	//Creates either an RGBA texture or an RGB texture depending on whether or not the file
	//contains alpha bits... 24 bit TGA textures have not been tested...

	FILE *file = fopen (fileName, "rb");
	if (file == NULL) {/*::log ("\nUnable to open texture %s", fileName);*/ return NULL;}

	#define logError(message) {log (message, fileName); fclose (file); return NULL;}
	
	struct TGAHeader {
		BYTE idLength, colorMapType, imageType, colorMapSpecification [5];
		short xOrigin, yOrigin, imageWidth, imageHeight;
		BYTE pixelDepth, imageDescriptor;
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

	//Create a texture to store the data in.
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

	char *shortName = bareFileName (fileName);
	texture->textureName = new char [strlen (shortName) + 1];
	strcpy (texture->textureName, shortName);
    return texture;
	#undef logError
}

Texture *readCompressedDDS (FILE *file, const DDSURFACEDESC2 &description) {
	GLenum dataFormat; int factor;
	switch (description.ddpfPixelFormat.dwFourCC) {
		case FOURCC_DXT1:
			dataFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			factor = 2;
			break;
		case FOURCC_DXT3:
			dataFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			factor = 4;
			break;
		case FOURCC_DXT5:
			dataFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			factor = 4;
			break;
		default:
			halt ("\nUnsupported compressed texture format!!");
			fclose (file);
			return false;
	}

	//How big is it going to be including all mipmaps?
	long blockWidth = (description.dwWidth + 3) / 4;
	long blockHeight = (description.dwHeight + 3) / 4;
	long blockSize = (description.ddpfPixelFormat.dwFourCC == FOURCC_DXT1) ? 8 : 16;

	//Because Microsoft is lame and doesn't encode linear size in their
	//DDS images, we need to compute it here.
	long linearSize = blockWidth * blockHeight * blockSize;
	//assert (description.dwLinearSize == linearSize || description.dwLinearSize == 0);
	long bufferSize = description.dwMipMapCount > 1 ? linearSize * factor : linearSize;
	
	BYTE *bytes = new BYTE [bufferSize];
	fread (bytes, 1, bufferSize, file);

	//Close the file pointer.
	fclose (file);

	long width = description.dwWidth; long height = description.dwHeight;

	Texture *texture = new Texture (width, height, RGBAType); //useAlpha ? RGBAType : RGBType);

	long bytesPerPixel = (description.ddpfPixelFormat.dwFourCC == FOURCC_DXT1) ? 3 : 4;
	long mipmaps = (description.dwFlags & DDSD_MIPMAPCOUNT) ? description.dwMipMapCount : 1; //By default, only one level...
	//assert (mipmaps > 0);

	//Initialize the texture
	glBindTexture (GL_TEXTURE_2D, texture->textureHandle);

	bool forceClamp = false;
	GLint wrapping = forceClamp ? GL_CLAMP : GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);

	float anisotropyValue;
	glGetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropyValue);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyValue);

	//Load the mipmaps
	long offset = 0;
	for (long i = 0; i < mipmaps && (width > 0 || height > 0); ++i)	{
		if (width == 0) width = 1;
		if (height == 0) height = 1;
		int size = ((width+3)/4) * ((height+3)/4) * blockSize;
		//::log ("\nUniversal converter can't currently handle compressed mipmaps...");
		//return NULL;
		glCompressedTexImage2DARB (GL_TEXTURE_2D, i, dataFormat, width, height, 
		  0, size, bytes + offset);
    	//logCurrentOpenGLError ("Compressed texture image 2D");
		offset += size; width >>= 1; height >>= 1;
	}

	return texture;

}

Texture *readUncompressedDDS (FILE *file, const DDSURFACEDESC2 &description) {
	//::log ("\nLoading uncompressed DDS file...");
	//how big is it going to be including all mipmaps?  This is a slight over-estimate for
	//non-compressed mipmapped DDS files, because they only contain the exact number of bits needed
	//for the mipmaps.  Doesn't matter -- fread will only read all the bits in the file.

	long width = description.dwWidth; long height = description.dwHeight;
	Texture *texture = new Texture (width, height, RGBAType); //useAlpha ? RGBAType : RGBType);

	if (description.ddpfPixelFormat.dwRGBBitCount < 24)
		halt ("\nDon't currently handle 16-bit OR less Uncompressed DDS textures...");
	//long bytesPerPixel = description.ddpfPixelFormat.dwRGBBitCount / 8;
	bool hasAlpha = description.ddpfPixelFormat.dwRGBAlphaBitMask != 0;
	long mipmaps = (description.dwFlags & DDSD_MIPMAPCOUNT) ? description.dwMipMapCount : 1; //By default, only one level...
	//assert (mipmaps > 0);

	long linearSize = width * height * (hasAlpha ? 4 : 3);
	//assert (description.dwLinearSize == linearSize || description.dwLinearSize == 0);

	long bufferSize = mipmaps == 1 ? linearSize : linearSize * 2;
	BYTE *bytes = new BYTE [bufferSize];
	//Read the bits.
	long actualSize = fread (bytes, 1, bufferSize, file);
	//Close the file pointer.
	fclose (file);
	
	//Initialize the texture
	glBindTexture (GL_TEXTURE_2D, texture->textureHandle);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	bool forceClamp = false;
	GLint wrapping = forceClamp ? GL_CLAMP : GL_REPEAT;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//If we don't have mipmaps, then use linear min filtering (else we get a white image)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmaps == 1 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);

	float anisotropyValue;
	glGetFloatv (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &anisotropyValue);
	glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropyValue);

	//Fill in all the mipmap levels.
	{
		long offset = 0;
		long numComponents = hasAlpha ? 4 : 3;
		for (long i = 0; i < mipmaps && (width > 0 || height > 0); ++i)	{
			if (width == 0) width = 1;
			if (height == 0) height = 1;
			glTexImage2D (GL_TEXTURE_2D, i, numComponents, width, height, 0, hasAlpha ? GL_BGRA_EXT : GL_BGR_EXT, GL_UNSIGNED_BYTE, bytes + offset);
			//logCurrentOpenGLError ("Non-compressed DDS texture image 2D");
			offset += width * height * numComponents;
			width >>= 1;
			height >>= 1;
		}
	}
	return texture;
}

Texture *readDDSTexture (char *fileName) {
	//Try to open the file.
	FILE *file = fopen (fileName, "rb");
	if (file == NULL) {
		//::log ("\nUnable to open texture %s", fileName);
		return NULL;
	}

	//Verify the type of file.
	char filecode[4];
	fread (filecode, 1, 4, file);
	if (strncmp (filecode, "DDS ", 4) != 0) {
		fclose (file);
		return NULL;
	}

	//Get the surface description.
	DDSURFACEDESC2 description;
	fread (&description, sizeof (description), 1, file);

	bool compressed = (description.ddpfPixelFormat.dwFlags & DDPF_RGB) == 0;
	Texture *texture = compressed ? 
		readCompressedDDS (file, description) : 
		readUncompressedDDS (file, description); 
		
	if (texture != NULL) {
		char *shortName = bareFileName (fileName);
		texture->textureName = new char [strlen (shortName) + 1];
		strcpy (texture->textureName, shortName);
	}
	return texture;
}

bool Texture::readTextureExtent (char *fileName, long &width, long &height) {
	//Read just enough of a texture to determine it's extent; i.e., width and height.
	//Returns true if successful; false otherwise. Special case 3 possibilities...
	char *suffix = fileSuffix (fileName);
	bool readRGBTextureExtent (char *fileName, long &width, long &height); //Forward reference.
	bool readTGATextureExtent (char *fileName, long &width, long &height); //Forward reference.
	bool readDDSTextureExtent (char *fileName, long &width, long &height); //Forward reference.
	bool readAVITextureExtent (char *fileName, long &width, long &height); //Forward reference.
	if (stricmp (suffix, "BMP") == 0) return readRGBTextureExtent (fileName, width, height);
	if (stricmp (suffix, "TGA") == 0) return readTGATextureExtent (fileName, width, height);
	if (stricmp (suffix, "DDS") == 0) return readDDSTextureExtent (fileName, width, height);
	if (stricmp (suffix, "AVI") == 0) return readAVITextureExtent (fileName, width, height);
	halt ("\nUnknown texture type requested for \"%s\"...", fileName); return false;
}

bool readRGBTextureExtent (char *fileName, long &width, long &height) {
	//Read just enough of a texture to determine it's extent; i.e., width and height.
	//Returns true if successful; false otherwise.

	//Get Microsoft to read it (they must know how).
    HBITMAP bitmapHandle = (HBITMAP) LoadImage (NULL, fileName, 
    	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	if (bitmapHandle == NULL) {/*::log ("\nFile \"%s\" not found...", fileName);*/ return false;}

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
	if (file == NULL) {/*::log ("\nUnable to open texture %s", fileName);*/ return false;}

	struct TGAHeader {
		BYTE idLength, colorMapType, imageType, colorMapSpecification [5];
		short xOrigin, yOrigin, imageWidth, imageHeight;
		BYTE pixelDepth, imageDescriptor;
	};

	TGAHeader header;
	if (fread (&header, 1, sizeof (header), file) != sizeof (header)) {
		::log ("\nTGA file \"%s\" appears to be truncated."); return false;
	}
	fclose (file); width = header.imageWidth; height = header.imageHeight; 
	return true;
}


bool readDDSTextureExtent (char *fileName, long &width, long &height) {
	//Try to open the file
	FILE *file = fopen (fileName, "rb");
	if (file == NULL) {
		//::log ("\nUnable to open texture %s", fileName);
		return false;
	}

	//Verify the type of file .
	char filecode [4];
	fread (filecode, 1, 4, file);
	if (strncmp (filecode, "DDS ", 4) != 0) {fclose (file); return false;}

	//Get the surface description.
	DDSURFACEDESC2 description;
	fread (&description, sizeof (description), 1, file);

	height = description.dwHeight; width = description.dwWidth;
	fclose (file); 
	return true;
}

#include <vfw.h>
bool readAVITextureExtent (char *fileName, long &width, long &height) {
	//Current avi textures were built assuming 128x128 rather than the actual size.
	//Note that the real size for "skiing.avi" is 256x256.

	//::log ("\nTrying to read extent for %s", fileName);

	AVIFileInit ();
		//Try to open the AVI file.
		PAVIFILE file;
		if (AVIFileOpen (&file, fileName, OF_READ, NULL) != 0) {
			AVIFileExit (); return false;
		}

		AVIFILEINFO fileInformation;
		//Find out extent of each frame.
		if (AVIFileInfo (file, &fileInformation, sizeof (AVIFILEINFO)) != 0) {
			::log ("\nFailed to decode frame extent for AVI file \"%s\".", fileName);
			AVIFileRelease (file); AVIFileExit (); return false;
		}
		extern long GlobalTextureWidth, GlobalTextureHeight;
		width = fileInformation.dwWidth; 
		height = fileInformation.dwHeight;

		AVIFileRelease (file); 
	AVIFileExit (); 
	
	const bool fakingExtent = false;
	if (fakingExtent) {
		::log ("\nExtent of \"%s\" is %dx%d but using ", fileName, width, height);
		width = height = 256; 
		::log ("%dx%d instead...", width, height);
		return true;
	}
	return true; //Success
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
