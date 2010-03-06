#include "Texture.h"

long Texture::pack(const long red, const long green, const long blue, const long alpha) {
	return  ((red << 0) & 0X000000FF) |
			((green << 8) & 0X0000FF00) |
			((blue << 16) & 0X00FF0000) |
			((alpha << 24) & 0XFF000000);
}

Texture::Texture(long width, long height, TextureType type) {
	this->type = type;
	this->width = width;
	this->height = height;
	this->bytes = new long[width * height]; 
	this->textureHandle = -1;
	this->textureLoaded = false;
	this->textureName = NULL;
	glGenTextures(1, &this->textureHandle);
}

Texture::~Texture() {
	this->unload();
	if(bytes != NULL) {
		delete [] bytes;
	}
	delete [] textureName;
}

Texture * Texture::readTexture(char * fullPathName) {
	char * suffix = strrchr(fullPathName, '.');
	if(_stricmp(suffix, ".bmp") == 0) {
		return readBMPTexture(fullPathName);
	}
	else if(_stricmp(suffix, ".tga") == 0) {
		return readTGATexture(fullPathName);
	}
	quit("Unknown texture type for \"%s\".\n", fullPathName);
	return NULL;
}	

Texture * Texture::readBMPTexture(char *fullPathName) {
	//Read a texture and fill in all attributes; returns NULL if unsuccessful.
	//Creates a full RGBA texture rather than optimize with just a RGB texture...

	//Get Microsoft to read it (they must know how).
    HBITMAP bitmapHandle = (HBITMAP) LoadImage (NULL, fullPathName, 
    	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);	
	if(bitmapHandle == NULL) {
		return NULL;
	}

    //Find out how big it is.
	BITMAP bitmap; GetObject (bitmapHandle, sizeof (bitmap), &bitmap);
	long width = bitmap.bmWidth;	
	long byteWidth = bitmap.bmWidthBytes; 
	long height = bitmap.bmHeight;
	long pixelSize = bitmap.bmBitsPixel;
	long planes = bitmap.bmPlanes;
	char * bits = (char *) bitmap.bmBits;
	
	// check that the dimensions of the image are powers of 2
	if(!(width & ~(width - 1)) == width || !(height & ~(height - 1)) == height) {
	
		//OpenGL needs a power of 2 (it is possible to force OpenGL to resize it but that's slow).
		printf("\nBitmap \"%s\" is not a power of 2; width %d, height %d.", fullPathName, width, height);
	}

	//Allocate space for RGBA format.
	Texture *texture = new Texture (width, height, RGBAType);
	if(texture == NULL) {DeleteObject (bitmapHandle); return NULL;}

	//Prepare to transfer the bits.
	long * destination = texture->bytes; //REVERSES: destination += height * width; //past the end
    BYTE * source = (BYTE *) bits; //at the beginning

	//Move the bits from the bitmap handle to the texture.	 
	long red, green, blue; long alpha = 255; //0=>transparent, 255=>opaque
	if(pixelSize == 24) {
		//Successively read 3 RGB bytes at a time, write RGBA bytes. 
    	for(long j=height;j>0;j--) {
			//REVERSES: destination -= width; 
			long * to = destination; BYTE *from = source;
    		for (int i = 0; i < width; i++) { 
				blue = *from++;	green = *from++; red = *from++;
				*to++ = pack (red, green, blue, alpha);
    		}   
			destination += width; 											   
			source += byteWidth;
		} 
	}
	else if (pixelSize == 8) { 
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
		delete texture; DeleteObject (bitmapHandle); return NULL;
	}

	texture->textureName = new char [strlen (fullPathName) + 1];
	strcpy_s (texture->textureName, strlen(fullPathName) + 1, fullPathName);
    DeleteObject (bitmapHandle); return texture;
}

