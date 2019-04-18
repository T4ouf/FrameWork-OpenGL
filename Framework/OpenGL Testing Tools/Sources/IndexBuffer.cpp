#include "IndexBuffer.h"
#include "Macros.h"

IndexBuffer::IndexBuffer(const unsigned int* donnees, unsigned int nombreIndice):m_Count(nombreIndice){

	//Pr�caution suppl�mentaire (cf ligne glBufferData)
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	//On g�n�re un buffer (il va mettre l'indice du buffer dans la variable buffer)
	GLCALL(glGenBuffers(1, &m_RendererID));
	//On lui pr�cise ce que l'on va utiliser (ici un simple tableau), On lie notre buffer sur lequel on va travailler
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
						  
	//On le remplit de donn�es (des indices)
	// <!> si pb avec sizeof(unsigned int) => mettre sizeof(GLuint)
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, nombreIndice * sizeof(unsigned int), donnees, GL_STATIC_DRAW));

}

IndexBuffer::~IndexBuffer(){

	GLCALL(glDeleteBuffers(1, &m_RendererID));

}

void IndexBuffer::Bind() const{

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

}

void IndexBuffer::UnBind() const{

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
