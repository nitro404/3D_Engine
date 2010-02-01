
//**************************************************************************************************//
//                                    OpenGLExtensionManager                                        //
//**************************************************************************************************//

#include "includes.all" 

//**************************************************************************************************//
//                                    OpenGLExtensionManager                                        //
//**************************************************************************************************//

OpenGLExtensionManager *openGLExtensionManager = NULL;

void OpenGLExtensionManager::setup () {
	openGLExtensionManager = new OpenGLExtensionManager;
	openGLExtensionManager->findExtensions ();

	::log ("\nGL_ARB_texture_compression %s supported...",
		openGLExtensionManager->isTextureCompressionExtensionPresent ? "IS" : "IS NOT");
}

void OpenGLExtensionManager::wrapup () {
	delete openGLExtensionManager;
}

bool OpenGLExtensionManager::isExtensionSupported (const char *allExtensions, const char *extension) {
	//Extension names should not have spaces.
	char *where = strchr (extension, ' ');
	if (where || *extension == '\0') return false;

	//It takes a bit of care to be fool-proof about parsing the OpenGL extensions string. 
	//Don't be fooled by sub-strings, etc.
	const char *start = allExtensions;
	for (;;) {
		where = strstr ((const char *) start, extension);
		if (!where)
			return false;
		char *terminator = where + strlen (extension);
		if (where == start || *(where - 1) == ' ') {
			if (*terminator == ' ' || *terminator == '\0') {
				return true;
			}
		}
		start = terminator;
	}
}

void OpenGLExtensionManager::findExtensions () {

	const bool loggingExtensions = true; //Set to true to have extensions logged...
	char *extensions = (char *) glGetString (GL_EXTENSIONS);

    isMultitexturingPresent = isExtensionSupported (extensions, "GL_ARB_multitexture");
	log ("\nMultitexturing %s present.", isMultitexturingPresent ? "IS" : "IS NOT");
	
	//if (isMultitexturingPresent) {
	//	GLint numTexUnits;
	//	glGetIntegerv (GL_MAX_TEXTURE_UNITS_ARB, &numTexUnits);
	//	::log ("\nThere are %d texture units.", numTexUnits);
	//}
	
REMOVE (
	int units; glGetIntegerv (GL_MAX_TEXTURE_UNITS_ARB, &units); numberOfTextureUnits = units;
	::log ("\nNumber of texture units is %d.", numberOfTextureUnits);
	
	int buffers; glGetIntegerv (GL_AUX_BUFFERS, &buffers);
	::log ("\nNumber of auxiliary buffers is %d.", buffers);
	//glColorMask (TRUE, FALSE, FALSE, TRUE);
	//if (isSwapIntervalExtensionPresent) {(*wglSwapIntervalEXT) (1);}
	//if (isSwapIntervalExtensionPresent) {(*wglSwapIntervalEXT) (2);}

	//#define wglProcedure PFNWGLGETEXTENSIONSSTRINGARBPROC
	//wglProcedure wglGetExtensionsStringARB = (wglProcedure) wglGetProcAddress ("wglGetExtensionsStringARB");
	//#undef wglProcedure 
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC) wglGetProcAddress ("wglGetExtensionsStringARB");
	char *wglExtensions = wglGetExtensionsStringARB == NULL ? "" : (char *) wglGetExtensionsStringARB (wglGetCurrentDC ());
);

	if (loggingExtensions) {
		log ("\nExtensions:\n\t"); 
		for (char *next = extensions; *next; next++) {
			if (*next == ' ') ::log ("\n\t"); else ::log ("%c", *next);
		}
		log ("\nWGL Extensions:\n\t"); 
		REMOVE (
		for (next = wglExtensions; *next; next++) {
			if (*next == ' ') ::log ("\n\t"); else ::log ("%c", *next);
		}
		);
	} else {
		::log ("\nExtensions: NOT CURRENTLY SHOWN");
	}

	static bool isImagingPresent = isExtensionSupported (extensions, "GL_ARB_imaging"); //Not yet used...
	::log ("\nGL_ARB_imaging is %sdefined", isImagingPresent ? "" : "NOT ");

	//FIXME ("FIX QUERYING FOR SUBTRACT BLEND FUNC");
	REMOVE (
	if (isImagingPresent) {
		glBlendEquation = (PFNGLBLENDEQUATIONPROC) wglGetProcAddress ("glBlendEquation");
		glBlendColor = (PFNGLBLENDCOLORPROC) wglGetProcAddress ("glBlendColor");
	}

    isSwapIntervalExtensionPresent = isExtensionSupported (extensions, "WGL_EXT_swap_control");
	log ("\nSwap extension %s present.", isSwapIntervalExtensionPresent ? "IS" : "IS NOT");

	glActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress ("glActiveTextureARB");

	glMultiTexCoord2dARB = (PFNGLMULTITEXCOORD2DARBPROC) wglGetProcAddress ("glMultiTexCoord2dARB");
    glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress ("glMultiTexCoord2fARB");
	glMultiTexCoord2fvARB = (PFNGLMULTITEXCOORD2FVARBPROC) wglGetProcAddress ("glMultiTexCoord2fvARB");
    glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress ("glMultiTexCoord3fARB");
	glMultiTexCoord3dARB = (PFNGLMULTITEXCOORD3DARBPROC) wglGetProcAddress ("glMultiTexCoord3dARB");

	glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress ("glClientActiveTextureARB");
	//wglGetSwapIntervalEXT = (voidFunctionReturningInt) wglGetProcAddress ("wglGetSwapIntervalEXT");
	wglSwapIntervalEXT = (intFunctionReturningBool) wglGetProcAddress ("wglSwapIntervalEXT");
	isSwapIntervalExtensionPresent = isSwapIntervalExtensionPresent && wglSwapIntervalEXT != NULL;
	log ("\nSwap control function %s present.", isSwapIntervalExtensionPresent ? "IS" : "IS NOT");

    isCombinerExtensionPresent = isExtensionSupported (extensions, "GL_ARB_texture_env_combine");
	log ("\nCombiners %s present.", isCombinerExtensionPresent ? "ARE" : "ARE NOT");
    isATICombiner3ExtensionPresent = isExtensionSupported (extensions, "GL_ATI_texture_env_combine3");
	log ("\nATI Combiner3 %s present.", isATICombiner3ExtensionPresent ? "IS" : "IS NOT");
);

	//STUPID NVIDIA - they don't bother reporting 'crossbar' support when 'combine4' is present.  However,
	//'combine4' is a superset of the 'crossbar' functionality.
	isCrossbarExtensionPresent = 
		isExtensionSupported (extensions, "GL_ARB_texture_env_crossbar") ||
		isExtensionSupported (extensions, "GL_NV_texture_env_combine4");
	::log ("\nCombiner crossbar %s present", isCrossbarExtensionPresent ? "IS" : "IS NOT");

