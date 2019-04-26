#include "ObjectFactory.h"
#include "Object.h"
#include "MeshFactory.h"
#include "Mesh.h"
#include "PhysicObject.h"

Object* ObjectFactory::CreateCube(glm::vec3 position, float size, float mass, bool isAnchor, Material* material) {
	
	Mesh* graphics = MeshFactory::CreateCube(size, material);
	PhysicObject* physic = new PhysicObject(&position, size, size, size, mass, isAnchor);

	return new Object(graphics, physic, position);
}

Object * ObjectFactory::CreateBoxe(glm::vec3 position, float xSize, float ySize, float zSize, float mass, bool isAnchor, Material* material){

	Mesh* graphics = MeshFactory::CreateBoxe(xSize,ySize,zSize, material);
	PhysicObject* physic = new PhysicObject(&position, xSize, ySize, zSize, mass, isAnchor);

	return new Object(graphics, physic, position);

}
