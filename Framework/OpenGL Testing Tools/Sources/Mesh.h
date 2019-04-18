#pragma once

#include "glm/glm.hpp"

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Renderer.h"

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh {

private:
	VertexArray* m_VertexArray;
	IndexBuffer* m_IndexBuffer;
	Material* m_Material;


public:

	Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer, Material* material);
	Mesh(const Mesh* mesh);
	~Mesh();
	
	
	inline void SetMaterial(Material* material) { m_Material = material; }
	inline Material* GetMaterial() const { return m_Material; }
	
	void Render(Renderer& renderer);

};
