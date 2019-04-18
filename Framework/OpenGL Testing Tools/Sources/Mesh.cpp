#include "Mesh.h"

//Default constructor
Mesh::Mesh(VertexArray* vertexArray, IndexBuffer* indexBuffer, Material* material) : 
	m_VertexArray(vertexArray), m_IndexBuffer(indexBuffer),m_Material(material){

}

Mesh::Mesh(const Mesh* mesh) : 
	m_VertexArray(mesh->m_VertexArray), m_IndexBuffer(mesh->m_IndexBuffer), m_Material(mesh->m_Material) {

}

Mesh::~Mesh(){
	delete m_VertexArray;
	delete m_IndexBuffer;
}

void Mesh::Render(Renderer& renderer){

	m_VertexArray->Bind();
	m_IndexBuffer->Bind();
	m_Material->Bind();
	renderer.Draw(*m_VertexArray,*m_IndexBuffer,*m_Material);
	m_IndexBuffer->UnBind();
	m_VertexArray->UnBind();
	m_Material->UnBind();

}
