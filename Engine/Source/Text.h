#ifndef TEXT_H
#define TEXT_H

#include "Includes.h"
#include "Colour.h"
#include "Font.h"

class Text {
public:
	Text(int xPos, int yPos, Colour fontColour, Font * font, bool externalFont, const char * text, ...);
	~Text();

	void setColour(Colour newColour);
	void setPosition(int x, int y);
	char * getText();
	void setText(const char * text, ...);

	int getSize() const;

	void draw();
	void draw(const char * text);

private:
	char * text;

	int x, y;

	bool externalFont;

	Font * font;
	Colour colour;
};

#endif // TEXT_H