REMOVE (
    bool isGL_ARB_texture_env_combine4Present = isExtensionSupported (wglExtensions, "GL_ARB_texture_env_combine4");
	::log ("\nGL_ARB Combine4 support %s present.", isGL_ARB_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isGL_NV_texture_env_combine4Present = isExtensionSupported (wglExtensions, "GL_NV_texture_env_combine4");
	::log ("\nGL_NV Combine4 support %s present.", isGL_NV_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isGL_EXT_texture_env_combine4Present = isExtensionSupported (wglExtensions, "GL_EXT_texture_env_combine4");
	::log ("\nGL_EXT Combine4 support %s present.", isGL_EXT_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isGL_ATI_texture_env_combine4Present = isExtensionSupported (wglExtensions, "GL_ATI_texture_env_combine4");
	::log ("\nGL_ATI Combine4 support %s present.", isGL_ATI_texture_env_combine4Present ? "IS" : "IS NOT");

    bool isARB_texture_env_combine4Present = isExtensionSupported (wglExtensions, "ARB_texture_env_combine4");
	::log ("\nARB Combine4 support %s present.", isARB_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isNV_texture_env_combine4Present = isExtensionSupported (wglExtensions, "NV_texture_env_combine4");
	::log ("\nNV Combine4 support %s present.", isNV_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isEXT_texture_env_combine4Present = isExtensionSupported (wglExtensions, "EXT_texture_env_combine4");
	::log ("\nEXT Combine4 support %s present.", isEXT_texture_env_combine4Present ? "IS" : "IS NOT");
    bool isATI_texture_env_combine4Present = isExtensionSupported (wglExtensions, "ATI_texture_env_combine4");
	::log ("\nATI Combine4 support %s present.", isATI_texture_env_combine4Present ? "IS" : "IS NOT");

	isARBTextureEnvDot3Present = isExtensionSupported (extensions, "GL_ARB_texture_env_dot3");
	::log ("\nCombiner Dot3 extension %s present", isARBTextureEnvDot3Present ? "IS" : "IS NOT");

	isNvidiaRegisterCombinerExtensionPresent = isExtensionSupported (extensions, "GL_NV_register_combiners") != 0;
	::log ("\nNvidia register combiners %s present.", isNvidiaRegisterCombinerExtensionPresent ? "ARE" : "ARE NOT");

	if (isNvidiaRegisterCombinerExtensionPresent) {
		//Bind all the necessary register combiner functions
		glCombinerParameterfvNV = (PFNGLCOMBINERPARAMETERFVNVPROC) wglGetProcAddress ("glCombinerParameterfvNV");
		glCombinerParameterivNV = (PFNGLCOMBINERPARAMETERIVNVPROC) wglGetProcAddress ("glCombinerParameterivNV");
		glCombinerParameterfNV = (PFNGLCOMBINERPARAMETERFNVPROC) wglGetProcAddress ("glCombinerParameterfNV");
		glCombinerParameteriNV = (PFNGLCOMBINERPARAMETERINVPROC) wglGetProcAddress ("glCombinerParameteriNV");
		glCombinerInputNV = (PFNGLCOMBINERINPUTNVPROC) wglGetProcAddress ("glCombinerInputNV");
		glCombinerOutputNV = (PFNGLCOMBINEROUTPUTNVPROC) wglGetProcAddress ("glCombinerOutputNV");
		glFinalCombinerInputNV = (PFNGLFINALCOMBINERINPUTNVPROC) wglGetProcAddress ("glFinalCombinerInputNV");
		glGetCombinerInputParameterfvNV = (PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) wglGetProcAddress ("glGetCombinerInputParameterfvNV");
		glGetCombinerInputParameterivNV = (PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) wglGetProcAddress ("glGetCombinerInputParameterivNV");
		glGetCombinerOutputParameterfvNV = (PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) wglGetProcAddress ("glGetCombinerOutputParameterfvNV");
		glGetCombinerOutputParameterivNV = (PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) wglGetProcAddress ("glGetCombinerOutputParameterivNV");
		glGetFinalCombinerInputParameterfvNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) wglGetProcAddress ("glGetFinalCombinerInputParameterfvNV");
		glGetFinalCombinerInputParameterivNV = (PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) wglGetProcAddress ("glGetFinalCombinerInputParameterivNV");
	}
		
	isNvidiaRegisterCombiner2ExtensionPresent = isExtensionSupported (extensions, "GL_NV_register_combiners2") != 0;
	::log ("\nNvidia register combiners2 %s present.", isNvidiaRegisterCombiner2ExtensionPresent ? "ARE" : "ARE NOT");

	if (isNvidiaRegisterCombiner2ExtensionPresent) {
		int combiners; glGetIntegerv (GL_NUM_GENERAL_COMBINERS_NV, &combiners); 
		::log ("\nNumber of NVDIA combiners is %d.", combiners);
		int maximumCombiners; glGetIntegerv (GL_MAX_GENERAL_COMBINERS_NV, &maximumCombiners);
		::log ("\nMaximum number of NVDIA combiners is %d.", maximumCombiners);
		numberOfCombiners = maximumCombiners;
		glCombinerStageParameterfvNV = (PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) wglGetProcAddress ("glCombinerStageParameterfvNV");
		glGetCombinerStageParameterfvNV = (PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) wglGetProcAddress ("glGetCombinerStageParameterfvNV");
	} else {
		numberOfCombiners = numberOfTextureUnits;
	}

	isATIFragmentShaderExtensionPresent = isExtensionSupported (extensions, "GL_ATI_fragment_shader");
	::log ("\nATI fragment shaders %s present.", isATIFragmentShaderExtensionPresent ? "ARE" : "ARE NOT");
	
	if (isATIFragmentShaderExtensionPresent) {
		glGenFragmentShadersATI   = (PFNGLGENFRAGMENTSHADERSATIPROC)   wglGetProcAddress ("glGenFragmentShadersATI");
		glBindFragmentShaderATI   = (PFNGLBINDFRAGMENTSHADERATIPROC)   wglGetProcAddress ("glBindFragmentShaderATI");
		glDeleteFragmentShaderATI = (PFNGLDELETEFRAGMENTSHADERATIPROC) wglGetProcAddress ("glDeleteFragmentShaderATI");
		glBeginFragmentShaderATI  = (PFNGLBEGINFRAGMENTSHADERATIPROC)  wglGetProcAddress ("glBeginFragmentShaderATI");
		glEndFragmentShaderATI    = (PFNGLENDFRAGMENTSHADERATIPROC)    wglGetProcAddress ("glEndFragmentShaderATI");
		glPassTexCoordATI         = (PFNGLPASSTEXCOORDATIPROC)         wglGetProcAddress ("glPassTexCoordATI");
		glSampleMapATI            = (PFNGLSAMPLEMAPATIPROC)            wglGetProcAddress ("glSampleMapATI");
		
		glColorFragmentOp1ATI = (PFNGLCOLORFRAGMENTOP1ATIPROC) wglGetProcAddress ("glColorFragmentOp1ATI");
		glColorFragmentOp2ATI = (PFNGLCOLORFRAGMENTOP2ATIPROC) wglGetProcAddress ("glColorFragmentOp2ATI");
		glColorFragmentOp3ATI = (PFNGLCOLORFRAGMENTOP3ATIPROC) wglGetProcAddress ("glColorFragmentOp3ATI");
		
		glAlphaFragmentOp1ATI = (PFNGLALPHAFRAGMENTOP1ATIPROC) wglGetProcAddress ("glAlphaFragmentOp1ATI");
		glAlphaFragmentOp2ATI = (PFNGLALPHAFRAGMENTOP2ATIPROC) wglGetProcAddress ("glAlphaFragmentOp2ATI");
		glAlphaFragmentOp3ATI = (PFNGLALPHAFRAGMENTOP3ATIPROC) wglGetProcAddress ("glAlphaFragmentOp3ATI");
		
		glSetFragmentShaderConstantATI = (PFNGLSETFRAGMENTSHADERCONSTANTATIPROC) wglGetProcAddress("glSetFragmentShaderConstantATI");
	}
);

	isARBVertexProgramExtensionPresent =  isExtensionSupported (extensions, "GL_ARB_vertex_program");
	::log ("\nARB Vertex program extension %s present.", isARBVertexProgramExtensionPresent ? "IS" : "IS NOT");
	isARBFragmentProgramExtensionPresent =  isExtensionSupported (extensions, "GL_ARB_fragment_program");
	::log ("\nARB Fragment program extension %s present.", isARBFragmentProgramExtensionPresent ? "IS" : "IS NOT");

REMOVE (
	if (isARBVertexProgramExtensionPresent || isARBFragmentProgramExtensionPresent) {
		glVertexAttrib1sARB = (PFNGLVERTEXATTRIB1SARB) wglGetProcAddress ("glVertexAttrib1sARB");
		glVertexAttrib1fARB = (PFNGLVERTEXATTRIB1FARB) wglGetProcAddress ("glVertexAttrib1fARB");
		glVertexAttrib1dARB = (PFNGLVERTEXATTRIB1DARB) wglGetProcAddress ("glVertexAttrib1dARB");
		glVertexAttrib2sARB = (PFNGLVERTEXATTRIB2SARB) wglGetProcAddress ("glVertexAttrib2sARB");
		glVertexAttrib2fARB = (PFNGLVERTEXATTRIB2FARB) wglGetProcAddress ("glVertexAttrib2fARB");
		glVertexAttrib2dARB = (PFNGLVERTEXATTRIB2DARB) wglGetProcAddress ("glVertexAttrib2dARB");
		glVertexAttrib3sARB = (PFNGLVERTEXATTRIB3SARB) wglGetProcAddress ("glVertexAttrib3sARB");
		glVertexAttrib3fARB = (PFNGLVERTEXATTRIB3FARB) wglGetProcAddress ("glVertexAttrib3fARB");
		glVertexAttrib3dARB = (PFNGLVERTEXATTRIB3DARB) wglGetProcAddress ("glVertexAttrib3dARB");
		glVertexAttrib4sARB = (PFNGLVERTEXATTRIB4SARB) wglGetProcAddress ("glVertexAttrib4sARB");
		glVertexAttrib4fARB = (PFNGLVERTEXATTRIB4FARB) wglGetProcAddress ("glVertexAttrib4fARB");
		glVertexAttrib4dARB = (PFNGLVERTEXATTRIB4DARB) wglGetProcAddress ("glVertexAttrib4dARB");
		glVertexAttrib4NubARB = (PFNGLVERTEXATTRIB4NUBARB) wglGetProcAddress ("glVertexAttrib4NubARB");
		glVertexAttrib1svARB = (PFNGLVERTEXATTRIB1SVARB) wglGetProcAddress ("glVertexAttrib1svARB");
		glVertexAttrib1fvARB = (PFNGLVERTEXATTRIB1FVARB) wglGetProcAddress ("glVertexAttrib1fvARB");
		glVertexAttrib1dvARB = (PFNGLVERTEXATTRIB1DVARB) wglGetProcAddress ("glVertexAttrib1dvARB");
		glVertexAttrib2svARB = (PFNGLVERTEXATTRIB2SVARB) wglGetProcAddress ("glVertexAttrib2svARB");
		glVertexAttrib2fvARB = (PFNGLVERTEXATTRIB2FVARB) wglGetProcAddress ("glVertexAttrib2fvARB");
		glVertexAttrib2dvARB = (PFNGLVERTEXATTRIB2DVARB) wglGetProcAddress ("glVertexAttrib2dvARB");
		glVertexAttrib3svARB = (PFNGLVERTEXATTRIB3SVARB) wglGetProcAddress ("glVertexAttrib3svARB");
		glVertexAttrib3fvARB = (PFNGLVERTEXATTRIB3FVARB) wglGetProcAddress ("glVertexAttrib3fvARB");
		glVertexAttrib3dvARB = (PFNGLVERTEXATTRIB3DVARB) wglGetProcAddress ("glVertexAttrib3dvARB");
		glVertexAttrib4bvARB = (PFNGLVERTEXATTRIB4BVARB) wglGetProcAddress ("glVertexAttrib4bvARB");
		glVertexAttrib4svARB = (PFNGLVERTEXATTRIB4SVARB) wglGetProcAddress ("glVertexAttrib4svARB");
		glVertexAttrib4ivARB = (PFNGLVERTEXATTRIB4IVARB) wglGetProcAddress ("glVertexAttrib4ivARB");
		glVertexAttrib4ubvARB = (PFNGLVERTEXATTRIB4UBVARB) wglGetProcAddress ("glVertexAttrib4ubvARB");
		glVertexAttrib4usvARB = (PFNGLVERTEXATTRIB4USVARB) wglGetProcAddress ("glVertexAttrib4usvARB");
		glVertexAttrib4uivARB = (PFNGLVERTEXATTRIB4UIVARB) wglGetProcAddress ("glVertexAttrib4uivARB");
		glVertexAttrib4fvARB = (PFNGLVERTEXATTRIB4FVARB) wglGetProcAddress ("glVertexAttrib4fvARB");
		glVertexAttrib4dvARB = (PFNGLVERTEXATTRIB4DVARB) wglGetProcAddress ("glVertexAttrib4dvARB");
		glVertexAttrib4NbvARB = (PFNGLVERTEXATTRIB4NBVARB) wglGetProcAddress ("glVertexAttrib4NbvARB");
		glVertexAttrib4NsvARB = (PFNGLVERTEXATTRIB4NSVARB) wglGetProcAddress ("glVertexAttrib4NsvARB");
		glVertexAttrib4NivARB = (PFNGLVERTEXATTRIB4NIVARB) wglGetProcAddress ("glVertexAttrib4NivARB");
		glVertexAttrib4NubvARB = (PFNGLVERTEXATTRIB4NUBVARB) wglGetProcAddress ("glVertexAttrib4NubvARB");
		glVertexAttrib4NusvARB = (PFNGLVERTEXATTRIB4NUSVARB) wglGetProcAddress ("glVertexAttrib4NusvARB");
		glVertexAttrib4NuivARB = (PFNGLVERTEXATTRIB4NUIVARB) wglGetProcAddress ("glVertexAttrib4NuivARB");
		glVertexAttribPointerARB = (PFNGLVERTEXATTRIBPOINTERARB) wglGetProcAddress ("glVertexAttribPointerARB");
		glEnableVertexAttribArrayARB = (PFNGLENABLEVERTEXATTRIBARRAYARB) wglGetProcAddress ("glEnableVertexAttribArrayARB");
		glDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYARB) wglGetProcAddress ("glDisableVertexAttribArrayARB");
		glProgramStringARB = (PFNGLPROGRAMSTRINGARB) wglGetProcAddress ("glProgramStringARB");
		glBindProgramARB = (PFNGLBINDPROGRAMARB) wglGetProcAddress ("glBindProgramARB");
		glDeleteProgramsARB = (PFNGLDELETEPROGRAMSARB) wglGetProcAddress ("glDeleteProgramsARB");
		glGenProgramsARB = (PFNGLGENPROGRAMSARB) wglGetProcAddress ("glGenProgramsARB");
		glProgramEnvParameter4dARB = (PFNGLPROGRAMENVPARAMETER4DARB) wglGetProcAddress ("glProgramEnvParameter4dARB");
		glProgramEnvParameter4dvARB = (PFNGLPROGRAMENVPARAMETER4DVARB) wglGetProcAddress ("glProgramEnvParameter4dvARB");
		glProgramEnvParameter4fARB = (PFNGLPROGRAMENVPARAMETER4FARB) wglGetProcAddress ("glProgramEnvParameter4fARB");
		glProgramEnvParameter4fvARB = (PFNGLPROGRAMENVPARAMETER4FVARB) wglGetProcAddress ("glProgramEnvParameter4fvARB");
		glProgramLocalParameter4dARB = (PFNGLPROGRAMLOCALPARAMETER4DARB) wglGetProcAddress ("glProgramLocalParameter4dARB");
		glProgramLocalParameter4dvARB = (PFNGLPROGRAMLOCALPARAMETER4DVARB) wglGetProcAddress ("glProgramLocalParameter4dvARB");
		glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARB) wglGetProcAddress ("glProgramLocalParameter4fARB");
		glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARB) wglGetProcAddress ("glProgramLocalParameter4fvARB");
		glGetProgramEnvParameterdvARB = (PFNGLGETPROGRAMENVPARAMETERDVARB) wglGetProcAddress ("glGetProgramEnvParameterdvARB");
		glGetProgramEnvParameterfvARB = (PFNGLGETPROGRAMENVPARAMETERFVARB) wglGetProcAddress ("glGetProgramEnvParameterfvARB");
		glGetProgramLocalParameterdvARB = (PFNGLGETPROGRAMLOCALPARAMETERDVARB) wglGetProcAddress ("glGetProgramLocalParameterdvARB");
		glGetProgramLocalParameterfvARB = (PFNGLGETPROGRAMLOCALPARAMETERFVARB) wglGetProcAddress ("glGetProgramLocalParameterfvARB");
		glGetProgramivARB = (PFNGLGETPROGRAMIVARB) wglGetProcAddress ("glGetProgramivARB");
		glGetProgramStringARB = (PFNGLGETPROGRAMSTRINGARB) wglGetProcAddress ("glGetProgramStringARB");
		glGetVertexAttribdvARB = (PFNGLGETVERTEXATTRIBDVARB) wglGetProcAddress ("glGetVertexAttribdvARB");
		glGetVertexAttribfvARB = (PFNGLGETVERTEXATTRIBFVARB) wglGetProcAddress ("glGetVertexAttribfvARB");
		glGetVertexAttribivARB = (PFNGLGETVERTEXATTRIBIVARB) wglGetProcAddress ("glGetVertexAttribivARB");
		glGetVertexAttribPointervARB = (PFNGLGETVERTEXATTRIBPOINTERVARB) wglGetProcAddress ("glGetVertexAttribPointervARB");
		glIsProgramARB = (PFNGLISPROGRAMARB) wglGetProcAddress ("glIsProgramARB");
	}
);

REMOVE (
	if (isARBFragmentProgramExtensionPresent) {
		GLint value = 0;

		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &value);
		::log ("\n\t%d native temporaries allowed", value);

		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_NATIVE_PARAMETERS_ARB, &value);
		::log ("\n\t%d native parameters allowed", value);

		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &value);
		::log ("\n\t%d native attributes allowed", value);


		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &value);
		::log ("\n\t%d alu instructions allowed", value);
		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &value);
		::log ("\n\t%d texture instructions allowed", value);
		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &value);
		::log ("\n\t%d texture indirections allowed", value);

		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &value);
		::log ("\n\t%d native alu instructions allowed", value);
		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &value);
		::log ("\n\t%d native texture instructions allowed", value);
		glGetProgramivARB (GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &value);
		::log ("\n\t%d native texture indirections allowed", value);
	}
);

