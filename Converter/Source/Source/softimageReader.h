
#ifndef softimageReaderModule
#define softimageReaderModule

//C:\Program Files\Softimage\XSIFTK_3.6.2\export\h\Core
#include "SemanticLayer.h"
//#include "../XSIFTK_3.6.2/export/h/FTK/SemanticLayer.h"
//#include "../XSIFTK_3.6.2/export/h/FTK/SemanticLayerWilfModified.h"
#include <string>

//WARNING: Could not complete it (don't know how to get the texture associated with a mesh)...

class SoftimageImporter {
    public:
        SoftimageImporter (CSLModel* root);
        virtual ~SoftimageImporter ();
        UniversalObject *load ();
		
    protected:
        void importModel (CSLModel* XSIModel);
        void importMesh (CSLMesh* XSIMesh);
		
        CSLModel* sceneRoot;
		UniversalObject *universalObject;
};

#endif //softimageReaderModule
