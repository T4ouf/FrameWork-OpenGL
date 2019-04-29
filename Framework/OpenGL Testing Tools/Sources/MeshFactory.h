#pragma once
#include "Material.h"
#include "Mesh.h"

#include <vector>

namespace MeshFactory {
	
	/* MODEL LOADING (not done yet)
	namespace {
		Vertex* SphereData = nullptr;
		unsigned int sphereVertices = 0;
	}
	void LoadOBJ(std::string path, std::vector<Vertex>& data);
	*/

	Mesh* CreateQuad(float x, float y, float width, float height, Material* material);
	Mesh* CreateQuad(const glm::vec2& position, const glm::vec2& size, Material* material);

	Mesh* CreateCube(float size, Material* material);
	Mesh* CreateBoxe(float Xsize, float Ysize, float Zsize, Material* material);
	Mesh* CreateSphere(float radius, Material* material);


}