
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"


//*****************************************************************************************//
//                                      Utilities                                          //
//*****************************************************************************************//

char *indentation (long indent) {
	static char indents [256] = {"\0"};
	if (*indents == '\0') {
		for (long index = 0; index < 256; index++) {
			indents [index] = '\t';
		}
		indents [255] = '\0';
	}
	return &indents [255 - indent];
}

char *duplicate (char *string) {
	char *duplicate = new char [strlen (string) + 1];
	strcpy (duplicate, string);
	return duplicate;
}

void convertToLowercase (char *name) {
	for (long index = strlen (name) - 1; index >= 0; index--) {
		name [index] = tolower (name [index]);
	}
}

char privatePathName [256];

char *pathNameWithoutSuffix (char *string) {
	//For "c:\junk\text.bmp", returns "c:\junk\text"... WARNING: Second invocation clears old path name...
	strcpy (privatePathName, string);
	char *suffix = strrchr (privatePathName, '.');
	if (suffix != NULL) *suffix = '\0';
	return privatePathName;
}

char privateShortName [256];

char *shortName (char *string) {
	//For "c:\junk\text.bmp", returns "text"... WARNING: Second invocation clears old short name...
	strcpy (privateShortName, string);
	char *suffix = strrchr (privateShortName, '.');
	if (suffix != NULL) *suffix = '\0';

	char *prefix1 = strrchr (privateShortName, '\\'); if (prefix1 != NULL) prefix1++; //Skip '\\'...
	char *prefix2 = strrchr (privateShortName, '/'); if (prefix2 != NULL) prefix2++; //Skip '/'...
	char *prefix3 = strrchr (privateShortName, ':'); if (prefix3 != NULL) prefix3++; //Skip ':'...

	char *shortName = (char *) maximum (maximum (maximum ((long) prefix1, (long) prefix2), (long) prefix3), (long) privateShortName);
	return shortName;
}

//*****************************************************************************************//
//                                   UniversalObject                                       //
//*****************************************************************************************//

bool tracing = false;

//Note: input.getline (line, 256, c) => picks up everything up to c exclusive and discards c.
#define CLEAR_THE_LINE input.getline (line, 256, '\n');
#define GET_NEXT_NUMBER input.getline (line, 256, ',');
#define GET_LAST_NUMBER input.getline (line, 256, ';'); 
#define GET_FINAL_NUMBER input.getline (line, 256, '\n'); 
#define NEXT_STRING CLEAR_THE_LINE
#define NEXT_HEADER input.getline (line, 256, ':');

//-----------------------------------------------------------------------------------------//
//                                     UniversalObject                                     //
//-----------------------------------------------------------------------------------------//

WorldcraftFGDReader *worldCraftFGDReader;

void UniversalObject::setup () {
	extern char *fgdFilePath;
	worldCraftFGDReader = new WorldcraftFGDReader (fgdFilePath);
	worldCraftFGDReader->absorbAllVariables ();
}

void UniversalObject::wrapup () {
	delete worldCraftFGDReader;
}

void UniversalObject::tick () {
	//Tick all the objects in the world such as doors, sun, weather...
}

void UniversalObject::draw () {
	//Draw all the objects in the world such as terrain, buildings, etc.
	//The objects...
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.draw ();
	endloop
}

void UniversalObject::draw (Point &position) {
	glPushMatrix ();
		glTranslated (position.x, position.y, position.z);
		draw ();
	glPopMatrix ();
}

void UniversalObject::computeBoundingBox (BoundingBox &box) {
	box.reset ();
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.computeBoundingBox (box, object.transformation.normal ());
	endloop
}

void UniversalObject::log () {
	//For debugging, it might be useful to output the entire world into the log file.

	//The simple stuff first.
//	::log ("\n\nWorld...\nStart position [%3.2f,%3.2f,%3.2f]", startPosition.x, startPosition.y, startPosition.z);

	//The objects...
	loopVector (objectIndex, objects)
		Object &object = *(objects [objectIndex]);
		object.log (objectIndex);
	endloop
}

void UniversalObject::unload () {
	//Unload all of the textures prior to deleting the world... If you don't, you will
	//reach a point where you will run in slow motion as the card thrashes around trying
	//to allow you to run in the memory it has left... Rebooting will fix it...
}

UniversalObject *UniversalObject::load (char *filename) {
	::ifstream input; input.open (filename); 
	if (input.bad ()) {prompt ("Unable to open input file \"%s\".", filename); return NULL;}
	UniversalObject *universalObject = new UniversalObject;
	universalObject->load (input);
	input.close ();
	//Why else would we load it if not to re-export it with some fix of some kind...
	universalObject->weldAlmostEqualPoints ();
	/*
	//Additional passes seem to make further changes... So try a few times...
	for (long index = 0; index < 4; index++) {
		::log ("\nPASS2 =========================>");
		universalObject->weldAlmostEqualPoints ();
	}
	*/
	return universalObject;
}

