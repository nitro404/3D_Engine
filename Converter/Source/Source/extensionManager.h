
//**************************************************************************************************//
//                                      OpenGLExtensionManager                                      //
//**************************************************************************************************//

#ifndef extensionManagerModule
#define extensionManagerModule 

#include "glati.h"

#define REMOVE(code) 

class OpenGLExtensionManager {
public:
	static void setup ();
	static void wrapup ();

	inline bool shadersSupported () {return isARBVertexProgramExtensionPresent && isARBFragmentProgramExtensionPresent;}

	bool isMultitexturingPresent;
	long numberOfTextureUnits;

	bool isCombinerExtensionPresent;
	bool isCrossbarExtensionPresent;
	bool isARBTextureEnvDot3Present;

	bool isNvidiaRegisterCombinerExtensionPresent;
	bool isNvidiaRegisterCombiner2ExtensionPresent;
	bool isATICombiner3ExtensionPresent;
	long numberOfCombiners;

	bool isATIFragmentShaderExtensionPresent;
	bool isARBVertexProgramExtensionPresent;
	bool isARBFragmentProgramExtensionPresent;
	bool isTextureCompressionExtensionPresent;
	bool isSwapIntervalExtensionPresent;
    bool isRenderingBufferSupported;
	bool isMimapGeneratingRenderingBufferSupported;
	bool isARBTextureBorderClampSupported;
	bool isEXTTextureEdgeClampSupported;
	bool isCompiledVertexArrayExtensionPresent;
	bool isDrawRangeElementsExtensionPresent;
	long maxDrawRangeVertices;
	long maxDrawRangeIndices;
	
	bool isATIVertexArrayObjectExtensionPresent;
	bool isNvidiaVertexArrayRangeExtensionPresent;
	bool isARBVertexBufferObjectExtensionPresent;
	bool isARBOcclusionQueryExtensionPresent;
	bool isEXTFrameBufferExtensionPresent;

	void findExtensions ();

protected:
	bool isExtensionSupported (const char *allExtensions, const char *extension);
};

extern OpenGLExtensionManager *openGLExtensionManager;

//Function pointers to all extension functions.
REMOVE (
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
extern PFNGLBLENDCOLORPROC glBlendColor;

extern PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
extern PFNGLMULTITEXCOORD2DARBPROC glMultiTexCoord2dARB;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
extern PFNGLMULTITEXCOORD2FVARBPROC glMultiTexCoord2fvARB;
extern PFNGLMULTITEXCOORD3DARBPROC glMultiTexCoord3dARB;
extern PFNGLMULTITEXCOORD3FARBPROC glMultiTexCoord3fARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
);

typedef BOOL (*intFunctionReturningBool) (int);
extern intFunctionReturningBool wglSwapIntervalEXT;

