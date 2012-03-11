#ifndef CAMERA_H
#define CAMERA_H

#include <PxRigidDynamic.h>
#include "Includes.h"
#include "Point.h"
#include "CullingManager.h"

using namespace physx;

class Camera {
public:
	Camera();
	~Camera();

	bool init();

	void toggleFly();
	
	void beginCamera();
	void endCamera();

	Point forwardVector() const;
	
	void moveBy(Point & translation);
	void rotateBy(Point & rotation);

	void reset(Point & position);
	void update(double timeElapsed);
	void handleCollisions(double timeElapsed);

public:
	double xRotation;
	double yRotation;

	bool fly;
	PxRigidDynamic * collisionMesh;

	DualTransformation cameraMatrix;
	DualTransformation lastCameraMatrix;

	bool translateLeft;
	bool translateRight;
	bool translateAhead;
	bool translateBack;
	bool translateUp;
	bool translateDown;

	bool rotateLeft;
	bool rotateRight;
	bool rotateUp;
	bool rotateDown;

	CullingManager manager;

	bool initialized;
};

#endif // CAMERA_H
