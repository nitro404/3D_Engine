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
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');
	
	input.getline(line, 256, '\n');
	value[0] = '\0';
	sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
	texture = textures.at(atoi(value));
	
	//Input the points.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfPoints = atoi(line);
	input.getline(line, 256, '\n');
	for(int pointIndex=0;pointIndex<numberOfPoints;pointIndex++) {
		GamePoint * point = new GamePoint;
		input.getline(line, 256, ','); point->x = atof (line);
		input.getline(line, 256, ','); point->y = atof (line);
		input.getline(line, 256, ','); point->z = atof (line);
		input.getline(line, 256, ',');
		input.getline(line, 256, ',');
		input.getline(line, 256, ',');
		input.getline(line, 256, ','); point->tx = atof (line);
		input.getline(line, 256, '\n'); point->ty = atof (line);
		points.push_back (point);
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}
