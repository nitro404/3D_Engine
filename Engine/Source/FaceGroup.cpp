#include "FaceGroup.h"

FaceGroup::FaceGroup(GamePoint * vertices, int width, int height, int squareSections)
{

	this->vertices = new GamePoint[width * height];
	this->width = width;
	this->height = height;

	verticesBuffer = NULL;

	if (TERRAIN_IMPLEMENTATION == USE_BUFFERS) {
		glEnableClientState (GL_VERTEX_ARRAY);
		glGenBuffers (1, &verticesBuffer);
		glBindBuffer (GL_ARRAY_BUFFER, verticesBuffer);
		glBufferData (GL_ARRAY_BUFFER, width * height * sizeof (GamePoint), &vertices[0], GL_DYNAMIC_DRAW);
		//updateBuffers();
	}

	groups = vector<SubGroup*>();

	int xStep = width / squareSections;
	int yStep = height / squareSections;

	for (int i = 0;i < squareSections;i++) {
		int xStart = xStep * i;
		int xEnd;
		if (i == squareSections - 1) {
			xEnd = width;
		} else {
			xEnd = xStep * (i + 1) + 1;
		}
		for (int j = 0;j < squareSections;j++) {
			int yStart = yStep * j;
			int yEnd;
			if (j == squareSections - 1) {
				yEnd = height;
			} else {
				yEnd = yStep * (j + 1) + 1;
			}
			groups.push_back(new SubGroup(vertices, width, height, xStart, yStart, xEnd, yEnd));
		}
	}
}

FaceGroup::~FaceGroup() {
	for (unsigned int i = 0; i < groups.size();i++) {
		delete groups.at(i);
	}
	if(vertices != NULL) {
		delete [] vertices;
	}
}

//updates the buffer on the video card to use new points
//that are located in the same vertex array used to create
//this object.  The dimensions of the array had better not have
//changed.  This is a complete refesh, so it may take a LONG time
void FaceGroup::updateBuffers() {
	if (verticesBuffer != NULL) {
		glBufferSubData(verticesBuffer, 0, width * height * sizeof(GamePoint), vertices);
	}
}

void FaceGroup::draw() {
	if (TERRAIN_IMPLEMENTATION == USE_BUFFERS) {
		glBindBuffer (GL_ARRAY_BUFFER, verticesBuffer);
		
		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (3, GL_DOUBLE, sizeof(GamePoint), 0);
		
		glEnableClientState (GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer (2, GL_DOUBLE, sizeof(GamePoint), (void *) (6 * sizeof(double)) );

	}
	for (unsigned int i = 0;i < groups.size();i++) {
		groups.at(i)->draw(verticesBuffer);
	}
	if (TERRAIN_IMPLEMENTATION == USE_BUFFERS) {
		glDisableClientState (GL_VERTEX_ARRAY);
	}
}

SubGroup::SubGroup(GamePoint *vertices, int verticesWidth, int verticesHeight, int xStart, int yStart, int xEnd, int yEnd) {
	this->vertices = vertices;
	width = xEnd - xStart;
	height = yEnd - yStart;

	indicesBuffer = 0;
	indicesSize = 0;
	int index = 0;
	
	if (TERRAIN_IMPLEMENTATION == USE_QUADS) {

	indices = new unsigned int[width * height];

	for (int j = yStart;j < yEnd;j++) {
		for (int i = xStart;i < xEnd;i++) {
			indices[index++] = j * verticesWidth + i;
		}
	}
	} else {
		indices = new unsigned int[2 * (width + 1) * (height - 1)];

		for (int j = yStart; j < yEnd - 1;j++) {
			//write the first coordinate of the row
			indices[index++] = j * verticesWidth + xStart;
			int lastIndex;
			for (int i = xStart; i < xEnd;i++) {

				//top
				indices[index++] = j * verticesWidth + i;

				//bottom
				lastIndex = (j + 1) * verticesWidth + i;
				indices[index++] = lastIndex;
			}
			//repeat the last coordinate of the row
			indices[index++] = lastIndex;
		}
	}

	indicesSize = index;

	if (TERRAIN_IMPLEMENTATION == USE_BUFFERS) {
		glGenBuffers (1, &indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		glBufferData (GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof (GLuint), &indices[0], GL_STATIC_DRAW);
		//glBufferSubData(indicesBuffer, 0, width * height * sizeof(int), indices);
	}

}

SubGroup::~SubGroup(void)
{
	if (indices != NULL) {
		delete [] indices;
	}
}

//assumes that the appropriate texture has already been activated, and
//that any transformations have already been performed.
void SubGroup::draw(GLuint verticesBuffer) {
	if (TERRAIN_IMPLEMENTATION == USE_QUADS) {
		drawQuads();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_STRIPS) {
		drawStrips();
	}
	else if (TERRAIN_IMPLEMENTATION == USE_BUFFERS) {
		drawBuffers(verticesBuffer);
	}
}

void SubGroup::drawQuads() {
	GamePoint * point;
	for(int j=0;j<height - 1;j++) {
		for(int i=0;i<width - 1;i++) {
			glBegin(GL_POLYGON);
				// top-left
				point = &vertices[indices[i * width + j]];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-left
				point = &vertices[indices[i * width + j + 1]];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// bottom-right
				point = &vertices[indices[(i + 1) * width + j + 1]];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);

				// top-right
				point = &vertices[indices[(i + 1) * width + j]];
				glTexCoord2d(point->tx, point->ty);
				glVertex3d(point->x, point->y, point->z);
			glEnd();
		}
	}
}

void SubGroup::drawStrips() {
	GamePoint * point;

	glBegin(GL_TRIANGLE_STRIP);

	for (unsigned int i = 0;i < indicesSize;i++) {
		point = &vertices[indices[i]];
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
	}
	/*for (int j = 0; j < height - 1;j++) {
		//write the first coordinate of the row
		point = &vertices[indices[j * width]];
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
		for (int i = 0; i < width;i++) {

			//top
			point = &vertices[indices[j * width + i]];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);

			//bottom
			point = &vertices[indices[(j + 1) * width + i]];
			glTexCoord2d(point->tx, point->ty);
			glVertex3d(point->x, point->y, point->z);
			
		}
		//repeat the last coordinate of the row
		glTexCoord2d(point->tx, point->ty);
		glVertex3d(point->x, point->y, point->z);
	}*/
	glEnd();
}

void SubGroup::drawBuffers(GLuint verticesBuffer) {
	
	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

	glDrawElements (GL_TRIANGLE_STRIP, indicesSize, GL_UNSIGNED_INT, 0);

	//glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

}

