
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                      Object                                             //
//*****************************************************************************************//

void Object::export (::ofstream &output) {
	//YOU DO THIS...
}

void Object::log () {
	//The type (it's implicit... it will be different for subclasses)...
	::log ("\nGeneral Object");

	//The faces...
	loopVector (faceIndex, faces)
		Face &face = *(faces [faceIndex]);
		::log ("\n\tFace %d:", faceIndex);
		::log ("\n\t\tTexture \"%s\":", face.textureName);
		GamePointCollection &points = face.points;
		loopVector (pointIndex, points)
			GamePoint &point = *(points [pointIndex]);
			::log ("\n\t\t%d: point [%3.2f,%3.2f,%3.2f] normal: [%3.2f,%3.2f,%3.2f] texture: [%3.2f,%3.2f].",
				pointIndex, point.x, point.y, point.z, point.nx, point.ny, point.nz, point.tx, point.ty);
			//Access point's x, y, z, nx, ny, nz, tx, ty
		endloop
	endloop
}

void Object::copyFaces (FaceCollection &faces) {
	//Copy into the the existing face collection.
	loopVector (faceIndex, faces)
		Face *oldFace = faces [faceIndex];
		//Make a new texture name.
		char *newTextureName = new char [strlen (oldFace->textureName) + 1];
		strcpy (newTextureName, oldFace->textureName);
		//Make a new face with new points, texture name (and the plane).
		Face *newFace = new Face (oldFace->plane, newTextureName);
		//Copy the point collection entries.
		loopVector (pointIndex, oldFace->points)
			GamePoint *oldPoint = oldFace->points [pointIndex];
			GamePoint *newPoint = new GamePoint;
			*newPoint = *oldPoint; //Copy the data.
			newFace->points.push_back (newPoint);
		endloop
		this->faces.push_back (newFace);
	endloop
}
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
	startPosition.zero (); //In case there is no "info_player_start"...
};

World::~World () {
	deleteObjectCollectionEntries (objects);
};

World *World::read () {
	//Prompts for a ".map" file, reads it in, creates a World object, invokes
	//"add (properties,faces) for each Object, and returns the World object (or NULL if
	//not successful).

	//Read in the worldcraft ".map" data.
	char *worldcraftFilename = promptForMapRead ();
	if (worldcraftFilename == NULL) return NULL; //User must have cancelled.
	return read (worldcraftFilename);
}

World *World::read (char *filename) {
	//Prompts for a ".map" file, reads it in, creates a World object, invokes
	//"add (properties,faces) for each Object, and returns the World object (or NULL if
	//not successful).

	//Read in the worldcraft ".map" data.
	WorldcraftGroupCollection *groups = worldcraftReader (filename);

	//Create a World object and loop through the Worldcraft groups adding each object to the world.
	World *world = new World ();
	loopVector (groupIndex, *groups)
		WorldcraftGroup *group = (*groups) [groupIndex];
		char *type = (char *) get (group, "classname");
		convertToLowercase (type);
		if (type == NULL) {::log ("\nCould not extract the class name, ignored item..."); continue;}
		WorldcraftObjectCollection *objects = (WorldcraftObjectCollection *) get (group, "objects");
		loopVector (objectIndex, *objects)
			WorldcraftObject &object = *((*objects) [objectIndex]);
			object.type = type; object.properties = group;
			world->add (object);
		endloop
		if (objects->size () == 0) {
			WorldcraftObject *object = new WorldcraftObject ();
			object->type = type; object->properties = group;
			world->add (*object);
		}
	endloop

	//Now that we're done, delete the worldcraft data and return the world (it's made copies of everything).
	deleteWorldcraftGroupCollection (groups);
	return world;
}

Point worldcraftToStandardPoint (Point &worldcraftPoint) {
	//Convert points to a standard coordinate system (worldcraft uses x +ve is right, y+ve is back, z+ve is up).
	extern double worldcraftToMeters ();
	Point standardPoint = {
		worldcraftPoint.x * worldcraftToMeters (), 
		worldcraftPoint.z * worldcraftToMeters (),
		-worldcraftPoint.y * worldcraftToMeters ()};
	return standardPoint;
}

void World::OLDadd (WorldcraftObject &worldcraftObject) {
	//Create a NON-worldcraft object corresponding to the worldcraft object.

	//For debugging, let's see what the worldcraft object looks like.
	if (tracing) {::log ("\n\nInput object..."); worldcraftObject.log ();}

	if (stricmp (worldcraftObject.type, "worldspawn") == 0) {
		//It's a standard static world object.
		Object *object = new Object ();
		object->copyFaces (*worldcraftObject.faces);
		objects.push_back (object);
		//And then let's see what the normal object looks like...
		if (tracing) {::log ("\n\nOutput object..."); object->log ();}
	} else if (stricmp (worldcraftObject.type, "info_player_start") == 0) {
		//It's special, record the position in the world object.
		char *origin = (char *) get (worldcraftObject.properties, "origin");
		sscanf (origin, "%lf %lf %lf", &startPosition.x, &startPosition.y, &startPosition.z);
		startPosition = worldcraftToStandardPoint (startPosition);
		if (tracing) {::log ("\n\nOutput object... info_player_start at [%3.2f,%3.2f,%3.2f]", 
			startPosition.x, startPosition.y, startPosition.z);}
	} else if (stricmp (worldcraftObject.type, "Environment") == 0) {
		//It's probably special too...
		//... for you to do...
		if (tracing) {::log ("\n\nOutput object... Environment..."); worldcraftObject.log ();}
	} else if (stricmp (worldcraftObject.type, "Vehicle") == 0) {
		//It's a special world object (how about making it a subclass of Object).
		//Get it's faces as done in static world object case above and
		//look at code in "worldcraftObject.log ();" to see how to get
		//specific properties (which come in as strings...).
		//... for you to do... 
		if (tracing) {::log ("\n\nOutput object... Vehicle..."); worldcraftObject.log ();}
	} else {
		::log ("\nUnknown object type \"%s\" ignored.", worldcraftObject.type);
	}

	//And then let's see what the normal object looks like...
}

