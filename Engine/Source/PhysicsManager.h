#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <PxPhysicsAPI.h>
#include <PxPhysics.h>
#include <PxExtensionsAPI.h>
#include <PxScene.h>
#include <PxDefaultAllocator.h>
#include <PxDefaultErrorCallback.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxTkStream.h>
#include "World.h"
#include "Point.h"
#include "Terrain.h"

using namespace physx;
using namespace PxToolkit;

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

	PxRigidDynamic * createBoxMesh(const Point & position, const Point & velocity, float width, float height, float depth, float mass);
	PxRigidDynamic * createSphereMesh(const Point & position, const Point & velocity, float radius, float mass);
	PxRigidStatic * createWorldMesh(const World & world);

	bool sweepSphereHits(PxRigidDynamic & sphere, const Point & from, const Point & to, Point & intersection) const;

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
