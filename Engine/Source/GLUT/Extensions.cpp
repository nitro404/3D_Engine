#include "Extensions.h"

//ARB vertex buffer objects
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLISBUFFERARBPROC glIsBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB = NULL;
PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB = NULL;
PFNGLMAPBUFFERARBPROC glMapBufferARB = NULL;
PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB = NULL;
PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB = NULL;
PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB = NULL;

bool isExtensionSupported (const char *allExtensions, const char *extension) {
	//Extension names should not have spaces.
	const char *where = strchr (extension, ' ');
	if (where || *extension == '\0') return false;

	//It takes a bit of care to be fool-proof about parsing the OpenGL extensions string. 
	//Don't be fooled by sub-strings, etc.
	const char *start = allExtensions;
	while(true) {
		where = strstr ((const char *) start, extension);
		if (!where)
			return false;
		const char *terminator = where + strlen (extension);
		if (where == start || *(where - 1) == ' ') {
			if (*terminator == ' ' || *terminator == '\0') {
				return true;
			}
		}
		start = terminator;
	}
}

void setupOpenGLExtensions () {
	static bool alreadyExecuted = false;
	if (alreadyExecuted) return;
	alreadyExecuted = true;

	const bool loggingExtensions = true; //Set to true to have extensions logged...
	char *extensions = (char *) glGetString (GL_EXTENSIONS);

	bool isARBVertexBufferObjectExtensionPresent = isExtensionSupported (extensions, "GL_ARB_vertex_buffer_object");
	//::log ("\nARB vertex buffer object extension %s supported", isARBVertexBufferObjectExtensionPresent ? "IS" : "IS NOT");
	if (isARBVertexBufferObjectExtensionPresent) {
		glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress ("glBindBufferARB");
		glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress ("glDeleteBuffersARB");
		glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress ("glGenBuffersARB");
		glIsBufferARB = (PFNGLISBUFFERARBPROC) wglGetProcAddress ("glIsBufferARB");
		glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress ("glBufferDataARB");
		glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) wglGetProcAddress ("glBufferSubDataARB");
		glGetBufferSubDataARB = (PFNGLGETBUFFERSUBDATAARBPROC) wglGetProcAddress ("glGetBufferSubDataARB");
		glMapBufferARB = (PFNGLMAPBUFFERARBPROC) wglGetProcAddress ("glMapBufferARB");
		glUnmapBufferARB = (PFNGLUNMAPBUFFERARBPROC) wglGetProcAddress ("glUnmapBufferARB");
		glGetBufferParameterivARB = (PFNGLGETBUFFERPARAMETERIVARBPROC) wglGetProcAddress ("glGetBufferParameterivARB");
		glGetBufferPointervARB = (PFNGLGETBUFFERPOINTERVARBPROC) wglGetProcAddress ("glGetBufferPointervARB");
	}
}