REMOVE (
	isCompiledVertexArrayExtensionPresent = isExtensionSupported (extensions, "GL_EXT_compiled_vertex_array");
	::log ("\nCompiled vertex array %s present", isCompiledVertexArrayExtensionPresent ? "IS" : "IS NOT");
	if (isCompiledVertexArrayExtensionPresent) {
		glLockArraysEXT = (PFNGLLOCKARRAYSEXTPROC) wglGetProcAddress ("glLockArraysEXT");
		glUnlockArraysEXT = (PFNGLUNLOCKARRAYSEXTPROC) wglGetProcAddress ("glUnlockArraysEXT");
	}	
);

	isTextureCompressionExtensionPresent = isExtensionSupported (extensions, "GL_EXT_texture_compression_s3tc");
	::log ("\nS3TC texture compression %s present.", isTextureCompressionExtensionPresent ? "IS" : "IS NOT");
	if (isTextureCompressionExtensionPresent) {
		glCompressedTexImage2DARB = (PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) wglGetProcAddress ("glCompressedTexImage2DARB");
	} else prompt ("\nDDS files cannot be read since texture compression extension not available...");

REMOVE (
	isDrawRangeElementsExtensionPresent = isExtensionSupported (extensions, "GL_EXT_draw_range_elements");
	maxDrawRangeIndices = 0;
	maxDrawRangeVertices = 0;
	::log ("\nDraw Range Elements %s present", isDrawRangeElementsExtensionPresent ? "IS" : "IS NOT");
);
	if (isDrawRangeElementsExtensionPresent) {
REMOVE (
		glDrawRangeElementsEXT = (PFNGLDRAWRANGEELEMENTSEXTPROC) wglGetProcAddress ("glDrawRangeElementsEXT");
);
//REMOVE (
//		GLint maxIndices, maxVertices;
//		glGetIntegerv (GL_MAX_ELEMENTS_INDICES_EXT, &maxIndices);
//		glGetIntegerv (GL_MAX_ELEMENTS_VERTICES_EXT, &maxVertices);
//);
REMOVE (
		maxDrawRangeIndices = maxIndices;
		maxDrawRangeVertices = maxVertices;
		::log ("\n\tMax indices is %d, max vertices is %d", maxIndices, maxVertices);
);
	}

