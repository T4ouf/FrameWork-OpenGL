#pragma once


class VertexBuffer {

private:
	//Id du renderer (comme tout objet en OpenGL, le renderer a un ID)
	unsigned int m_RendererID;

public:

	//Constructeur du VertexBuffer => on lui donne des données et leur taille (en octet)
	VertexBuffer(const void* donnees, unsigned int taille);
	~VertexBuffer(); //Destructeur du Vertex Buffer


	//Pour lier notre VertexBuffer au renderer
	void Bind() const;

	//délier notre VertexBuffer au renderer
	void UnBind() const;
};