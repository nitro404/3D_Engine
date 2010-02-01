
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

//#include "includes.all"

//*****************************************************************************************//
//                                     RhinoReader                                         //
//*****************************************************************************************//

//#include "stdafx.h"
#include "rhinoReader.h"
#include "..\OpenNURBS\opennurbs.h"
#include <assert.h>

//#include "math.hpp"
#include "logging.h"
#include "points.h"

#include <fstream.h>
#include <crtdbg.h>

struct OpenNurbsMesh {
	ON_Mesh mesh;
	ON_3dmObjectAttributes attributes;
};

struct LoaderInfo {
	int version;
	int fileSize;
	ON_String startSectionComments;
	ON_3dmProperties properties;
	ON_3dmSettings settings;

	ON_SimpleArray <ON_Bitmap *> bitmaps;
	ON_ClassArray <ON_Material> materials;
	ON_ClassArray <ON_Layer> layers;
	ON_ClassArray <ON_Group> groups;
	ON_ClassArray <OpenNurbsMesh> meshes;
	ON_SimpleArray <ON_3dPoint> points;
};

RhinoMeshSection::RhinoMeshSection () : vertices (NULL), indices (NULL), vertexCount (0), indexCount (0) {
	textureName [0] = '\0';
	properties = NULL;
}
RhinoMeshSection::~RhinoMeshSection ()  {
	delete [] vertices; 
	delete [] indices; 
	vertices = NULL; indices = NULL; 
}

RhinoMesh::~RhinoMesh ()  {
	delete [] sections;	
	delete [] points;
	sections = NULL; points = NULL;
	name [0] = '\0';
}

RhinoMesh::RhinoMesh () : sections (NULL), sectionsCount (0), points (NULL), pointsCount (0) {
	name [0] = '\0';
}

RhinoLoader::RhinoLoader() {
	if (false) {
		 int debugFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
		debugFlag = _CRTDBG_ALLOC_MEM_DF;
		//debugFlag |= _CRTDBG_CHECK_ALWAYS_DF; //Check memory on each (de)allocation.
		debugFlag |= _CRTDBG_CHECK_CRT_DF;
		//debugFlag |= _CRTDBG_DELAY_FREE_MEM_DF; //Don't release freed memory.
		debugFlag |= _CRTDBG_LEAK_CHECK_DF; //Check for memory leaks when exiting.
		_CrtSetDbgFlag (debugFlag);
	}

	loaderInfo = new LoaderInfo;
}

RhinoLoader::~RhinoLoader () {
	delete loaderInfo; loaderInfo = NULL;
}

#define ImplementingRhinoModelProperties
#include "rhinoModelProperties.h"
#undef ImplementingRhinoModelProperties

RhinoMesh *RhinoLoader::loadMesh (const char *filename) {
	if (!readFile (filename)) return NULL;
	return createMesh ();
}

RhinoMesh *RhinoLoader::createMesh () {
	RhinoMesh *mesh = new RhinoMesh;
	if (!buildMesh (mesh)) {delete mesh; mesh = NULL;}
	return mesh;
}

