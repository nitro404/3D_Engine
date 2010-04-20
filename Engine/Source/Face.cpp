#include "Face.h"

void Face::draw() {
	texture->activate();
	
	// enable/disable blending
	if(texture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	// render the faces
	glBegin(GL_POLYGON);
		for(unsigned int pointIndex=0;pointIndex<points.size();pointIndex++) {
			GamePoint & point = *points.at(pointIndex);
			glTexCoord2d(point.tx, point.ty);
			glVertex3d(point.x, point.y, point.z);
		}
	glEnd();
}

void Face::draw(Colour & colour) {
	draw(colour, false);
}

void Face::draw(Colour & colour, bool drawBothSides) {

	texture->activate();

	// render the inside of the box if you are inside it
	if(drawBothSides) { glDisable(GL_CULL_FACE); }
	glColor4d(colour.red, colour.green, colour.blue, colour.alpha);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	// render the faces
	glBegin(GL_POLYGON);
		for(unsigned int pointIndex=0;pointIndex<points.size();pointIndex++) {
			GamePoint & point = *points.at(pointIndex);
			glTexCoord2d(point.tx, point.ty);
			glVertex3d(point.x, point.y, point.z);
		}
	glEnd();

	glColor4d(1, 1, 1, 1);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void Face::import(ifstream &input, vector<Texture *> & textures) {
char line[256];
	char key[256];
	char value[256];

	// input the header
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');
	
	input.getline(line, 256, '\n');
	value[0] = '\0';
	sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
	texture = textures.at(atoi(value));
	
	// input the points
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfPoints = atoi(line);
	input.getline(line, 256, '\n');
	for(int pointIndex=0;pointIndex<numberOfPoints;pointIndex++) {
		GamePoint * point = new GamePoint;
		input.getline(line, 256, ','); point->x = atof (line);
		input.getline(line, 256, ','); point->y = atof (line);
		input.getline(line, 256, ','); point->z = atof (line);
		input.getline(line, 256, ','); point->nx = atof (line);
		input.getline(line, 256, ','); point->ny = atof (line);
		input.getline(line, 256, ','); point->nz = atof (line);
		input.getline(line, 256, ','); point->tx = atof (line);
		input.getline(line, 256, '\n'); point->ty = atof (line);
		points.push_back (point);
	}
}

