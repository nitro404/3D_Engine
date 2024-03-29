#include "AnimatedFace.h"

void AnimatedFace::update(double timeElapsed) {
	// choose the current texture based on the animation sequence
	texture = animatedTexture->textures.at((int) textureIndex);
	textureIndex += animationSpeed * timeElapsed;
	if(textureIndex >= animatedTexture->frames) {
		textureIndex = 0;
	}
}

void AnimatedFace::draw() {
	texture->activate();

	if(texture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	glBegin(GL_POLYGON);
		for(unsigned int pointIndex=0;pointIndex<points.size();pointIndex++) {
			GamePoint & point = *points.at(pointIndex);
			glTexCoord2d(point.tx, point.ty);
			glVertex3d(point.x, point.y, point.z);
		}
	glEnd();
}

void AnimatedFace::draw(Colour & colour) {
	draw(colour, false);
}

void AnimatedFace::draw(Colour & colour, bool drawBothSides) {
	texture->activate();

	// draw the inside of the pool if you are inside of it, so you can see it
	if(drawBothSides) { glDisable(GL_CULL_FACE); }
	glColor4d(colour.red, colour.green, colour.blue, colour.alpha);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void AnimatedFace::import(ifstream & input, vector<AnimatedTexture * > & animatedTextures) {
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
	
	// get the animated texture
	animatedTexture = animatedTextures.at(atoi(value));
	if(animationSpeed < 0) {
		animationSpeed = animatedTexture->speed;
	}

	// input the points
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfPoints = atoi(line);
	input.getline(line, 256, '\n');
	for(int pointIndex=0;pointIndex<numberOfPoints;pointIndex++) {
		GamePoint * point = new GamePoint;
		input.getline(line, 256, ','); point->x = atof(line);
		input.getline(line, 256, ','); point->y = atof(line);
		input.getline(line, 256, ','); point->z = atof(line);
		input.getline(line, 256, ','); point->nx = atof(line);
		input.getline(line, 256, ','); point->nx = atof(line);
		input.getline(line, 256, ','); point->nz = atof(line);
		input.getline(line, 256, ','); point->tx = atof(line);
		input.getline(line, 256, '\n'); point->ty = atof(line);
		points.push_back(point);
	}
}
