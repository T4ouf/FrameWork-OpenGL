#pragma once

#include "Renderer.h"

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
	//slot param optionnel (0 si non spécifié) qui indique quel slot on souhaite utiliser pour notre texture
	void Bind( unsigned int slot = 0) const;

	//délier notre Texture au renderer
	void UnBind() const;

	inline int GetLargeur() const { return m_Largeur; }
	inline int GetHauteur() const { return m_Hauteur; }
};