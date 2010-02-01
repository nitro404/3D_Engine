//*****************************************************************************************//
//                                        Object                                           //
//*****************************************************************************************//

#ifndef objectModule
#define objectModule 

class Object {
public:
	DualTransformation transformation;
	StringDictionary properties;
	FaceCollection faces;

	Object () {};
	~Object () {deleteFaceCollectionEntries (faces);}

	void log ();
	
	void import (ifstream &input, World * world);
	void export (long indent, ofstream &output, long objectIndex, vector<char*> & textureNames);
};

declareCollection (Object);

#endif //objectModule