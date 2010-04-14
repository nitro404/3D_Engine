#include "World.h"

World::World(Variables * externalSettings) : skybox(NULL), settings(externalSettings) {
	startPosition = Point(0, 0, 0);
	testShader = new Shader("test.vtx", "test.frg", settings->getValue("Shader Directory"));
}

World::~World() {
	delete testShader;
	if(skybox != NULL) { delete skybox; }
	delete [] sortedObjects;
	delete [] sortedWater;
	delete [] sortedSprites;
	delete [] spriteWater;
	for(unsigned int i=0;i<objects.size();i++) {
		delete objects.at(i);
	}
	for(unsigned int i=0;i<water.size();i++) {
		delete water.at(i);
	}
	for(unsigned int i=0;i<sprites.size();i++) {
		delete sprites.at(i);
	}
	for(unsigned int i=0;i<waypoints.size();i++) {
		delete waypoints.at(i);
	}
}

void World::tick () {
	playerPosition = player->playerMatrix.position();
	
	if(skybox != NULL) {
		skybox->tick();
	}
	for(unsigned int i=0;i<objects.size();i++) {
		objects.at(i)->tick();
	}
	for(unsigned int i=0;i<water.size();i++) {
		water.at(i)->tick();
	}
	for(unsigned int i=0;i<sprites.size();i++) {
		sprites.at(i)->tick();
	}
}

void World::draw() {
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

	Sprite * temp2;
	for(unsigned int i=0;i<sprites.size();i++) {
		for(unsigned int j=i;j<sprites.size();j++) {
			if(compareDistance(*sortedSprites[i], *sortedSprites[j])) {
				temp2 = sortedSprites[i];
				sortedSprites[i] = sortedSprites[j];
				sortedSprites[j] = temp2;
			}
		}
	}

	underWater = -1;
	for(unsigned int i=0;i<water.size();i++) {
		if(sortedWater[i]->insideOf(playerPosition)) {
			underWater = i;
			break;
		}
	}

	for(unsigned int i=0;i<sprites.size();i++) {
		spriteWater[i] = -1;
		for(unsigned int j=0;j<water.size();j++) {
			if(sortedWater[j]->insideOf(sortedSprites[i]->position)) {
				spriteWater[i] = j;
				break;
			}
		}
	}

	for(unsigned int i=0;i<objects.size();i++) {
		sortedObjects[i]->draw();
	}
	for(unsigned int i=0;i<sprites.size();i++) {
		if(spriteWater[i] == -1) {
			sortedSprites[i]->draw();
		}
	}
	if(water.size() > 0) {
		for(unsigned int i=0;i<water.size();i++) {
			for(unsigned int j=0;j<sprites.size();j++) {
				if(!(spriteWater[j] == i && underWater == i)) {
					sortedSprites[j]->draw();
				}
			}
			sortedWater[i]->draw(underWater == i);
			for(unsigned int j=0;j<sprites.size();j++) {
				if(spriteWater[j] == i && underWater == i) {
					sortedSprites[j]->draw();
				}
			}
		}
	}
}

void World::drawSkybox() {
	if(skybox != NULL) {
		skybox->draw();
	}
}

bool World::compareDistance(const Object & x, const Object & y) {
	return x.distanceFrom(playerPosition) < y.distanceFrom(playerPosition);
}

void World::import(char * fileName, vector<Texture *> & textures, vector<char *> & heightMaps, vector<AnimatedTexture *> & animatedTextures) {
	char line[256];
	char key[256];
	char value[256];
	
	ifstream input;
	input.open(fileName); 
	if(input.bad()) {
		quit("Unable to open world file: \"%s\".", fileName);
	}
	
	// input the header.
	input.getline(line, 256, '\n');

	// input the start position
	input.getline(line, 256, ':');
	input.getline(line, 256, ',');
	double xStartPos = atof(line);
	input.getline(line, 256, ',');
	double yStartPos = atof(line);
	input.getline(line, 256, ';');
	double zStartPos = atof(line);
	startPosition = Point(xStartPos, yStartPos, zStartPos);
	input.getline(line, 256, '\n');
	
	// input the waypoints
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfWaypoints = atoi(line);
	for(int waypointIndex=0;waypointIndex<numberOfWaypoints;waypointIndex++) {
		// create the corresponding objects
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
	for(unsigned int i=0;i<waypoints.size();i++) {
		// parse through the string of neighbour waypoint values
		head = waypoints.at(i)->neighbourValues;
		bool hasText = false;
		for(unsigned int j=0;j<strlen(head);j++) {
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
	
	// input the objects
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfObjects = atoi(line);
	input.getline(line, 256, '\n');
	for(int objectIndex=0;objectIndex<numberOfObjects;objectIndex++) {
		// input the header
		input.getline(line, 256, ':');
		input.getline(line, 256, ';');
		int currentIndex = atoi(line);
		input.getline(line, 256, '\n');
		
		// input the object type
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		
		// create the corresponding objects
		if(_stricmp(value, "static geometry") == 0) {
			Geometry * geometry = new Geometry(testShader);
			geometry->import(input, textures);
			objects.push_back(geometry);
		}
		else if(_stricmp(value, "environment") == 0) {
			if(skybox != NULL) { delete skybox; }
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
		else if(_stricmp(value, "waterfall") == 0) {
			Waterfall * waterfall = new Waterfall;
			waterfall->import(input, animatedTextures);
			water.push_back(waterfall);
		}
		else {
			printf("WARNING: Encountered unexpected object when parsing world: \"%s\"", value);
		}
	}
	sortedObjects = new Object*[objects.size()];
	for(unsigned i=0;i<objects.size();i++) {
		sortedObjects[i] = objects.at(i);
	}
	sortedWater = new Pool*[water.size()];
	for(unsigned i=0;i<water.size();i++) {
		sortedWater[i] = water.at(i);
	}
	sortedSprites = new Sprite*[sprites.size()];
	for(unsigned i=0;i<sprites.size();i++) {
		sortedSprites[i] = sprites.at(i);
	}
	spriteWater = new int[sprites.size()];
	for(unsigned int i=0;i<sprites.size();i++) {
		spriteWater[i] = -1;
	}
	
	input.close();
}
