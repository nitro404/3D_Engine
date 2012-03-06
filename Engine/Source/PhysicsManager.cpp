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

PxRigidDynamic * PhysicsManager::createBoxMesh(const Point & position, const Point & velocity, float width, float height, float depth, float mass) {
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

PxRigidStatic * PhysicsManager::createWorldMesh(const World & world) {
	vector<PxVec3> vertices;
	vector<PxU32> indices;

	for(unsigned int i=0;i<world.objects.size();i++) {
		if(typeid(*world.objects[i]) == typeid(Geometry)) {
			const Geometry * g = dynamic_cast<const Geometry *>(world.objects[i]);

			for(unsigned int j=0;j<g->faces.size();j++) {
				const Face * f = g->faces[j];
				int l = vertices.size();

				for(unsigned int k=0;k<f->points.size();k++) {
					const GamePoint * p = f->points[k];
					const Point p2 = Point(p->x, p->y, p->z) * (*g->transformation);
					vertices.push_back(PxVec3(p2.x, p2.y, p2.z));
				}

				for(unsigned int k=2;k<f->points.size();k++) {
					indices.push_back((PxU32) l);
					indices.push_back((PxU32) l + k - 1);
					indices.push_back((PxU32) l + k);
				}
			}
		}
		else if(typeid(*world.objects[i]) == typeid(Terrain)) {
			// TODO: add collision mesh for terrain
		}
	}

	PxTriangleMeshDesc d;
	d.points.count = vertices.size();
	d.triangles.count = indices.size() / 3;
	d.points.stride = sizeof(PxVec3);
	d.triangles.stride = sizeof(PxU32) * 3;
	d.points.data = &vertices[0];
	d.triangles.data = &indices[0];

	// TODO: fix cooking
	PxCooking * cooker = PxCreateCooking(PX_PHYSICS_VERSION, &m_system->getFoundation(), PxCookingParams());
	MemoryWriteBuffer buffer;
	cooker->cookTriangleMesh(d, buffer);
	PxTriangleMesh * triangleMesh = m_system->createTriangleMesh(MemoryReadBuffer(buffer.data));
	Transformation t = Transformation();
	PxRigidStatic * triangleMeshActor = m_system->createRigidStatic(PxTransform(*((PxMat44 *) &t)));
	PxMaterial * worldMaterial = m_system->createMaterial(0.5f, 0.5f, 0.1f);
	PxShape * triangleMeshShape = triangleMeshActor->createShape(PxTriangleMeshGeometry(triangleMesh), *worldMaterial);

	cooker->release();
	worldMaterial->release();

	m_scene->addActor(*triangleMeshActor);

	return triangleMeshActor;
}

void PhysicsManager::update(double timeElapsed) {
	m_scene->simulate(PxReal(timeElapsed));
}

void PhysicsManager::fetchResults(bool block) {
	m_scene->fetchResults(block);
}