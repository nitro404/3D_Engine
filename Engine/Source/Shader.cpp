#include "Shader.h"

Shader::Shader(const char * vertexShaderFileName, const char * fragmentShaderFileName, const char * shaderDirectory) : vertexShaderHandle(0), fragmentShaderHandle(0) {
	if(!GLEW_VERSION_2_0) { return; }

	if(vertexShaderFileName == NULL && fragmentShaderFileName == NULL) {
		quit("Must specify both the vertex and fragment shader.");
	}

	char input[256];

	string vertexShaderPath;
	string fragmentShaderPath;

	// generate the vertex shader path
	if(shaderDirectory != NULL) {
		vertexShaderPath.append(shaderDirectory);
		if(_stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("\\")) != 0 &&
		   _stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("/")) != 0) {
			vertexShaderPath.append("\\");
		}
	}

	vertexShaderPath.append(vertexShaderFileName);

	// generate the fragment shader path
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

	// read the vertex shader file
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

	// read the fragment shader file
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

	// create the shader program
	programHandle = glCreateProgram();

	// create the shaders
	vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

	// compile the vertex shader
	const char * vertexShaderTemp = vertexShaderData.c_str();
	glShaderSource(vertexShaderHandle, 1, (const GLchar **) (&vertexShaderTemp), NULL);
	compileShader(vertexShaderHandle, vertexShaderFileName);
	if(glGetError() != GL_NO_ERROR) {
		quit("Error creating vertex shader.");
	}
	
	// compile the fragment shader
	const char * fragmentShaderTemp = fragmentShaderData.c_str();
	glShaderSource(fragmentShaderHandle, 1, (const GLchar **) (&fragmentShaderTemp), NULL);
	compileShader(fragmentShaderHandle, fragmentShaderFileName);
	if(glGetError() != GL_NO_ERROR) {
		quit("Error creating fragment shader.");
	}

	// attach the shaders to the program
	glAttachShader(programHandle, vertexShaderHandle);
	glAttachShader(programHandle, fragmentShaderHandle);

	// link the program
	glLinkProgram(programHandle);
}

Shader::~Shader() { }

void Shader::activate() {
	if(!GLEW_VERSION_2_0) { return; }

	glUseProgram(programHandle);
}

void Shader::deactivate() {
	if(!GLEW_VERSION_2_0) { return; }

	glUseProgram(0);
}

/*
void sendUniform(const char * name, const int id) {
	GLuint location = getUniformLocation(name);
	glUniform1i(location, id);
}

void sendUniform4x4(const char * name, const float * matrix, bool transpose = false) {
	GLuint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
}

void sendUniform3x3(const char * name, const Transformation * matrix, bool transpose = false) {
	GLuint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void sendUniform(const char * name, const Colour * colour) {
	GLuint location = getUniformLocation(name);
	glUniform4f(location, colour->red, colour->green, colour->blue, colour->alpha);
}

void sendUniform(const char * name, const float x, const float y, const float z) {
	GLuint location = getUniformLocation(name);
	glUniform3f(location, x, y, z);
}

void sendUniform(const char * name, const float scalar) {
	GLuint location = getUniformLocation(name);
	glUniform1f(location, scalar);
}

void bindAttrib(unsigned int index, const char * attribName) {
	glBindAttribLocation(m_programID, index, attribName);
}
*/

bool Shader::compileShader(GLenum shader, const char * shaderName) {
	glCompileShader(shader);
	GLint result = 0xDEADBEEF;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	// get shader debugging information
	if(!result) {
		printf("Could not compile shader %d", shader);
		if(shaderName != NULL) {
			printf(": \"%s\"", shaderName);
		}
		printf("\n");
		outputShaderLog(shader);
		return false;
	}

	return true;
}

void Shader::outputShaderLog(unsigned int shaderID) {
	vector<char> infoLog;
	GLint infoLen;
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
	infoLog.resize(infoLen);

	cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
	glGetShaderInfoLog(shaderID, infoLog.size(), &infoLen, &infoLog[0]);

	cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
#ifdef _WIN32
//	MessageBox(NULL, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
#endif
}

Shader * Shader::import(ifstream & input, const char * shaderDirectory) {
	if(shaderDirectory == NULL || !GLEW_VERSION_2_0) { return NULL; }

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
