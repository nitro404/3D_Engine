#ifndef _COLOUR_H
#define _COLOUR_H

#include <iostream>

using namespace std;

class Colour {
public:
	double red, green, blue, alpha;
	
	Colour(int red=0, int green=0, int blue=0, int alpha=0);
	Colour(const Colour & x);
	Colour & operator = (const Colour & x);
	~Colour();
	
	void setRed(int red);
	void setGreen(int green);
	void setBlue(int blue);
	void setAlpha(int alpha);

	bool operator == (const Colour & x) const;
	bool operator != (const Colour & x) const;
	
	void printOn(ostream & o) const;
};

ostream & operator << (ostream & o, const Colour & x);

#endif
