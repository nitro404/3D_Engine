//*****************************************************************************************//
//                                        FaceGroup                                        //
//*****************************************************************************************//

#ifndef faceGroupModule
#define faceGroupModule 

class FaceGroup {		
	GLenum drawingHint; //GL_STATIC_DRAW or GL_DYNAMIC_DRAW
	GLenum triangleType; //GL_TRIANGLES or GL_TRIANGLE_STRIP;
	GamePoint *vertices; long verticesByteSize; long verticesCount;
	unsigned short *indices; long indicesByteSize; long indicesCount;
	GLuint verticesBuffer; GLuint indicesBuffer;
public:
	Texture *texture;

	FaceGroup (texture *texture, GamePoint *vertices, long verticesCount, unsigned short *indices = NULL, long indicesCount = 0) {
		this->texture = texture;
		this->vertices = vertices; this-> verticesCount = verticesCount; 
		verticesByteSize = verticesCount * sizeof (GamePoint);
		this->indices = indices; this-> indicesCount = indicesCount; 
		indicesByteSize = indicesCount * sizeof (unsigned short);
		verticesBuffer = 0; indicesBuffer = 0;
		hint (GL_STATIC_DRAW); type (GL_TRIANGLE_STRIP);
	}
	~FaceGroup () {delete [] vertices; delete [] indices;}

	//GL_STATIC_DRAW or GL_DYNAMIC_DRAW 
	GLenum hint () {return drawingHint;}
	void hint (GLenum newHint) {drawingHint = newHint;}
	
	//GL_TRIANGLES or GL_TRIANGLE_STRIP;
	GLenum type () {return triangleType;}
	void type (GLenum newType) {triangleType = newType;}

private:	
	void setupBufferHandles () {	
		glGenBuffers (1, &verticesBuffer);
		if (indices != NULL) glGenBuffers (1, &indicesBuffer);
	}
	void wrapupBufferHandles () {	
		glDeleteBuffers (1, &verticesBuffer);
		if (indices != NULL) glDeleteBuffers (1, &indicesBuffer);
	}
	void allocateBufferSpaceOnCard () {//Private (used by load)…
		//Needs to be done ONCE... Not using vertices/indices.
		void *initialData = NULL;
		GLenum type1 = GL_ARRAY_BUFFER;
		glBindBuffer (type1, verticesBuffer);
		glBufferData (type1, verticesByteSize, initialData, hint ()); 

		if (indices == NULL) return;	

		GLenum type2 = GL_ELEMENT_ARRAY_BUFFER;
		glBindBuffer (type2, indicesBuffer);
		glBufferData (type2, indicesByteSize, initialData, hint ());
	}
public:		
	static setup () {
		#if (USING_NEW_OPENGL)
			//Nothing more to do...
		#else
			setupOpenGLExtensions ();
		#endif
	}
	static wrapup () {}

	void load () {
		setupBufferHandles ();
		allocateBufferSpaceOnCard ();
		update ();
	}
	void unload () {
		wrapupBufferHandles ();
	} 	
	void update () {			
		GLenum type1 = GL_ARRAY_BUFFER;
		glBindBuffer (type1, verticesBuffer);
		glBufferSubData (type1, 0, verticesByteSize, vertices); 

		if (indices == NULL) return;
		
		GLenum type2 = GL_ELEMENT_ARRAY_BUFFER;
		glBindBuffer (type2, indicesBuffer);
		glBufferSubData (type2, 0, indicesByteSize, indices);	
	}
private:
	void activateVertices () {
		if (glBindBuffer == 0) ::halt ("\nYou forgot to say FaceGroup::setup ()...");
		if (verticesBuffer == 0) ::halt ("\nYou forgot to load your face group onto the card...");

		glBindBuffer (GL_ARRAY_BUFFER, verticesBuffer);
		long stride = sizeof (GamePoint);
		long doubleSize = sizeof (double);

		#if (USING_NEW_OPENGL)
			glVertexPointer (3, GL_DOUBLE, stride, 0); 
			glEnableClientState (GL_VERTEX_ARRAY);
			glTexCoordPointer (2, GL_DOUBLE, stride, 3 * doubleSize); 
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			//glNormalPointer (GL_DOUBLE, stride, 5 * doubleSize); 
			//glEnableClientState (GL_NORMAL_ARRAY);
		#else
			glVertexPointer (3, GL_DOUBLE, stride, (void *) (0)); 
			glEnableClientState (GL_VERTEX_ARRAY);
			glTexCoordPointer (2, GL_DOUBLE, stride, (void *) (3 * doubleSize)); 
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			//glNormalPointer (GL_DOUBLE, stride, (void *) (5 * doubleSize));
			//glEnableClientState (GL_NORMAL_ARRAY);
		#endif

		//GamePoint test;
		//if ((char *) &test->tx - (char *) &test.x) != 3 * doubleSize) ::halt ("\nYou forgot to change the "
		//	"declaration order \nof your game points to x, y, z, tx, ty, nx, ny, nz...");
	}
	void activateIndices () {		
		//Invoked if indices != NULL…

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	}
	void deactivate () {
		glBindBuffer (GL_ARRAY_BUFFER, 0);
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
	}

public:
	
	void draw () {
		if (texture == NULL) glDisable (GL_TEXTURE_2D); else texture->activate ();
		activateVertices ();
		if (indices == NULL) {
			glDrawArrays (type (), 0, verticesCount);
		} else {
			activateIndices (); //This implies indices are on the card...
			//To use indices on the card, supply an offset to start from. To use indices on CPU, supply address to start from.
			const bool usingIndicesOnCard = true; const long offsetOfIndices = 0;
			void *startOfIndices = usingIndicesOnCard ? (void *) offsetOfIndices : indices; 
			glDrawElements (type (), indicesCount, GL_UNSIGNED_SHORT, startOfIndices);
		}
		deactivate ();
	}
};

#endif