//*****************************************************************************************//
//                                        Extensions                                       //
//*****************************************************************************************//

#ifndef extensionsModule
#define extensionsModule 

//ARB vertex buffer objects
#include "ARBvertexBufferObject.h"
extern PFNGLBINDBUFFERARBPROC glBindBufferARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;
extern PFNGLGENBUFFERSARBPROC glGenBuffersARB;
extern PFNGLISBUFFERARBPROC glIsBufferARB;
extern PFNGLBUFFERDATAARBPROC glBufferDataARB;
extern PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;
extern PFNGLGETBUFFERSUBDATAARBPROC glGetBufferSubDataARB;
extern PFNGLMAPBUFFERARBPROC glMapBufferARB;
extern PFNGLUNMAPBUFFERARBPROC glUnmapBufferARB;
extern PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB;
extern PFNGLGETBUFFERPOINTERVARBPROC glGetBufferPointervARB;

void setupOpenGLExtensions ();

#if (USING_NEW_OPENGL)
	//Nothing more to do...
#else
	#define glGenBuffers glGenBuffersARB
	#define glDeleteBuffers glDeleteBuffersARB
	#define glBindBuffer glBindBufferARB
	#define GL_ARRAY_BUFFER GL_ARRAY_BUFFER_ARB
	#define GL_ELEMENT_ARRAY_BUFFER GL_ELEMENT_ARRAY_BUFFER_ARB
	#define GL_DYNAMIC_DRAW GL_DYNAMIC_DRAW_ARB
	#define GL_STATIC_DRAW GL_STATIC_DRAW_ARB
	#define glBufferData glBufferDataARB
	#define glBufferSubData glBufferSubDataARB
#endif

#endif //extensionsModule