bool RhinoLoader::readFile (const char *filename) {
	bool returnCode = false;
	ON_3dmObjectAttributes objectAttributes;

	// If you are using OpenNURBS as a Windows DLL, then you MUST use
	// ON::OpenFile () to open the file.  If you are not using OpenNURBS
	// as a Windows DLL, then you may use either ON::OpenFile () or fopen ()
	// to open the file.
	FILE *fp = ON::OpenFile (filename, "rb "); if(fp == NULL) return false;
	ON_BinaryFile file (ON::read3dm, fp);

	for (;;) {
		// STEP 1: REQUIRED - Read start section
		if (!file.Read3dmStartSection (&loaderInfo->version, loaderInfo->startSectionComments)) break;

		// STEP 2: REQUIRED - Read properties section
		if (!file.Read3dmProperties (loaderInfo->properties)) break;

		// STEP 3: REQUIRED - Read properties section
		if (!file.Read3dmSettings (loaderInfo->settings)) break;

		// STEP 4: REQUIRED - Read embedded bitmap table
		if (!file.BeginRead3dmBitmapTable ()) break;
			for (;;) {
				ON_Bitmap *bitmap;
				int rc = file.Read3dmBitmap (&bitmap);
				if (rc == 0) break; // end of bitmap table
				if (rc < 0) break; // error reading table
				loaderInfo->bitmaps.Append (bitmap);
			}
			if (!file.EndRead3dmBitmapTable ()) break;

		// STEP 5: REQUIRED - Read render material table
		if (!file.BeginRead3dmMaterialTable ()) break;
			for (;;) {
				ON_Material *material = NULL;
				int rc = file.Read3dmMaterial (&material);
				if (rc == 0) break; // end of material table
				if (rc < 0) break; // error reading table
				loaderInfo->materials.Append (*material);
			}
			if (!file.EndRead3dmMaterialTable ()) break;

		// STEP 6: REQUIRED - Read layer table
		if (!file.BeginRead3dmLayerTable ()) break;
			for (;;) {
				ON_Layer *layer = NULL;
				int rc = file.Read3dmLayer (&layer);
				if (rc == 0) break; // end of layer table
				if (rc < 0) break; // error reading table
				loaderInfo->layers.Append (*layer);
			}
			if (!file.EndRead3dmLayerTable ()) break;

		// STEP 7: REQUIRED - Read group table
		if (!file.BeginRead3dmGroupTable ()) break;
			for (;;) {
				ON_Group *group = NULL;
				int rc = file.Read3dmGroup (&group);
				if (rc == 0) break; // end of group table
				if (rc < 0) break;
				loaderInfo->groups.Append (*group);
			}
			if (!file.EndRead3dmGroupTable ()) break;

		// STEP 8: REQUIRED - Read render lights table
		if (!file.BeginRead3dmLightTable ()) break;
			for (;;) {
				ON_Light *light = NULL;
				objectAttributes.Default ();
				int rc = file.Read3dmLight (&light, &objectAttributes);
				if (rc == 0) break; // end of light table
				if (rc < 0) break; // error reading table
				//OpenNurbsLight &OpenNurbsLight = lights.AppendNew ();
				//OpenNurbsLight.light (light);
				//OpenNurbsLight.attributes = objectAttributes;
			}
			if (!file.EndRead3dmLightTable ()) break;

		// STEP 9: REQUIRED - Read object (geometry and annotation) table
		if (!file.BeginRead3dmObjectTable ()) break;

			//Read all meshes.
			for (;;) {
				//ON_Object *object = new OpenNurbsObject;
				ON_Object *openNurbsObject = NULL;
				ON_3dmObjectAttributes attributes;

				//Only reading mesh objects (for now)
				int rc = file.Read3dmObject (&openNurbsObject, &attributes, ON::mesh_object | ON::point_object);
				if (rc == 0) break; // end of object table
				if (rc < 0) break; // error reading table
				if (rc == 2) continue; // object should be filtered out, as defined above
				if (rc == 3) continue; // object format is newer than the OpenNURBS dll can handle. time to upgrade!
				if (openNurbsObject == NULL) break;

				

				ON_Mesh *mesh = ON_Mesh::Cast (openNurbsObject);
				if (mesh != NULL) {
					//Clean up the mesh before we go any further
					mesh->ConvertQuadsToTriangles (); 
					mesh->CullDegenerateFaces ();
					mesh->ComputeFaceNormals (); 
					mesh->UnitizeFaceNormals ();
					mesh->ComputeVertexNormals (); 
					mesh->UnitizeVertexNormals ();

					OpenNurbsMesh meshObject;
					meshObject.attributes = attributes;
					meshObject.mesh = *mesh;
					loaderInfo->meshes.Append (meshObject); //Copy mesh into list
					continue;
				}
				ON_Point *point = ON_Point::Cast (openNurbsObject);
				if (point != NULL) {
					loaderInfo->points.Append (*point);
					continue;
				}
			}

			if (!file.EndRead3dmObjectTable ()) break;


		// STEP 10: REQUIRED - Read user tables
		// read user tables as anonymous tables
		// If you develop a plug-ins or application that uses OpenNURBS files,
		// you can store anything you want in a user table.
		for (;;) {
			ON_UUID uuid;
			
			if (!file.BeginRead3dmUserTable (uuid)) break; // no more user tables in the file
			ON_3dmGoo dummy;
			//OpenNurbsUserDefinedObject &userData = userDefinedObjects.AppendNew ();
			//userData.uuid = uuid;
			if (!file.Read3dmAnonymousUserTable (dummy)) break; //error reading table
			if (!file.EndRead3dmUserTable ()) break;
		}
		returnCode = true;
		break;
  }
  return returnCode;
}

char *asString (ON_String &name) {
	if (name.IsEmpty ()) return "";
	const char *constantName = name;
	return (char *) constantName;
}