Texture *readUnknownTexture (char *textureName, const bool haltIfNotFound = false) {
	extern char **textureSearchDirectories; extern long textureSearchDirectoriesSize;
	static char *suffixes [] = {".dds", ".tga", ".bmp"};
	static char fileName [500]; 

	for (long suffixIndex = 0; suffixIndex < 3; suffixIndex++) {

		for (long directoryIndex = 0; directoryIndex < textureSearchDirectoriesSize; directoryIndex++) {
			//strcpy (fileName, textureDirectory ());
			strcpy (fileName, textureSearchDirectories [directoryIndex]);
			strcat (fileName, "\\");
			strcat (fileName, textureName);
			strcat (fileName, suffixes [suffixIndex]);

			Texture *texture = Texture::readTexture (fileName);
			if (texture != NULL) return texture;
		}
	}
	//::log ("\nBefore readUnknownTexture prompt, textureDirectory () is \"%s\"...", textureDirectory ());

	strcpy (fileName, textureDirectory ());
	strcat (fileName, "\\");
	strcat (fileName, textureName);
	strcat (fileName, ".*");
	char *pathName = promptForTexture (fileName); 
	//::log ("\nAfter readUnknownTexture prompt, textureDirectory () is \"%s\"...", textureDirectory ());
	if (pathName == NULL) {if (haltIfNotFound) halt ("Quitting since user could not find texture..."); return NULL;}

	strcpy (fileName, pathName); //Make a copy of the name in case other prompts overwrite it...
	Texture *texture = Texture::readTexture (fileName);
	if (texture != NULL) {
		char *directoryEnd = strrchr (fileName, '\\'); if (directoryEnd == NULL) directoryEnd = strrchr (fileName, '/');
		if (directoryEnd == NULL) directoryEnd = strrchr (fileName, ':');
		if (directoryEnd == NULL) halt ("\nQuitting, File \"%s\" not in a directory...", fileName);
		*directoryEnd = '\0';
		addToTextureSearchPaths (fileName);
		return texture;
	}
	if (haltIfNotFound) halt ("\nQuitting... Could not read texture \"%s\"...", pathName);
	return NULL; //Can't be found...
}

void UniversalObject::load (::ifstream &input) {
	char line [256]; //Working variable...
	
	//Input the header.
	CLEAR_THE_LINE

	//Input the objects.
	NEXT_HEADER
	GET_LAST_NUMBER long objectsSize = atoi (line);
	for (long objectIndex = 0; objectIndex < objectsSize; objectIndex++) {
		Object *object = new Object;
		object->load (input, this);
		objects.push_back (object);
	}
}

void UniversalObject::dump (char *filename) {
	//Write out the universalObject (asking different components to write themselves out).

	::ofstream output (filename);
	if (output.bad()) {prompt ("Unable to create output file \"%s\".", filename); return;}
	output.precision (16);
	dump (output);

	//Finish up...
	output.close ();
}

void UniversalObject::dump (::ofstream &output) {
	//Output a simple header..."
	output << "Universal object";

	//Note the texture names...
	vector<char*> textureNames; addTextureNames (textureNames);

	//Note the look names...
	vector<char*> lookNames; addLookNames (lookNames);

	//Output the objects...
	output << "\nObjects: " << objects.size () << ";";
	loopVector (objectIndex, objects)
        objects [objectIndex]->dump (0, output, objectIndex, textureNames, lookNames);
	endloop
}

void UniversalObject::addTextureNames (vector<char*> &textureNames) {
	loopVector (objectIndex, objects)
		objects [objectIndex]->addTextureNames (textureNames);
	endloop
}

void UniversalObject::addLookNames (vector<char*> &lookNames) {
	loopVector (objectIndex, objects)
		objects [objectIndex]->addLookNames (lookNames);
	endloop
}

Texture *UniversalObject::textureFor (char *name) {
	//Return the texture for this name; if not there, add it (
	for (long textureIndex = 0; textureIndex < textures.size (); textureIndex++) {
		Texture *texture = textures [textureIndex];
		if (stricmp (texture->textureName, name) == 0) return texture;
	}
	Texture *texture = readUnknownTexture (name); 
	if (texture == NULL) texture = new Texture (name);
	textures.push_back (texture);
	return texture;
}

void UniversalObject::loadAllTextures () {
	for (long textureIndex = 0; textureIndex < textures.size (); textureIndex++) {
		Texture *texture = textures [textureIndex];
		texture->load ();
	}
}

UniversalObject *UniversalObject::read (char *filename) {
	char *suffix = strrchr (filename, '.') + 1;
	if (stricmp (suffix, "map") == 0) return readMapFile (filename);
	if (stricmp (suffix, "3dm") == 0) return readRhinoFile (filename);
	if (stricmp (suffix, "xsi") == 0) return readSoftimageFile (filename);
	if (stricmp (suffix, "mesh") == 0) return readLearningDimensionsFile (filename);
	if (stricmp (suffix, "uni") == 0) return UniversalObject::load (filename);
	halt ("\nUnknown file type \".%s\"...", suffix);
}

void UniversalObject::weldAlmostEqualPoints () {   
	loopVector (objectIndex, objects)
        Object &object = *(objects [objectIndex]);
		object.weldAlmostEqualPoints ();
	endloop
}

//-----------------------------------------------------------------------------------------//
//                       Worldcraft (.map files) specific methods.                         //
//-----------------------------------------------------------------------------------------//

UniversalObject *UniversalObject::readMapFile (char *filename) {
	//Creates a universal object, invokes "add (aWorldcraftObject) for each WorldcrafObject, 
	//and returns the universal object (or NULL if not successful). Method add deals
	//with retrieving and converting everything to the proper universal object form...
	
	//Read in the worldcraft ".map" data.
	WorldcraftGroupCollection *groups = worldcraftReader (filename);

	//Create a universal object and loop through the Worldcraft groups adding each object to the world.
	UniversalObject *universalObject = new UniversalObject ();
	loopVector (groupIndex, *groups)
		WorldcraftGroup *group = (*groups) [groupIndex];
		char *type = (char *) get (group, "classname");
		convertToLowercase (type);
		if (type == NULL) {::log ("\nCould not extract the class name, ignored item..."); continue;}
		WorldcraftObjectCollection *objects = (WorldcraftObjectCollection *) get (group, "objects");
		loopVector (objectIndex, *objects)
			WorldcraftObject &object = *((*objects) [objectIndex]);
			object.type = type; object.properties = group;
			universalObject->add (object);
		endloop
		if (objects->size () == 0) {
			WorldcraftObject *object = new WorldcraftObject ();
			object->type = type; object->properties = group;
			universalObject->add (*object);
		}
	endloop
	
	//Now that we're done, delete the worldcraft data and return the world (it's made copies of everything).
	deleteWorldcraftGroupCollection (groups);
	
	//But just before we do that, perform some standardization...
	universalObject->standardizeMapFile ();
	
	return universalObject;
}

