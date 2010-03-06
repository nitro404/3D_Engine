#include "World.h"

char textureDirectory [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};

Point World::playerPosition = Point(0, 0, 0);

void World::tick () {
	unsigned int i;
	playerPosition = player->playerMatrix.position();
	
	underWater = checkUnderWater();
	
	if(skybox != NULL) {
		skybox->tick();
	}
	for(i=0;i<objects.size();i++) {
		objects.at(i)->tick();
	}
	for(i=0;i<water.size();i++) {
		water.at(i)->tick();
	}
	for(i=0;i<sprites.size();i++) {
		sprites.at(i)->tick();
	}
}

bool World::compareDistance(const Object & x, const Object & y) {
	return x.distanceFrom(playerPosition) < y.distanceFrom(playerPosition);
}

void World::sortObjects() {
	Object * temp;
	for(unsigned int i=0;i<objects.size();i++) {
		for(unsigned int j=i;j<objects.size();j++) {
			if(compareDistance(*sortedObjects[i], *sortedObjects[j])) {
				temp = sortedObjects[i];
				sortedObjects[i] = sortedObjects[j];
				sortedObjects[j] = temp;
			}
		}
	}
}

void World::sortWater() {
	Pool * temp;
	for(unsigned int i=0;i<water.size();i++) {
		for(unsigned int j=i;j<water.size();j++) {
			if(compareDistance(*sortedWater[i], *sortedWater[j])) {
				temp = sortedWater[i];
				sortedWater[i] = sortedWater[j];
				sortedWater[j] = temp;
			}
		}
	}
}

void World::sortSprites() {
	Sprite * temp;
	for(unsigned int i=0;i<sprites.size();i++) {
		for(unsigned int j=i;j<sprites.size();j++) {
			if(compareDistance(*sortedSprites[i], *sortedSprites[j])) {
				temp = sortedSprites[i];
				sortedSprites[i] = sortedSprites[j];
				sortedSprites[j] = temp;
			}
		}
	}
}

bool World::checkUnderWater() const {
	for(unsigned int i=0;i<water.size();i++) {
		if(water.at(i)->insideOf(playerPosition)) {
			return true;
		}
	}
	return false;
}

