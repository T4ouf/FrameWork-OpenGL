#pragma once
#include "Material.h"
#include "Mesh.h"

namespace MeshFactory {
	Mesh* CreateQuad(float x, float y, float width, float height, Material* material);
	Mesh* CreateQuad(const glm::vec2& position, const glm::vec2& size, Material* material);

	Mesh* CreateCube(float size, Material* material);
	Mesh* CreateSphere(float radius, Material* material);


}