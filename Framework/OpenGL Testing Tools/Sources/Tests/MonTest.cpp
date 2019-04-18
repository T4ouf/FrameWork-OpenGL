#include "MonTest.h"

//Pour les calculs matriciels
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Mesh.h"
#include "MeshFactory.h"
#include "Object.h"

/**
* Constructeur de MonTest 
* C'est ici que l'initialise tout ce dont notre application aura besoin
*/
test::MonTest::MonTest():
	//On initialise les matrices de vue et de projection
	m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
	m_Vue(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {

	//Création du shader
	shader = new Shader("Ressources/Shaders/Material.shader");

	//Material : Gold PRESQUE OK ! => manque texture
	Gold = new Material(shader, nullptr,
		glm::vec3(0.329412f, 0.223529f, 0.027451f),
		glm::vec3(0.780392f, 0.568627f, 0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f);


	m_Quad = MeshFactory::CreateQuad(0, 0, 50, 50, Gold);
	m_Cube = MeshFactory::CreateCube(100, Gold);

	/*TODO : A ENLEVER !*/
	Object* o = new Object(nullptr, nullptr, glm::vec3(2.0f, 0.0f,0.0f));
	o->translate(glm::vec3(10.0f,50.0f,25.0f));
	o->translate(glm::vec3(10.0f, -50.0f, -25.0f));
	o->scale(0.5f);
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
	Renderer* renderer = Renderer::getInstance();

	//Dessin 1
	{
		m_Quad->Render(*renderer);

		m_Cube->Render(*renderer);
		
	}

}

void test::MonTest::OnImGuiRender(){

}