//NVidia Combiners.
REMOVE (
extern PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV;
extern PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV;
extern PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV;
extern PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV;
extern PFNGLCOMBINERINPUTNVPROC glCombinerInputNV;
extern PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV;
extern PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV;
extern PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV;
extern PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV;
extern PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV;
extern PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV;

extern PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV;
extern PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV;

//Texture compression
);
extern PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB;
REMOVE (

//ATI fragment shaders
extern PFNGLGENFRAGMENTSHADERSATIPROC   glGenFragmentShadersATI;
extern PFNGLBINDFRAGMENTSHADERATIPROC   glBindFragmentShaderATI;
extern PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI;
extern PFNGLBEGINFRAGMENTSHADERATIPROC  glBeginFragmentShaderATI;
extern PFNGLENDFRAGMENTSHADERATIPROC    glEndFragmentShaderATI;
extern PFNGLPASSTEXCOORDATIPROC         glPassTexCoordATI;
extern PFNGLSAMPLEMAPATIPROC            glSampleMapATI;
extern PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI;
extern PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI;
extern PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI;
extern PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI;
extern PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI;
extern PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI;
extern PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI;

//ARB vertex program extension
extern PFNGLVERTEXATTRIB1SARB glVertexAttrib1sARB;
extern PFNGLVERTEXATTRIB1FARB glVertexAttrib1fARB;
extern PFNGLVERTEXATTRIB1DARB glVertexAttrib1dARB;
extern PFNGLVERTEXATTRIB2SARB glVertexAttrib2sARB;
extern PFNGLVERTEXATTRIB2FARB glVertexAttrib2fARB;
extern PFNGLVERTEXATTRIB2DARB glVertexAttrib2dARB;
extern PFNGLVERTEXATTRIB3SARB glVertexAttrib3sARB;
extern PFNGLVERTEXATTRIB3FARB glVertexAttrib3fARB;
extern PFNGLVERTEXATTRIB3DARB glVertexAttrib3dARB;
extern PFNGLVERTEXATTRIB4SARB glVertexAttrib4sARB;
extern PFNGLVERTEXATTRIB4FARB glVertexAttrib4fARB;
extern PFNGLVERTEXATTRIB4DARB glVertexAttrib4dARB;
extern PFNGLVERTEXATTRIB4NUBARB glVertexAttrib4NubARB;

extern PFNGLVERTEXATTRIB1SVARB glVertexAttrib1svARB;
extern PFNGLVERTEXATTRIB1FVARB glVertexAttrib1fvARB;
extern PFNGLVERTEXATTRIB1DVARB glVertexAttrib1dvARB;
extern PFNGLVERTEXATTRIB2SVARB glVertexAttrib2svARB;
extern PFNGLVERTEXATTRIB2FVARB glVertexAttrib2fvARB;
extern PFNGLVERTEXATTRIB2DVARB glVertexAttrib2dvARB;
extern PFNGLVERTEXATTRIB3SVARB glVertexAttrib3svARB;
extern PFNGLVERTEXATTRIB3FVARB glVertexAttrib3fvARB;
extern PFNGLVERTEXATTRIB3DVARB glVertexAttrib3dvARB;
extern PFNGLVERTEXATTRIB4BVARB glVertexAttrib4bvARB;
extern PFNGLVERTEXATTRIB4SVARB glVertexAttrib4svARB;
extern PFNGLVERTEXATTRIB4IVARB glVertexAttrib4ivARB;
extern PFNGLVERTEXATTRIB4UBVARB glVertexAttrib4ubvARB;
extern PFNGLVERTEXATTRIB4USVARB glVertexAttrib4usvARB;
extern PFNGLVERTEXATTRIB4UIVARB glVertexAttrib4uivARB;
extern PFNGLVERTEXATTRIB4FVARB glVertexAttrib4fvARB;
extern PFNGLVERTEXATTRIB4DVARB glVertexAttrib4dvARB;
extern PFNGLVERTEXATTRIB4NBVARB glVertexAttrib4NbvARB;
extern PFNGLVERTEXATTRIB4NSVARB glVertexAttrib4NsvARB;
extern PFNGLVERTEXATTRIB4NIVARB glVertexAttrib4NivARB;
extern PFNGLVERTEXATTRIB4NUBVARB glVertexAttrib4NubvARB;
extern PFNGLVERTEXATTRIB4NUSVARB glVertexAttrib4NusvARB;
extern PFNGLVERTEXATTRIB4NUIVARB glVertexAttrib4NuivARB;

extern PFNGLVERTEXATTRIBPOINTERARB glVertexAttribPointerARB;
extern PFNGLENABLEVERTEXATTRIBARRAYARB glEnableVertexAttribArrayARB;
extern PFNGLDISABLEVERTEXATTRIBARRAYARB glDisableVertexAttribArrayARB;
extern PFNGLPROGRAMSTRINGARB glProgramStringARB;
extern PFNGLBINDPROGRAMARB glBindProgramARB;
extern PFNGLDELETEPROGRAMSARB glDeleteProgramsARB;
extern PFNGLGENPROGRAMSARB glGenProgramsARB;

extern PFNGLPROGRAMENVPARAMETER4DARB glProgramEnvParameter4dARB;
extern PFNGLPROGRAMENVPARAMETER4DVARB glProgramEnvParameter4dvARB;
extern PFNGLPROGRAMENVPARAMETER4FARB glProgramEnvParameter4fARB;
extern PFNGLPROGRAMENVPARAMETER4FVARB glProgramEnvParameter4fvARB;

extern PFNGLPROGRAMLOCALPARAMETER4DARB glProgramLocalParameter4dARB;
extern PFNGLPROGRAMLOCALPARAMETER4DVARB glProgramLocalParameter4dvARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARB glProgramLocalParameter4fARB;
extern PFNGLPROGRAMLOCALPARAMETER4FVARB glProgramLocalParameter4fvARB;

extern PFNGLGETPROGRAMENVPARAMETERDVARB glGetProgramEnvParameterdvARB;
extern PFNGLGETPROGRAMENVPARAMETERFVARB glGetProgramEnvParameterfvARB;

extern PFNGLGETPROGRAMLOCALPARAMETERDVARB glGetProgramLocalParameterdvARB;
extern PFNGLGETPROGRAMLOCALPARAMETERFVARB glGetProgramLocalParameterfvARB;

extern PFNGLGETPROGRAMIVARB glGetProgramivARB;
extern PFNGLGETPROGRAMSTRINGARB glGetProgramStringARB;
extern PFNGLGETVERTEXATTRIBDVARB glGetVertexAttribdvARB;
extern PFNGLGETVERTEXATTRIBFVARB glGetVertexAttribfvARB;
extern PFNGLGETVERTEXATTRIBIVARB glGetVertexAttribivARB;
extern PFNGLGETVERTEXATTRIBPOINTERVARB glGetVertexAttribPointervARB;
extern PFNGLISPROGRAMARB glIsProgramARB;

//PBuffer extension
extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB; 
extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB; 

extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
extern PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;
extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;

//Compiled vertex arrays
extern PFNGLLOCKARRAYSEXTPROC glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT;

//Draw Range Elements
extern PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT;

//ATI vertex array objects
extern PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI;
extern PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI;
extern PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI;
extern PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI;
extern PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI;
extern PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI;
extern PFNGLARRAYOBJECTATIPROC glArrayObjectATI;
extern PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI;
extern PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectivATI;

//NVIDIA vertex array range
extern PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV;
extern PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV;
extern PFNWGLALLOCATEMEMORYNVPROC wglAllocateMemoryNV;
extern PFNWGLFREEMEMORYNVPROC wglFreeMemoryNV;

//ARB vertex buffer objects
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

//ARB occlusion query
extern PFNGLGENQUERIESPROC glGenQueries;
extern PFNGLDELETEQUERIESPROC glDeleteQueries;
extern PFNGLISQUERYPROC glIsQuery;
extern PFNGLBEGINQUERYPROC glBeginQuery;
extern PFNGLENDQUERYPROC glEndQuery;
extern PFNGLGETQUERYIVPROC glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv;

//EXT frame buffer
extern PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT;
extern PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT;
);

#endif //extensionManagerModule