void World::add (WorldcraftObject &worldcraftObject) {
	//Create a NON-worldcraft object corresponding to the worldcraft object.

	//Variables to remember the object and property dictionary processed last time.
	static void *lastObject = NULL;
	static WorldcraftGroup *lastProperties = NULL; 

	//We are starting a new group of objects if it has a different property dictionary.
	const bool startingNewGroup = worldcraftObject.properties != lastProperties;
	lastProperties = worldcraftObject.properties; //Prepare for next time...

	//For debugging, let's see what the worldcraft object looks like.
	if (tracing) {::log ("\n\nInput object..."); worldcraftObject.log ();}

	if (stricmp (worldcraftObject.type, "worldspawn") == 0) {
		//It's a standard static world object. Create an object, duplicate the faces
		//and discard the properties...
		const bool wantToGroupObjects = false;
		const bool needNewObject = startingNewGroup || !wantToGroupObjects;
		Object *object = needNewObject ? new Object () : (Object *) lastObject;
		object->copyFaces (*worldcraftObject.faces);
		if (needNewObject) {objects.push_back (object);}
		lastObject = object; //Prepare for next time...
		//And then let's see what the normal object looks like...
		if (tracing) {::log ("\n\nOutput object%s...", wantToGroupObjects ? " SO FAR" : ""); object->log ();}
	} else if (stricmp (worldcraftObject.type, "info_player_start") == 0) {
		//It's special, discard the faces, and record the position (extracted from the "origin property") 
		//into the world object's "startPosition (with suitable conversion)".
		startPosition = worldcraftToStandardPoint (worldcraftObject.asPointProperty ("origin", "0 0 0"));
		lastObject = NULL; //Prepare for next time...
		if (tracing) {
			::log ("\n\nOutput object... PLAYER at [%3.2f,%3.2f,%3.2f]", 
				startPosition.x, startPosition.y, startPosition.z);
		}
	} else if (stricmp (worldcraftObject.type, "Environment") == 0) {
		//It's special too... We don't want the objects... just some of the properties.
		//They could be recorded directly into the world too (OR in a special environment object)... 
		//FOR YOU TO DO... SEE VEHICLE BELOW FOR MORE EXAMPLES USING PROPERTIES...
		lastObject = NULL; //Prepare for next time...
		if (tracing) {::log ("\n\nOutput object... Environment..."); worldcraftObject.log ();}
	} else if (stricmp (worldcraftObject.type, "Vehicle") == 0) {
		//It's a special world object (make it a subclass of Object for simplicity)... Replace the
		//multiple worldcraft objects by ONE object with the combined faces and record some properties. 

		//???????????????????????????????????????????????????????????????????????????????
		#define Vehicle Object /*Delete this line once you have created a Vehicle class*/
		//???????????????????????????????????????????????????????????????????????????????

		const bool wantToGroupObjects = true;
		const bool needNewObject = startingNewGroup || !wantToGroupObjects;
		Vehicle *vehicle = needNewObject ? new Vehicle () : (Vehicle *) lastObject;
		vehicle->copyFaces (*worldcraftObject.faces);
		if (needNewObject) {objects.push_back (vehicle);}
		lastObject = vehicle; //Prepare for next time...
		
		//Pick up useful properties (uncomment set methods below to record in vehicle)...
		char *targetname = worldcraftObject.asStringProperty ("targetname", "Unknown");
		long style = worldcraftObject.asIntegerProperty ("style", "0");

		//vehicle->name (targetname);
		//vehicle->girlfriend (girlfriend);
		//vehicle->style (style);

		if (tracing) {::log ("\n\nOutput object%s...", wantToGroupObjects ? " SO FAR" : ""); vehicle->log ();}
	} else {
		::log ("\nUnknown object type \"%s\" ignored.", worldcraftObject.type);
	}
}

void World::export (char *filename) {
	//Write out your world (perhaps asking different components to write themselves out).

	//FOR YOU TO DO....... THOUGH HERE'S A START...
	::ofstream output (filename);
	if (output.bad ()) {prompt ("Unable to create output file \"%s\".", filename); return;}
	
	//The simple stuff.
	output << "\nWorld version 1";
	output << "\n" << startPosition.x << " " << startPosition.y << " " << startPosition.z;

	//The objects: A version WITH loop macros.
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.export (output);
	endloop

	output.close ();
}

void World::log () {
	//For debugging, it might be useful to output the entire world into the log file.

	//The simple stuff first.
	if (!tracing) return;
	::log ("\n\nLogging World...\nStart position [%3.2f,%3.2f,%3.2f]", startPosition.x, startPosition.y, startPosition.z);

	//The objects: A version WITH loop macros.
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.log ();
	endloop
	
	/*
	//The objects: A version WITHOUT loop macros.
	for (long objectIndex = 0; objectIndex < objects.size (); objectIndex++) {
		Object &object = *(objects [objectIndex]);
		object.log ();
	}
	*/
}

void World::finalize () {
}
