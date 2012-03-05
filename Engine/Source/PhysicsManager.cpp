#include "PhysicsManager.h"

PhysicsManager::PhysicsManager() : m_system(NULL), m_scene(NULL), m_initialized(false) {
	
}

PhysicsManager::~PhysicsManager() {
	if(m_scene != NULL) { m_scene->release(); }
	if(m_system != NULL) { m_system->release(); }
}

bool PhysicsManager::init() {
	if(m_initialized) { return false; }

	m_system = PxCreatePhysics(PX_PHYSICS_VERSION, defaultAllocatorCallback, gameErrorCallback, PxTolerancesScale(), true);
	if(m_system == NULL) { return false; }
	if(!PxInitExtensions(*m_system)) { return false; }

	if(!initScene()) {
		m_system->release();
		m_system = NULL;
		return false;
	}

	m_initialized = true;

	return true;
}

bool PhysicsManager::initScene() {
	if(m_scene != NULL) { m_scene->release(); }
	m_scene = NULL;

	PxSceneDesc sceneDescription(m_system->getTolerancesScale());
	sceneDescription.gravity = PxVec3(0.0f, -9.81f, 0.0f);

	if(sceneDescription.cpuDispatcher == NULL) {
		PxDefaultCpuDispatcher * cpuDispatcher = PxDefaultCpuDispatcherCreate(1);
		if(cpuDispatcher == NULL) { return false; }
		sceneDescription.cpuDispatcher = cpuDispatcher;
	}

	if(sceneDescription.filterShader == NULL) {
		sceneDescription.filterShader = PxDefaultSimulationFilterShader;
	}

	m_scene = m_system->createScene(sceneDescription);
	if(m_scene == NULL) { return false; }

	return true;
}

bool PhysicsManager::isInitialized() const {
	return m_initialized;
}

void PhysicsManager::reset() {
	initScene();
}

PxRigidDynamic * PhysicsManager::createPhysicsBox(const Point & position, const Point & velocity, float width, float height, float depth, float mass) {
	PxRigidDynamic * boxActor = m_system->createRigidDynamic (PxTransform (PxVec3 (position.x, position.y, position.z))); 

	PxMaterial * boxMaterial = m_system->createMaterial(0.5f, 0.5f, 0.1f);
	PxShape * boxShape = boxActor->createShape(PxBoxGeometry(width * 0.5, height * 0.5, depth * 0.5), *boxMaterial); 

	PxReal boxDensity = 1.0;
	boxActor->setMass(mass);
	PxRigidBodyExt::updateMassAndInertia(*boxActor, boxDensity); 
	boxActor->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));

	boxMaterial->release();

	m_scene->addActor(*boxActor);

	return boxActor;
}

void PhysicsManager::update(double timeElapsed) {
	m_scene->simulate(PxReal(timeElapsed));
}

void PhysicsManager::fetchResults(bool block) {
	m_scene->fetchResults(block);
}