//-----------------------------------------------------------------------------------------//
//                           Rhino (.3dm files) specific methods.                          //
//-----------------------------------------------------------------------------------------//

UniversalObject *UniversalObject::readRhinoFile (char *filename) {	

	RhinoLoader rhinoLoader;
	RhinoMesh *rhinoMesh = rhinoLoader.loadMesh (filename);
	if (rhinoMesh == NULL) return NULL;

	UniversalObject *universalObject = new UniversalObject ();
	
	for (long i = 0; i < rhinoMesh->sectionsCount; i++) {
		RhinoMeshSection &section = rhinoMesh->sections [i];
		char *textureName = shortName (section.textureName);

		Object *object = new Object (); 
		universalObject->objects.push_back (object);
		object->properties.put ("useforcollision", section.useForCollision ? "1" : "0");
		object->properties.put ("normals", asString ("%d", section.normalType ()));
		object->properties.put ("material", asString ("%d", section.collisionMaterialType ()));

		for (long j = 0; j < section.indexCount; j += 3) {
			Face *face = new Face (duplicate (textureName));
			face->properties.put ("look", section.look);
			object->faces.push_back (face);

			RhinoVertex &rhinoVertex1 = section.vertices [section.indices [j]];
			RhinoVertex &rhinoVertex2 = section.vertices [section.indices [j + 1]];
			RhinoVertex &rhinoVertex3 = section.vertices [section.indices [j + 2]];

			GamePoint *vertex1 = new GamePoint;
			GamePoint *vertex2 = new GamePoint;
			GamePoint *vertex3 = new GamePoint;

			vertex1->x = rhinoVertex1.x; vertex1->y = rhinoVertex1.y; vertex1->z = rhinoVertex1.z;
			vertex1->tx = rhinoVertex1.tx; vertex1->ty = rhinoVertex1.ty;

			vertex2->x = rhinoVertex2.x; vertex2->y = rhinoVertex2.y; vertex2->z = rhinoVertex2.z;
			vertex2->tx = rhinoVertex2.tx; vertex2->ty = rhinoVertex2.ty;

			vertex3->x = rhinoVertex3.x; vertex3->y = rhinoVertex3.y; vertex3->z = rhinoVertex3.z;
			vertex3->tx = rhinoVertex3.tx; vertex3->ty = rhinoVertex3.ty;

			Vector vector2 = vertex3->asPoint () - vertex2->asPoint ();
			Vector vector1 = vertex2->asPoint () - vertex1->asPoint ();
			Vector normal = (vector1.cross (vector2)).normalized ();
			if (normal.squaredLength () < 0.5) normal = Up;

			vertex1->nx = normal.x; vertex1->ny = normal.y; vertex1->nz = normal.z;
			vertex2->nx = normal.x; vertex2->ny = normal.y; vertex2->nz = normal.z;
			vertex3->nx = normal.x; vertex3->ny = normal.y; vertex3->nz = normal.z;
			
			face->points.push_back (vertex1);
			face->points.push_back (vertex2);
			face->points.push_back (vertex3);
		}
	}

	return universalObject;
}

	
//-----------------------------------------------------------------------------------------//
//                       Softimage (.xsi files) specific methods.                          //
//-----------------------------------------------------------------------------------------//

UniversalObject *UniversalObject::readSoftimageFile (char *filename) {
    //Read the softimage file if it's valid, end gracefully if not...
    CSLScene scene;
    if (!(scene.Open (filename) == SI_SUCCESS && scene.Read () == SI_SUCCESS)) {
		::prompt ("\nCould not read file \"%s\"...", filename);
		return NULL;
	}

    SoftimageImporter *importer = new SoftimageImporter (scene.Root());
	UniversalObject *universalObject = importer->load ();

	delete importer;
    scene.Close ();
	return universalObject;
}

//-----------------------------------------------------------------------------------------//
//                   Learning Dimensions (.mesh files) specific methods.                   //
//-----------------------------------------------------------------------------------------//

char buffer [256]; //So we can refer to it outside of scan routine for error messages..."

void signalError (char *keyword) {			
	if (buffer [0] == '\0')
		halt ("\nLearning dimensions reader DID NOT FIND expected keyword \"%s\"...", keyword);
	else
		halt ("\nLearning dimensions reader expected keyword \"%s\" but found \"%s\" instead...", keyword, buffer);
}

bool scan (::ifstream &input, char *keyword) {
	//Returns true if the keyword is found, false otherwise...
	long keywordSize = 0; buffer [0] = '\0';
	int character; 
	while ((character = input.peek ()) != EOF) {
		if (isalpha (character)) {
			buffer [keywordSize++] = character; input.get ();
			while ((character = input.peek ()) != EOF && isalpha (character)) {
				buffer [keywordSize++] = character; input.get ();
			}

			buffer [keywordSize++] = '\0'; //End the string...
			return (stricmp (keyword, buffer) == 0);
		} else if (character == '\\' || character == '/') {
			char line [256]; //Working variable...
			CLEAR_THE_LINE //Only this style of comments is allowed in learning dimensions mesh...
		} else if (character == '\t' || character == ' ' || character == '\f' || character == '\n' || character == '\r') {
			input.get (); //Discard it and continue...
		} else {
			return false; //Found something other than a keyword.
		}
	}
	return false;
}
				
