#include "FaceGroup.h"

FaceGroup::FaceGroup(GamePoint * vertices, int width, int height)
{
	this->vertices = vertices;
	this->width = width;
	this->height = height;
}

FaceGroup::~FaceGroup(void)
{
	//does not own any objects
}

//assumes that the appropriate texture has already been activated, and
//that any transformations have already been performed.
void FaceGroup::draw() {
	if (TERRAIN_IMPLEMENTATION == USE_QUADS) {
		drawQuads();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_STRIPS) {
		drawStrips();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS) {
		drawFull();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_FACE_GROUPS_AND_FRUSTUM_CULLING) {
		drawFrustrums();
	}
}

void FaceGroup::drawQuads() {
	GamePoint * point;
	for(int j=0;j<height - 1;j++) {
		for(int i=0;i<width - 1;i++) {
			glBegin(GL_POLYGON);
				// top-left
				point = &vertices[i * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-left
				point = &vertices[i * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-right
				point = &vertices[(i + 1) * width + j + 1];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// top-right
				point = &vertices[(i + 1) * width + j];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);
			glEnd();
		}
	}
}

void FaceGroup::drawStrips() {
	GamePoint * point;

	glBegin(GL_TRIANGLE_STRIP);
	for (int j = 0; j < height - 1;j++) {
		//write the first coordinate of the row
		point = &vertices[j * width];
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
		for (int i = 0; i < width;i++) {

			//top
			point = &vertices[j * width + i];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);

			//bottom
			point = &vertices[(j + 1) * width + i];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);
			
		}
		//repeat the last coordinate of the row
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
	}
	glEnd();
}

void FaceGroup::drawFrustrums() {
	
}

void FaceGroup::drawFull() {
	
}