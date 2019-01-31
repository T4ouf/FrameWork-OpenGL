#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//def d'une macro assert => si erreur : plante le programme sur la ligne d'erreur
// __debugbreak() spécifique au compilateur VS
#define ASSERT(x) if(!(x)) __debugbreak();

/*
Definition d'une macro de test pour nos erreurs OpenGL
Vide les erreurs,
appelle la fonction OpenGL,
Verifie que pas de problème,
*/
#define GLCALL(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x,__FILE__,__LINE__)) //#x est la conversion en string de x

/*
2 façons de débugger du code OpenGL
1) Old_School : glGetError() renvoit les erreurs que OpenGL a stocké, à chaque appel de fct openGL on vérifie
que tout est ok (glGetError() = GL_NO_ERROR)
Avant l'appel d'une fonction OpenGL on vide le buffer des erreurs (la fonction GLClearError())

2) Moderne (>= OpenGL 4.3) : glDebugMessageCallback(params) => peut effectuer un traitement de l'erreur
en passant une fonction en paramètre + Message d'erreur en Anglais
*/
void GLClearError();

/*
Fonction qui teste un appel OpenGL et qui va afficher l'erreur si il y en a une
*/
bool GLLogCall(const char* fonction, const char* fichier, int ligne);


class Renderer {
//Static ou non ? a reflechir...

private: 

public:

	//Nettoie les buffer de dessin OpenGL
	void Clear() const;

	//méthode qui va s'occuper de dessiner
	//On lui donne des données (VertexArray + IndexBuffer) + des consignes (shader) et il va dessiner
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

};