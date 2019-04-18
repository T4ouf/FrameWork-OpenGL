#include "Renderer.h"

Renderer* Renderer::s_Instance = nullptr;

Renderer::Renderer(){

}

Renderer::~Renderer(){
}

Renderer* Renderer::getInstance(){

	if (s_Instance == nullptr) {
		s_Instance = new Renderer();
	}

	return s_Instance;
}

void Renderer::Clear() const{

	GLCALL(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

	//On utilise notre shader
	shader.Bind();
	//Liaison de notre vertexArray avec le context courant (pour pouvoir l'utiliser)
	va.Bind();
	//On lie nos indices au renderer
	ib.Bind();


	//On ne dessine plus un tableau mais des éléments du tableau:
	//				primitive, nb d'indices, type, index buffer (if not bound)
	GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Material & material) const{

	//On utilise notre shader
	material.Bind();
	//Liaison de notre vertexArray avec le context courant (pour pouvoir l'utiliser)
	va.Bind();
	//On lie nos indices au renderer
	ib.Bind();


	//On ne dessine plus un tableau mais des éléments du tableau:
	//				primitive, nb d'indices, type, index buffer (if not bound)
	GLCALL(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));
}
