#pragma once
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "Macros.h"
#include <memory>

class Renderer{

private: 

	static Renderer* s_Instance;

	Renderer();
	~Renderer();

public:

	static Renderer* getInstance();

	//Nettoie les buffer de dessin OpenGL
	void Clear() const;

	//Old method pour les tests
	//méthode qui va s'occuper de dessiner
	//On lui donne des données (VertexArray + IndexBuffer) + des consignes (shader) et il va dessiner
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

	//Old method pour les tests
	//méthode qui va s'occuper de dessiner
	//On lui donne des données (VertexArray + IndexBuffer) + des consignes (Material) et il va dessiner
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Material& material) const;
};