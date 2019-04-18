#include "TestMaterial.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>


namespace test {

	// settings
	const unsigned int SCR_WIDTH3 = 960;
	const unsigned int SCR_HEIGHT3 = 540;

	// camera
	Camera camera3(glm::vec3(50.0f, 300.0f, 500.0f));
	float lastX3 = SCR_WIDTH3 / 2.0f;
	float lastY3 = SCR_HEIGHT3 / 2.0f;
	bool firstMouse3 = true;

	// timing
	float deltaTime3 = 0.0f;	// time between current frame and last frame
	float lastFrame3 = 0.0f;


	void processInput3(GLFWwindow *window);

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	void framebuffer_size_callback3(GLFWwindow* window, int width, int height);


	// glfw: whenever the mouse moves, this callback is called
	void mouse_callback3(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	void scroll_callback3(GLFWwindow* window, double xoffset, double yoffset);


	TestMaterial::TestMaterial()
		: m_CouleurLumiere{ 1.0f,1.0f,1.0f,1.0f },
		m_Proj(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 2000.0f)),
		m_Vue(glm::mat4(1.0f)),	m_angle(0.0f), m_TranslationB(300.0f, 50.0f, -250.0f){

		m_Vue = glm::translate(m_Vue, glm::vec3(0.0f, 0.0f, -300.0f));


		GLFWwindow* fen = glfwGetCurrentContext();

		//On initialise nos fonctions liées aux captures d'évènements (resized windows, mouse, etc.)
		glfwSetFramebufferSizeCallback(fen, framebuffer_size_callback3);
		glfwSetCursorPosCallback(fen, mouse_callback3);
		glfwSetScrollCallback(fen, scroll_callback3);

		// Demande à GLFW de capturer la souris (pour la caméra)
		glfwSetInputMode(fen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Ici aucun partage des points 36 points (6 faces * 2 triangles * 3 points chaque).
		//Coords puis UV puis Normales
		float positions2[] = {

			-50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,	 1.0f, 0.0f,
			50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
			50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,	 1.0f, 1.0f,
			-50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
			50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,	 1.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,	 1.0f, 1.0f,
			50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,	 1.0f, 1.0f,
			-50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

			-50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-50.0f,  50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,
			50.0f,  50.0f, -50.0f,  1.0f,  0.0f,  0.0f,	 1.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,	 0.0f, 1.0f,
			50.0f, -50.0f,  50.0f,  1.0f,  0.0f,  0.0f,	 0.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,	 1.0f, 0.0f,

			-50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,	  1.0f, 1.0f,
			50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,	  1.0f, 0.0f,
			50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,	  1.0f, 0.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,	 1.0f, 1.0f,
			50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,	 1.0f, 0.0f,
			-50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f

		};

			
		unsigned int indices2[] = {

			0,1,2,	//Devant
			3,4,5,

			6,7,8,	//Gauche
			9,10,11,

			12,13,14,	//Derrière
			15,16,17,

			18,19,20,	//Dessous
			21,22,23,

			24,25,26,	//Droite
			27,28,29,

			30,31,32,	//Dessus
			33,34,35

		};



		//Tableau contenant les positions des points (8 pour un cube) et leur UV coords (2) 
		//Ici, on partage tous les points au max (avec chaque face)
		float LampPositions[] = {

			-50.0f, -50.0f, 50.0f,	//0
			50.0f,	-50.0f, 50.0f,	//1
			50.0f,	 50.0f, 50.0f,	//2
			-50.0f,  50.0f, 50.0f,	//3

			-50.0f, -50.0f,-50.0f,	//4
			50.0f,	-50.0f, -50.0f,	//5
			50.0f,	 50.0f, -50.0f,	//6
			-50.0f,  50.0f, -50.0f	//7

		};

		//Pour éviter la redondance (créer 2x ou plus le même point) on crée un IndiceBuffer (tab d'indice)
		//qui va stocker les indices des points à tracer
		unsigned int LampIndices[] = {

			0,1,2,	//Devant
			2,3,0,

			0,4,3,	//Gauche
			3,7,4,

			4,5,6,	//Derrière
			6,7,4,

			4,5,0,	//Dessous
			0,1,5,

			5,1,6,	//Droite
			6,2,5,

			6,2,3,	//Dessus
			3,7,6

		};

		//On autorise et paramètre la transparence
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//On active le test de profondeur (z-buffer test)
		glEnable(GL_DEPTH_TEST);
		//On accepte que les pixels qui sont devant (les autres ne seront pas dessinés par dessus)
		glDepthFunc(GL_LESS);

		m_VAO = std::make_unique<VertexArray>();

		//On stocke nos données (les positions)
		//						nb de pts * nombre de floats/pt * size(float)
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions2, 36 * 8 * sizeof(float));
		//Définition de notre layout pour notre VertexArray
		VertexBufferLayout layout;

		//On utilse 3 floats par points (positions) => layout regroupe ce qui va ensemble
		//layout location 0 (cf vertex shader)
		layout.Push<float>(3);

		// On utilise 3 points par Normales
		//layout location 2 (cf vertex shader)
		layout.Push<float>(3);

		// On utilise 2 points par UV coords
		//layout location 1 (cf vertex shader)
		layout.Push<float>(2);

		//layout.Push<float>(3); si on utilse 3 floats par points (positions en 3D)
		//layout.Push<float>(3); si on utilse 3 floats par normale (Normales en 3D)
		//layout.Push<float>(2); si on utilse 2 floats par UV coord (UV coord pour texture)
		//etc..

		//On regroupe tout dans notre vertex Array (on donne du sens à nos données)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//On stocke et lie à l'index buffer les indices
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices2, 36);
		
