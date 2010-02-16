#include "Face.h"

void Face::draw() {
	texture->activate();
	
	if(texture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	glBegin(GL_POLYGON);
	for(int pointIndex=0;pointIndex<points.size();pointIndex++) {
		GamePoint & point = *points.at(pointIndex);
		glTexCoord2d(point.tx, point.ty);
		glVertex3d(point.x, point.y, point.z);
	}
	glEnd();
}

void Face::draw(Colour & colour) {
	texture->activate();
	
	glDisable(GL_CULL_FACE);
	glBegin(GL_POLYGON);
	glColor4d(colour.red, colour.green, colour.blue, colour.alpha);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);
	for(int pointIndex=0;pointIndex<points.size();pointIndex++) {
		GamePoint & point = *points.at(pointIndex);
		glTexCoord2d(point.tx, point.ty);
		glVertex3d(point.x, point.y, point.z);
	}
	glColor4d(1, 1, 1, 1);
	glDisable(GL_BLEND);
	glEnd();
	glEnable(GL_CULL_FACE);
}

void Face::import(ifstream &input, vector<Texture *> & textures) {
	char * line;
	char * key;
	char * value;
	line = new char[256];
	key = new char[256];
	value = new char[256];

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	int currentIndex = atoi(line);
	CLEAR_THE_LINE;
	
	char * textureIndex;
	SKIP_TO_ENDLINE;
	value[0] = '\0';
	sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
	textureIndex = new char[strlen(value) + 1];
	strcpy(textureIndex, value);

	texture = textures.at(atoi(textureIndex));
	delete [] textureIndex;
	
	//Input the points.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	int numberOfPoints = atoi(line);
	CLEAR_THE_LINE;
	for(int pointIndex=0;pointIndex<numberOfPoints;pointIndex++) {
		GamePoint * point = new GamePoint;
		SKIP_TO_COMMA; point->x = atof (line);
		SKIP_TO_COMMA; point->y = atof (line);
		SKIP_TO_COMMA; point->z = atof (line);
		SKIP_TO_COMMA;
		SKIP_TO_COMMA;
		SKIP_TO_COMMA;
		SKIP_TO_COMMA; point->tx = atof (line);
		SKIP_TO_ENDLINE; point->ty = atof (line);
		points.push_back (point);
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}