Texture * Texture::readTGATexture(char * fullPathName) {
	//Creates either an RGBA texture or an RGB texture depending on whether or not the file
	//contains alpha bits... 24 bit TGA textures have not been tested...

	FILE * file;
	int errNo = fopen_s(&file,fullPathName, "rb");
	if(errNo != 0) {
		return NULL;
	}

//	#define logError(message) {log (message, fullPathName); fclose (file); return NULL;}
	struct TGAHeader {
		byte idLength, colorMapType, imageType, colorMapSpecification [5];
		short xOrigin, yOrigin, imageWidth, imageHeight;
		byte pixelDepth, imageDescriptor;
	};

	TGAHeader header;
	if (fread (&header, 1, sizeof (header), file) != sizeof (header)) {
		printf("TGA file \"%s\" appears to be truncated.\n", fullPathName);
	}
	if (header.colorMapType != 0) { //1=>has color map, 0=>does not have color map.
		printf("Can't read \"%s\" since it's paletted.\n", fullPathName);
	}
	if (header.imageType != 2) { //0..11; 2=>uncompressed true-color
		printf("Can't read \"%s\" since it's compressed or not true color.", fullPathName);
	}
	if (header.pixelDepth != 32 && header.pixelDepth != 24) {
		printf("File \"%s\" is a %d bit .TGA file, need 24 or 32.\n", fullPathName, header.pixelDepth); 
		fclose(file);
		return NULL;
	}

	bool hasAlpha = header.pixelDepth == 32;
	bool useAlpha = header.pixelDepth == 32; //If hardwired to true, will create a full RGBA texture.

	//Allocate space for RGBA format.
	const long width = header.imageWidth; const long height = header.imageHeight; 
	Texture *texture = new Texture (width, height, useAlpha ? RGBAType : RGBType);
	if(texture == NULL) {
		printf("Failed to create textures for \"%s\".\n");
	}

	//Prepare to copy the bits from the file.
	long numberOfPixels = width * height;
	long bytesSize = numberOfPixels * (useAlpha ? 4 : 3);
	BYTE *localBytes = (BYTE *) texture->bytes; BYTE *destination = (BYTE *) localBytes;
 
	long bytesRead = fread (localBytes, sizeof (BYTE), bytesSize, file);
	if(bytesRead == 0) {
		delete texture;
		printf("Unable to read all the bytes in file \"%s\".", fullPathName);
	}
	fclose(file); //From here on, no longer have "close the file" as a pending action...

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

	texture->textureName = new char [strlen (fullPathName) + 1];
	strcpy_s (texture->textureName, strlen(fullPathName) + 1, fullPathName);
    return texture;
}

void Texture::activate() {
	if(!textureLoaded) {
		printf("Texture \"%s\" not loaded onto video card.\n", textureName);
		return;
	}
	if(textureHandle == -1) {
		glDisable(GL_TEXTURE_2D);
		return;
	}

	glEnable(GL_TEXTURE_2D); //Turn on texturing.
	glBindTexture(GL_TEXTURE_2D, textureHandle); //Bind the current texture.
}

void Texture::load(bool mipmapping, bool forceClamp) {
	//Give the texture to the game card.
	if(textureHandle == -1 || bytes == NULL) {
		return;
	}
	
	static long alignment[2] = {4, 1};
	static GLint wrap[2] = {GL_REPEAT, GL_REPEAT}; //GL_CLAMP is currently not used
	static GLint components[2] = {GL_RGBA8, GL_RGB8};
	static GLenum format[2] = {GL_RGBA, GL_RGB};
	
	textureLoaded = true;
	activate();
	
	glPixelStorei(GL_PACK_ALIGNMENT, alignment[type]);
	GLint wrapping = forceClamp ? GL_CLAMP : wrap[type];
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLint minFilter = mipmapping ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

	if(mipmapping) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, components[type], width, height, format[type], GL_UNSIGNED_BYTE, bytes);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, components[type], width, height, 0, format[type], GL_UNSIGNED_BYTE, bytes);
	}
	
	if(bytes != NULL) {
		delete [] bytes;
		bytes = NULL;
	}
}

void Texture::unload() {
	if(textureHandle != -1) {
		glDeleteTextures(1, & textureHandle);
	}

	textureHandle = -1;
	textureLoaded = false;
}
