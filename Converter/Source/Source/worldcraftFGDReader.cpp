
//*****************************************************************************************//
//                                      Includes                                           //
//*****************************************************************************************//

#include "includes.all"

//*****************************************************************************************//
//                                 WorldcraftFGDReader                                     //
//*****************************************************************************************//

const bool loggingClassesAsBeingRead = false;
const bool loggingResult = false;

WorldcraftFGDReader::WorldcraftFGDReader (char *filename) {
	::log ("\nRead worldcraft FGD file \"%s\".", filename); //Remove if you wish (it's just for debugging).
	file.open (filename, ios::in); 
	if (file.fail ()) {halt ("\nFile \"%s\" could not be opened.", filename);}
	::log (" SUCCESSFUL"); //Remove if you wish (it's just for debugging).

	skipSeparators ();
	while (file.peek () != EOF) {
		readClass ();
		skipSeparators ();
	}
	file.close ();

	if (loggingResult) log ();
}

void WorldcraftFGDReader::skipNonCommentSeparators () {
	//... but not // comments like the next function...
	char character; 
	while ((character = file.peek ()) != EOF) {
		choose
			when (strrchr (" \t\n",character) != NULL) 
				file.get (); //It's whitespace... discard...
			otherwise
				return; //It's a non-separator...
		endchoose
	}
}

void WorldcraftFGDReader::skipSeparators () {
	//... and comments that start with //...
	char character; 
	while ((character = file.peek ()) != EOF) {
		choose
			when (character == '/') 
				file.get (); character = file.peek ();
				if (character == '/') {
					//It's a comment...
					const long garbageSize = 1000; char garbage [garbageSize];
					file.getline (garbage, garbageSize);
				} else {
					//It's not, back out of it...
					file.putback (character);
				}
			when (strrchr (" \t\n",character) != NULL) 
				file.get (); //It's whitespace... discard...
			otherwise
				return; //It's a non-separator...
		endchoose
	}
}

char *WorldcraftFGDReader::upTo (char character) {
	const long workAreaSize = 1000; static char workArea [workAreaSize];
	file.getline (workArea, workAreaSize, character);
	if (strlen (workArea) > workAreaSize - 10) 
		halt ("\nSpanned %d characters in order to find character '%c'...", workAreaSize, character);
	return workArea;
}

char *WorldcraftFGDReader::nextLine () {return upTo ('\n');}

char *WorldcraftFGDReader::nextString () {
	if (file.peek () != '"') halt ("Expected the double quote \"...");
	file.get ();
	const long workAreaSize = 1000; static char workArea [workAreaSize];
	file.getline (workArea, workAreaSize, '"');
	return workArea;
}

char *WorldcraftFGDReader::nextIdentifier () {
	//We allow - and _ as identifier characters... We also convert everything to lowercase.
	//Oh, and we also allow identifier to be just digits....

	const long workAreaSize = 1000; static char workArea [workAreaSize]; long size = 0; 

	const bool allowNumericsAsIdentifier = true;

	if (!allowNumericsAsIdentifier) {
		//Deal with the first character...
		char *leader = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";
		if (strrchr (leader, file.peek ()) == NULL) halt ("Expected an identifier, not \"%s\"...", nextLine ());
		workArea [size++] = tolower (file.get ());
	}

	//Deal with the others...
	char *trailer = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-0123456789";
	char character;
	while ((character = file.peek ()) != EOF && strrchr (trailer, character) != NULL) {
		workArea [size++] = tolower (file.get ());
	}
	workArea [size++] = '\0';
	return workArea;
}

char *WorldcraftFGDReader::nextWord () {
	//... or string. A quoted string counts as a word.
	skipSeparators ();
	return file.peek () == '"' ? nextString () : nextIdentifier ();
}

char *WorldcraftFGDReader::copy (CString &string) {
	long size = string.GetLength () + 1;
	char *copy = new char [size];
	CopyMemory (copy, (LPCTSTR) string, size);
	return copy;
}

char *WorldcraftFGDReader::copy (char *string) {
	long size = strlen (string) + 1;
	char *copy = new char [size];
	CopyMemory (copy, string, size);
	return copy;
}

