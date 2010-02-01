
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

char textureDirectory [_MAX_DIR] = {'T','E','X','T','U','R','E','S','\0'};
//char textureDirectory [_MAX_DIR] = {'.','.','\\','.','.','\\','T','E','X','T','U','R','E','S','\0'};

bool printObjects = true;

//*****************************************************************************************//
//                                       World                                             //
//*****************************************************************************************//

void World::setup () {
	//Nothing to do so far.
}

void World::wrapup () {
}

void World::tick () {
	//Tick all the objects in the world such as doors, sun, weather...
	for(int i=0;i<objects.size();i++) {
		objects.at(i)->tick();
	}
}

void World::draw () {
	//Draw all the objects in the world such as terrain, buildings, etc.
	
	for(int i=0;i<objects.size();i++) {
		objects.at(i)->draw();
	}
}

void World::log () {
	//For debugging, it might be useful to output the entire world into the log file.

	//The simple stuff first.
	::log ("\n\nWorld...\nStart position [%3.2f,%3.2f,%3.2f]", startPosition.x, startPosition.y, startPosition.z);

	//The objects: A version WITH loop macros.
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
//		object.log ();
	endloop
	
	//The objects: A version WITHOUT loop macros.
	//for (long objectIndex = 0; objectIndex < objects.size (); objectIndex++) {
	//	Object &object = *(objects [objectIndex]);
	//	object.log ();
	//}
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
//TODO: Create environment object??????
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
			sprite->import(input, textures);
			objects.push_back(sprite);
		}
		else if (stricmp (type, "waypoint") == 0) {
//create waypoint structures
			Waypoint * waypoint = new Waypoint;
			waypoint->import(input, textures);
			waypoints.push_back(waypoint);
		}
		else if (stricmp (type, "pool") == 0) {
			Pool * pool = new Pool;
			pool->import(input, textures);
			objects.push_back(pool);
		}
		
		delete [] type;
	}
	
	if(printObjects) {
		for(int i=0;i<objects.size();i++) {
			cout << *(objects.at(i)) << endl;
		}
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