#pragma once

#include "Includes.h"

class Font {
public:
	Font(int windowWidth, int windowHeight, const char * fontFace, int fontSize, int fontWeight, bool italic, bool underline, bool strikeout, float angle);
	Font(const Font & x);
	Font & operator = (const Font & x);	
	~Font();

	int getSize() const;
	GLuint getBase() const;

private:
	void createFont();

public:
	const static int NORMAL;
	const static int REGULAR;
	const static int BOLD;

	HDC deviceContext;
	GLuint fontBase;

private:
	char * face;
	int size;
	int weight;
	bool italic;
	bool underline;
	bool strikeout;
	float angle;

	int windowWidth;
	int windowHeight;
};
