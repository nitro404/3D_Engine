#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>
#include <PxExtensionsAPI.h>
#include <PxScene.h>
#include <PxDefaultAllocator.h>
#include <PxDefaultErrorCallback.h>
#include <PxRigidDynamic.h>
#include "Point.h"

using namespace physx;

class GameErrorCallback : public PxErrorCallback {
public:
	GameErrorCallback() { }
	~GameErrorCallback() { }

	virtual void reportError(PxErrorCode::Enum code, const char * message, const char * file, int line) { }
};

class PhysicsManager {
public:
	PhysicsManager();
	~PhysicsManager();

	bool init();
	bool initScene();
	bool isInitialized() const;
	void reset();

	PxRigidDynamic * createPhysicsBox(const Point & position, const Point & velocity, float width, float height, float depth, float mass);

	void update(double timeElapsed);
	void fetchResults(bool block = false);

private:
	PxPhysics * m_system;
	PxScene * m_scene;
	PxDefaultAllocator defaultAllocatorCallback;
	PxDefaultErrorCallback defaultErrorCallback;
	GameErrorCallback gameErrorCallback;
	bool m_initialized;
};

#endif // PHYSICS_MANAGER_H
