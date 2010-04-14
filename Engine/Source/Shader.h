#pragma once

#include "Includes.h"

class Shader {
public:
	Shader(const char * vertexShaderFileName, const char * fragmentShaderFileName, const char * shaderDirectory);
	~Shader();

	void activate();
	static void deactivate();

private:
	GLenum programHandle;
	GLenum vertexShaderHandle;
	GLenum fragmentShaderHandle;
};
