#pragma once

#include <string>
#include "Macros.h"

class Texture {

private:

	//notre id de texture
	unsigned int m_RendererID;

	std::string m_CheminFichier;
	unsigned char* m_BufferLocal;
	int m_Largeur, m_Hauteur, m_BPP; //Bit Par Pixel
	

public:

	Texture(const std::string& chemin);
	~Texture();

	//Pour lier notre Texture au renderer
	//slot param optionnel (0 si non sp�cifi�) qui indique quel slot on souhaite utiliser pour notre texture
	void Bind( unsigned int slot = 0) const;

	//d�lier notre Texture au renderer
	void UnBind() const;

	inline int GetLargeur() const { return m_Largeur; }
	inline int GetHauteur() const { return m_Hauteur; }
};