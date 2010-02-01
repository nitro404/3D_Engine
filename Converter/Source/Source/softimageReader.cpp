
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                   SoftimageReader                                       //
//*****************************************************************************************//

SoftimageImporter::SoftimageImporter (CSLModel *root){
    this->sceneRoot = root;
}

SoftimageImporter::~SoftimageImporter () {
}

UniversalObject *SoftimageImporter::load () {
	universalObject = new UniversalObject;
    importModel (sceneRoot);
	return universalObject;
}

void SoftimageImporter::importModel (CSLModel* XSIModel) {
    if (XSIModel->GetPrimitiveType () == CSLTemplate::SI_MESH)
        importMesh ((CSLMesh *) XSIModel->Primitive ());
	
    CSLModel **childrenList = XSIModel->GetChildrenList ();

    for  (long childIndex = 0; childIndex < XSIModel->GetChildrenCount (); childIndex++) {
        importModel  (childrenList [childIndex]);
    }
}

void SoftimageImporter::importMesh (CSLMesh* XSIMesh) {
	Object *object = new Object (); 
	universalObject->objects.push_back (object);

    CSLTriangleList **trianglesLists = XSIMesh->TriangleLists ();
	long trianglesListsSize = XSIMesh->GetTriangleListCount ();

    //Assume only one triangle list for now...
    CSLTriangleList *triangles = trianglesLists [0]; 
	long trianglesSize = triangles->GetTriangleCount ();

    CSIBCVector3D *positions = XSIMesh->Shape ()->GetVertexListPtr (); 
	long positionsSize = XSIMesh->Shape ()->GetVertexCount ();

    CSIBCVector3D *normals = XSIMesh->Shape ()->GetNormalListPtr ();
	long normalsSize = XSIMesh->Shape ()->GetNormalCount ();
	
    //CSIBCVector4D *colors = XSIMesh->Shape ()->GetColorIndicesPtr ();
	long colorsSize = XSIMesh->Shape ()->GetColorCount ();
    bool hasVertexColors = colorsSize > 0; hasVertexColors = false; //DISABLE (WE DON'T HAVE COLOR)...

    //For now, assume only one set of UV's
    CSLShape_35 *shape = ((CSLShape_35 *) XSIMesh->Shape ());
    CSIBCVector2D* textureCoordinates = shape->UVCoordArrays () [0]->GetUVCoordListPtr ();
    long textureCoordinatesSize = shape->UVCoordArrays () [0]->GetUVCoordCount ();

	/*
	::log ("\nMaterial is \"%s\"...", triangles->GetMaterial ()->Type () == CSLTemplate::ETemplateType::SI_MATERIAL 
		? "CSLMaterial"
		: (triangles->GetMaterial ()->Type () == CSLTemplate::ETemplateType::XSI_MATERIAL ? "CSLXSIMaterial" : "UNKNOWN"));

	
	CSLXSIMaterialInfo* GetMaterialInfo();

	CSLMaterial *material = (CSLMaterial *) triangles->GetMaterial ();
	CSLTexture2D *texture = material->Texture2D ();
	SI_Char *imageName = texture->GetImageFileName ();
	char *textureName = imageName == NULL ? "dummy" : imageName;
	*/

	char *textureName = triangles->GetMaterial ()->Type () == CSLTemplate::ETemplateType::SI_MATERIAL 
		? (char *) ((CSLMaterial *) triangles->GetMaterial ())->Texture2D ()->GetImageFileName ()
		: "dummy"; //It's type will be CSLTemplate::ETemplateType::XSI_MATERIAL and the material can be case to CSLXSIMaterial
			//which contains lots of shaders... Don't know how to get the texture from these shaders...

    //Iterate through all the triangles... There will often be less positions than normals and UV's.
    for (long t = 0; t < trianglesSize; ++t) {
		Face *face = new Face (duplicate (shortName (textureName)));
		object->faces.push_back (face);

        for (int p = 0; p < 3; ++p) {
            CSIBCVector3D position = positions [triangles->GetVertexIndicesPtr () [t*3+p]];
            CSIBCVector3D normal = normals [triangles->GetNormalIndicesPtr () [t*3+p]];
            CSIBCVector2D textureCoordinate = textureCoordinates [triangles->GetUVIndicesPtr (0) [t*3+p]];
            //CSIBCVector4D color; if (hasVertexColors) color = triangles->GetColorIndicesPtr () [t*3+p];

			GamePoint *vertex = new GamePoint;
			face->points.push_back (vertex);

			vertex->x = position.GetX ();
			vertex->y = position.GetY ();
			vertex->z = position.GetZ ();

			vertex->nx = normal.GetX ();
			vertex->ny = normal.GetY ();
			vertex->nz = normal.GetZ ();

			vertex->tx = textureCoordinate.GetX ();
			vertex->ty = 1.0 - textureCoordinate.GetY ();
        }
    }
}	