void scanInsisting (::ifstream &input, char *keyword) {
	if (scan (input, keyword)) return;
	signalError (keyword);
}

void scanInsistingWas (::ifstream &input, char *keyword) {
	//Like above but looks at result of previous scan's buffer...
	if (stricmp (keyword, buffer) == 0) return; //OK.
	signalError (keyword);
}

UniversalObject *UniversalObject::readLearningDimensionsFile (char *filename) {
	
	//FORMAT: All indices start at 0. Bone information (if any) is discarded...
	//	version 1
	//	textures
	//		nameIndex0 name0
	//		nameIndex1 name1
	//			...
	//	end
	//	nodes
	//		 boneIndex0 boneName0 parentIndex0
	//		 boneIndex1 boneName1 parentIndex1
	//			 ...
	//	end
	//	mesh	submeshName //Repeats mesh ... endmesh for each mesh
	//	time 0
	//		vertexIndex x y z nx ny nz textureIndex tx ty numberOfBones boneIndex0 boneWeight0 ...
	//			...
	//	end
	//	endmesh
	
	::ifstream input; input.open (filename); 
	if (input.bad ()) {prompt ("Unable to open input file \"%s\".", filename); return NULL;}
	UniversalObject *universalObject = new UniversalObject;

	char line [256]; //Working variable...

	long texturesLimit = 100; long texturesSize = 0;
	char **textureNames = new char * [texturesLimit];

	
	scanInsisting (input, "version"); 
		CLEAR_THE_LINE //And skip the version number...

	scanInsisting (input, "textures");
	while (!scan (input, "end")) { 

		//Expecting index string pairs...
		long nameIndex; input >> nameIndex;

		if (nameIndex != texturesSize) halt ("\nTexture indices out of sequence in learning dimensions file...");

		if (texturesSize >= texturesLimit) {
			//Need to grow the collection of names...
			long newLimit = texturesLimit * 2;
			char **newTextureNames = new char * [newLimit];
			for (long index = 0; index < texturesLimit; index++) {
				newTextureNames [index] = textureNames [index];
			}
			delete [] textureNames; textureNames = newTextureNames;
		}
		
		NEXT_STRING
		char textureName [256]; textureName [0] = '\0';
		sscanf (line, " \"%[^\"]\"", textureName);
		convertToLowercase (textureName);
		textureNames [texturesSize++] = duplicate (shortName (textureName));
	}

	scanInsisting (input, "nodes");
	while (!scan (input, "end")) { 
		CLEAR_THE_LINE //Ignoring all bones...
	}

	while (scan (input, "mesh")) { 
		CLEAR_THE_LINE //Discard the mesh name...

		Object *object = new Object (); 
		universalObject->objects.push_back (object);

		scanInsisting (input, "time");
		CLEAR_THE_LINE //Discard the time number "0"
		while (!scan (input, "end")) { 
			//All points come in groups of threes to form one triangle...
			Face *face = NULL;

			for (long pointIndex = 0; pointIndex < 3; pointIndex++) {

				long vertexIndex; input >> vertexIndex;
				double x; input >> x;
				double y; input >> y; 
				double z; input >> z; 
				double nx; input >> nx;
				double ny; input >> ny;
				double nz; input >> nz;
				long textureIndex; input >> textureIndex; 
				double tx; input >> tx;
				double ty; input >> ty;
				CLEAR_THE_LINE //Ignoring all the rest which deals with bones...

				if (pointIndex == 0) {
					face = new Face (duplicate (textureNames [textureIndex]));
					object->faces.push_back (face);
				}
				GamePoint *vertex = new GamePoint;

				vertex->x = x; vertex->y = y; vertex->z = z;
				vertex->nx = nx; vertex->ny = ny; vertex->nz = nz;
				vertex->tx = tx; vertex->ty = ty;
				
				face->points.push_back (vertex);
			}
		}
		
		CLEAR_THE_LINE //To line after end...
		scanInsisting (input, "endMesh");
		CLEAR_THE_LINE //To line after endMesh...
	}

	if (input.peek () != EOF) prompt ("\nIgnoring extra unread data at the end of learning dimensions file...");

	input.close ();
	delete [] textureNames;
	return universalObject;
}

//-----------------------------------------------------------------------------------------//
//                                        Object                                           //
//-----------------------------------------------------------------------------------------//

void Object::tick () {
	//This could make an object rotate or change size or do nothing.
}

void Object::draw () {
	//Draw the faces in this object and in the sub-objects.
	glPushMatrix ();
		glMultMatrixd (transformation.normal ());
		loopVector (faceIndex, faces)
			Face &face = *(faces [faceIndex]);
			face.draw ();
		endloop
			
		loopVector (subObjectIndex, objects)
			Object &object = *(objects [subObjectIndex]);
			object.draw ();
		endloop
	glPopMatrix ();
}

void Object::computeBoundingBox (BoundingBox &externalBox, Transformation &transformation) {
	//Compute the object's bounding box and integrate with external box...
	box.reset ();
	loopVector (faceIndex, faces)
		Face &face = *(faces [faceIndex]);
		face.computeBoundingBox (box, transformation);
	endloop
		
	Transformation product;
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		product.multiply (transformation, object.transformation.normal ());
		object.computeBoundingBox (box, product);
	endloop
		
	externalBox.add (box);
}

