#pragma once


class IndexBuffer {

private:
	//Id du renderer (comme tout objet en OpenGL, le renderer a un ID)
	unsigned int m_RendererID;
	unsigned int m_Count; //On Compte le nombre d'indices que stocke le IB

public:

	//Constructeur du IndexBuffer => on lui donne des données (l'id des indices) et leur nombre
	//Ici on suppose que l'on n'aura pas de modèle utilisant plus de 2^32 points (sinon unsigned long)
	IndexBuffer(const unsigned int* donnees, unsigned int nombreIndice);
	~IndexBuffer(); //Destructeur du Vertex Buffer


	//Pour lier notre IndexBuffer au renderer
	void Bind() const;

	//délier notre IndexBuffer au renderer
	void UnBind() const;

	inline unsigned int getCount() const { return m_Count; }

};