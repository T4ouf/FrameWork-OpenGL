#pragma once

#include "VertexBuffer.h"

//juste pour éviter des pb d'inclusion
class VertexBufferLayout;

class VertexArray {

private:

	unsigned int m_RendererID;

public:
	//Constructeur du vertex Array
	VertexArray();

	//Destructeur du vertex array
	~VertexArray();

	//Ajout du Vertex Buffer (les données) et le layout à notre VertexArray
	void AddBuffer( const VertexBuffer& vb, const VertexBufferLayout& layout);

	//Pour lier notre VertexArray au renderer
	void Bind() const;

	//délier notre VertexArray au renderer
	void UnBind() const;
};