bool RhinoLoader::buildMesh (RhinoMesh *mesh) {

	const bool tracing = true;
	if (tracing) {::log ("\nBegin buildMesh (RhinoMesh *mesh)");}

	//Initialize the mesh (number of sections)
	mesh->sectionsCount = loaderInfo->meshes.Count ();
	mesh->sections = new RhinoMeshSection [mesh->sectionsCount]; //'mesh' destructor will clean this up for us

	//Make each ON_Mesh a section of our mesh.
	if (tracing) {::log (" with %d sections...", loaderInfo->meshes.Count ());}
	//long finalSectionCount = 0;
	int currentSectionIndex = 0;
	for (int i = 0; i < loaderInfo->meshes.Count (); i++) {
		ON_Mesh &onMesh = loaderInfo->meshes [i].mesh;

		//const ON_Geometry *geometry = ON_Geometry::Cast (object);
		//if (geometry == NULL) return NULL;
		ModelPropertiesData *properties = ModelPropertiesData::Cast (onMesh.GetUserData (ModelPropertiesData::dataUUID));
		if (properties == NULL) {
			if (tracing) {::log ("\n\tTHERE ARE no PROPERTIES...");}
		} else {
			if (tracing) {
				ON_String meshName = loaderInfo->meshes [i].attributes.m_name;
				::log ("\n\tProperties for section %d mesh %s", i, meshName);
				::log ("\n\t\tLOD %d", properties->lod);
				::log ("\n\t\tUse for collision %s", properties->useForCollision ? "TRUE" : "FALSE");
				::log ("\n\t\tNormals %d", properties->normalType);
				::log ("\n\t\tMaterial %d", properties->collisionMaterialType);
				::log ("\n\t\tCenter at origin: %s", properties->centerAtOrigin ? "TRUE" : "FALSE");
				::log ("\n\t\tMake unit size: %s", properties->makeUnitSize ? "TRUE" : "FALSE");
			}
	//		if (properties->lod != 0) continue; //Don't use this mesh
		}

		if (currentSectionIndex == 0) {
			//Take the first ON_Mesh and use its name
			ON_String meshName = loaderInfo->meshes [currentSectionIndex].attributes.m_name;
			//bool isEmpty = meshName.IsEmpty ();
			//const char *constantEmpty = "";
			//char *empty = "";
			//const char *name = meshName.IsEmpty () ? empty : meshName;

			//char *test = asString (meshName);
			//assert (meshName.Length () < 255);
			strcpy (mesh->name, asString (meshName));
			mesh->name [meshName.Length ()] = '\0';
		}
		
		RhinoMeshSection &section = mesh->sections [currentSectionIndex];
		section.modelProperties (onMesh);
		section.vertexCount = onMesh.VertexCount ();
		section.vertices = new RhinoVertex [section.vertexCount];

		section.look [0] = '\0';
		if (properties != NULL && properties->effectName.Length () > 0) {
			strcpy (section.look, properties->effectName);
		} else {
			strcpy (section.look, "dummy");
		}
		if (tracing) {::log ("\n\tLook name \"%s\"...", section.look);}

		//Copy texture/material information
		int materialIndex = loaderInfo->meshes [currentSectionIndex].attributes.m_material_index;
		BYTE r, g, b, a;
		
		if (materialIndex < 0 || materialIndex >= loaderInfo->materials.Count ()) {
			section.textureName [0] = '\0';
			
			//section.textureName = NULL;
			r = g = b = a = 255;
			if (tracing) {::log ("\n\tTexture name \"%s\"...", section.textureName);}
		} else {
			//Copy the texture name (char at a time -- since Rhino uses wide char strings.
			ON_Material &material = loaderInfo->materials [materialIndex];
			const ON_wString &textureName = material.TextureBitmapFileName ();
			ON_String texName = textureName;
			long lastSlash = texName.ReverseFind ('\\');

			if (lastSlash >= 0) {
				texName = texName.Right (texName.Length () - (lastSlash + 1));
			}

			if (texName.Length () < 1) {
				section.textureName [0] = '\0';
			} else {
				assert (texName.Length () < 255);
				strcpy (section.textureName, texName);
				section.textureName [texName.Length ()] = '\0';
			}
			if (tracing) {::log ("\n\tTexture name \"%s\"... SUFFIX TO BE STRIPPED OUT", section.textureName);}

			r = material.Diffuse ().Red ();
			g = material.Diffuse ().Green ();
			b = material.Diffuse ().Blue ();
			a = material.Diffuse ().Alpha ();
		}

		//Copy vertices
		for (int j = 0; j < section.vertexCount; j++) {

			// OpenNURBS uses a right-handed Z-is-up coordinate system.
			// X'= X, Y' = Z, Z' = -Y converts it to our right-handed Y-is-up coordinate system.

			RhinoVertex &vertex = section.vertices [j];
			vertex.x = onMesh.m_V [j].x; vertex.y = onMesh.m_V [j].z; vertex.z = -onMesh.m_V [j].y;
			if (onMesh.m_N.Count () != 0) {
				vertex.nx = onMesh.m_N [j].x; vertex.ny = onMesh.m_N [j].z; vertex.nz = -onMesh.m_N [j].y;
			}
			if (onMesh.m_T.Count () != 0) {
				vertex.tx = onMesh.m_T [j].x; vertex.ty = onMesh.m_T [j].y;
			}
			vertex.r = r; vertex.g = g; vertex.b = b; vertex.a = a; 
		}

		//Copy indices
		section.indexCount = onMesh.TriangleCount () * 3;
		section.indices = new long [section.indexCount];
		long index = 0;
		for (j = 0; j < onMesh.FaceCount (); j++) {
			ON_MeshFace &face = onMesh.m_F [j];
			section.indices [index++] = face.vi [0];
			section.indices [index++] = face.vi [1];
			section.indices [index++] = face.vi [2];
		}

		//If we get here, we have added this section to the mesh.
		currentSectionIndex++;
	}

	//Copy over the connection points to the mesh.
	mesh->pointsCount = loaderInfo->points.Count ();
	mesh->points = new RhinoVertex [mesh->pointsCount];
	for (i = 0; i < mesh->pointsCount; i++) {
		// OpenNURBS uses a right-handed Z-is-up coordinate system.
		// X'= X, Y' = Z, Z' = -Y converts it to our right-handed Y-is-up coordinate system.
		mesh->points [i].x = loaderInfo->points [i].x;
		mesh->points [i].y = loaderInfo->points [i].z;
		mesh->points [i].z = -loaderInfo->points [i].y;
	}

	//Make sure it's pointing at the right number of sections now.
	mesh->sectionsCount = currentSectionIndex;
	if (tracing) {::log ("\nEnd buildMesh (RhinoMesh *mesh)");}
	return true;
}

