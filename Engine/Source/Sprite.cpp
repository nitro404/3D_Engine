#include "Sprite.h"

double Sprite::distanceFrom(Point & p) const {
	return sqrt( pow(p.x - position.x, 2) + pow(p.y - position.y, 2) + pow(p.z - position.z, 2) );
}

void Sprite::switchDestinations() {
	position = destination->getPosition();
	lastWaypoint = destination;
	destination = lastWaypoint->getRandomNeighbour();
	distanceToTravel = sqrt(pow(destination->getPosition().x - lastWaypoint->getPosition().x, 2) + pow(destination->getPosition().y - lastWaypoint->getPosition().y, 2) + pow(destination->getPosition().z - lastWaypoint->getPosition().z, 2));
	unitVector = Vector((destination->getPosition().x - lastWaypoint->getPosition().x) / distanceToTravel, (destination->getPosition().y - lastWaypoint->getPosition().y) / distanceToTravel, (destination->getPosition().z - lastWaypoint->getPosition().z) / distanceToTravel);
	distanceTravelled = 0;
}

void Sprite::tick() {
	if(origin != NULL && origin->hasNeighbours()) {
		position += unitVector * (movementSpeed * DT);
		distanceTravelled = sqrt(pow(position.x - lastWaypoint->getPosition().x, 2) + pow(position.y - lastWaypoint->getPosition().y, 2) + pow(position.z - lastWaypoint->getPosition().z, 2));
		if(distanceTravelled >= distanceToTravel) {
			switchDestinations();
		}
	}
}

void Sprite::draw() {
	Transformation cameraMatrix;
	glGetMatrixd(cameraMatrix);
	Point newPosition = position * cameraMatrix;
	
	if(picture->type == RGBAType) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
	
	picture->activate();
	
	glPushIdentity();
		
		if(origin == NULL) {
			glTranslated(newPosition.x, newPosition.y, newPosition.z);
		}
		else {
			glTranslated(newPosition.x, newPosition.y + (extent.y/2), newPosition.z);
		}
		glScaled(extent.x, extent.y, extent.z);
		
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
		
	glPopMatrix();
}

void Sprite::import(ifstream & input, vector<Texture *> & textures, vector<Waypoint *> & waypoints) {
	char * line;
	char * key;
	char * value;
	char * str;
	line = new char[256];
	key = new char[256];
	value = new char[256];
	
	//Input the position
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
	
	//Input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf(line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);

		//Parse properties to local variables
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
	}
	
	//Input the bounding box
	Point max, min;
	
	// Input the maximum
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	x = atof(line);
	input.getline(line, 256, ',');
	y = atof(line);
	input.getline(line, 256, ';');
	z = atof(line);
	max = Point(x, y, z);
	input.getline(line, 256, '\n');
	
	// Input the minimum
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	x = atof(line);
	input.getline(line, 256, ',');
	y = atof(line);
	input.getline(line, 256, ';');
	z = atof(line);
	min = Point(x, y, z);
	input.getline(line, 256, '\n');
	
	// Calculate the center of the sprite
	center.x = (max.x + min.x) / 2;
	center.y = (max.y + min.y) / 2;
	center.z = (max.z + min.z) / 2;
	
	// Calculate the bottom center of the sprite
	bottomCenter.x = (max.x + min.x) / 2;
	bottomCenter.y = (max.y + min.y) / 2;
	bottomCenter.z = min.z;
	
	// Calculate the size of the sprite
	extent.x = max.x - min.x;
	extent.y = max.y - min.y;
	extent.z = max.z - min.z;
	
	if(origin != NULL) {
		lastWaypoint = origin;
		position = origin->getPosition();
		if(origin->hasNeighbours()) {
			destination = lastWaypoint->getRandomNeighbour();
			distanceToTravel = sqrt(pow(destination->getPosition().x - lastWaypoint->getPosition().x, 2) + pow(destination->getPosition().y - lastWaypoint->getPosition().y, 2) + pow(destination->getPosition().z - lastWaypoint->getPosition().z, 2));
			unitVector = Vector((destination->getPosition().x - lastWaypoint->getPosition().x) / distanceToTravel, (destination->getPosition().y - lastWaypoint->getPosition().y) / distanceToTravel, (destination->getPosition().z - lastWaypoint->getPosition().z) / distanceToTravel);
			distanceTravelled = 0;
		}
	}
	
	delete [] line;
	delete [] key;
	delete [] value;
}

