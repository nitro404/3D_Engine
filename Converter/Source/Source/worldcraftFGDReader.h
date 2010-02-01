
#ifndef worldcraftFGDReaderModule
#define worldcraftFGDReaderModule

#include <string>

declareCollection (char);
typedef charCollection StringCollection;

declareDictionary (char);
typedef charDictionary StringDictionary;

class Variable {
public:
	Variable () {}
	~Variable () {
		deletecharDictionaryEntries (choices);
	}

	CString name;
	CString description;
	CString type;
	CString defaultValue;
	StringDictionary choices;

	void log ();
};

declareCollection (Variable);
preDeclareDictionary (Type);

class Type {
public:
	Type () {};
	~Type () {
		deletecharCollectionEntries (baseTypes);
		deleteVariableCollectionEntries (variables);
	}

	CString name;
	CString worldcraftType;

	StringCollection baseTypes;
	VariableCollection variables;
	VariableCollection allVariables; //Don't delete entries (they are owned by variables in different types).

	void log ();

	void mapAllVariables (TypeDictionary &types, PropertyDictionary &properties); //Except origin, angles, and pitch.
	void absorbAllVariables (TypeDictionary &types, VariableCollection &absorbingCollection);

	CString asTypedPropertyValue (CString &type, CString &value);
};

declareDictionary (Type);







class WorldcraftFGDReader {
    public:
        WorldcraftFGDReader (char *filename);
        virtual ~WorldcraftFGDReader () {deleteTypeDictionaryEntries (types);}
		
	private:	
		::ifstream file;
	public:
		TypeDictionary types;
    protected:

		void skipNonCommentSeparators ();
		void skipSeparators ();
		char *upTo (char character);
		char *nextLine ();
		char *nextString ();
		char *nextIdentifier ();
		char *nextWord ();
		char *copy (CString &string);
		char *copy (char *string);
		char *readType ();
		void readChoices (StringDictionary &dictionary, CString &defaultValue);
		void readClass ();
		void readClassBody (VariableCollection &variables);
		Variable *readVariable ();

	public:
		void log ();

		void mapAllVariables (PropertyDictionary &properties) {//Except origin, angles, and pitch.
printf("THIS IS LOCATED IN worldcraftFGDReader.h line 93\n");
			CString *typeName = properties.get ("type");
			if (typeName == NULL) return;
printf("typeName=\"%s\"", (char *) (LPCTSTR) *typeName); fflush(stdout);
			Type *type = get (types, (char *) (LPCTSTR) *typeName);
printf("Maybe not...\n"); fflush(stdout);
			if (type == NULL) return;
			type->mapAllVariables (types, properties); //Except origin, angles, and pitch.
		}

		void absorbAllVariables () {
			loopDictionary (key, type, Type, types)
				type->absorbAllVariables (types, type->allVariables);
			endloop
		}
};

#endif //worldcraftFGDReaderModule