		m_Texture = new Texture("Ressources/Textures/texture-caisse.png");

		//Création du shader
		m_Shader = new Shader("Ressources/Shaders/Material.shader");

		//Material : Gold PRESQUE OK ! => manque texture
		Gold = new Material(m_Shader, m_Texture,
			glm::vec3(0.329412f, 0.223529f, 0.027451f),
			glm::vec3(0.780392f, 0.568627f, 0.113725f),
			glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f);

		Ruby = new Material(m_Shader, nullptr,
			glm::vec3(0.1745f, 0.01175f, 0.01175f),
			glm::vec3(0.61424f, 0.04136f, 0.04136f),
			glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f * 128);

		Chrome = new Material(m_Shader, nullptr,
			glm::vec3(0.25f, 0.25f, 0.25f),
			glm::vec3(0.4f, 0.4f, 0.4f),
			glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f * 128);

		Pearl = new Material(m_Shader, nullptr,
			glm::vec3(0.25f, 0.20725f, 0.20725f),
			glm::vec3(1.0f, 0.829f, 0.829f),
			glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f * 128);

		Copper = new Material(m_Shader, nullptr,
			glm::vec3(0.19125f, 0.0735f, 0.0225f),
			glm::vec3(0.7038f, 0.27048f, 0.0828f),
			glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f * 128);

		Obsidian = new Material(m_Shader, nullptr,
			glm::vec3(0.05375f, 0.05f, 0.06625f),
			glm::vec3(0.18275f, 0.17f, 0.22525f),
			glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f * 128);

		BlackRubber = new Material(m_Shader, nullptr,
			glm::vec3(0.02f, 0.02f, 0.02f),
			glm::vec3(0.01f, 0.01f, 0.01f),
			glm::vec3(0.4f, 0.4f, 0.4f), 10.0f);

		//On unbind tout...
		m_Shader->UnBind();
		m_IndexBuffer->UnBind();
		m_VertexBuffer->UnBind();
		m_VAO->UnBind();

			
		m_LampVAO = std::make_unique<VertexArray>();

		m_LampVB = std::make_unique<VertexBuffer>(LampPositions, 8 * 3 * sizeof(float));

		//Définition de notre layout pour notre VertexArray
		VertexBufferLayout LampLayout;

		//On utilse 3 floats par points (positions) => layout regroupe ce qui va ensemble
		//layout location 0 (cf vertex shader)
		LampLayout.Push<float>(3);

		m_LampVAO->AddBuffer(*m_LampVB, LampLayout);

		//On stocke et lie à l'index buffer les indices
		m_LampIB = std::make_unique<IndexBuffer>(LampIndices, 36);

		//Shader de la lampe
		m_LampShader = std::make_unique<Shader>("Ressources/Shaders/SourceLumiere.shader");
			
