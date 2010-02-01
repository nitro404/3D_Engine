/////////////////////////////////////////////////////////////////////////////////
// ModelPropertiesData declaration
/////////////////////////////////////////////////////////////////////////////////

#ifndef rhinoModelProperties
#define rhinoModelProperties

//#include "assert.h"

class ModelPropertiesData : public ON_UserData
{
	ON_OBJECT_DECLARE(ModelPropertiesData);

protected:
	void initialize () {
		nearRange = farRange = 0.0;
		lod = 0;
		useForCollision = makeUnitSize = centerAtOrigin = false;
		normalType = 0;
		collisionMaterialType = 0;
		sortType = 1;
	}
public:
	// Each kind of user data needs a UUID.
	// This uuid is different from the ON_Object uuid used in the
	// ON_OBJECT_IMPLEMENT macro.
	static GUID dataUUID; 

	// All constructors must initialize ON_UserData::m_userdata_uuid
	// to the UUID that identifies this kind of user data.
	ModelPropertiesData();
	ModelPropertiesData(const ModelPropertiesData&);

	virtual ModelPropertiesData& operator=(const ModelPropertiesData&);
	~ModelPropertiesData();


	// In order to get your user data to persist in files, you must override the following...
	BOOL Write(ON_BinaryArchive&) const;
	BOOL Read(ON_BinaryArchive&);
  
	// You must override ON_UserData::GetDescription().
	BOOL GetDescription( ON_wString& );

public:
	// TODO: Add other class information here
	double nearRange;
	double farRange;
	long lod;
	BOOL useForCollision;
	BOOL makeUnitSize, centerAtOrigin;

//	long newVersionIndicator;
//	ON_String attachmentName;

	long normalType; //0 = sharp, 1 = smooth, 2 = concave, 3 = convex, 4 = as-is
	long sortType; //0 == sort by face, 1 == sort by object
	long shaderType;
	ON_String effectName;

	long collisionMaterialType; 
	
	enum Constants {MAX_SHADER_NUM = 14, MAX_NORMAL_TYPE = 4};

	ON_String pointName;
	ON_String pointParentName;
};

#endif //rhinoModelProperties

//This is kind of strange -- but I do this so that the definition can be only in the header,
//which allows me to use the same file in other projects.
#ifdef ImplementingRhinoModelProperties

///////////////////////////////////////////////////////////////////////////////
//Model properties data implementation

ON_OBJECT_IMPLEMENT(ModelPropertiesData, ON_UserData, "E3ADE7DB-F5D5-4451-BF47-A9402E53856F");
// {E3ADE7DB-F5D5-4451-BF47-A9402E53856F}
GUID ModelPropertiesData::dataUUID = { 0xe3ade7db, 0xf5d5, 0x4451, { 0xbf, 0x47, 0xa9, 0x40, 0x2e, 0x53, 0x85, 0x6f } };


