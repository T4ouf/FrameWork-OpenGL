#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "Tests/Test.h"
#include "Tests/TestClearCouleur.h"
#include "Tests/TestTexture2D.h"
#include "Tests/Test3D.h"
#include "Tests/TestIB3D.h"
#include "Tests/TestLumiere.h"
#include "Tests/MonTest.h"
#include "Tests/TestMaterial.h"
#include "Tests/TestImGui.h"
//#include "Tests/TestPhysics.h"
//#include "Tests/TestMovement.h"
#include "Tests/TestMesh.h"
#include "Tests/TestScene.h"
#include "Tests/TestGame.h"

#include "Application.h"

using namespace std;

int main() {

	//On vérifie que l'on a réussi à initialiser glfw
	if (glfwInit() == false) {

		cerr << "ERREUR INITIALISATION GLFW !";
		return -1;
	}

	//Avec les glfwWindowHint, on def les paramètre pour la prochaine création de fenetre

	//4 AA
	glfwWindowHint(GLFW_SAMPLES,4);

	
	//def la version de OpenGL utilisée: ici 3.3 (Modern OpenGL)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	

	//vérifie que openGL version >= 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	


	//Création d'une fenêtre OpenGL
	GLFWwindow* fen;
	//					largeur, hauteur,   titre
	fen = glfwCreateWindow(960,   540,    "Ma 4e fenetre", NULL, NULL);

	
	//On vérifie que la fenêtre s'est bien créée:
	//Si il y a eu un problème:
	if (!fen) {
		cerr << "ERREUR DANS LA CREATION DE LA FENETRE !";
		glfwTerminate();	//On termine le processus
		return -1;

	}

	//On met notre fenêtre comme context principal
	glfwMakeContextCurrent(fen);

	//on def la freq de rafraichissement
	// 1 = frequence de rafraichissement du moniteur (60FPS)
	glfwSwapInterval(1);

	
	//autorise les versions les plus récentes de openGL
	glewExperimental = true;

	// /!\initialise glew APRES l'avoir lié au context/!\ 
	if (glewInit() != GLEW_OK) {

		cerr << "ERREUR INITIALISATION DE GLEW !";
		glfwTerminate();	//On termine le processus
		return -1;

	}


	cout << "Version de OpenGL : " << glGetString(GL_VERSION) << endl;

	{
		
		//On autorise et paramètre la transparence
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		
		//On active le test de profondeur (z-buffer test)
		glEnable(GL_DEPTH_TEST);
		//On accepte que les pixels qui sont devant (les autres ne seront pas dessinés par dessus)
		glDepthFunc(GL_LESS);
		

		Renderer* renderer = Renderer::getInstance();

		// Prépare à l'utilisation de ImGui
		ImGui::CreateContext();

		//Initialise ImGui
		ImGui_ImplGlfwGL3_Init(fen, true);

		// Choix du style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		//On stocke le test en cours
		test::Test* TestEnCours = nullptr;

		test::TestMenu* menuTest = new test::TestMenu(TestEnCours);

		//Le premier test en cours est le menu
		TestEnCours = menuTest;

		//On ajoute les tests disponibles au menu des tests
		menuTest->AjouteTest<test::MonTest>("Mon Test");
		menuTest->AjouteTest<test::TestClearCouleur>("Couleur du Vide");
		menuTest->AjouteTest<test::TestTexture2D>("Texture 2D");
		menuTest->AjouteTest<test::Test3D>("3D");
		menuTest->AjouteTest<test::TestIB3D>("Cube IndexBuffer");
		menuTest->AjouteTest<test::TestLumiere>("Lumiere et Illuminations");
		menuTest->AjouteTest<test::TestMaterial>("Materials");
		menuTest->AjouteTest<test::TestImGui>("Interface");
		//menuTest->AjouteTest<testPhysics::TestPhysics>("Physique");
		//menuTest->AjouteTest<testMovement::TestMovement>("Mouvements");
		menuTest->AjouteTest<test::TestMesh>("Mesh");
		menuTest->AjouteTest<test::TestScene>("Scene");
		menuTest->AjouteTest<testGame::TestGame>("Jeu");

		
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		float incR = 0.002f;
		float incB = 0.005f;
		float incG = 0.007f;

		//Boucle pour afficher la fenetre
		while (!glfwWindowShouldClose(fen)) {

			GLCALL(glClearColor(r, g, b, 1.0f));
			GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			//Avant le dessin on nettoie tout
			renderer->Clear();

			//Petite animation colorée sur le menu principal
			if (TestEnCours == menuTest) {
				if (r >= 1) {
					incR = -incR;
				}
				if (r < 0) {
					incR = -incR;
				}
				if (b >= 1) {
					incB = -incB;
				}
				if (b < 0) {
					incB = -incB;
				}
				if (g >= 1) {
					incG = -incG;

				}
				if (g < 0) {
					incG = -incG;
				}

				r += incR + incB - incG;
				b += incR - incB + incG;
				g += -incR + incB + incG;
			}
	
			//Ouvre une fenêtre ImGui
			ImGui_ImplGlfwGL3_NewFrame();

			//Si il y a un test en cours (non null)
			if (TestEnCours) {

				TestEnCours->OnUpdate(0.0f);
				TestEnCours->OnRender();

				ImGui::Begin("Test");

				//Si on a appuyé sur la flèche pour revenir au menu des tests
				if (TestEnCours != menuTest && ImGui::Button("<-")) {

					delete TestEnCours;
					TestEnCours = menuTest;

				}


				TestEnCours->OnImGuiRender();
				ImGui::End();


			}

			//Dessine l'interface ImGUI
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			//On echange les framesBuffers 
			glfwSwapBuffers(fen);


			glfwPollEvents();

		}

		//On libère notre mémoire proprement
		delete TestEnCours;
		if (TestEnCours != menuTest) {
			delete menuTest;
		}
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	return 0;
}

