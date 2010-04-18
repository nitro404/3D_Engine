#include "Shader.h"

Shader::Shader(const char * vertexShaderFileName, const char * fragmentShaderFileName, const char * shaderDirectory) : vertexShaderHandle(0), fragmentShaderHandle(0) {
	if(vertexShaderFileName == NULL && fragmentShaderFileName == NULL) {
		quit("Must specify both the vertex and fragment shader.");
	}

	char input[256];

	string vertexShaderPath;
	string fragmentShaderPath;

	if(shaderDirectory != NULL) {
		vertexShaderPath.append(shaderDirectory);
		if(_stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("\\")) != 0 &&
		   _stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("/")) != 0) {
			vertexShaderPath.append("\\");
		}
	}

	vertexShaderPath.append(vertexShaderFileName);

	if(shaderDirectory != NULL) {
		fragmentShaderPath.append(shaderDirectory);
		if(_stricmp(fragmentShaderPath.substr(fragmentShaderPath.length() - 1, fragmentShaderPath.length()).c_str(), ("\\")) != 0 &&
		   _stricmp(fragmentShaderPath.substr(fragmentShaderPath.length() - 1, fragmentShaderPath.length()).c_str(), ("/")) != 0) {
			fragmentShaderPath.append("\\");
		}
	}

	fragmentShaderPath.append(fragmentShaderFileName);

	string vertexShaderData;
	string fragmentShaderData;

	ifstream vin(vertexShaderPath.c_str());
	if(!vin.is_open()) {
		quit("Unable to read vertex shader: \"%s\"", vertexShaderPath.c_str());
	}

	do {
		vin.getline(input, 256);
		if(input != NULL) {
			vertexShaderData.append(input);
		}
	} while(!vin.eof());

	if(vin.is_open()) { vin.close(); }

	if(vertexShaderData.length() == 0) {
		quit("Empty vertex shader: \"%s\".", vertexShaderPath.c_str());
	}

	ifstream fin(fragmentShaderPath.c_str());
	if(!fin.is_open()) {
		quit("Unable to read fragment shader: \"%s\"", fragmentShaderPath.c_str());
	}

	do {
		fin.getline(input, 256);
		if(input != NULL) {
			fragmentShaderData.append(input);
		}
	} while(!fin.eof());

	if(fin.is_open()) { fin.close(); }

	if(fragmentShaderData.length() == 0) {
		quit("Empty fragment shader: \"%s\".", fragmentShaderPath.c_str());
	}

	programHandle = glCreateProgramObjectARB();
	if(glGetError() != GL_NO_ERROR) {
		quit("Unable to acquire shader program handle.");
	}

	vertexShaderHandle = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	const char * vertexShaderTemp = vertexShaderData.c_str();
	glShaderSourceARB(vertexShaderHandle, 1, (const GLcharARB **) (&vertexShaderTemp), NULL);
	glCompileShader(vertexShaderHandle);
	if(glGetError() != GL_NO_ERROR) {
		quit("Error creating vertex shader.");
	}
	glAttachObjectARB(programHandle, vertexShaderHandle);
	
	fragmentShaderHandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
	const char * fragmentShaderTemp = fragmentShaderData.c_str();
	glShaderSourceARB(fragmentShaderHandle, 1, (const GLcharARB **) (&fragmentShaderTemp), NULL);
	glCompileShader(fragmentShaderHandle);
	if(glGetError() != GL_NO_ERROR) {
		quit("Error creating fragment shader.");
	}
	glAttachObjectARB(programHandle, fragmentShaderHandle);

	glLinkProgramARB(programHandle);
}

Shader::~Shader() { }

void Shader::activate() {
	glUseProgramObjectARB(programHandle);
}

void Shader::deactivate() {
	glUseProgramObjectARB(0);
}

Shader * Shader::import(ifstream & input, const char * shaderDirectory) {
	if(shaderDirectory == NULL) { return NULL; }

	char line[256];
	char key[256];
	char value[256];
	char * str;
	
	// input the header
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int currentIndex = atoi(line);
	input.getline(line, 256, '\n');

	// input the properties
	input.getline(line, 256, ':');
	input.getline(line, 256, ';');
	int numberOfProperties = atoi(line);
	input.getline(line, 256, '\n');
	char * vertexShaderFileName;
	char * fragmentShaderFileName;
	for(int propertyIndex=0;propertyIndex<numberOfProperties;propertyIndex++) {
		input.getline(line, 256, '\n');
		value[0] = '\0';
		sscanf_s(line, " \"%[^\"]\" => \"%[^\"]\"", key, 256, value, 256);
		str = new char[strlen(value) + 1];
		strcpy_s(str, strlen(value) + 1, value);
		
		// parse properties to local variables
		if(_stricmp(key, "vertex_shader") == 0) {
			vertexShaderFileName = str;
		}
		else if(_stricmp(key, "fragment_shader") == 0) {
			fragmentShaderFileName = str;
		}
		else {
			delete [] str;
		}
	}

	Shader * shader = new Shader(vertexShaderFileName, fragmentShaderFileName, shaderDirectory);

	delete [] vertexShaderFileName;
	delete [] fragmentShaderFileName;

	return shader;
}