/////////////////////////////////////////////////////////////////////////////////
// Constructors
/////////////////////////////////////////////////////////////////////////////////
ModelPropertiesData::ModelPropertiesData()
{
	m_userdata_uuid = dataUUID;
	
	// If m_userdata_copycount is 0, user data is not copied when object is copied.
	// if > 0, user data is copied and m_copycount is incremented when parent object
	// is copied.  The user data's operator=() is used to copy.
	m_userdata_copycount = 1;

	initialize ();
}
/////////////////////////////////////////////////////////////////////////////////
ModelPropertiesData::ModelPropertiesData(const ModelPropertiesData& src) 
	: ON_UserData(src) // critical - be sure to call base class copy constructor
{
	m_userdata_copycount = 1;
	m_userdata_uuid = dataUUID;

	initialize ();
	this->operator= (src);
}
/////////////////////////////////////////////////////////////////////////////////
ModelPropertiesData& ModelPropertiesData::operator=(const ModelPropertiesData& src)
{
	if(this != &src)
	{
		ON_UserData::operator=(src); // be sure to call base class operator=
		//m_MyInteger = src.m_MyInteger;
		//m_MyString = src.m_MyString;
		nearRange = src.nearRange;
		farRange = src.farRange;
		lod = src.lod;
		useForCollision = src.useForCollision;
		centerAtOrigin = src.centerAtOrigin;
		makeUnitSize = src.makeUnitSize;
		shaderType = src.shaderType;
		collisionMaterialType = src.collisionMaterialType;
		pointName = src.pointName;
		pointParentName = src.pointParentName;
		effectName = src.effectName;
	}

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////
// Destructor
/////////////////////////////////////////////////////////////////////////////////
ModelPropertiesData::~ModelPropertiesData()
{
	// TODO: do something
}

/////////////////////////////////////////////////////////////////////////////////
// Read()
/////////////////////////////////////////////////////////////////////////////////
BOOL ModelPropertiesData::Read( ON_BinaryArchive& file )
{
	BOOL rc = TRUE;

	int major = -1, minor = -1;
	long newVersionIndicator;
	ON_String dummyString;
	//if (rc) rc = file.Read3dmChunkVersion (&major, &minor);
	//Rhino::Print ("version is %d.%d", major, minor);
	if (true || (major == 0 && minor == 0)) {
		if (rc) rc = file.ReadDouble (&nearRange);
		if (rc) rc = file.ReadDouble (&farRange);
		if (rc) rc = file.ReadLong (&lod);
		if (rc) rc = file.ReadInt (&useForCollision);
		if (rc) rc = file.ReadInt (&makeUnitSize);
		if (rc) rc = file.ReadInt (&centerAtOrigin);

		
		//This is ugly, but these are hold-overs from a different
		//version of the ModelPropertiesData.  I didn't include
		//a file version in the data I wrote out, so I have no
		//way of getting rid of these.  I've added a file-version
		//indicator in later versions (see below).
		if (rc) rc = file.ReadLong (&newVersionIndicator);
		if (rc) rc = file.ReadString (dummyString);

		if (rc) rc = file.ReadLong (&normalType);
		if (rc) rc = file.ReadLong (&shaderType);
	}

	//HACKHACKHACKHACKHACKHACKHACKHACKHACK
	//HACKHACKHACKHACKHACKHACKHACKHACKHACK
	//This is disgusting, but it's my own fault for forgetting to
	//put in a file version indicator.
	if (newVersionIndicator == 0xafafafaf) { //This is the new version!! (newVersionIndicator would be 0 otherwise)
		//This must be the new version file, so read the additional stuff in.
		if (rc) rc = file.Read3dmChunkVersion (&major, &minor);

		#ifdef FOR_RHINO_PLUGIN
		Rhino::Print ("\nReading a v%d.%d property set.", major, minor);
		#endif

		if (major == 0 && minor == 4) { //VERSION 0.4
			// - adds 'collisionMaterialType' before point properties
			if (rc) rc = file.ReadString (effectName);
			if (rc) rc = file.ReadLong (&collisionMaterialType);
			if (rc) rc = file.ReadString (pointName);
			if (rc) rc = file.ReadString (pointParentName);
			if (rc) rc = file.ReadLong (&sortType);
		}
		else if (major == 0 && minor == 3) { // VERSION 0.3
			// - adds 'collisionMaterialType' before point properties
			if (rc) rc = file.ReadString (effectName);
			if (rc) rc = file.ReadLong (&collisionMaterialType);
			if (rc) rc = file.ReadString (pointName);
			if (rc) rc = file.ReadString (pointParentName);
		} 
		else if (major == 0 && minor == 2) { //VERSION 0.2:
			
			// - adds 'effect name' for 'custom' shaders before point properties
			if (rc) rc = file.ReadString (effectName);
			if (rc) rc = file.ReadString (pointName);
			if (rc) rc = file.ReadString (pointParentName);
		} 
		else if (major == 0 && minor == 1) { //VERSION 0.1
			if (rc) rc = file.ReadString (pointName);
			if (rc) rc = file.ReadString (pointParentName);
//			Rhino::Print ("\nRead point name %s", pointName);
//			Rhino::Print ("\nRead point parent name %s", pointParentName);
		}
	} else {
		#ifdef FOR_RHINO_PLUGIN
		Rhino::Print ("\nReading a v%d.%d property set.", 0, 0);
		#endif FOR_RHINO_PLUGIN
	}
	return rc;
}

/////////////////////////////////////////////////////////////////////////////////
// Write()
/////////////////////////////////////////////////////////////////////////////////
BOOL ModelPropertiesData::Write(ON_BinaryArchive& file) const {
	BOOL rc = TRUE;
//	if (rc) rc = file.Write3dmChunkVersion (0, 1); //Version 0.1
	if (rc) rc = file.WriteDouble (nearRange);
	if (rc) rc = file.WriteDouble (farRange);
	if (rc) rc = file.WriteLong (lod);
	if (rc) rc = file.WriteInt (useForCollision);
	if (rc) rc = file.WriteInt (makeUnitSize);
	if (rc) rc = file.WriteInt (centerAtOrigin);

	//HACKHACKHACKHACKHACKHACKHACKHACKHACK
	//HACKHACKHACKHACKHACKHACKHACKHACKHACK
	//This is disgusting, but it's my own fault for forgetting to
	//put in a file version indicator.
	if (rc) rc = file.WriteLong ((long)0xafafafaf); //--> indicate this is a newer version (HIDEOUS!!)
	//HACKHACKHACKHACKHACKHACKHACKHACKHACK
	
	if (rc) rc = file.WriteString ("This is not being used");
	if (rc) rc = file.WriteLong (normalType);
	if (rc) rc = file.WriteLong (shaderType);

	//Start new version stuff here!!

	if (true) { // VERSION 0.4
		if (rc) rc = file.Write3dmChunkVersion (0, 4); //VERSION 0.4.
		if (rc) rc = file.WriteString (effectName);
		if (rc) rc = file.WriteLong (collisionMaterialType);
		if (rc) rc = file.WriteString (pointName);
		if (rc) rc = file.WriteString (pointParentName);
		if (rc) rc = file.WriteLong (sortType);
	}
	else if (false) { // VERSION 0.3
		if (rc) rc = file.Write3dmChunkVersion (0, 3); //VERSION 0.3.
		if (rc) rc = file.WriteString (effectName);
		if (rc) rc = file.WriteLong (collisionMaterialType);
		if (rc) rc = file.WriteString (pointName);
		if (rc) rc = file.WriteString (pointParentName);
	} else if (false) { //VERSION 0.2
		if (rc) rc = file.Write3dmChunkVersion (0, 2); //VERSION 0.2.
		if (rc) rc = file.WriteString (effectName);
		if (rc) rc = file.WriteString (pointName);
		if (rc) rc = file.WriteString (pointParentName);
	} 
	else if (false) {
		if (rc) rc = file.Write3dmChunkVersion (0, 1); //VERSION 0.1.
		if (rc) rc = file.WriteString (pointName);
		if (rc) rc = file.WriteString (pointParentName);
	}
	return rc;
}

/////////////////////////////////////////////////////////////////////////////////
// GetDescription()
/////////////////////////////////////////////////////////////////////////////////
BOOL ModelPropertiesData::GetDescription(ON_wString& s) {
	s = L"ModelPropertiesData";
	return TRUE;
}
#endif //ImplementingRhinoModelProperties