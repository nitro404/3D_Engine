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
	PxRigidDynamic * actor = m_system->createRigidDynamic(PxTransform(PxVec3(position.x, position.y, position.z))); 
	actor->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
	actor->setMass(mass);

	PxMaterial * material = m_system->createMaterial(0.5, 0.5, 0.1);
	PxShape * shape = actor->createShape(PxBoxGeometry(width * 0.5, height * 0.5, depth * 0.5), *material); 
	PxRigidBodyExt::updateMassAndInertia(*actor, 1.0); 

	material->release();

	m_scene->addActor(*actor);

	return actor;
}

PxRigidDynamic * PhysicsManager::createSphereMesh(const Point & position, const Point & velocity, float radius, float mass) {
	PxRigidDynamic * actor = m_system->createRigidDynamic(PxTransform(PxVec3(position.x, position.y, position.z)));
	actor->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
	actor->setMass(mass);

	PxMaterial * material = m_system->createMaterial(0.5, 0.5, 0.1);
	PxShape * shape = actor->createShape(PxSphereGeometry(radius), *material);
	PxRigidBodyExt::updateMassAndInertia(*actor, 1.0);

	material->release();

	return actor;
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
			Terrain * t = dynamic_cast<Terrain *>(world.objects[i]);

			PxHeightFieldSample * points = new PxHeightFieldSample[(t->getWidth() + 1) * (t->getHeight() + 1)];

			for(int i=0;i<t->getWidth();i++) {
				for(int j=0;j<t->getHeight();j++) {
					PxHeightFieldSample & point = points[(t->getHeight() - j) + (i * (t->getHeight() + 1))]; // ???
					point.height = (PxI16) t->getPoint(i, j)->y; // ???
					point.clearTessFlag();
					point.materialIndex0 = 0;
					point.materialIndex1 = 0;
				}
			}

			PxHeightFieldDesc d;
			d.format = PxHeightFieldFormat::eS16_TM;
			d.nbColumns = t->getHeight();
			d.nbRows = t->getWidth();
			d.samples.data = points;
			d.samples.stride = sizeof(PxHeightFieldSample);

			PxHeightField * heightField = m_system->createHeightField(d);
			const Point & p = t->transformation->position();
			PxTransform transform = PxTransform(PxVec3(p.x, p.y, p.z)); // ???
			PxRigidStatic * terrainActor = m_system->createRigidStatic(transform);

			PxMaterial * material = m_system->createMaterial(0.5f, 0.5f, 0.1f);
			PxMaterial * materials[] = { material };
			PxShape * terrainShape = terrainActor->createShape(PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), 1.0, t->getTileSizeX(), t->getTileSizeZ()), materials, 1);

			material->release();
			delete [] points;
			heightField->release();

			t->setCollisionMesh(terrainActor);
		}
	}

	if(vertices.size() > 0 && indices.size() > 0) {
		PxTriangleMeshDesc d;
		d.points.count = vertices.size();
		d.triangles.count = indices.size() / 3;
		d.points.stride = sizeof(PxVec3);
		d.triangles.stride = sizeof(PxU32) * 3;
		d.points.data = &vertices[0];
		d.triangles.data = &indices[0];

		PxCooking * cooker = PxCreateCooking(PX_PHYSICS_VERSION, &m_system->getFoundation(), PxCookingParams());
		MemoryWriteBuffer buffer;
		cooker->cookTriangleMesh(d, buffer);
		PxTriangleMesh * triangleMesh = m_system->createTriangleMesh(MemoryReadBuffer(buffer.data));
		PxRigidStatic * triangleMeshActor = m_system->createRigidStatic(PxTransform::createIdentity());
		PxMaterial * worldMaterial = m_system->createMaterial(0.5f, 0.5f, 0.1f);
		PxShape * triangleMeshShape = triangleMeshActor->createShape(PxTriangleMeshGeometry(triangleMesh), *worldMaterial);

		cooker->release();
		worldMaterial->release();

		m_scene->addActor(*triangleMeshActor);

		return triangleMeshActor;
	}

	return NULL;
}

bool PhysicsManager::sweepSphereHits(PxRigidDynamic & sphere, const Point & from, const Point & to, Point & intersection) const {
	if(sphere.getNbShapes() != 1) { return false; }

	PxShape * shapes[1];
	sphere.getShapes(shapes, 1);
	PxSphereGeometry geometry;
	shapes[0]->getSphereGeometry(geometry);

	Point temp = (to - from);
	Point direction = temp.normalized();
	double distance = temp.length();
	PxSweepHit hit;

	if(m_scene->sweepSingle(geometry, PxTransform(PxVec3(from.x, from.y, from.z)), PxVec3(direction.x, direction.y, direction.z), distance, PxSceneQueryFlag::eBLOCKING_HIT, hit)) {
		intersection = from + (direction * (hit.distance / distance));

		return true;
	}
	return false;
}

void PhysicsManager::update(double timeElapsed) {
	m_scene->simulate(PxReal(timeElapsed));
}

void PhysicsManager::fetchResults(bool block) {
	m_scene->fetchResults(block);
}