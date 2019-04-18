#pragma once

#include "Object.h"
#include "Material.h"

namespace ObjectFactory {

	Object* CreateCube(glm::vec3 position, float size, float mass, bool isAnchor, Material* material);

}