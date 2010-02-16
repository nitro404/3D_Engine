#include "Colour.h"

Colour::Colour(int red, int green, int blue, int alpha) {
	setRed(red);
	setGreen(green);
	setBlue(blue);
	setAlpha(alpha);
}

Colour::Colour(const Colour & x) {
	this->red = x.red;
	this->green = x.green;
	this->blue = x.blue;
	this->alpha = x.alpha;
}

Colour & Colour::operator = (const Colour & x) {
	this->red = x.red;
	this->green = x.green;
	this->blue = x.blue;
	this->alpha = x.alpha;

	return *this;
}

Colour::~Colour() { }

void Colour::setRed(int red) {
	if(red >= 0 && red <= 255) {
		this->red = red / 255.0;
	}
}

void Colour::setGreen(int green) {
	if(green >= 0 && green <= 255) {
		this->green = green / 255.0;
	}
}

void Colour::setBlue(int blue) {
	if(blue >= 0 && blue <= 255) {
		this->blue = blue / 255.0;
	}
}

void Colour::setAlpha(int alpha) {
	if(alpha >= 0 && alpha <= 255) {
		this->alpha = alpha / 255.0;
	}
}

bool Colour::operator == (const Colour & x) const {
	return this->red == x.red &&
		   this->green == x.green &&
		   this->blue == x.blue &&
		   this->alpha == x.alpha;
}

bool Colour::operator != (const Colour & x) const {
	return !operator == (x);
}

void Colour::printOn(ostream & o) const {
	o << "(" << this->red << ", " << this->green << ", " << this->blue << ", " << this->alpha << ")";
}

ostream & operator << (ostream & o, const Colour & x) {
	x.printOn(o);
	return o;
}