void Object::logFaces () {
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

void Object::log () {
	//The type (it's implicit... it will be different for subclasses)...
	::log ("\nObject");
	transformation.log (); //Not indented properly yet...

	//The properties...
	::log ("\n\tProperties: ");
	loopPropertyDictionary (name, value, properties)
		::log ("\n\t\t\"%s\" => \"%s\"", name, (LPCTSTR) *value);
	endloop

	//The faces...
	logFaces ();
	
	//The sub-objects...
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.log ();
	endloop	
	::log ("\nEnd object");
}

void Object::log (long objectIndex) {
	//The type (it's implicit... it will be different for subclasses)...
	::log ("\nObject %d:", objectIndex);
	transformation.log (); //Not indented properly yet...

	//The properties...
	::log ("\n\tProperties: ");
	loopPropertyDictionary (name, value, properties)
		::log ("\n\t\t\"%s\" => \"%s\"", name, (LPCTSTR) *value);
	endloop

	//The faces...
	logFaces ();

	//The sub-objects...
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.log (subObjectIndex);
	endloop	
	::log ("\nEnd object %d", objectIndex);
}

void Object::load (::ifstream &input, UniversalObject *universalObject) {
	char line [256]; //Working variable...

	//Input the header.
	NEXT_HEADER
	GET_LAST_NUMBER long objectIndex = atoi (line); //Only useful for debugging or browsing.

	//Input the transformation.
	NEXT_HEADER CLEAR_THE_LINE

	//The standard transformation matrix elements...
	Transformation &normal = transformation.normal ();
	GET_NEXT_NUMBER normal.m11 = atof (line);
	GET_NEXT_NUMBER normal.m12 = atof (line);
	GET_NEXT_NUMBER normal.m13 = atof (line);
	GET_NEXT_NUMBER normal.m14 = atof (line);
	
	GET_NEXT_NUMBER normal.m21 = atof (line);
	GET_NEXT_NUMBER normal.m22 = atof (line);
	GET_NEXT_NUMBER normal.m23 = atof (line);
	GET_NEXT_NUMBER normal.m24 = atof (line);
	
	GET_NEXT_NUMBER normal.m31 = atof (line);
	GET_NEXT_NUMBER normal.m32 = atof (line);
	GET_NEXT_NUMBER normal.m33 = atof (line);
	GET_NEXT_NUMBER normal.m34 = atof (line);
	
	GET_NEXT_NUMBER normal.m41 = atof (line);
	GET_NEXT_NUMBER normal.m42 = atof (line);
	GET_NEXT_NUMBER normal.m43 = atof (line);
	GET_LAST_NUMBER normal.m44 = atof (line);
	CLEAR_THE_LINE

	//The inverse transformation matrix elements...
	Transformation &inverse = transformation.inverse;
	GET_NEXT_NUMBER inverse.m11 = atof (line);
	GET_NEXT_NUMBER inverse.m12 = atof (line);
	GET_NEXT_NUMBER inverse.m13 = atof (line);
	GET_NEXT_NUMBER inverse.m14 = atof (line);
	
	GET_NEXT_NUMBER inverse.m21 = atof (line);
	GET_NEXT_NUMBER inverse.m22 = atof (line);
	GET_NEXT_NUMBER inverse.m23 = atof (line);
	GET_NEXT_NUMBER inverse.m24 = atof (line);
	
	GET_NEXT_NUMBER inverse.m31 = atof (line);
	GET_NEXT_NUMBER inverse.m32 = atof (line);
	GET_NEXT_NUMBER inverse.m33 = atof (line);
	GET_NEXT_NUMBER inverse.m34 = atof (line);
	
	GET_NEXT_NUMBER inverse.m41 = atof (line);
	GET_NEXT_NUMBER inverse.m42 = atof (line);
	GET_NEXT_NUMBER inverse.m43 = atof (line);
	GET_LAST_NUMBER inverse.m44 = atof (line);
	CLEAR_THE_LINE

	//Input the properties
	NEXT_HEADER
	GET_LAST_NUMBER long propertiesSize = atoi (line); CLEAR_THE_LINE
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		NEXT_STRING
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		properties.put (key, value);
	}

	//Input the faces.
	NEXT_HEADER
	GET_LAST_NUMBER long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->load (input, universalObject);
		faces.push_back (face);
	}

	//Input the sub-objects.
	NEXT_HEADER
	GET_LAST_NUMBER long subObjectsSize = atoi (line);
	for (long subObjectIndex = 0; subObjectIndex < subObjectsSize; subObjectIndex++) {
		Object *object = new Object;
		object->load (input, universalObject);
		objects.push_back (object);
	}
}

void Object::dump (long indent, ::ofstream &output, long objectIndex, vector<char*> &textures, vector<char*> &looks) {
	//Output a simple header..."
	output << "\n" << indentation (indent) << "Object: " << objectIndex << ";";

	//Output the transformation...
	output << "\n" << indentation (indent + 1) << "Transformation: //m11 m12 m13 m14 m21 m22 ... m44";
	{
		Transformation &t = transformation.normal ();
		output << "\n" << indentation (indent + 2) 
			<< t.m11 << ", " << t.m12 << ", " << t.m13 << ", " << t.m14 << ", "
			<< t.m21 << ", " << t.m22 << ", " << t.m23 << ", " << t.m24 << ", "
			<< t.m31 << ", " << t.m32 << ", " << t.m33 << ", " << t.m34 << ", "
			<< t.m41 << ", " << t.m42 << ", " << t.m43 << ", " << t.m44 << "; //Standard...";
	}
	{
		Transformation &t = transformation.inverse;
		output << "\n" << indentation (indent + 2)
			<< t.m11 << ", " << t.m12 << ", " << t.m13 << ", " << t.m14 << ", "
			<< t.m21 << ", " << t.m22 << ", " << t.m23 << ", " << t.m24 << ", "
			<< t.m31 << ", " << t.m32 << ", " << t.m33 << ", " << t.m34 << ", "
			<< t.m41 << ", " << t.m42 << ", " << t.m43 << ", " << t.m44 << "; //Inverse...";
	}

	//Output the properties...
	const bool namingUnnamedObjectObjects = false; //If we change our minds, flip the boolean.
	const bool needNewName = namingUnnamedObjectObjects && properties.get ("name") == NULL;
	output << "\n" << indentation (indent + 1) << "Properties: " << (properties.size () + needNewName) << ";";
	loopPropertyDictionary (name, value, properties)
		output << "\n" << indentation (indent + 2) << "\"" << name << "\" => \"" << (LPCTSTR) *value << "\"";
	endloop
	if (needNewName) {
		output << "\n" << indentation (indent + 2) << "\"name\" => \"" << asString ("Object%d", objectIndex) << "\"";
	}

	//Output the faces...
	output << "\n" << indentation (indent + 1) << "Faces: " << faces.size () << ";";
	loopVector (faceIndex, faces)
		faces [faceIndex]->dump (indent + 1, output, faceIndex, textures, looks);
	endloop

	//output the sub-objects.
	output << "\n" << indentation (indent + 1) << "Sub-objects: " << objects.size () << ";";
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.dump (indent + 1, output, subObjectIndex, textures, looks);
	endloop
}

