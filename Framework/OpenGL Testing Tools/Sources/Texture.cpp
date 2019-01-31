#include "Texture.h"

#include "stb_image\stb_image.h"


Texture::Texture(const std::string & chemin) :
	m_RendererID(0), m_CheminFichier(chemin), m_BufferLocal(nullptr), 
	m_Largeur(0), m_Hauteur(0), m_BPP(0){
	

	//On spécifie que l'on veut lire l'image en partant du bas
	//(0,0) en OpenGL est en bas à gauche
	//(0,0à en général est en haut à gauche
	stbi_set_flip_vertically_on_load(1);

	//On charge l'image...
	//		chemin	, largeur (va être MAJ), hauteur (va être MAJ), BPP (va être MAJ), nb de canal (=4 => RGBA)
	m_BufferLocal = stbi_load(chemin.c_str(), &m_Largeur, &m_Hauteur, &m_BPP, 4);

	//On génère la texture et on la bind
	GLCALL(glGenTextures(1, &m_RendererID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//SUIVANT : 4 PARAM OBLIGATOIRES POUR LES TEXTURES (sinon noir)

	//On explique à OpenGL quoi faire si on redimensionne l'image (par rapport à sa taille d'origine)
	// => interprétation linéaire des pixels
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//On précise à OpenGL que l'on ne veut pas étirer nos textures (laisse du vide)
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//On donne nos données à OpenGL pour qu'il charge dans le GPU la texture (et soit prêt à l'utiliser)
	//glTexImage2D(Ce que l'on veut comme texture, niv de la texture, 
	// le format dans lequel OpenGL va stocker la texture (ici 8 bit par channel), hauteur, largeur, bordure (en px),
	// Le format dans lequel sont nos données actuellement, le type de données que l'on donne, les données) 			
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, 
		GL_RGBA8, m_Largeur, m_Hauteur, 0, 
		GL_RGBA, GL_UNSIGNED_BYTE, m_BufferLocal));

	//Une fois envoyée au GPU, on unbind notre texture d'openGL
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));


	//Si on veut faire du traitement d'image, on ne doit pas free nos données 
	//(on fera le process sur le buffer local)

	//Ici on ne va pas faire de process sur l'image => on libère la mémoire du CPU de l'image...
	//Si on a des données dans le buffer local
	if (m_BufferLocal) {
		//on libère ces données de notre CPU
		stbi_image_free(m_BufferLocal);
	}
}

Texture::~Texture(){

	//destruction de notre texture par openGL
	GLCALL(glDeleteTextures(1, &m_RendererID));

}

void Texture::Bind(unsigned int slot) const {

	GLCALL(glActiveTexture(GL_TEXTURE0 + slot));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::UnBind() const {

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));

}