REMOVE (
    bool WGL_ARB_pixel_format_supported = isExtensionSupported (wglExtensions, "WGL_ARB_pixel_format");
    bool WGL_ARB_pbuffer_supported = isExtensionSupported (wglExtensions, "WGL_ARB_pbuffer");
	isRenderingBufferSupported = WGL_ARB_pixel_format_supported && WGL_ARB_pbuffer_supported;
	isMimapGeneratingRenderingBufferSupported = isExtensionSupported (extensions, "GL_SGIS_generate_mipmap");
	::log ("\nPixel buffer support %s present.", isRenderingBufferSupported ? "IS" : "IS NOT");
	::log ("\nPixel buffer mipmap generation %s present.", isMimapGeneratingRenderingBufferSupported ? "IS" : "IS NOT");
);
	
	if (isRenderingBufferSupported) {
REMOVE (
		wglBindTexImageARB = (PFNWGLBINDTEXIMAGEARBPROC) wglGetProcAddress ("wglBindTexImageARB");
		wglReleaseTexImageARB = (PFNWGLRELEASETEXIMAGEARBPROC) wglGetProcAddress ("wglReleaseTexImageARB");
		
		wglGetPixelFormatAttribivARB =(PFNWGLGETPIXELFORMATATTRIBIVARBPROC) wglGetProcAddress ("wglGetPixelFormatAttribivARB");
		wglGetPixelFormatAttribfvARB =(PFNWGLGETPIXELFORMATATTRIBFVARBPROC) wglGetProcAddress ("wglGetPixelFormatAttribfvARB");
		wglChoosePixelFormatARB =(PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress ("wglChoosePixelFormatARB");

		wglCreatePbufferARB = (PFNWGLCREATEPBUFFERARBPROC) wglGetProcAddress ("wglCreatePbufferARB");
		wglQueryPbufferARB = (PFNWGLQUERYPBUFFERARBPROC) wglGetProcAddress ("wglQueryPbufferARB");
		wglGetPbufferDCARB = (PFNWGLGETPBUFFERDCARBPROC) wglGetProcAddress ("wglGetPbufferDCARB");
		wglSetPbufferAttribARB = (PFNWGLSETPBUFFERATTRIBARBPROC) wglGetProcAddress ("wglSetPbufferAttribARB");
		wglReleasePbufferDCARB = (PFNWGLRELEASEPBUFFERDCARBPROC) wglGetProcAddress ("wglReleasePbufferDCARB");
		wglDestroyPbufferARB = (PFNWGLDESTROYPBUFFERARBPROC) wglGetProcAddress ("wglDestroyPbufferARB");

		wglMakeContextCurrentARB = (PFNWGLMAKECONTEXTCURRENTARBPROC) wglGetProcAddress ("wglMakeContextCurrentARB");
);
	}	

	isARBTextureBorderClampSupported = isExtensionSupported (extensions, "GL_ARB_texture_border_clamp");
	::log ("\nGL_ARB_texture_border_clamp %s supported.", isARBTextureBorderClampSupported ? "IS" : "IS NOT");

	isEXTTextureEdgeClampSupported = isExtensionSupported (extensions, "GL_EXT_texture_edge_clamp");
	::log ("\nGL_EXT_texture_edge_clamp %s supported.", isEXTTextureEdgeClampSupported ? "IS" : "IS NOT");

	bool isCubeMapSupported = isExtensionSupported (extensions, "GL_ARB_texture_cube_map");
	::log ("\nGL_ARB_texture_cube_map %s supported", isCubeMapSupported ? "IS" : "IS NOT");
	if (isCubeMapSupported) {
REMOVE (
		GLint maxCubeMapDimension = 0;
		glGetIntegerv (GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB, &maxCubeMapDimension);
		::log ("\n\tMax cube map size is %d x %d", maxCubeMapDimension, maxCubeMapDimension);
);
	}

	GLint maxTextureDimension = 0;
	glGetIntegerv (GL_MAX_TEXTURE_SIZE, &maxTextureDimension);
	::log ("\nMaximum texture size is %d x %d", maxTextureDimension, maxTextureDimension);

	isATIVertexArrayObjectExtensionPresent = isExtensionSupported (extensions, "GL_ATI_vertex_array_object");
	::log ("\nATI vertex array object extension %s supported", isATIVertexArrayObjectExtensionPresent ? "IS" : "IS NOT");
	if (isATIVertexArrayObjectExtensionPresent) {
REMOVE (
		glNewObjectBufferATI = (PFNGLNEWOBJECTBUFFERATIPROC) wglGetProcAddress ("glNewObjectBufferATI");
		glIsObjectBufferATI = (PFNGLISOBJECTBUFFERATIPROC) wglGetProcAddress ("glIsObjectBufferATI");
		glUpdateObjectBufferATI = (PFNGLUPDATEOBJECTBUFFERATIPROC) wglGetProcAddress ("glUpdateObjectBufferATI");
		glGetObjectBufferfvATI = (PFNGLGETOBJECTBUFFERFVATIPROC) wglGetProcAddress ("glGetObjectBufferfvATI");
		glGetObjectBufferivATI = (PFNGLGETOBJECTBUFFERIVATIPROC) wglGetProcAddress ("glGetObjectBufferivATI");
    	glFreeObjectBufferATI = (PFNGLFREEOBJECTBUFFERATIPROC) wglGetProcAddress ("glFreeObjectBufferATI");
		glArrayObjectATI = (PFNGLARRAYOBJECTATIPROC) wglGetProcAddress ("glArrayObjectATI");
		glGetArrayObjectfvATI = (PFNGLGETARRAYOBJECTFVATIPROC) wglGetProcAddress ("glGetArrayObjectfvATI");
		glGetArrayObjectivATI = (PFNGLGETARRAYOBJECTFVATIPROC) wglGetProcAddress ("glGetArrayObjectivATI");
);
	}

	isNvidiaVertexArrayRangeExtensionPresent = isExtensionSupported (extensions, "GL_NV_vertex_array_range");
	::log ("\nNVIDIA vertex array range extension %s supported", isNvidiaVertexArrayRangeExtensionPresent ? "IS" : "IS NOT");
	if (isNvidiaVertexArrayRangeExtensionPresent) {
REMOVE (
		glFlushVertexArrayRangeNV = (PFNGLFLUSHVERTEXARRAYRANGENVPROC) wglGetProcAddress ("glFlushVertexArrayRangeNV");
		glVertexArrayRangeNV = (PFNGLVERTEXARRAYRANGENVPROC) wglGetProcAddress ("glVertexArrayRangeNV");
		wglAllocateMemoryNV = (PFNWGLALLOCATEMEMORYNVPROC) wglGetProcAddress ("wglAllocateMemoryNV");
		wglFreeMemoryNV = (PFNWGLFREEMEMORYNVPROC) wglGetProcAddress ("wglFreeMemoryNV");
);
	}

	isARBVertexBufferObjectExtensionPresent = isExtensionSupported (extensions, "GL_ARB_vertex_buffer_object");
	::log ("\nARB vertex buffer object extension %s supported", isARBVertexBufferObjectExtensionPresent ? "IS" : "IS NOT");
	if (isARBVertexBufferObjectExtensionPresent) {
REMOVE (
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
);
	}

	isARBOcclusionQueryExtensionPresent = isExtensionSupported (extensions, "GL_ARB_occlusion_query");
	::log ("\nARB occlusion query extension %s supported", isARBOcclusionQueryExtensionPresent ? "IS" : "IS NOT");
	if (isARBOcclusionQueryExtensionPresent) {
REMOVE (
		glGenQueries = (PFNGLGENQUERIESPROC) wglGetProcAddress ("glGenQueriesARB");
		glDeleteQueries = (PFNGLDELETEQUERIESPROC) wglGetProcAddress ("glDeleteQueriesARB");
		glIsQuery = (PFNGLISQUERYPROC) wglGetProcAddress ("glIsQueryARB");
		glBeginQuery = (PFNGLBEGINQUERYPROC) wglGetProcAddress ("glBeginQueryARB");
		glEndQuery = (PFNGLENDQUERYPROC) wglGetProcAddress ("glEndQueryARB");
		glGetQueryiv = (PFNGLGETQUERYIVPROC) wglGetProcAddress ("glGetQueryivARB");
		glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) wglGetProcAddress ("glGetQueryObjectivARB");
		glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) wglGetProcAddress ("glGetQueryObjectuivARB");
);
	}

	isEXTFrameBufferExtensionPresent = isExtensionSupported (extensions, "GL_EXT_framebuffer_object");
	::log ("\nEXT frame buffer extension %s supported", isEXTFrameBufferExtensionPresent ? "IS" : "IS NOT");
	if (isEXTFrameBufferExtensionPresent) {
REMOVE (
		glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC) wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC) wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC) wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC) wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC) wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC) wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC) wglGetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) wglGetProcAddress("glGenFramebuffersEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC) wglGetProcAddress("glGenerateMipmapEXT");
);
	}
	::log ("\n\n");
}

