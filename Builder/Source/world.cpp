
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                       World                                             //
//*****************************************************************************************//

bool tracing = true;

void World::setup () {
	//Nothing to do so far.
}

void World::wrapup () {
}

World::World () {
	startPosition.zero (); //A default starting location...
};

World::~World () {
	deleteObjectCollectionEntries (objects);
};

char realInputFilename [MAX_PATH];

World *World::read () {
	//Prompts for a ".uni" file, creates a World object, reads the file into the world
	//and returns it (or NULL if the user cancelled the read or the file could not be read)...

	char *filename = promptForUniRead ();
	if (filename == NULL) return NULL; //User must have cancelled.
	return import (filename);
}

World *World::import (char *filename) {
	ifstream input; input.open (filename); 
	if (input.bad ()) {prompt ("Unable to open input file \"%s\".", filename); return NULL;}
	World *world = new World;
	world->import (input);
	input.close ();
	return world;
}

void World::import (ifstream &input) {
	char line [256]; //Working variable...
	
	//Input the header.
	CLEAR_THE_LINE;

	//Input the objects.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long objectsSize = atoi (line);
	for (long objectIndex = 0; objectIndex < objectsSize; objectIndex++) {
		Object *object = new Object;
		object->import (input, this);

		char *type = get (object->properties, "type");

		if (stricmp (type, "static geometry") == 0) {
			//Standard object...
			objects.push_back (object);
		}
		else if (stricmp (type, "info_player_start") == 0) {
			startPosition = object->transformation.position ();
			delete object;
		}
		else if (stricmp(type, "environment") == 0 ||
			     stricmp(type, "vehicle") == 0 ||
				 stricmp(type, "rotator") == 0 ||
				 stricmp(type, "translator") == 0 ||
				 stricmp(type, "sprite") == 0 ||
				 stricmp(type, "waypoint") == 0 ||
				 stricmp(type, "pool") == 0) {
			objects.push_back (object);
		}
		else {
			prompt ("\nIgnored unexpected object of type \"%s\"...", type);
			delete object;
		}
	}
}

World *World::write () {
	//Write the world in ".wrl" format (YOUR format). Return the world if successful;
	//otherwise NULL;
	char *outputFilename = promptForWorldWrite ();
	if (outputFilename == NULL) {return NULL;} //User must have cancelled.

	if (strstr (outputFilename, ".wrl") == NULL) {
		//prompt ("\nPlease ensure that the filename ends in \".wrl\"... Try again...");
		//return NULL;
		char modifiedOutputFilename [MAX_PATH];
		strcpy (modifiedOutputFilename, outputFilename);
		strcat (modifiedOutputFilename, ".wrl");
		outputFilename = modifiedOutputFilename;
	}

	this->export (outputFilename);
	return this;
}

World *World::write (char * outputFilename) {
	//Write the world in ".wrl" format (YOUR format). Return the world if successful;
	//otherwise NULL;
	if (outputFilename == NULL) {return NULL;} //User must have cancelled.

	if (strstr (outputFilename, ".wrl") == NULL) {
		//prompt ("\nPlease ensure that the filename ends in \".wrl\"... Try again...");
		//return NULL;
		char modifiedOutputFilename [MAX_PATH];
		strcpy (modifiedOutputFilename, outputFilename);
		strcat (modifiedOutputFilename, ".wrl");
		outputFilename = modifiedOutputFilename;
	}

	this->export (outputFilename);
	return this;
}

void World::export (char *filename) {
	//Write out the universalObject (asking different components to write themselves out).

	ofstream output (filename);
	if (output.bad()) {prompt ("Unable to create output file \"%s\".", filename); return;}
	output.precision (16);
	export (output);

	//Finish up...
	output.close ();
}

void World::export (ofstream &output) {
	//Output a simple header..."
	output << "World";
	
	//Output the start position
	output << "\nStart position: " <<
		startPosition.x << ", " << 
		startPosition.y << ", " << 
		startPosition.z << ";";
	
	//Gather the texture names and then output them...
	vector<char *> textureNames;
	boolean duplicateTextureName;
	loopVector (objectIndex, objects)
		loopVector (faceIndex, objects [objectIndex]->faces)
			Face &face = *(objects [objectIndex]->faces [faceIndex]);
			duplicateTextureName = false;
			for(int i=0;i<textureNames.size();i++) {
				if(strcmp(textureNames.at(i), face.textureName) == 0) {
					duplicateTextureName = true;
				}
			}
			if(!duplicateTextureName) {
				textureNames.push_back(face.textureName);
			}
		endloop
	endloop
	output << "\nTextures: " << textureNames.size() << ";";
	for(int i=0;i<textureNames.size();i++) {
		output << "\n" << indentation (1) << textureNames.at(i);
	}
	
	//Output the objects...
	output << "\nObjects: " << objects.size () << ";";
	loopVector (objectIndex, objects)
        objects [objectIndex]->export (0, output, objectIndex, textureNames);
	endloop
}

void World::log () {
	//For debugging, it might be useful to output the entire world into the log file.

	//The simple stuff first.
	if (!tracing) return;
	::log ("\n\nLogging World...\nStart position [%3.2f,%3.2f,%3.2f]", startPosition.x, startPosition.y, startPosition.z);

	//The objects...
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.log ();
	endloop
}