char *WorldcraftFGDReader::readType () {
	//Look for a commented type such as "Integer" or "Enumeration" either immediately after an initial 
	//value such as "10" or 0 below or after an =. Two examples are shown below:
		//numberofarcs(string) : "Number of arcs" : "10" //Integer
		//startthickness(choices) : "Thickness at source" : 0 = //Enumeration

	//Remember where we were since we undo the read when done...
	::streampos previousPosition = file.tellg (); 

	char character; 
	while ((character = file.peek ()) != EOF) {
		choose 
			when (character == '/') 
				char *type = "string";
				file.get (); //Discard first /.
				character = file.peek ();
				
				if (character == '/') {
					//It's a comment...
					file.get (); //Discard second /.
					type = nextWord ();
				} 
				//Otherwise, it's not, but quit anyway since we won't ever find one...
				file.seekg (previousPosition);
				return type;
			when (strrchr (" \t\n",character) != NULL)
				file.get (); //Discard and continue...
			when (character == '=') 
				file.get (); //Discard and continue...
			otherwise
				file.seekg (previousPosition);
				return "string";
		endchoose
	}
	file.seekg (previousPosition);
	return "string";
}

void WorldcraftFGDReader::readChoices (StringDictionary &dictionary, CString &defaultValue) {
	//Each choice is of the form: Value : Description. We pick up each part as a string...

	char character;
	skipSeparators ();
	while ((character = file.peek ()) != EOF && character != ']') {
		CString value = nextWord ();
		upTo (':'); 
		CString description = nextWord ();
		put (dictionary, (char *) (LPCTSTR) value, copy (description)); //A copy is needed since descriptions are store as char * (NOT CString)...
		
		if (defaultValue == "") defaultValue = value;

		skipSeparators ();
	}

	file.get (); //Discard ].
}

void WorldcraftFGDReader::readClass () {
	//Process @
	if (file.peek () != '@') {
		halt ("Expected @ to start a class definition but instead found \"%s\"...", 
			nextLine ());
	}
	file.get (); //Discard @.

	Type *type = new Type;
	type->worldcraftType = nextWord ();

	skipSeparators ();
	char character;
	while ((character = file.peek ()) != EOF && character != '=') {
		CString property = nextWord ();
		if (property == "base") {
			skipSeparators ();
			if (file.peek () != '(') halt ("Expected parameters after \"%s\"...", (LPCTSTR) property);
			file.get (); //Discard (.
			skipSeparators ();
			while ((character = file.peek ()) != EOF && character != ')') {
				CString parameter = nextWord ();
				type->baseTypes.push_back (copy (parameter)); //A Copy since base type items are char * (not CString)...
				skipSeparators ();
				if (file.peek () == ',') {
					file.get (); //Discard ','.
					skipSeparators ();
				}
			}
			file.get (); //Discard ).
		} else {
			upTo (')'); //Discard entire description...
		}
		skipSeparators ();
	}
			
	file.get (); //Discard =.

	type->name = nextWord ();
	CString nameInformation = upTo ('['); //Discard...

	readClassBody (type->variables);

	if (loggingClassesAsBeingRead) {
		::log ("\n\nDEBUGGING: jUST READ A CLASS");
		type->log ();
	}

	put (types, (char *) (LPCTSTR) type->name, type);
}

void WorldcraftFGDReader::readClassBody (VariableCollection &variables) {
	skipSeparators ();
	char character;
	while ((character = file.peek ()) != EOF && character != ']') {
		variables.push_back (readVariable ());
		skipSeparators ();
	}
	file.get (); //Discard ].

	skipSeparators ();
}

Variable *WorldcraftFGDReader::readVariable () {
	//Variables are a bit complicated... just worked it out by looking at the FGD file...

	Variable *variable = new Variable;
	//Consider three examples.
		//lightradius(float) : "Radius" : "2.0" //Float
	//and 
		//stockobjectname(rhinomodel) : "Model" //NameOrNil
	//and
		//target1(string) : "Target1" //NameOrNil
	//and also 
		//kind(Choices) : "Style" : 5 = //Enumeration
		//[
		//    0 : "SlowWarningLight"
		//    1 : "FastWarningLight"
		//]
	variable->name = nextWord (); //i.e. lightradius OR kind
if (variable->name == "target4") {
	::log ("");
}

	CString worldcraftType = "";
	skipSeparators ();
	if (file.peek () == '(') {
		file.get (); //Discard (.
		worldcraftType = nextWord (); //i.e. float OR rhinomodel OR choices
		skipSeparators ();
		file.get (); //Discard ).
		skipSeparators ();
	}

	if (worldcraftType == "flags") {
		//We don't use flags at all... so skip it...
		if (file.peek () == '=') {
			upTo (']'); //Past [ and ]...
		}
	} else {
		upTo (':');
		variable->description = nextWord (); //i.e. Radius OR Model OR Style
		skipNonCommentSeparators ();
		if (file.peek () == '/') {
			//Note: readType below will back up to the starting point once done...
			variable->type = readType (); //i.e. Float OR Enumeration
			skipSeparators ();
		} else if (file.peek () == ':') {
			file.get (); //Discard :.
			skipSeparators ();
			variable->defaultValue = nextWord (); //i.e. 2.0 or 5
			//Note: readType below will skip = if necessary but backs up to the starting point once done...
			variable->type = readType (); //i.e. Float OR Enumeration
			skipSeparators ();
		}
		if (worldcraftType == "choices") {
			if (file.peek () == '=') {
				//Already have the type from above IF THERE WAS AN INITIAL ':' ABOVE. In
				//case that is not the case, we do it again...			
				variable->type = readType (); //i.e. Float OR Enumeration
				upTo ('[');
				readChoices (variable->choices, variable->defaultValue);
				skipSeparators ();
			}
		}
	}

	return variable;
}

