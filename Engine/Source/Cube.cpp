#include "Game.h"
#include "Cube.h"

Cube::Cube(const DualTransformation & cubeTransformation, int cubeSize, const Point & cubeVelocity, int cubeMass, const Colour & cubeColour, Texture ** cubeTextures) : size(cubeSize), mass(cubeMass), colour(cubeColour), textures(NULL), cubeCollisionMesh(NULL), loaded(false) {
	transformation = new DualTransformation(cubeTransformation);
	velocity = Point(cubeVelocity);

	if(cubeTextures != NULL) {
		textures = new Texture*[6];
		for(int i=0;i<6;i++) {
			textures[i] = cubeTextures[i];
		}
	}
}

Cube::~Cube() {
	unload();

	if(textures != NULL) {
		delete [] textures;
	}
}

bool Cube::load() {
	if(loaded) { return true; }

	cubeList = glGenLists(6);

	float w = (textures == NULL || textures[0] == NULL) ? 1 : (float) (textures[0]->width - 1) / (float) textures[0]->width;

	static GLfloat n[6][3] = {
		{-1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0},
		{1.0, 0.0, 0.0},
		{0.0, -1.0, 0.0},
		{0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}
	};

	static GLint faces[6][4] = {
		{0, 1, 2, 3},
		{3, 2, 6, 7},
		{7, 6, 5, 4},
		{4, 5, 1, 0},
		{5, 6, 2, 1},
		{7, 4, 0, 3}
	};

	GLfloat v[8][3];
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	GLdouble tex[24][2] = {
		{1 - w, w}, {1 - w, 1 - w}, {w, 1 - w}, {w, w},
		{w, 1 - w}, {w, w}, {1 - w, w}, {1 - w, 1 - w},
		{1 - w, w}, {1 - w, 1 - w}, {w, 1 - w}, {w, w},
		{w, w}, {1 - w, w}, {1 - w, 1 - w}, {w, 1 - w},
		{1 - w, w}, {1 - w, 1 - w}, {w, 1 - w}, {w, w},
		{1 - w, 1 - w}, {w, 1 - w}, {w, w}, {1 - w, w}
	};

	GLint j = 0;
	int k = 0;
	for(GLint i=5;i>=0;i--) {
		glNewList(cubeList + j, GL_COMPILE);
			glBegin(GL_QUADS);
				glNormal3fv(&n[i][0]);
				glTexCoord2d(tex[k][0], tex[k][1]);
				glVertex3fv(&v[faces[i][0]][0]);
				glTexCoord2d(tex[k+1][0], tex[k+1][1]);
				glVertex3fv(&v[faces[i][1]][0]);
				glTexCoord2d(tex[k+2][0], tex[k+2][1]);
				glVertex3fv(&v[faces[i][2]][0]);
				glTexCoord2d(tex[k+3][0], tex[k+3][1]);
				glVertex3fv(&v[faces[i][3]][0]);
			glEnd();
		glEndList();
		j++;
		k += 4;
	}

	cubeCollisionMesh = Game::physics->createBoxMesh(transformation->position(), velocity, size, size, size, mass);

	loaded = true;

	return true;
}

bool Cube::unload() {
	if(!loaded) { return true; }

	cubeCollisionMesh->release();

	glDeleteLists(cubeList, 6);

	loaded = false;

	return true;
}

bool Cube::isLoaded() const {
	return loaded;
}

void Cube::update(double timeElapsed) {
	if(!loaded) { return; }
}

void Cube::draw() {
	if(!loaded) { return; }

	glEnable(GL_TEXTURE_2D);

	glColor4f(colour.red, colour.green, colour.blue, colour.alpha);

	Transformation t = *((Transformation *) &PxMat44(cubeCollisionMesh->getGlobalPose()));

	glPushMatrix();
//		Transformation & normal = transformation->normal();
//		glMultMatrixd(normal);
		glMultMatrixf(t);
		for(int i=0;i<6;i++) {
			if(textures != NULL && textures[i] != NULL) {
				textures[i]->activate();
			}
			glCallList(cubeList + i);
		}
	glPopMatrix();
}
