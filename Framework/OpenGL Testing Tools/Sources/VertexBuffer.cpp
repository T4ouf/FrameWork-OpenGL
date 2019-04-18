#include "VertexBuffer.h"
#include "Macros.h"

#include <iostream>

VertexBuffer::VertexBuffer(const void* donnees, unsigned int taille){

	//On génère un buffer (il va mettre l'indice du buffer dans la variable buffer)
	GLCALL(glGenBuffers(1, &m_RendererID));
	//On lui précise ce que l'on va utiliser (ici un simple tableau), On lie notre buffer sur lequel on va travailler
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	//On le remplit de données
	GLCALL(glBufferData(GL_ARRAY_BUFFER, taille, donnees, GL_STATIC_DRAW));

}

VertexBuffer::~VertexBuffer(){

	GLCALL(glDeleteBuffers(1, &m_RendererID));

}

void VertexBuffer::Bind() const{

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

}

void VertexBuffer::UnBind() const{

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
