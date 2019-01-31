#include "Renderer.h"
#include <iostream>
#include <iomanip>

/*
2 façons de débugger du code OpenGL
1) Old_School : glGetError() renvoit les erreurs que OpenGL a stocké, à chaque appel de fct openGL on vérifie
que tout est ok (glGetError() = GL_NO_ERROR)
Avant l'appel d'une fonction OpenGL on vide le buffer des erreurs (la fonction GLClearError())

2) Moderne (>= OpenGL 4.3) : glDebugMessageCallback(params) => peut effectuer un traitement de l'erreur
en passant une fonction en paramètre + Message d'erreur en Anglais
*/
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

/*
Fonction qui teste un appel OpenGL et qui va afficher l'erreur si il y en a une
*/
bool GLLogCall(const char* fonction, const char* fichier, int ligne) {

	//Tant que l'on a des erreurs OpenGL
	while (GLenum erreur = glGetError()) {
		std::cerr << "[OpenGL Error]\nCode erreur (hexadecimal) : 0X" << std::hex << erreur <<
			std::dec << "\nligne : " << ligne <<
			"\nDans :" << fonction << 
			"\nfichier : " << fichier;
		return false;
	}
	return true;
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
	GLCALL(glDrawElements(GL_TRIANGLES, ib.getCompte(), GL_UNSIGNED_INT, nullptr));
}