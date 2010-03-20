#include "Text.h"

Text::Text(int windowWidth,
		   int windowHeight,
		   int xPos,
		   int yPos,
		   Colour fontColour,
		   Font * font,
		   bool isExternalFont,
		   const char * text, ...)
			: x(xPos),
			  y(yPos),
			  font((isExternalFont) ? font : NULL),
			  colour(fontColour),
			  externalFont(isExternalFont) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	if(text == NULL) {
		this->text = new char[1];
		this->text[0] = '\0';
	}
	else {
		char buffer[256];
		va_list parameters;
		va_start(parameters, text);
		vsprintf_s(buffer, 256, text, parameters);
		va_end(parameters);

		this->text = new char[strlen(buffer) + 1];
		strcpy_s(this->text, strlen(buffer) + 1, buffer);
	}

	if(font == NULL) {
		this->font = new Font(windowWidth, windowHeight, "Arial", 24, Font::REGULAR, false, false, false, 0);
		externalFont = false;
	}
	if(this->font == NULL) {
		this->font = new Font(*font);
		externalFont = false;
	}
}

Text::~Text() {
	delete [] text;
	if(!externalFont) {
		delete font;
	}
}

int Text::getSize() const { return font->getSize(); }

void Text::setColour(Colour newColour) { colour = newColour; }

void Text::setPosition(int x, int y) {
	this->x = x;
	this->y = y;
}

char * Text::getText() {
	return text;
}

void Text::setText(const char * text, ...) {
	delete [] text;

	char buffer[256];
	va_list parameters;
	va_start(parameters, text);
	vsprintf_s(buffer, 256, text, parameters);
	va_end(parameters);

	this->text = new char[strlen(buffer) + 1];
	strcpy_s(this->text, strlen(buffer) + 1, buffer);
}

void Text::draw() {
	draw(this->text);
}

void Text::draw(const char * text) {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity ();
		glOrtho(0.0,(GLfloat) windowWidth, 0.0, (GLfloat) windowHeight, -100.0f, 100.0f);
		glMatrixMode (GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity ();
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				glColor4f(colour.red, colour.green, colour.blue, colour.red);
				glRasterPos2i(x, y);
				glPushAttrib(GL_LIST_BIT);	// pushes the display list bits
//				glListBase(font->fontBase - 32); // sets the base character to 32
				glListBase(font->getBase()); // sets the base character to 32
				glCallLists(strlen(text), GL_UNSIGNED_BYTE, text); // draws the display list text
				glPopAttrib(); // pops the display list bits
				glColor4f(1, 1, 1, 1);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}
