#ifndef OBJECT_H
#define OBJECT_H

#include "Includes.h"
#include "Colour.h"
#include "Point.h"
#include "Transformation.h"
#include "Camera.h"
#include "Face.h"
#include "BoundingBox.h"

extern double DT;

class Object {
public:
	Object() : transformation(NULL), box(NULL) { } 
	virtual ~Object() {
		if(transformation != NULL) { delete transformation; }
		if(box != NULL) { delete box; }
	}
	
	virtual double distanceFrom(Point & p) const { return 0; }
	
	virtual void tick() { }
	virtual void draw() { }

	virtual void drawWithCull() {
		if(box != NULL) {
			BoundingBox copy = *box;
			copy.offsetBy(transformation->position());
			if(camera->manager.shouldCull(copy)) {
				return;
			}
		}
		draw();
	}

	virtual void drawBoundingBox() {
		glColor4d(0, 1, 0, 0.15);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

		Point bottomLeftBack = box->getCorner(BoundingBox::BOTTOM_LEFT_BACK);
		Point bottomLeftFront = box->getCorner(BoundingBox::BOTTOM_LEFT_FRONT);
		Point bottomRightBack = box->getCorner(BoundingBox::BOTTOM_RIGHT_BACK);
		Point bottomRightFront = box->getCorner(BoundingBox::BOTTOM_RIGHT_FRONT);
		Point topLeftBack = box->getCorner(BoundingBox::TOP_LEFT_BACK);
		Point topLeftFront = box->getCorner(BoundingBox::TOP_LEFT_FRONT);
		Point topRightBack = box->getCorner(BoundingBox::TOP_RIGHT_BACK);
		Point topRightFront = box->getCorner(BoundingBox::TOP_RIGHT_FRONT);

		glPushMatrix();
			glTranslated(transformation->position().x, transformation->position().y, transformation->position().z);
			glBegin(GL_QUADS);
				// front
				glVertex3f(bottomLeftFront.x, bottomLeftFront.y, bottomLeftFront.z);
				glVertex3f(bottomRightFront.x, bottomRightFront.y, bottomRightFront.z);
				glVertex3f(topRightFront.x, topRightFront.y, topRightFront.z);
				glVertex3f(topLeftFront.x, topRightFront.y, topRightFront.z);

				// back
				glVertex3f(topLeftBack.x, topLeftBack.y, topLeftBack.z);
				glVertex3f(topRightBack.x, topRightBack.y, topRightBack.z);
				glVertex3f(bottomRightBack.x, bottomRightBack.y, bottomRightBack.z);
				glVertex3f(bottomLeftBack.x, bottomLeftBack.y, bottomLeftBack.z);

				// top
				glVertex3f(topLeftFront.x, topLeftFront.y, topLeftFront.z);
				glVertex3f(topRightFront.x, topRightFront.y, topRightFront.z);
				glVertex3f(topRightBack.x, topRightBack.y, topRightBack.z);
				glVertex3f(topLeftBack.x, topLeftBack.y, topLeftBack.z);

				// bottom
				glVertex3f(bottomLeftBack.x, bottomLeftBack.y, bottomLeftBack.z);
				glVertex3f(bottomRightBack.x, bottomRightBack.y, bottomRightBack.z);
				glVertex3f(bottomRightFront.x, bottomRightFront.y, bottomRightFront.z);
				glVertex3f(bottomLeftFront.x, bottomLeftFront.y, bottomLeftFront.z);

				// bottom
				glVertex3f(bottomLeftBack.x, bottomLeftBack.y, bottomLeftBack.z);
				glVertex3f(bottomRightBack.x, bottomRightBack.y, bottomRightBack.z);
				glVertex3f(bottomRightFront.x, bottomRightFront.y, bottomRightFront.z);
				glVertex3f(bottomLeftFront.x, bottomLeftFront.y, bottomLeftFront.z);

				// left
				glVertex3f(bottomLeftBack.x, bottomLeftBack.y, bottomLeftBack.z);
				glVertex3f(bottomLeftFront.x, bottomLeftFront.y, bottomLeftFront.z);
				glVertex3f(topLeftFront.x, topLeftFront.y, topLeftFront.z);
				glVertex3f(topLeftBack.x, topLeftBack.y, topLeftBack.z);

				// right
				glVertex3f(topRightBack.x, topRightBack.y, topRightBack.z);
				glVertex3f(topRightFront.x, topRightFront.y, topRightFront.z);
				glVertex3f(bottomRightFront.x, bottomRightFront.y, bottomRightFront.z);
				glVertex3f(bottomRightBack.x, bottomRightBack.y, bottomRightBack.z);
			glEnd();
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glColor4d(1, 1, 1, 1);
	}

	virtual void import(ifstream & input, vector<Texture *> & textures) { }

public:
	DualTransformation * transformation;

protected:
	BoundingBox * box;
};

#endif // OBJECT_H
