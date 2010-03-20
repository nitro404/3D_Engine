#include "Font.h"

const int Font::NORMAL = FW_NORMAL;
const int Font::REGULAR = FW_REGULAR;
const int Font::BOLD = FW_BOLD;

Font::Font(int windowWidth,
		   int windowHeight,
		   const char * fontFace,
		   int fontSize,
		   int fontWeight,
		   bool fontItalic,
		   bool fontUnderline,
		   bool fontStrikeout,
		   float fontAngle)
			: face(NULL),
			  size((fontSize < 0) ? 12 : fontSize),
			  weight((fontWeight == NORMAL || fontWeight == REGULAR || fontWeight == BOLD) ? fontWeight : NORMAL),
			  italic(fontItalic),
			  underline(fontUnderline),
			  strikeout(fontStrikeout),
			  angle((fontAngle < 0 || fontAngle > 360) ? 0 : fontAngle) {
	if(fontFace == NULL) {
		this->face = new char[6];
		strcpy_s(this->face, 6, "Arial");
	}
	else {
		face = new char[strlen(fontFace) + 1];
		strcpy_s(this->face, strlen(fontFace) + 1, fontFace);
	}

	createFont();
}

Font::Font(const Font & x) {
	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->underline = x.underline;
	this->strikeout = x.strikeout;
	this->angle = x.angle;

	createFont();
}

Font & Font::operator = (const Font & x) {
	delete [] this->face;
	ReleaseDC(NULL, deviceContext);
	glDeleteLists(fontBase, 96);

	this->face = new char[strlen(x.face) + 1];
	strcpy_s(this->face, strlen(x.face) + 1, x.face);

	this->size = x.size;
	this->weight = x.weight;
	this->italic = x.italic;
	this->underline = x.underline;
	this->strikeout = x.strikeout;
	this->angle = x.angle;

	createFont();

	return *this;
}

Font::~Font() {
	delete [] face;
	ReleaseDC(NULL, deviceContext);
	glDeleteLists(fontBase, 96);
}

int Font::getSize() const { return size; }

GLuint Font::getBase() const { return fontBase - 32; }

void Font::createFont() {
	// setup the font
	deviceContext = GetDC(NULL);
	HFONT font; // windows font ID
	fontBase = glGenLists(96); // storage for 96 characters
	font = CreateFont (
		-size,							// height of font
		0,								// width of font
		0,								// angle of escapement
		angle,							// orientation angle
		FW_BOLD,						// font weight
		italic,							// italic
		underline,						// underline
		strikeout,						// strikeout
		ANSI_CHARSET,					// character set identifier
		OUT_TT_PRECIS,					// output precision
		CLIP_DEFAULT_PRECIS,			// clipping precision
		ANTIALIASED_QUALITY,			// output quality
		FF_DONTCARE | DEFAULT_PITCH,	// family and pitch
		face);							// font name
	
	SelectObject(deviceContext, font);			//Selects The Font We Want
	wglUseFontBitmaps(deviceContext, 32, 96, fontBase); //Builds 96 characters starting at character 32
}
