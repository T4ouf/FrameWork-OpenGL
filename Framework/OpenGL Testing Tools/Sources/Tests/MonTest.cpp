#include "MonTest.h"

//Pour les calculs matriciels
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/**
* Constructeur de MonTest 
* C'est ici que l'initialise tout ce dont notre application aura besoin
*/
test::MonTest::MonTest():
	//On initialise les matrices de vue et de projection
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_Vue(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {

	//Tableau contenant les positions des points (4 pour un rectangle) 
	float positions[] = {
		100.0f, 100.0f,	//0
		200.0f, 100.0f, //1
		200.0f, 200.0f, //2
		100.0f, 200.0f  //3
	};

	//Pour éviter la redondance (créer 2x ou plus le même point) on crée un IndiceBuffer (tab d'indice)
	//qui va stocker les indices des points à tracer
	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	//On crée le VertexArray
	m_VAO = std::make_unique<VertexArray>();

	//On stocke nos données (les positions)
	//										nb de pts * nombre de floats/pt * size(float)
	m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 2 * sizeof(float));

	//Définition de notre layout pour notre VertexArray
	VertexBufferLayout layout;
	//On utilse 2 floats par points (positions) => layout regroupe ce qui va ensemble
	//layout location 0 (cf vertex shader)
	layout.Push<float>(2);

	//On regroupe tout dans notre vertex Array (on donne du sens à nos données)
	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	//On stocke et lie à l'index buffer les indices
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	//Création du shader
	m_Shader = std::make_unique<Shader>("Ressources/Shaders/MonShader.shader");

	m_Shader->Bind();

	//Uniform --> envoi de données du CPU vers les shaders pour être utilisé comme une variable dans le shader
	//Utilisation uniform après bind avec le shader qui utilise l'uniform
	//Uniform appelé à chaque dessin (draw call)
	m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);
}

/**
* Destructeur de MonTest (c'est normalement ici que l'on libère la mémoire)
*/
test::MonTest::~MonTest(){

	//Rien à faire ici => smarts pointeurs (unique_ptr) sont détruits lorsqu'ils sont hors de portée

}

/**
* Fonction de mise à jour (si l'on veut faire des choses qui dépendent du temps)
* Pas utilisée dans ce test...
*/
void test::MonTest::OnUpdate(float delta){

}

/**
* Fonction qui dessine (fait le rendu) de ce que l'on souhaite afficher
* Appelée à chaque dessin dans la boucle de rendu (rendering loop)
*/
void test::MonTest::OnRender(){

	GLCALL(glClearColor(0.0f,0.0f,0.0f, 1.0f));
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	//Ici notre renderer ne stocke rien => on peut se permettre de le créer à chaque frame
	Renderer renderer;

	//Dessin 1
	{
		//Definition d'une matrice du modèle => Considère toutes les transformations 
		//aka translation, rotation, scaling
		//à appliquer à notre modèle...
		//Ici on le déplace vers en haut à droite
		glm::mat4 modele = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));

		//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
		// /!\ ORDRE IMPORTANT ICI /!\ 
		glm::mat4 mvp = m_Proj * m_Vue * modele;

		//Seul moyen de ne pas gérer les uniform à la main => materials
		//materials = shader + des donnees utiles au dessin
		m_Shader->Bind();

		//On envoit la matrice de projection au shader
		m_Shader->SetUniformMat4f("u_MVP", mvp);

		//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

}

void test::MonTest::OnImGuiRender(){

}