		m_LampShader->UnBind();
		m_LampIB->UnBind();
		m_LampVB->UnBind();
		m_LampVAO->UnBind();
			
		/*
		//Material : ruby PRESQUE OK ! => manque texture
		m_Material = std::make_unique<Material>(m_Shader,m_Texture,
												glm::vec3(0.1745f, 0.01175f, 0.01175f),
												glm::vec3(0.61424f, 0.04136f, 0.04136f),
												glm::vec3(0.727811f, 0.626959f, 0.626959f),0.6f*128);
		*/
		//Material : PRESQUE OK ! => manque texture
		m_Material = Ruby;

			
		m_Material->Bind();

		//On spécifie au shader (via un uniform) quel slot de texture on utilise
		//m_Shader->SetUniform1i("u_Texture", 0);
	
	}
	

	TestMaterial::~TestMaterial(){
		delete Gold;
		delete Ruby;
		delete Chrome;
		delete Pearl;
		delete Copper;
		delete Obsidian;
	}

	void TestMaterial::OnUpdate(float delta){

		//Maj du temps pour la camera2
		float currentFrame = glfwGetTime();
		deltaTime3 = 60 * (currentFrame - lastFrame3);
		lastFrame3 = currentFrame;
	}

	void TestMaterial::OnRender(){

		GLCALL(glClearColor(m_ClearCouleur[0], m_ClearCouleur[1], m_ClearCouleur[2], 0.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//Version controlée de la position de la lampe
		//glm::vec3 PositionLampe = m_TranslationB;

		//Lampe qui bouge
//		glm::vec3 PositionLampe(m_TranslationB.x*glm::cos(glfwGetTime()), m_TranslationB.y * glm::cos(glfwGetTime()), m_TranslationB.z * glm::sin(glfwGetTime()));
		glm::vec3 PositionLampe(m_TranslationB.x, m_TranslationB.y, m_TranslationB.z);

		GLFWwindow* fen = glfwGetCurrentContext();
		// input
		// -----
		processInput3(fen);
		
		//Ici notre renderer ne stocke rien => on peut se permettre de le créer à chaque frame
		Renderer* renderer = Renderer::getInstance();


		glm::mat4 vue = camera3.GetViewMatrix();
		//m_Texture->Bind();

		//Dessin 1
		{

			//On unbind tout...
			m_VertexBuffer->Bind();
			m_VAO->Bind();
			m_IndexBuffer->Bind();
			
			m_Material->Bind();
			//m_Shader->Bind();

			//Uniform --> envoi de données du CPU vers les shaders pour être utilisé comme une variable dans le shader
			//Utilisation uniform après bind avec le shader qui utilise l'uniform
			//Uniform appelé à chaque dessin (draw call)
			m_Shader->SetUniform3f("u_ObjectColor", 0.3f, 0.7f, 1.0f);
			m_Shader->SetUniform3f("u_LightColor", m_CouleurLumiere[0], m_CouleurLumiere[1], m_CouleurLumiere[2]);
			m_Shader->SetUniform3f("u_LightPos", PositionLampe.x, PositionLampe.y, PositionLampe.z);
			m_Shader->SetUniform3f("u_ViewPos", camera3.m_position.x, camera3.m_position.y, camera3.m_position.z);


			//Definition d'une matrice du modèle => Considère toutes les transformations 
			//aka translation, rotation, scaling
			//à appliquer à notre modèle...
			//Ici on le déplace vers en haut à droite
			glm::mat4 modele = glm::mat4(1.0f);

			m_Vue = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0.0f, 1.0f, 0.0f));
			m_Vue = glm::translate(m_Vue, m_TranslationB);


			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(0.0f,  100.0f, 0.0f),
				glm::vec3(-100.0f,  -100.0f, 0.0f),
				glm::vec3(-100.0f,  -100.0f, 100.0f),

			};

			for (int i = 0; i < 1; i++) {

				modele = glm::translate(glm::mat4(1.0f), cubePositions[i]);

				//déplace tous les cubes de leur position initiale
				modele = glm::translate(modele, glm::vec3(-50.0f,0.0f,0.0f));

				//float angle = 20.0f * i;
				//modele = glm::rotate(modele, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

				//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
				// /!\ ORDRE IMPORTANT ICI /!\ 
				//glm::mat4 mvp = m_Proj * m_Vue * modele;

				//On envoit la matrice de projection au shader
				m_Shader->SetUniformMat4f("u_ProjectionMat", m_Proj);
				m_Shader->SetUniformMat4f("u_ViewMat", vue);
				m_Shader->SetUniformMat4f("u_ModelMat", modele);

				m_Shader->SetUniformMat4f("u_InverseModelMat", glm::inverse(modele));

				m_Material->Bind();

				//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
				renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Material);
			}

			m_Shader->UnBind();
			m_IndexBuffer->UnBind();
			m_VertexBuffer->UnBind();
			m_VAO->UnBind();

		}
		
		

		//Dessin 2
		{


			m_LampVB->Bind();
			m_LampVAO->Bind();
			m_LampIB->Bind();
			m_LampShader->Bind();

			m_LampShader->SetUniform4f("u_LightColor", m_CouleurLumiere[0], m_CouleurLumiere[1],
				m_CouleurLumiere[2], m_CouleurLumiere[3]);

			glm::mat4 modele = glm::mat4(1.0f);

			//BUG LE SCALE N'A PAS LIEU...
			//=>Scale à la main...
			modele = glm::mat4(0.5f)*modele;
			modele = glm::translate(glm::mat4(1.0f), PositionLampe);

			//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
			// /!\ ORDRE IMPORTANT ICI /!\ 
			glm::mat4 mvp = m_Proj * vue * modele;

			//On envoit la matrice de projection au shader
			m_LampShader->SetUniformMat4f("u_MVP", mvp);

			//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
			renderer->Draw(*m_LampVAO, *m_LampIB, *m_LampShader);

			m_LampShader->UnBind();
			m_LampIB->UnBind();
			m_LampVB->UnBind();
			m_LampVAO->UnBind();
		}

	}

	void TestMaterial::OnImGuiRender(){
		ImGui::ColorEdit3("Couleur du Vide", m_ClearCouleur);

		ImGui::ColorEdit3("Couleur de la lumiere", m_CouleurLumiere);

		//Creation du contenu de la fenêtre imGui

		//ImGui::SliderFloat("Rotation camera2", &m_angle, 0.0f, 2 * glm::pi<float>());

		ImGui::SliderFloat3("Position lumiere", &m_TranslationB.x, -1500.0f, 1500.0f);

		if (ImGui::Button("gold")){
			m_Material = Gold;
		}
		else if(ImGui::Button("ruby")){
			m_Material = Ruby;
		}
		else if (ImGui::Button("chrome")) {
			m_Material = Chrome;
		}
		else if (ImGui::Button("pearl")) {
			m_Material = Pearl;
		}
		else if (ImGui::Button("copper")) {
			m_Material = Copper;
		}
		else if (ImGui::Button("obsidian")) {
			m_Material = Obsidian;
		}
		else if (ImGui::Button("black rubber")) {
			GLCALL(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
			GLCALL(glClear(GL_COLOR_BUFFER_BIT));
			m_Material = BlackRubber;
		}


	}




	// Execute des fonctions selon la touche presse :
	// Demande à GLFW si une touche qui nous interesse est pressé et agit en consequence
	void processInput3(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//on quitte le mode camera2
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(window, NULL);
		}


		//on rentre en mode camera
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouse_callback3);
		}


		//ZQSD mode
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera3.ProcessKeyboard(FORWARD, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera3.ProcessKeyboard(BACKWARD, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera3.ProcessKeyboard(LEFT, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera3.ProcessKeyboard(RIGHT, deltaTime3);

		//Mode fleches
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera3.ProcessKeyboard(FORWARD, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera3.ProcessKeyboard(BACKWARD, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera3.ProcessKeyboard(LEFT, deltaTime3);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera3.ProcessKeyboard(RIGHT, deltaTime3);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback3(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void mouse_callback3(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse3)
		{
			lastX3 = xpos;
			lastY3 = ypos;
			firstMouse3 = false;
		}

		float xoffset = xpos - lastX3;
		float yoffset = lastY3 - ypos; // reversed since y-coordinates go from bottom to top

		lastX3 = xpos;
		lastY3 = ypos;

		camera3.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback3(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera3.ProcessMouseScroll(yoffset);
	}

}