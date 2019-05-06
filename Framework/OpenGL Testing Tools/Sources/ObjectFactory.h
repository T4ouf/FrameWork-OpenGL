#pragma once

#include "Object.h"
#include "Material.h"

namespace ObjectFactory {

	Object* CreateCube(glm::vec3 position, float size, float mass, bool isAnchor, Material* material, double bounceCoeff = 1.0f);
	Object* CreateBoxe(glm::vec3 position, float xSize, float ySize,float zSize, float mass, bool isAnchor, Material* material, double bounceCoeff = 1.0f);

}