void World::draw() {
	unsigned int i, j;
	sortObjects();
	sortWater();
	sortSprites();
	
	if(skybox != NULL) {
		skybox->draw();
	}
	for(i=0;i<objects.size();i++) {
		sortedObjects[i]->draw();
	}
	for(i=0;i<water.size();i++) {
		if(!underWater) {
			for(j=0;j<sprites.size();j++) {
				if(sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		else {
			for(j=0;j<sprites.size();j++) {
				if(!sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		sortedWater[i]->draw();
		if(underWater) {
			for(j=0;j<sprites.size();j++) {
				if(sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		else {
			for(j=0;j<sprites.size();j++) {
				if(!sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
	}
}

void World::import(char * fileName, vector<Texture *> & textures, vector<char *> & heightMaps, vector<AnimatedTexture *> & animatedTextures) {
	char line[256];
	char key[256];
	char value[256];
	unsigned int i, j;
	
	ifstream input;
	input.open(fileName); 
	if(input.bad()) {
		quit("Unable to open world file: \"%s\".", fileName);
	}
	
	//Input the header.
	input.getline(line, 256, '\n');

	//Input the start position
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	double xStartPos = atof(line);
	input.getline(line, 256, ',');
	double yStartPos = atof(line);
	input.getline(line, 256, ';');
	double zStartPos = atof(line);
	startPosition = Point(xStartPos, yStartPos, zStartPos);
	input.getline(line, 256, '\n');
	
	//Input the waypoints
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfWaypoints = atoi(line);
	for(int waypointIndex=0;waypointIndex<numberOfWaypoints;waypointIndex++) {
		//Create the corresponding objects
		Waypoint * waypoint = new Waypoint;
		waypoint->import(input);
		waypoints.push_back(waypoint);
	}
	
	// collect waypoints and add neighbouring waypoints to each waypoint
	char * separator;
	char * head;
	char * start;
	char * end;
	bool valid;
	bool endOfString;
	for(i=0;i<waypoints.size();i++) {
		// parse through the string of neighbour waypoint values
		head = waypoints.at(i)->neighbourValues;
		bool hasText = false;
		for(j=0;j<strlen(head);j++) {
			if(head[j] != ' ' && head[j] != '\t') {
				hasText = true;
				break;
			}
		}
		if(hasText && strlen(head) > 0) {
			do {
				// trim whitespace
				valid = true;
				endOfString = false;
				separator = strchr(head, ',');
				if(separator == NULL) {
					separator = head + ((strlen(head) - 1) * sizeof(char) + sizeof(char));
					endOfString = true;
				}
				else {
					*separator = '\0';
				}
				start = head;
				end = separator - sizeof(char);
				// trim the front of the string
				while(*start == ' ' || *start == '\t') {
					*start = '\0';
					start += sizeof(char);
				}
				// trim the end of the string
				while(*end == ' ' || *end == '\t') {
					*end = '\0';
					end -= sizeof(char);
				}
				if(start > end) {
					valid = false;
				}
				head = start;
				
				// set each waypoint as a neighbour of the other
				if(valid) {
					waypoints.at(i)->addNeighbour(waypoints.at(atoi(head)));
				}
				
				// iterate to the next element in the string
				head = separator + sizeof(char);
			} while(!endOfString);
		}
	}
	
	//Input the objects.
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfObjects = atoi(line);
	input.getline(line, 256, '\n');
	for(int objectIndex=0;objectIndex<numberOfObjects;objectIndex++) {
		//Input the header.
		input.getline(line, 256, ':');
		input.getline(line, 256, ';');
		int currentIndex = atoi(line);
		input.getline(line, 256, '\n');
		
		//Input the object type
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		
		//Create the corresponding objects
		if(_stricmp(value, "static geometry") == 0) {
			Geometry * geometry = new Geometry;
			geometry->import(input, textures);
			objects.push_back(geometry);
		}
		else if(_stricmp(value, "environment") == 0) {
			skybox = new Environment;
			skybox->import(input, textures);
		}
		else if(_stricmp(value, "vehicle") == 0) {
			Vehicle * vehicle = new Vehicle;
			vehicle->import(input, textures);
			objects.push_back (vehicle);
		}
		else if(_stricmp(value, "rotator") == 0) {
			Rotator * rotator = new Rotator;
			rotator->import(input, textures);
			objects.push_back(rotator);
		}
		else if(_stricmp(value, "translator") == 0) {
			Translator * translator = new Translator;
			translator->import(input, textures);
			objects.push_back(translator);
		}
		else if(_stricmp(value, "sprite") == 0) {
			Sprite * sprite = new Sprite;
			sprite->import(input, textures, waypoints);
			sprites.push_back(sprite);
		}
		else if(_stricmp(value, "pool") == 0) {
			Pool * pool = new Pool;
			pool->import(input, animatedTextures);
			water.push_back(pool);
		}
		else if (_stricmp(value, "terrain") == 0) {
			Terrain * terrain = new Terrain;
			terrain->import(input, textures, heightMaps);
			objects.push_back(terrain);
		}
		else {
			printf("WARNING: Encountered unexpected object when parsing world: \"%s\"", value);
		}
	}
	sortedObjects = new Object*[objects.size()];
	for(i=0;i<objects.size();i++) {
		sortedObjects[i] = objects.at(i);
	}
	sortedWater = new Pool*[water.size()];
	for(i=0;i<water.size();i++) {
		sortedWater[i] = water.at(i);
	}
	sortedSprites = new Sprite*[sprites.size()];
	for(i=0;i<sprites.size();i++) {
		sortedSprites[i] = sprites.at(i);
	}
	
	input.close();
}