void Object::addTextureNames (vector<char*> &textureNames) {
	loopVector (faceIndex, faces)
		faces [faceIndex]->addTextureNames (textureNames);
	endloop
		
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.addTextureNames (textureNames);
	endloop
}

void Object::addLookNames (vector<char*> &lookNames) {
	loopVector (faceIndex, faces)
		faces [faceIndex]->addLookNames (lookNames);
	endloop

	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.addLookNames (lookNames);
	endloop
}

const double maximumVertexSeparation = 0.03;
const double minimumEdgeLength = 1.0;

bool Object::isWeldCandidate (GamePoint &edge0Start, GamePoint &edge0End, GamePoint &edge1Start, GamePoint &edge1End) {
	Point start0 = edge0Start.asPoint (); Point end0 = edge0End.asPoint ();
	Point start1 = edge1Start.asPoint (); Point end1 = edge1End.asPoint ();

	//If the starting points are too far apart, they can't be candidates...
	const double closeEnoughToMerge = squared (maximumVertexSeparation); 
	const double squaredDistanceBetweenPoints = start0.squaredDistanceTo (start1);
	if (squaredDistanceBetweenPoints > closeEnoughToMerge) return false;

	//If the starting points are already equal, they can't be candidates...
	const double reallyCloseSquared = squaredEpsilon ();
	if (squaredDistanceBetweenPoints < reallyCloseSquared) return false;

	//If the edges are too short, they can't be candidates...
	const double longEnough = squared (minimumEdgeLength); 
	if (start0.squaredDistanceTo (end0) < longEnough) return false;
	if (start1.squaredDistanceTo (end1) < longEnough) return false;

	//If the edges are NOT almost parallel, they can't be candidates...
	if ((end0 - start0).angleInDegreesBetweenVectors (end1 - start1) > 10.0) return false;

	//I guess, these start points should be merged...
	return true;
}

/*
void Object::weldFaces (Face &face1, Face &face2) {
	//Build a plane for face1...
	Plane face1Plane (face1.points [0]->asPoint (), face1.points [1]->asPoint (), face1.points [2]->asPoint ());

	//Cycle through face1's edges in counter-clockwise order..
	for (long startIndex1 = face1.points.size () - 1, endIndex1 = 0; endIndex1 < face1.points.size (); startIndex1 = endIndex1, endIndex1++) {
		GamePoint &start1 = *(face1.points [startIndex1]); GamePoint &end1 = *(face1.points [endIndex1]);

		//Cycle through face2's edges in clockwise order... and also track the previous edge to intersect with face1's plane.
		for (long previousIndex2 = 1, startIndex2 = 0, endIndex2 = face2.points.size () - 1; endIndex2 >= 0; previousIndex2 = startIndex2, startIndex2 = endIndex2, endIndex2--) {
			GamePoint &previous2 = *(face2.points [previousIndex2]); GamePoint &start2 = *(face2.points [startIndex2]); GamePoint &end2 = *(face2.points [endIndex2]);
			if (isWeldCandidate (start1, end1, start2, end2)) {
				//The previous edge should hit face1's plane in the neighborhood of start1 and start2. So...

				//If there is no intersection, we can't find a suitable replacement point...
				double t; Vector direction; if (!face1Plane.intersectsInfiniteLine (previous2.asPoint (), start2.asPoint (), direction, t)) continue; 
				
				//If it intersects too far away from the line end point, we can't find a suitable replacement point...
				if (t < 0.5 && t > 1.5) continue;

				//If the intersection is too far from BOTH start points, we can't find a suitable replacement point...
				Point intersection = previous2.asPoint () + direction * t;
				const double closeEnoughToMerge = squared (maximumVertexSeparation); 
				if (intersection.squaredDistanceTo (start1.asPoint ()) > closeEnoughToMerge && intersection.squaredDistanceTo (start2.asPoint ()) > closeEnoughToMerge) continue;

				//So this one works... Replace BOTH points by this intersection point...
				static long count = 0;
				::log ("\n%d: %3.7f meters apart; Welding [%3.3f %3.3f %3.3f] and [%3.3f %3.3f %3.3f] as part of an edge of length %3.2f and moving them distances %3.4f and %3.4f.", 
					count++,
					start1.asPoint ().distanceTo (start2.asPoint ()),
					start1.x, start1.y, start1.z, start2.x, start2.y, start2.z, 
					start1.asPoint ().distanceTo (end1.asPoint ()), 
					start1.asPoint ().distanceTo (intersection), start2.asPoint ().distanceTo (intersection));

				start1.x = start2.x = intersection.x; 
				start1.y = start2.y = intersection.y;
				start1.z = start2.z = intersection.z;
				
			}
		}
	}
}
*/