//**************************************************************************************************//
//                                  Extension function pointers                                     //
//**************************************************************************************************//

REMOVE (
PFNGLBLENDEQUATIONEXTPROC glBlendEquation = NULL;
PFNGLBLENDCOLORPROC glBlendColor = NULL;

PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB          = NULL;	
PFNGLMULTITEXCOORD2DARBPROC			glMultiTexCoord2dARB		= NULL;
PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB        = NULL;
PFNGLMULTITEXCOORD2FVARBPROC		glMultiTexCoord2fvARB        = NULL;
PFNGLMULTITEXCOORD3DARBPROC			glMultiTexCoord3dARB		= NULL;
PFNGLMULTITEXCOORD3FARBPROC			glMultiTexCoord3fARB        = NULL;

PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB	= NULL;
//voidFunctionReturningInt			wglGetSwapIntervalEXT		= NULL;
intFunctionReturningBool			wglSwapIntervalEXT			= NULL;

//Nvidia combiners
PFNGLCOMBINERPARAMETERFVNVPROC glCombinerParameterfvNV = NULL;
PFNGLCOMBINERPARAMETERFNVPROC glCombinerParameterfNV = NULL;
PFNGLCOMBINERPARAMETERIVNVPROC glCombinerParameterivNV = NULL;
PFNGLCOMBINERPARAMETERINVPROC glCombinerParameteriNV = NULL;
PFNGLCOMBINERINPUTNVPROC glCombinerInputNV = NULL;
PFNGLCOMBINEROUTPUTNVPROC glCombinerOutputNV = NULL;
PFNGLFINALCOMBINERINPUTNVPROC glFinalCombinerInputNV = NULL;
PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC glGetCombinerInputParameterfvNV = NULL;
PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC glGetCombinerInputParameterivNV = NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC glGetCombinerOutputParameterfvNV = NULL;
PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC glGetCombinerOutputParameterivNV = NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC glGetFinalCombinerInputParameterfvNV = NULL;
PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC glGetFinalCombinerInputParameterivNV = NULL;
		
PFNGLCOMBINERSTAGEPARAMETERFVNVPROC glCombinerStageParameterfvNV = NULL;
PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC glGetCombinerStageParameterfvNV = NULL;

//Texture compression
);
PFNGLCOMPRESSEDTEXIMAGE2DARBPROC glCompressedTexImage2DARB = NULL;
REMOVE (

//ATI fragment shaders
PFNGLGENFRAGMENTSHADERSATIPROC   glGenFragmentShadersATI = NULL;
PFNGLBINDFRAGMENTSHADERATIPROC   glBindFragmentShaderATI = NULL;
PFNGLDELETEFRAGMENTSHADERATIPROC glDeleteFragmentShaderATI = NULL;
PFNGLBEGINFRAGMENTSHADERATIPROC  glBeginFragmentShaderATI = NULL;
PFNGLENDFRAGMENTSHADERATIPROC    glEndFragmentShaderATI = NULL;
PFNGLPASSTEXCOORDATIPROC         glPassTexCoordATI = NULL;
PFNGLSAMPLEMAPATIPROC            glSampleMapATI = NULL;

PFNGLCOLORFRAGMENTOP1ATIPROC glColorFragmentOp1ATI = NULL;
PFNGLCOLORFRAGMENTOP2ATIPROC glColorFragmentOp2ATI = NULL;
PFNGLCOLORFRAGMENTOP3ATIPROC glColorFragmentOp3ATI = NULL;

PFNGLALPHAFRAGMENTOP1ATIPROC glAlphaFragmentOp1ATI = NULL;
PFNGLALPHAFRAGMENTOP2ATIPROC glAlphaFragmentOp2ATI = NULL;
PFNGLALPHAFRAGMENTOP3ATIPROC glAlphaFragmentOp3ATI = NULL;

PFNGLSETFRAGMENTSHADERCONSTANTATIPROC glSetFragmentShaderConstantATI = NULL;


//ARB vertex program
PFNGLVERTEXATTRIB1SARB glVertexAttrib1sARB = NULL;
PFNGLVERTEXATTRIB1FARB glVertexAttrib1fARB = NULL;
PFNGLVERTEXATTRIB1DARB glVertexAttrib1dARB = NULL;
PFNGLVERTEXATTRIB2SARB glVertexAttrib2sARB = NULL;
PFNGLVERTEXATTRIB2FARB glVertexAttrib2fARB = NULL;
PFNGLVERTEXATTRIB2DARB glVertexAttrib2dARB = NULL;
PFNGLVERTEXATTRIB3SARB glVertexAttrib3sARB = NULL;
PFNGLVERTEXATTRIB3FARB glVertexAttrib3fARB = NULL;
PFNGLVERTEXATTRIB3DARB glVertexAttrib3dARB = NULL;
PFNGLVERTEXATTRIB4SARB glVertexAttrib4sARB = NULL;
PFNGLVERTEXATTRIB4FARB glVertexAttrib4fARB = NULL;
PFNGLVERTEXATTRIB4DARB glVertexAttrib4dARB = NULL;
PFNGLVERTEXATTRIB4NUBARB glVertexAttrib4NubARB = NULL;

PFNGLVERTEXATTRIB1SVARB glVertexAttrib1svARB = NULL;
PFNGLVERTEXATTRIB1FVARB glVertexAttrib1fvARB = NULL;
PFNGLVERTEXATTRIB1DVARB glVertexAttrib1dvARB = NULL;
PFNGLVERTEXATTRIB2SVARB glVertexAttrib2svARB = NULL;
PFNGLVERTEXATTRIB2FVARB glVertexAttrib2fvARB = NULL;
PFNGLVERTEXATTRIB2DVARB glVertexAttrib2dvARB = NULL;
PFNGLVERTEXATTRIB3SVARB glVertexAttrib3svARB = NULL;
PFNGLVERTEXATTRIB3FVARB glVertexAttrib3fvARB = NULL;
PFNGLVERTEXATTRIB3DVARB glVertexAttrib3dvARB = NULL;
PFNGLVERTEXATTRIB4BVARB glVertexAttrib4bvARB = NULL;
PFNGLVERTEXATTRIB4SVARB glVertexAttrib4svARB = NULL;
PFNGLVERTEXATTRIB4IVARB glVertexAttrib4ivARB = NULL;
PFNGLVERTEXATTRIB4UBVARB glVertexAttrib4ubvARB = NULL;
PFNGLVERTEXATTRIB4USVARB glVertexAttrib4usvARB = NULL;
PFNGLVERTEXATTRIB4UIVARB glVertexAttrib4uivARB = NULL;
PFNGLVERTEXATTRIB4FVARB glVertexAttrib4fvARB = NULL;
PFNGLVERTEXATTRIB4DVARB glVertexAttrib4dvARB = NULL;
PFNGLVERTEXATTRIB4NBVARB glVertexAttrib4NbvARB = NULL;
PFNGLVERTEXATTRIB4NSVARB glVertexAttrib4NsvARB = NULL;
PFNGLVERTEXATTRIB4NIVARB glVertexAttrib4NivARB = NULL;
PFNGLVERTEXATTRIB4NUBVARB glVertexAttrib4NubvARB = NULL;
PFNGLVERTEXATTRIB4NUSVARB glVertexAttrib4NusvARB = NULL;
PFNGLVERTEXATTRIB4NUIVARB glVertexAttrib4NuivARB = NULL;

PFNGLVERTEXATTRIBPOINTERARB glVertexAttribPointerARB = NULL;
PFNGLENABLEVERTEXATTRIBARRAYARB glEnableVertexAttribArrayARB = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARB glDisableVertexAttribArrayARB = NULL;
PFNGLPROGRAMSTRINGARB glProgramStringARB = NULL;
PFNGLBINDPROGRAMARB glBindProgramARB = NULL;
PFNGLDELETEPROGRAMSARB glDeleteProgramsARB = NULL;
PFNGLGENPROGRAMSARB glGenProgramsARB = NULL;

PFNGLPROGRAMENVPARAMETER4DARB glProgramEnvParameter4dARB = NULL;
PFNGLPROGRAMENVPARAMETER4DVARB glProgramEnvParameter4dvARB = NULL;
PFNGLPROGRAMENVPARAMETER4FARB glProgramEnvParameter4fARB = NULL;
PFNGLPROGRAMENVPARAMETER4FVARB glProgramEnvParameter4fvARB = NULL;

PFNGLPROGRAMLOCALPARAMETER4DARB glProgramLocalParameter4dARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4DVARB glProgramLocalParameter4dvARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARB glProgramLocalParameter4fARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FVARB glProgramLocalParameter4fvARB = NULL;

PFNGLGETPROGRAMENVPARAMETERDVARB glGetProgramEnvParameterdvARB = NULL;
PFNGLGETPROGRAMENVPARAMETERFVARB glGetProgramEnvParameterfvARB = NULL;

PFNGLGETPROGRAMLOCALPARAMETERDVARB glGetProgramLocalParameterdvARB = NULL;
PFNGLGETPROGRAMLOCALPARAMETERFVARB glGetProgramLocalParameterfvARB = NULL;

PFNGLGETPROGRAMIVARB glGetProgramivARB = NULL;
PFNGLGETPROGRAMSTRINGARB glGetProgramStringARB = NULL;
PFNGLGETVERTEXATTRIBDVARB glGetVertexAttribdvARB = NULL;
PFNGLGETVERTEXATTRIBFVARB glGetVertexAttribfvARB = NULL;
PFNGLGETVERTEXATTRIBIVARB glGetVertexAttribivARB = NULL;
PFNGLGETVERTEXATTRIBPOINTERVARB glGetVertexAttribPointervARB = NULL;
PFNGLISPROGRAMARB glIsProgramARB = NULL;

//Compiled vertex arrays
PFNGLLOCKARRAYSEXTPROC glLockArraysEXT = NULL;
PFNGLUNLOCKARRAYSEXTPROC glUnlockArraysEXT = NULL;

//Draw Range Elements
PFNGLDRAWRANGEELEMENTSEXTPROC glDrawRangeElementsEXT = NULL;
DWORD RangeElements_MaxVertexElements = 0;
DWORD RangeElements_MaxIndexElements = 0;

//ATI object buffer extension
PFNGLNEWOBJECTBUFFERATIPROC glNewObjectBufferATI = NULL;
PFNGLISOBJECTBUFFERATIPROC glIsObjectBufferATI = NULL;
PFNGLUPDATEOBJECTBUFFERATIPROC glUpdateObjectBufferATI = NULL;
PFNGLGETOBJECTBUFFERFVATIPROC glGetObjectBufferfvATI = NULL;
PFNGLGETOBJECTBUFFERIVATIPROC glGetObjectBufferivATI = NULL;
PFNGLFREEOBJECTBUFFERATIPROC glFreeObjectBufferATI = NULL;
PFNGLARRAYOBJECTATIPROC glArrayObjectATI = NULL;
PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectfvATI = NULL;
PFNGLGETARRAYOBJECTFVATIPROC glGetArrayObjectivATI = NULL;

//NVIDIA vertex array range extension
PFNGLFLUSHVERTEXARRAYRANGENVPROC glFlushVertexArrayRangeNV = NULL;
PFNGLVERTEXARRAYRANGENVPROC glVertexArrayRangeNV = NULL;
PFNWGLALLOCATEMEMORYNVPROC wglAllocateMemoryNV = NULL;
PFNWGLFREEMEMORYNVPROC wglFreeMemoryNV = NULL;

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

//ARB occlusion query
PFNGLGENQUERIESPROC glGenQueries = NULL;
PFNGLDELETEQUERIESPROC glDeleteQueries = NULL;
PFNGLISQUERYPROC glIsQuery = NULL;
PFNGLBEGINQUERYPROC glBeginQuery = NULL;
PFNGLENDQUERYPROC glEndQuery = NULL;
PFNGLGETQUERYIVPROC glGetQueryiv = NULL;
PFNGLGETQUERYOBJECTIVPROC glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIVPROC glGetQueryObjectuiv = NULL;

//EXT frame buffer
PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;
);
