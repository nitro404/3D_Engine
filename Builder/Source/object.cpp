
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                      Object                                             //
//*****************************************************************************************//

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

void Object::import (ifstream &input, World * world) {
	char line [256]; //Working variable...

	//Input the header.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long objectIndex = atoi (line); //Only useful for debugging or browsing.

	//Input the transformation.
	SKIP_TO_COLON; CLEAR_THE_LINE;

	//The standard transformation matrix elements...
	Transformation &normal = transformation.normal ();
	SKIP_TO_COMMA; normal.m11 = atof (line);
	SKIP_TO_COMMA; normal.m12 = atof (line);
	SKIP_TO_COMMA; normal.m13 = atof (line);
	SKIP_TO_COMMA; normal.m14 = atof (line);
	
	SKIP_TO_COMMA; normal.m21 = atof (line);
	SKIP_TO_COMMA; normal.m22 = atof (line);
	SKIP_TO_COMMA; normal.m23 = atof (line);
	SKIP_TO_COMMA; normal.m24 = atof (line);
	
	SKIP_TO_COMMA; normal.m31 = atof (line);
	SKIP_TO_COMMA; normal.m32 = atof (line);
	SKIP_TO_COMMA; normal.m33 = atof (line);
	SKIP_TO_COMMA; normal.m34 = atof (line);
	
	SKIP_TO_COMMA; normal.m41 = atof (line);
	SKIP_TO_COMMA; normal.m42 = atof (line);
	SKIP_TO_COMMA; normal.m43 = atof (line);
	SKIP_TO_SEMICOLON; normal.m44 = atof (line);
	CLEAR_THE_LINE;

	//The inverse transformation matrix elements...
	Transformation &inverse = transformation.inverse;
	SKIP_TO_COMMA; inverse.m11 = atof (line);
	SKIP_TO_COMMA; inverse.m12 = atof (line);
	SKIP_TO_COMMA; inverse.m13 = atof (line);
	SKIP_TO_COMMA; inverse.m14 = atof (line);
	
	SKIP_TO_COMMA; inverse.m21 = atof (line);
	SKIP_TO_COMMA; inverse.m22 = atof (line);
	SKIP_TO_COMMA; inverse.m23 = atof (line);
	SKIP_TO_COMMA; inverse.m24 = atof (line);
	
	SKIP_TO_COMMA; inverse.m31 = atof (line);
	SKIP_TO_COMMA; inverse.m32 = atof (line);
	SKIP_TO_COMMA; inverse.m33 = atof (line);
	SKIP_TO_COMMA; inverse.m34 = atof (line);
	
	SKIP_TO_COMMA; inverse.m41 = atof (line);
	SKIP_TO_COMMA; inverse.m42 = atof (line);
	SKIP_TO_COMMA; inverse.m43 = atof (line);
	SKIP_TO_SEMICOLON; inverse.m44 = atof (line);
	CLEAR_THE_LINE;

	//Input the properties
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long propertiesSize = atoi (line); CLEAR_THE_LINE;
	for (long propertiesIndex = 0; propertiesIndex < propertiesSize; propertiesIndex++) {
		SKIP_TO_ENDLINE;
		char key [256]; char value [256]; value [0] = '\0';
		sscanf (line, " \"%[^\"]\" => \"%[^\"]\"", key, value);
		convertToLowercase (key);
		char *string = new char [strlen (value) + 1]; strcpy (string, value);
		//properties.put (key, new CString (value));
		put (properties, key, string);
	}

	//Input the faces.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long facesSize = atoi (line);
	for (long faceIndex = 0; faceIndex < facesSize; faceIndex++) {
		Face *face = new Face;
		face->import (input, world);
		faces.push_back (face);
	}

	//Input the sub-objects.
	SKIP_TO_COLON;
	SKIP_TO_SEMICOLON; long subObjectsSize = atoi (line);
	if (subObjectsSize > 0) quit ("\nDid not expect to have to handle a sub-object...");

	//Because of the above, the following will not actually do anything...
	/*
	for (long subObjectIndex = 0; subObjectIndex < subObjectsSize; subObjectIndex++) {
		Object *object = new Object;
		object->import (input, object);
		objects.push_back (object);
	}
	*/
}

void Object::export (long indent, ofstream &output, long objectIndex, vector<char*> & textureNames) {
	boolean hideProperties = false;

	//Output a simple header..."
	output << "\n" << indentation (indent) << "Object: " << objectIndex << ";";
	
	//Output the object type
	loopDictionary (name, value, properties, char)
		if(strcmp(name, "type") == 0) {
			output << "\n" << indentation (indent + 1) << "\"" << name << "\" => \"" << value << "\"";
			if(strcmp(value, "static geometry") == 0) {
				hideProperties = true;
			}
		}
	endloop

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
	if(!hideProperties) {
		output << "\n" << indentation (indent + 1) << "Properties: " << (properties.size () - 1) << ";";
		loopDictionary (name, value, properties, char)
			if(strcmp(name, "type") != 0) {
				output << "\n" << indentation (indent + 2) << "\"" << name << "\" => \"" << value << "\"";
			}
		endloop
	}
	
	//Output the faces...
	output << "\n" << indentation (indent + 1) << "Faces: " << faces.size () << ";";
	loopVector (faceIndex, faces)
		faces [faceIndex]->export (indent + 1, output, faceIndex, textureNames);
	endloop

	//output the sub-objects.
	//Ah... But we don't have any... Take care at the other end 
	//NOT TO TRY TO READ IN SUB-OBJECTS...	
	/*
	output << "\n" << indentation (indent + 1) << "Sub-objects: " << objects.size () << ";";
	loopVector (subObjectIndex, objects)
		Object &object = *(objects [subObjectIndex]);
		object.dump (indent + 1, output, subObjectIndex);
	endloop
	*/
}