//Extensions
void RhinoMeshSection::modelProperties (ON_Mesh &onMesh) {
	const ON_Geometry *geometry = ON_Geometry::Cast (&onMesh);
	if (geometry == NULL) {properties = NULL; return;}
	properties = ModelPropertiesData::Cast (geometry->GetUserData (ModelPropertiesData::dataUUID));
}


long RhinoMeshSection::lod () {
	if (properties == NULL) return 0;
	return properties->lod;
}

double RhinoMeshSection::nearDistance () {
	if (properties == NULL) return 0.0;
	return properties->nearRange;
}

double RhinoMeshSection::farDistance () { 
	if (properties == NULL) return 0.0;
	return properties->farRange;
}

bool RhinoMeshSection::useForCollision () {
	if (properties == NULL) return false;
	return properties->useForCollision == 1 ? true : false;
}

bool RhinoMeshSection::centerAtOrigin () {
	if (properties == NULL) return false;
	return properties->centerAtOrigin == 1 ? true : false;
}

bool RhinoMeshSection::makeUnitSize () {
	if (properties == NULL) return false;
	return properties->makeUnitSize == 1 ? true : false;
}

long RhinoMeshSection::normalType () {
	if (properties == NULL || properties->normalType < 0 || properties->normalType > ModelPropertiesData::MAX_NORMAL_TYPE) 
		//Erroneous value
		return 0; //Sharp normals
	return properties->normalType;
}

long RhinoMeshSection::shaderType () {
	if (properties == NULL || properties->shaderType < 0 ||	properties->shaderType > ModelPropertiesData::MAX_SHADER_NUM) 
		// Erroneous value
		return 0; //Normal shader
	return properties->shaderType;
}

long RhinoMeshSection::sortType () {
	if (properties == NULL) return 0; //Sort by face.
	if (properties->sortType < 0 || properties->sortType > 1) return 0; //sort by face.
	return properties->sortType;

}

long RhinoMeshSection::collisionMaterialType () {
	if (properties == NULL || properties->collisionMaterialType < 0) {
		return 0; //Generic material
	}
	return properties->collisionMaterialType;
}