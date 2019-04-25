#pragma once

#include <GL/glew.h>
#include <vector>

#include "Macros.h"
#include "glm/glm.hpp"

/**
* Elements composant notre VBLayout (tout ce qu'il faut pour la fct VertexAttribPointer)
*/
struct VertexBufferElement {
	
	unsigned int type; //type de l'�l�ment (unsigned int car type enum OpenGL)
	unsigned int count; //Le nombre d'�lement du type
	
	unsigned char normalise;	//Bool�en qui sp�cifie si le VertexBufferElement est normalis�

	/**
	* Renvoit la taille en octet du type utilis�	
	*/
	static unsigned int GetSizeofType(unsigned int type) {

		switch (type) {
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		//si aucun type ok
		ASSERT(false);
		//alors taille vide
		return 0;

	}

};

class VertexBufferLayout {
private:

	//On stocke les �lements du layout
	std::vector<VertexBufferElement> m_Elements;

	//l'index du layout
	unsigned int m_Stride;

public:
	VertexBufferLayout() :
		m_Stride(0) {};

	//Si les donn�es rentr�es ne sont pas du bon type
	template<typename T>
	void Push(unsigned int compte) {
		static_assert(false);
	}

	//template sp�cialis� par type
	//ici on ajoute la sp�cialisation pour les floats
	template<>
	void Push<float>(unsigned int compte) {

		//on ajoute un element (VertexBufferElement)
		m_Elements.push_back({ GL_FLOAT, compte, GL_FALSE });

		m_Stride += VertexBufferElement::GetSizeofType(GL_FLOAT) * compte;

	}

	//ici on ajoute la sp�cialisation pour les unsigned int
	template<>
	void Push<unsigned int>(unsigned int compte) {

		//on ajoute un element (VertexBufferElement)
		m_Elements.push_back({ GL_UNSIGNED_INT, compte, GL_FALSE });

		m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT) * compte;
	}

	//ici on ajoute la sp�cialisation pour les octets
	template<>
	void Push<unsigned char>(unsigned int compte) {

		//on ajoute un element (VertexBufferElement)
		m_Elements.push_back({ GL_UNSIGNED_BYTE, compte, GL_TRUE});

		m_Stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE) * compte;

	}

	//ici on ajoute la sp�cialisation pour les vec2
	template<>
	void Push<glm::vec2>(unsigned int compte) {

		//on ajoute un element (VertexBufferElement) ici un vec2 (2 floats)
		Push<float>(2*compte);

	}

	//ici on ajoute la sp�cialisation pour les vec3
	template<>
	void Push<glm::vec3>(unsigned int compte) {

		//on ajoute un element (VertexBufferElement) ici un vec3 (3 floats)
		Push<float>(3 * compte);

	}

	inline std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};