bool Object::canWeldVertices (Plane &facePlane, Point &start, Point &end, Point &mergePoint1, Point &mergePoint2, Point &intersection) {
	//If there is no intersection with the plane, we can't find a suitable replacement point...
	double t; Vector direction; if (!facePlane.intersectsInfiniteLine (start, end, direction, t)) return false;
	
	//If it intersects the plane too far away from the end point, we can't find a suitable replacement point...
	if (t < 0.5 && t > 1.5) return false;

	//If the intersection is too far from BOTH start points, we can't find a suitable replacement point...
	intersection = start + direction * t;
	const double closeEnoughToMerge = squared (maximumVertexSeparation); 
	if (intersection.squaredDistanceTo (mergePoint1) > closeEnoughToMerge && intersection.squaredDistanceTo (mergePoint2) > closeEnoughToMerge) return false;

	//Ok...
	return true;
}

void Object::weldFaces (Face &face1, Face &face2) {
	//Build a plane for face1...
	Plane face1Plane (face1.points [0]->asPoint (), face1.points [1]->asPoint (), face1.points [2]->asPoint ());
	const long size1 = face1.points.size (); const long limit1 = size1 - 1; const long size2 = face2.points.size (); const long limit2 = size2 - 1;

	//Cycle through face1's edges in counter-clockwise order..
	for (long startIndex1 = limit1, endIndex1 = 0; endIndex1 < size1; startIndex1 = endIndex1, endIndex1++) {
		GamePoint &gameStart1 = *(face1.points [startIndex1]); GamePoint &gameEnd1 = *(face1.points [endIndex1]);
		Point start1 = gameStart1.asPoint (); Point end1 = gameEnd1.asPoint ();

		//Cycle through face2's edges in clockwise order... and also track the previous edge to intersect with face1's plane.
		for (long startIndex2 = 0, endIndex2 = limit2; endIndex2 >= 0; startIndex2 = endIndex2, endIndex2--) {
			GamePoint &gameStart2 = *(face2.points [startIndex2]); GamePoint &gameEnd2 = *(face2.points [endIndex2]);
			if (isWeldCandidate (gameStart1, gameEnd1, gameStart2, gameEnd2)) {
				Point start2 = gameStart2.asPoint (); Point end2 = gameEnd2.asPoint ();

				//The previous edge should hit face1's plane in the neighborhood of start1 and start2. 
				//Similarly, the next edge should hit face1's plane in the neighborhood of end1 and end2. 

				Point previous2 = face2.points [startIndex2 == limit2 ? 0 : startIndex2 + 1]->asPoint (); 
				Point next2 = face2.points [endIndex2 == 0 ? limit2 : endIndex2 - 1]->asPoint (); 

				static long count = 0; Point intersection;

				if (canWeldVertices (face1Plane, previous2, start2, start1, start2, intersection)) {
					//So this one works... Replace BOTH points by this intersection point...
					::log ("\n%d: %3.7f meters apart; Welding [%3.3f %3.3f %3.3f] and [%3.3f %3.3f %3.3f] as part of an edge of length %3.2f and moving them distances %3.4f and %3.4f.", 
						count++,
						start1.distanceTo (start2),
						start1.x, start1.y, start1.z, 
						start2.x, start2.y, start2.z, 
						start1.distanceTo (end1), 
						start1.distanceTo (intersection), start2.distanceTo (intersection));

					gameStart1.x = gameStart2.x = intersection.x; 
					gameStart1.y = gameStart2.y = intersection.y;
					gameStart1.z = gameStart2.z = intersection.z;
				}

				if (canWeldVertices (face1Plane, next2, end2, end1, end2, intersection)) {
					//So this one works... Replace BOTH points by this intersection point...
					::log ("\n%d: %3.7f meters apart; Welding [%3.3f %3.3f %3.3f] and [%3.3f %3.3f %3.3f] as part of an edge of length %3.2f and moving them distances %3.4f and %3.4f.", 
						count++,
						end1.distanceTo (end2),
						end1.x, end1.y, end1.z, 
						end2.x, end2.y, end2.z, 
						end1.distanceTo (end1), 
						end1.distanceTo (intersection), end2.distanceTo (intersection));

					gameEnd1.x = gameEnd2.x = intersection.x; 
					gameEnd1.y = gameEnd2.y = intersection.y;
					gameEnd1.z = gameEnd2.z = intersection.z;
				}
			}
		}
	}
}

void Object::weldAlmostEqualPoints () {
	loopVector (faceIndex1, faces)
		Face &face1 = *( faces [faceIndex1]);
		loopVector (faceIndex2, faces)
			if (faceIndex2 == faceIndex1) continue;
			Face &face2 = *(faces [faceIndex2]);
			weldFaces (face1, face2);
		endloop
	endloop
		
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.weldAlmostEqualPoints ();
	endloop
}