void WorldcraftFGDReader::log () {
	::log ("\nWorldcraft FGD file summary:");
	loopDictionary (name, type, Type, types) 
		type->log ();
	endloop
}


void Type::log () {
	::log ("\n\nType \"%s\": \"%s\" {", (char *) (LPCTSTR) name, (char *) (LPCTSTR) worldcraftType);
	if (baseTypes.size () > 0) {
		::log ("\n\tbase types: ");
		loopVector (baseTypeIndex, baseTypes)
			char *baseType = baseTypes [baseTypeIndex];
			::log ("\"%s\" ", baseType);
		endloop
	}
	
	loopVector (variableIndex, variables)
		Variable *variable = variables [variableIndex];
		variable->log ();
	endloop

	::log ("\n}");
}


void Variable::log () {
	::log ("\n\t\"%s\" description \"%s\" type \"%s\" initial value \"%s\"", 
		(char *) (LPCTSTR) name, (char *) (LPCTSTR) description, 
		(char *) (LPCTSTR) type, (char *) (LPCTSTR) defaultValue);

	if (choices.size () > 0) {
		::log (" from\n\t\t[");
		loopDictionary (value, description, char, choices) 
			::log ("\n\t\t\t\"%s\" : \"%s\"", value, description);
		endloop
		::log ("\n\t\t]");
	}
}

void Type::mapAllVariables (TypeDictionary &types, PropertyDictionary &properties) {//Except origin, angles, and pitch.
	static long infiniteLoopCounter = 0;
	if (++infiniteLoopCounter > 30) {
		halt ("\nInfinite loop in base class \"%s\"...", this->name); 
		--infiniteLoopCounter; return;
	}

	//Map the subclass variables first... in case the class overrides it...
	loopVector (index, baseTypes)
		char *baseType = baseTypes [index];
		Type *type = get (types, baseType);
		if (type == NULL) continue;
		type->mapAllVariables (types, properties);
	endloop

	//Now deal with the local variables...
	loopVector (variableIndex, variables)
		Variable *variable = variables [variableIndex];
		char *name = (char *) (LPCTSTR) variable->name;

		if (strcmp (name, "origin") == 0) continue;
		if (strcmp (name, "angles") == 0) continue;
		if (strcmp (name, "pitch") == 0) continue;

		CString *property = properties.get (name); 
		if (property == NULL)
			properties.put (name, asTypedPropertyValue (variable->type, variable->defaultValue));
		else
			properties.put (name, asTypedPropertyValue (variable->type, *property));
	endloop
		
	--infiniteLoopCounter; 
}

void Type::absorbAllVariables (TypeDictionary &types, VariableCollection &absorbingCollection) {
	static long infiniteLoopCounter = 0;
	if (++infiniteLoopCounter > 30) {
		halt ("\nInfinite loop in base class \"%s\"...", this->name); 
		--infiniteLoopCounter; return;
	}

	//Map the local variables first... AND ensure the base class variables DO NOT overrides it...
	loopVector (variableIndex, variables)
		Variable *variable = variables [variableIndex];

		bool alreadyContained = false;
		loopVector (absorbingCollectionVariableIndex, absorbingCollection)
			Variable *absorbingCollectionVariable = variables [absorbingCollectionVariableIndex];
			if (variable->name == absorbingCollectionVariable->name) {
				alreadyContained = true;
				break;
			}
		endloop

		if (!alreadyContained) {
			absorbingCollection.push_back (variable);
		}
	endloop
		
	//Now deal with the local variables...
	loopVector (index, baseTypes)
		char *baseType = baseTypes [index];
		Type *type = get (types, baseType);
		if (type == NULL) continue;
		type->absorbAllVariables (types, absorbingCollection);
	endloop
		
	--infiniteLoopCounter; 
}

CString Type::asTypedPropertyValue (CString &type, CString &value) {
	if (type.GetLength () == 0 || type == "string") return value;
	CString typedString = "<";
	typedString += type;
	typedString += "> ";
	typedString += value;
	return typedString;
}
