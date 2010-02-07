
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

char textureDirectory [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};
//char textureDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','T','E','X','T','U','R','E','S','\0'};

//*****************************************************************************************//
//                                       World                                             //
//*****************************************************************************************//

void World::setup () {
	
}

void World::wrapup () {

}

void World::tick () {
	for(int i=0;i<objects.size();i++) {
		objects.at(i)->tick();
	}
	for(i=0;i<sprites.size();i++) {
		sprites.at(i)->tick();
	}
}

void World::draw () {
	for(int i=0;i<objects.size();i++) {
		objects.at(i)->draw();
	}
	for(i=0;i<sprites.size();i++) {
		sprites.at(i)->draw();
	}
}

void World::read () {
	char *filename = promptForWorldRead ();
	import(filename);
}

void World::import (char * fileName) {
	ifstream input;
	input.open (fileName); 
	if (input.bad ()) {
		prompt ("Unable to open input file \"%s\".", fileName);
		exit(1);
	}
	import (input);
	input.close ();
}

void World::import (ifstream &input) {
	char line [256]; //Working variable...
	
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
	
	//Input the waypoints
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON;
	long waypointsSize = atoi (line);
	for (long waypointIndex = 0; waypointIndex < waypointsSize; waypointIndex++) {
		//Input the header.
		SKIP_TO_COLON;
		SKIP_TO_SEMICOLON;
		long waypointIndex = atoi (line); //Only useful for debugging or browsing.
		CLEAR_THE_LINE;
		
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
		long objectIndex = atoi (line); //Only useful for debugging or browsing.
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
			objects.push_back(sprite);
		}
		else if (stricmp (type, "pool") == 0) {
			Pool * pool = new Pool;
			pool->import(input, textures);
			objects.push_back(pool);
		}
		
		delete [] type;
	}
}

void World::unload () {
	//Unload all of the textures prior to deleting the world... If you don't, you will
	//reach a point where you will run in slow motion as the card thrashes around trying
	//to allow you to run in the memory it has left... Rebooting will fix it...
	for(int i=0;i<textures.size();i++) {
		delete textures.at(i);
	}
}