//Worldcraft (.map files) specific methods...
void Object::copyWorldcraftFaces (FaceCollection &faces) {
	//Copy into the the existing face collection.
	loopVector (faceIndex, faces)
		Face *oldFace = faces [faceIndex];
		//Make a new texture name.
		char *newTextureName = new char [strlen (oldFace->textureName) + 1];
		strcpy (newTextureName, oldFace->textureName);
		//Make a new face with new points, texture name (and the plane).
		Face *newFace = new Face (newTextureName);
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

//-----------------------------------------------------------------------------------------//
//                                        Face                                             //
//-----------------------------------------------------------------------------------------//

void Face::tick () {
	//This could change textures dynamically or change texture coordinates or do nothing.
}

void Face::draw () {
	//Draw this face...

	//Setup the texture (if any).
	if (texture == NULL) {
		glDisable (GL_TEXTURE_2D);
		glDisable (GL_BLEND);
	} else {
		texture->activate ();
		//Permit blending if it's a texture with alpha...
		if (texture->type == RGBAType) {
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable (GL_BLEND);
		} else {
			glDisable (GL_BLEND);
		}
	}

	//Draw the face polygon.
	glBegin (points.size () == 3 ? GL_TRIANGLES : GL_POLYGON);
		for (long pointIndex = 0; pointIndex < points.size (); pointIndex++) {
			GamePoint &point = *(points [pointIndex]);
			glNormal3d (point.nx, point.ny, point.nz);
			glTexCoord2d (point.tx, point.ty);
			glVertex3d (point.x, point.y, point.z); //Must be last.
		}
	glEnd ();
}

void Face::drawOutline () {
	//Draw this face in wireframe...

	glDisable (GL_TEXTURE_2D);
	glDisable (GL_BLEND);
	glColor4d (1.0, 0.0, 0.0, 1.0);
	glLineWidth (2.0); 

	//Draw the face polygon in wireframe.
	glBegin (GL_LINE_LOOP);
		for (long pointIndex = 0; pointIndex < points.size (); pointIndex++) {
			GamePoint &point = *(points [pointIndex]);
			glVertex3d (point.x, point.y, point.z);
		}
	glEnd ();

	glColor4d (1.0, 1.0, 1.0, 1.0);
	glLineWidth (1.0); 
	glEnable (GL_TEXTURE_2D);
}

void Face::computeBoundingBox (BoundingBox &box, Transformation &transformation) {
	for (long pointIndex = 0; pointIndex < points.size (); pointIndex++) {
		GamePoint &point = *(points [pointIndex]);
		box.add (*((Point *) &point.x) * transformation);
	}
}

void Face::load (::ifstream &input, UniversalObject *universalObject) {
	char line [256]; //Working variable...

	//Input the header.
	NEXT_HEADER
	GET_LAST_NUMBER long faceIndex = atoi (line); //Only useful for debugging or browsing.

	//Input the properties
	NEXT_HEADER
	GET_LAST_NUMBER long propertiesSize = atoi (line); CLEAR_THE_LINE
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		NEXT_STRING
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		properties.put (key, value);
	}

	//Input the texture.
	CString *textureName = properties.get ("texture");
	texture = textureName == NULL ? NULL : universalObject->textureFor ((char *) (LPCTSTR) *textureName);

	//Input the points.
	NEXT_HEADER
	GET_LAST_NUMBER long pointsSize = atoi (line); CLEAR_THE_LINE
	for (long pointIndex = 0; pointIndex < pointsSize; pointIndex++) {
		GamePoint *point = new GamePoint;
		GET_NEXT_NUMBER point->x = atof (line);
		GET_NEXT_NUMBER point->y = atof (line);
		GET_NEXT_NUMBER point->z = atof (line);
		GET_NEXT_NUMBER point->nx = atof (line);
		GET_NEXT_NUMBER point->ny = atof (line);
		GET_NEXT_NUMBER point->nz = atof (line);
		GET_NEXT_NUMBER point->tx = atof (line);
		GET_FINAL_NUMBER point->ty = atof (line);
		points.push_back (point);
	}
}

void Face::dump (long indent, ::ofstream &output, long faceIndex, vector<char*> &textures, vector<char*> &looks) {
	//Output a simple header..."
	output << "\n" << indentation (indent) << "Face: " << faceIndex << ";";

	//Output the properties...
	if (properties.get ("texture") == NULL && textureName != NULL) properties.put ("texture", textureName);
	output << "\n" << indentation (indent + 1) << "Properties: " << properties.size () << ";";
	loopPropertyDictionary (name, value, properties)
		output << "\n" << indentation (indent + 2) << "\"" << name << "\" => \"" << (LPCTSTR) *value << "\"";
	endloop

	//Output the points...
	output << "\n" << indentation (indent + 1) << "Points: " << points.size () << "; //x, y, z, nx, ny, nz, tx, ty";
	loopVector (pointIndex, points)
		GamePoint &point = *(points [pointIndex]);
		output << "\n" << indentation (indent + 2) << 
			point.x << ", " << point.y << ", " << point.z << ", " << 
			point.nx << ", " << point.ny << ", " << point.nz << ", " << 
			point.tx << ", " << point.ty << ";";
		static bool firstTime = false; //Set to true to see results of precision tests below...
		if (firstTime) {
			firstTime = false; 
			::log ("\nTEST PRECISION");
			::log ("\n1: %lf %lf %lf", point.x, point.y, point.z);
			::log ("\n2: %f %f %f", point.x, point.y, point.z);
			::log ("\n3: %2.8f %2.8f %2.8f", point.x, point.y, point.z);
			::log ("\n4: %e %e %e", point.x, point.y, point.z);
		}
	endloop
}

long Face::indexOf (vector<char*> &names, char *name) {
	//Returns a zero-based index if found; otherwise, -1.
//	loopVector (nameIndex, names)
	rawLoopVector (nameIndex, names)
		if (stricmp (name, names [nameIndex]) == 0) return nameIndex;
	endloop
	return -1;
}

void Face::addTextureNames (vector<char*> &textureNames) {
	if (textureName == NULL) return;

	//Add it if not there...
//	loopVector (textureIndex, textureNames)
	rawLoopVector (textureIndex, textureNames)
		if (stricmp (textureName, textureNames [textureIndex]) == 0) return;
	endloop
	textureNames.push_back (textureName);
}

void Face::addLookNames (vector<char*> &lookNames) {
return; //Don't have looks in this engine...
}

#undef CLEAR_THE_LINE 
#undef GET_NEXT_NUMBER 
#undef GET_LAST_NUMBER 
#undef GET_FINAL_NUMBER
#undef NEXT_STRING 
#undef NEXT_HEADER
