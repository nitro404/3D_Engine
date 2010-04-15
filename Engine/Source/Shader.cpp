#include "Shader.h"

Shader::Shader(const char * vertexShaderFileName, const char * fragmentShaderFileName, const char * shaderDirectory) : vertexShaderHandle(-1), fragmentShaderHandle(-1) {
	if(vertexShaderFileName == NULL && fragmentShaderFileName == NULL) {
		quit("Must specify at least one shader file.");
	}

	char input[256];

	string vertexShaderPath;
	string fragmentShaderPath;

	if(vertexShaderFileName != NULL) {
		if(shaderDirectory != NULL) {
			vertexShaderPath.append(shaderDirectory);
			if(_stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("\\")) != 0 &&
			   _stricmp(vertexShaderPath.substr(vertexShaderPath.length() - 1, vertexShaderPath.length()).c_str(), ("/")) != 0) {
				vertexShaderPath.append("\\");
			}
		}

		vertexShaderPath.append(vertexShaderFileName);
	}

	if(fragmentShaderFileName != NULL) {
		if(shaderDirectory != NULL) {
			fragmentShaderPath.append(shaderDirectory);
			if(_stricmp(fragmentShaderPath.substr(fragmentShaderPath.length() - 1, fragmentShaderPath.length()).c_str(), ("\\")) != 0 &&
			   _stricmp(fragmentShaderPath.substr(fragmentShaderPath.length() - 1, fragmentShaderPath.length()).c_str(), ("/")) != 0) {
				fragmentShaderPath.append("\\");
			}
		}

		fragmentShaderPath.append(fragmentShaderFileName);
	}

	string vertexShaderData;
	string fragmentShaderData;

	if(vertexShaderFileName != NULL) {
		ifstream vin(vertexShaderPath.c_str());
		if(vin.bad()) {
			quit("Unable to read shader: \"%s\"", vertexShaderPath.c_str());
		}

		do {
			vin.getline(input, 256);
			if(input != NULL) {
				vertexShaderData.append(input);
			}
		} while(!vin.eof());

		if(vin.is_open()) { vin.close(); }
	}

	if(fragmentShaderFileName != NULL) {
		ifstream fin(fragmentShaderPath.c_str());
		if(fin.bad()) {
			quit("Unable to read shader: \"%s\"", fragmentShaderPath.c_str());
		}

		do {
			fin.getline(input, 256);
			if(input != NULL) {
				fragmentShaderData.append(input);
			}
		} while(!fin.eof());

		if(fin.is_open()) { fin.close(); }
	}

	programHandle = glCreateProgramObjectARB();
	if(glGetError() != GL_NO_ERROR) {
		quit("Unable to acquire program object arb handle.");
	}

	if(vertexShaderData.length() > 0) {
		vertexShaderHandle = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		const char * vertexShaderTemp = vertexShaderData.c_str();
		glShaderSourceARB(vertexShaderHandle, 1, (const GLcharARB **) (&vertexShaderTemp), NULL);
		glCompileShader(vertexShaderHandle);
		glAttachObjectARB(programHandle, vertexShaderHandle);

		if(glGetError() != GL_NO_ERROR) {
			quit("Error creating vertex shader.");
		}
	}
	
	if(fragmentShaderData.length() > 0) {
		fragmentShaderHandle = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		const char * fragmentShaderTemp = fragmentShaderData.c_str();
		glShaderSourceARB(fragmentShaderHandle, 1, (const GLcharARB **) (&fragmentShaderTemp), NULL);
		glCompileShader(fragmentShaderHandle);
		glAttachObjectARB(programHandle, fragmentShaderHandle);

		if(glGetError() != GL_NO_ERROR) {
			quit("Error creating fragment shader.");
		}
	}

	glLinkProgramARB(programHandle);
}

Shader::~Shader() { }

void Shader::activate() {
	glUseProgramObjectARB(programHandle);
}

void Shader::deactivate() {
	glUseProgramObjectARB(0);
}