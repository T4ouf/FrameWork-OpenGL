#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Macros.h"


VertexArray::VertexArray(){

	//On g�n�re notre Vertex Array avec OpenGL
	GLCALL(glGenVertexArrays(1, &m_RendererID));

}

VertexArray::~VertexArray(){

	GLCALL(glDeleteVertexArrays(1, &m_RendererID));

}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout){

	//On se pr�pare � utiliser notre VertexArray (on le bind donc)
	Bind();

	//On se pr�pare � utiliser le buffer (bind)
	vb.Bind();


	//LA suite du code sert juste � sp�cifier automatiquement le layout que l'on va utiliser pour notre buffer


	//On utilise auto ici car type long � r�ecrire...

	//r�cup�re nos �lement pour pr�parer notre layout
	const auto& elements = layout.GetElements();
	
	unsigned int decalage = 0;

	for (unsigned int i = 0; i<elements.size(); i++) {
		
		//Pour chaque �lement,
		const auto& element = elements[i];

		//D�finition du Layout du buffer
		GLCALL(glEnableVertexAttribArray(i));
		GLCALL(glVertexAttribPointer(i, element.count, element.type,
				element.normalise, layout.GetStride(), (const void*)decalage));

		decalage = element.count * VertexBufferElement::GetSizeofType(element.type);

		}

	
}


//Pour lier notre VertexArray au renderer
void VertexArray::Bind() const {
	GLCALL(glBindVertexArray(m_RendererID));
}

//d�lier notre VertexArray au renderer
void VertexArray::UnBind() const {

	GLCALL(glBindVertexArray(0));

}