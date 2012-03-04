#include "Sprite.h"

Sprite::Sprite() : picture(NULL),
				   shader(NULL),
				   origin(NULL),
				   destination(NULL),
				   lastWaypoint(NULL),
				   movementSpeed(0),
				   distanceToTravel(0),
				   distanceTravelled(0) { }

Sprite::~Sprite() {
	delete [] name;
}

double Sprite::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - position.x, 2) + pow(p.y - position.y, 2) + pow(p.z - position.z, 2) );
}

void Sprite::switchDestinations() {
	position = destination->getPosition();
	lastWaypoint = destination;
	destination = lastWaypoint->getRandomNeighbour();
	distanceToTravel = sqrt(pow(destination->getPosition().x - lastWaypoint->getPosition().x, 2) + pow(destination->getPosition().y - lastWaypoint->getPosition().y, 2) + pow(destination->getPosition().z - lastWaypoint->getPosition().z, 2));
	if(distanceToTravel <= 0) {
		unitVector = Point(0, 0, 0);
	}
	else {
		unitVector = Point((destination->getPosition().x - lastWaypoint->getPosition().x) / distanceToTravel, (destination->getPosition().y - lastWaypoint->getPosition().y) / distanceToTravel, (destination->getPosition().z - lastWaypoint->getPosition().z) / distanceToTravel);
	}
	distanceTravelled = 0;
}

void Sprite::update(double timeElapsed) {
	if(origin != NULL && origin->hasNeighbours()) {
		position += unitVector * (movementSpeed * timeElapsed);
		distanceTravelled = sqrt(pow(position.x - lastWaypoint->getPosition().x, 2) + pow(position.y - lastWaypoint->getPosition().y, 2) + pow(position.z - lastWaypoint->getPosition().z, 2));
		if(distanceTravelled >= distanceToTravel) {
			switchDestinations();
		}
	}
}

void Sprite::draw() {
	if(shader != NULL) { shader->activate(); }

	Transformation cameraMatrix;
	glGetMatrixd(cameraMatrix);
	Point newPosition((position.x * cameraMatrix.m11) + (position.y * cameraMatrix.m21) + (position.z * cameraMatrix.m31) + cameraMatrix.m41,
					  (position.x * cameraMatrix.m12) + (position.y * cameraMatrix.m22) + (position.z * cameraMatrix.m32) + cameraMatrix.m42,
					  (position.x * cameraMatrix.m13) + (position.y * cameraMatrix.m23) + (position.z * cameraMatrix.m33) + cameraMatrix.m43); 
	
	if(picture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	picture->activate();
	
	glPushIdentity();
		if(origin == NULL) { glTranslated(newPosition.x, newPosition.y, newPosition.z); }
		else { glTranslated(newPosition.x, newPosition.y + (box->getExtent().y/2), newPosition.z); }

		glScaled(box->getExtent().x, box->getExtent().y, box->getExtent().z);
		glCallList(spriteList);
	glPopMatrix();

	if(shader != NULL) { shader->deactivate(); }
}

void Sprite::import(ifstream & input, vector<Texture *> & textures, vector<Waypoint *> & waypoints, vector<Shader *> shaders) {
	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	// input the position
	double x, y, z;
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	x = atof(line);
	input.getline(line, 256, ',');
	y = atof(line);
	input.getline(line, 256, ';');
	z = atof(line);
	position = Point(x, y, z);
	input.getline(line, 256, '\n');
	
	// input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);

		// parse properties to local variables
		if(_stricmp(key, "name") == 0) {
			name = str;
		}
		else if(_stricmp(key, "picture") == 0) {
			picture = textures.at(atoi(str));
			delete [] str;
		}
		else if(_stricmp(key, "waypoint") == 0) {
			int index = atoi(str);
			if(index >= 0) {
				origin = waypoints.at(index);
				position = origin->getPosition();
			}
			delete [] str;
		}
		else if(_stricmp(key, "movementSpeed") == 0) {
			movementSpeed = atoi(str);
			delete [] str;
		}
		else if(_stricmp(key, "shader") == 0) {
			int shaderIndex = atoi(str);
			if(shaderIndex >= 0 && shaderIndex < (int) shaders.size()) { shader = shaders.at(shaderIndex); }
			delete [] str;
		}
		else {
			printf("WARNING: Encountered unexpected property when parsing sprite object: \"%s\".\n", key);
			delete [] str;
		}
	}

	// input the bounding box
	box = BoundingBox::import(input);
	
	if(origin != NULL) {
		lastWaypoint = origin;
		position = origin->getPosition();
		if(origin->hasNeighbours()) {
			destination = lastWaypoint->getRandomNeighbour();
			distanceToTravel = sqrt(pow(destination->getPosition().x - lastWaypoint->getPosition().x, 2) + pow(destination->getPosition().y - lastWaypoint->getPosition().y, 2) + pow(destination->getPosition().z - lastWaypoint->getPosition().z, 2));
			if(distanceToTravel <= 0) {
				unitVector = Point(0, 0, 0);
			}
			else {
				unitVector = Point((destination->getPosition().x - lastWaypoint->getPosition().x) / distanceToTravel, (destination->getPosition().y - lastWaypoint->getPosition().y) / distanceToTravel, (destination->getPosition().z - lastWaypoint->getPosition().z) / distanceToTravel);
			}
			distanceTravelled = 0;
		}
	}

	spriteList = glGenLists(1);
	glNewList(spriteList, GL_COMPILE);
		glBegin(GL_POLYGON);
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 1);
			glVertex3d(-0.5, 0.5, 0);
			
			glNormal3d(0, 0, 1);
			glTexCoord2d(0, 0);
			glVertex3d(-0.5, -0.5, 0);
			
			glNormal3d(0, 0, 1);
			glTexCoord2d(1, 0);
			glVertex3d(0.5, -0.5, 0);
			
			glNormal3d(0, 0, 1);
			glTexCoord2d(1, 1);
			glVertex3d(0.5, 0.5, 0);
		glEnd();
	glEndList();
}
