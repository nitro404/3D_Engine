#include "includes.all"

char textureDirectory [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};

Point World::playerPosition = Point(0, 0, 0);

void World::tick () {
	int i;
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
	for(int i=0;i<objects.size();i++) {
		for(int j=i;j<objects.size();j++) {
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
	for(int i=0;i<water.size();i++) {
		for(int j=i;j<water.size();j++) {
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
	for(int i=0;i<sprites.size();i++) {
		for(int j=i;j<sprites.size();j++) {
			if(compareDistance(*sortedSprites[i], *sortedSprites[j])) {
				temp = sortedSprites[i];
				sortedSprites[i] = sortedSprites[j];
				sortedSprites[j] = temp;
			}
		}
	}
}

bool World::checkUnderWater() const {
	for(int i=0;i<water.size();i++) {
		if(water.at(i)->insideOf(playerPosition)) {
			return true;
		}
	}
	return false;
}

void World::draw() {
	int i;
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
			for(int j=0;j<sprites.size();j++) {
				if(sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		else {
			for(int j=0;j<sprites.size();j++) {
				if(!sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		sortedWater[i]->draw();
		if(underWater) {
			for(int j=0;j<sprites.size();j++) {
				if(sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
		else {
			for(int j=0;j<sprites.size();j++) {
				if(!sortedWater[i]->insideOf(sortedSprites[j]->position)) {
					sortedSprites[j]->draw();
				}
			}
		}
	}
}

void World::import(char * fileName) {
	char line [256];
	
	ifstream input;
	input.open(fileName); 
	if(input.bad()) {
		quit("Unable to open world file \"%s\".", fileName);
	}
	
	//Input the header.
	CLEAR_THE_LINE;

	//Input the start position
	SKIP_TO_COLON;
	SKIP_TO_COMMA;
	double xStartPos = atof(line);
	SKIP_TO_COMMA;
	double yStartPos = atof(line);
	SKIP_TO_SEMICOLON;
	double zStartPos = atof(line);
	startPosition = Point(xStartPos, yStartPos, zStartPos);
	CLEAR_THE_LINE;

	//Input the texture names and load the corresponding texture
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long texturesSize = atoi(line);
	CLEAR_THE_LINE;
	char * textureName;
	int startIndex;
	int i, j;
	string texturePath;
	string texturePathBMP;
	string texturePathTGA;
	Texture * newTexture;
	for(long textureIndex = 0; textureIndex < texturesSize; textureIndex++) {
		SKIP_TO_ENDLINE;
		startIndex = 0;
		for(i=startIndex;i<strlen(line);i++) {
			if(line[i] != ' ' && line[i] != '\t') {
				startIndex = i;
				break;
			}
		}
		textureName = new char[strlen(line) - startIndex + 1];
		j = 0;
		for(i=startIndex;i<strlen(line);i++) {
			textureName[j++] = line[i];
		}
		textureName[strlen(line) - startIndex] = '\0';

		//Load the texture
		texturePath.append(textureDirectory);
		texturePath.append("/");
		texturePath.append(textureName);
		texturePathTGA.append(texturePath);
		texturePathTGA.append(".tga");
		newTexture = Texture::readTexture((char *) texturePathTGA.c_str());
		if(newTexture == NULL) {
			texturePathBMP.append(texturePath);
			texturePathBMP.append(".bmp");
			newTexture = Texture::readTexture((char *) texturePathBMP.c_str());
		}
		if(newTexture != NULL) {
			newTexture->load();
			textures.push_back(newTexture);
		}
		else {
			prompt("Missing texture or unknown format: %s", textureName);
		}
		texturePath.erase();
		texturePathBMP.erase();
		texturePathTGA.erase();
		delete [] textureName;
	}
	
	//Input the animated textures 
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long animatedTexturesSize = atoi(line);
	CLEAR_THE_LINE;
	for(int atIndex=0;atIndex<animatedTexturesSize;atIndex++) {
		//Create the corresponding animated textures
		AnimatedTexture * animatedTexture = new AnimatedTexture;
		animatedTexture->import(input, textures);
		animatedTextures.push_back(animatedTexture);
	}
	
	//Input the waypoints
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long waypointsSize = atoi (line);
	for (long waypointIndex = 0; waypointIndex < waypointsSize; waypointIndex++) {
		//Create the corresponding objects
		Waypoint * waypoint = new Waypoint;
		waypoint->import(input);
		waypoints.push_back(waypoint);
	}

	// collect waypoints and add neighbouring waypoints to each waypoint object, then collect the waypoints in the main object collection
	char * separator;
	char * head;
	char * start;
	char * end;
	bool valid;
	bool endOfString;
	for(i=0;i<waypoints.size();i++) {
		// parse through the string of neighbour waypoint values
		head = waypoints.at(i)->neighbourValues;
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
				for(j=0;j<waypoints.size();j++) {
					if(stricmp(head, waypoints.at(j)->name) == 0 ){
						waypoints.at(i)->addNeighbour(waypoints.at(j));
						waypoints.at(j)->addNeighbour(waypoints.at(i));
					}
				}

			}
			
			// iterate to the next element in the string
			head = separator + sizeof(char);
		} while(!endOfString);
	}
	
	//Input the objects.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long objectsSize = atoi (line);
	for (long objectIndex = 0; objectIndex < objectsSize; objectIndex++) {
		//Input the header.
		SKIP_TO_COLON;
		SKIP_TO_SEMICOLON;
		int currentIndex = atoi (line);
		CLEAR_THE_LINE;
		
		//Input the object type
		SKIP_TO_ENDLINE;
		char obj_key [256];
		char obj_value [256];
		obj_value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", obj_key, obj_value);
		char * type = new char [strlen (obj_value) + 1];
		strcpy(type, obj_value);
		
		//Create the corresponding objects
		if (stricmp (type, "static geometry") == 0) {
			Geometry * geometry = new Geometry;
			geometry->import(input, textures);
			objects.push_back(geometry);
		}
		else if (stricmp (type, "environment") == 0) {
			skybox = new Environment;
			skybox->import(input, textures);
		}
		else if (stricmp (type, "vehicle") == 0) {
			Vehicle * vehicle = new Vehicle;
			vehicle->import(input, textures);
			objects.push_back (vehicle);
		}
		else if (stricmp (type, "rotator") == 0) {
			Rotator * rotator = new Rotator;
			rotator->import(input, textures);
			objects.push_back(rotator);
		}
		else if (stricmp (type, "translator") == 0) {
			Translator * translator = new Translator;
			translator->import(input, textures);
			objects.push_back(translator);
		}
		else if (stricmp (type, "sprite") == 0) {
			Sprite * sprite = new Sprite;
			sprite->import(input, textures, waypoints);
			sprites.push_back(sprite);
		}
		else if (stricmp (type, "pool") == 0) {
			Pool * pool = new Pool;
			pool->import(input, animatedTextures);
			water.push_back(pool);
		}
		
		delete [] type;
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
