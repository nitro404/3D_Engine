#ifndef CAMERA_H
#define CAMERA_H

#include "Includes.h"
#include "Point.h"
#include "CullingManager.h"

class Camera {
public:
	Camera();
	~Camera();
	
	void beginCamera();
	void endCamera();

	Point forwardVector() const;
	
	void moveBy(Point & translation);
	void rotateBy(Point & rotation);

	void reset(Point & position);
	void update(double timeElapsed);

public:
	double xRotation;
	double yRotation;

	DualTransformation cameraMatrix;

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
};

#endif // CAMERA_H
