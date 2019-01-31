#include "TestLumiere.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"




namespace test {

	// settings
	const unsigned int SCR_WIDTH2 = 960;
	const unsigned int SCR_HEIGHT2 = 540;

	// camera
	Camera camera2(glm::vec3(50.0f, 300.0f, 500.0f));
	float lastX2 = SCR_WIDTH2 / 2.0f;
	float lastY2 = SCR_HEIGHT2 / 2.0f;
	bool firstMouse2 = true;

	// timing
	float deltaTime2 = 0.0f;	// time between current frame and last frame
	float lastFrame2 = 0.0f;


	void processInput2(GLFWwindow *window);

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	void framebuffer_size_callback2(GLFWwindow* window, int width, int height);


	// glfw: whenever the mouse moves, this callback is called
	void mouse_callback2(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	void scroll_callback2(GLFWwindow* window, double xoffset, double yoffset);


	TestLumiere::TestLumiere()
		: m_CouleurLumiere{ 1.0f,1.0f,1.0f,1.0f },
		m_Proj(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 2000.0f)),
		m_Vue(glm::mat4(1.0f)),	m_angle(0.0f), m_TranslationB(300.0f, 50.0f, -250.0f){

		m_Vue = glm::translate(m_Vue, glm::vec3(0.0f, 0.0f, -300.0f));


		GLFWwindow* fen = glfwGetCurrentContext();

		//On initialise nos fonctions liées aux captures d'évènements (resized windows, mouse, etc.)
		glfwSetFramebufferSizeCallback(fen, framebuffer_size_callback2);
		glfwSetCursorPosCallback(fen, mouse_callback2);
		glfwSetScrollCallback(fen, scroll_callback2);

		// Demande à GLFW de capturer la souris (pour la caméra)
		glfwSetInputMode(fen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
			//Ici aucun partage des points 36 points (6 faces * 2 triangles * 3 points chaque).
			//Coords puis UV puis Normales
			float positions2[] = {
				-50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,
				50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,
				50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,
				50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,
				-50.0f,  50.0f, -50.0f,  0.0f,  0.0f, -1.0f,
				-50.0f, -50.0f, -50.0f,  0.0f,  0.0f, -1.0f,

				-50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,
				50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,
				50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,
				50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,
				-50.0f,  50.0f,  50.0f,  0.0f,  0.0f,  1.0f,
				-50.0f, -50.0f,  50.0f,  0.0f,  0.0f,  1.0f,

				-50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,
				-50.0f,  50.0f, -50.0f, -1.0f,  0.0f,  0.0f,
				-50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,
				-50.0f, -50.0f, -50.0f, -1.0f,  0.0f,  0.0f,
				-50.0f, -50.0f,  50.0f, -1.0f,  0.0f,  0.0f,
				-50.0f,  50.0f,  50.0f, -1.0f,  0.0f,  0.0f,

				50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,
				50.0f,  50.0f, -50.0f,  1.0f,  0.0f,  0.0f,
				50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,
				50.0f, -50.0f, -50.0f,  1.0f,  0.0f,  0.0f,
				50.0f, -50.0f,  50.0f,  1.0f,  0.0f,  0.0f,
				50.0f,  50.0f,  50.0f,  1.0f,  0.0f,  0.0f,

				-50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,
				50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,
				50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,
				50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,
				-50.0f, -50.0f,  50.0f,  0.0f, -1.0f,  0.0f,
				-50.0f, -50.0f, -50.0f,  0.0f, -1.0f,  0.0f,

				-50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,
				50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f,
				50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,
				50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,
				-50.0f,  50.0f,  50.0f,  0.0f,  1.0f,  0.0f,
				-50.0f,  50.0f, -50.0f,  0.0f,  1.0f,  0.0f

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
			m_VertexBuffer = std::make_unique<VertexBuffer>(positions2, 36 * 6 * sizeof(float));
			//Définition de notre layout pour notre VertexArray
			VertexBufferLayout layout;

			//On utilse 3 floats par points (positions) => layout regroupe ce qui va ensemble
			//layout location 0 (cf vertex shader)
			layout.Push<float>(3);

			// On utilise 2 points par UV coords
			//layout location 1 (cf vertex shader)
			//layout.Push<float>(2);

			// On utilise 3 points par Normales
			//layout location 2 (cf vertex shader)
			layout.Push<float>(3);

			//layout.Push<float>(3); si on utilse 3 floats par points (positions en 3D)
			//layout.Push<float>(3); si on utilse 3 floats par normale (Normales en 3D)
			//layout.Push<float>(2); si on utilse 2 floats par UV coord (UV coord pour texture)
			//etc..

			//On regroupe tout dans notre vertex Array (on donne du sens à nos données)
			m_VAO->AddBuffer(*m_VertexBuffer, layout);

			//On stocke et lie à l'index buffer les indices
			m_IndexBuffer = std::make_unique<IndexBuffer>(indices2, 36);

			//Création du shader
			m_Shader = std::make_unique<Shader>("Ressources/Shaders/Lumiere.shader");

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
			
			//m_Texture = std::make_unique<Texture>("Ressources/Textures/texture-caisse.png");

			//On spécifie au shader (via un uniform) quel slot de texture on utilise
			//m_Shader->SetUniform1i("u_Texture", 0);
	
	}
	

	TestLumiere::~TestLumiere(){


	}

	void TestLumiere::OnUpdate(float delta){

		//Maj du temps pour la camera2
		float currentFrame = glfwGetTime();
		deltaTime2 = 60 * (currentFrame - lastFrame2);
		lastFrame2 = currentFrame;
	}

	void TestLumiere::OnRender(){

		GLCALL(glClearColor(0.0f,0.0f,0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));



		//Version controlée de la position de la lampe
		//glm::vec3 PositionLampe = m_TranslationB;

		//Lampe qui bouge
		glm::vec3 PositionLampe(m_TranslationB.x*glm::cos(glfwGetTime()), m_TranslationB.y * glm::cos(glfwGetTime()), m_TranslationB.z * glm::sin(glfwGetTime()));

		GLFWwindow* fen = glfwGetCurrentContext();
		// input
		// -----
		processInput2(fen);
		
		//Ici notre renderer ne stocke rien => on peut se permettre de le créer à chaque frame
		Renderer renderer;


		glm::mat4 vue = camera2.GetViewMatrix();
		//m_Texture->Bind();

		//Dessin 1
		{

			//On unbind tout...
			m_VertexBuffer->Bind();
			m_VAO->Bind();
			m_IndexBuffer->Bind();
			m_Shader->Bind();

			//Uniform --> envoi de données du CPU vers les shaders pour être utilisé comme une variable dans le shader
			//Utilisation uniform après bind avec le shader qui utilise l'uniform
			//Uniform appelé à chaque dessin (draw call)
			m_Shader->SetUniform3f("u_ObjectColor", 0.3f, 0.7f, 1.0f);
			m_Shader->SetUniform3f("u_LightColor", m_CouleurLumiere[0], m_CouleurLumiere[1], m_CouleurLumiere[2]);
			m_Shader->SetUniform3f("u_LightPos", PositionLampe.x, PositionLampe.y, PositionLampe.z);
			m_Shader->SetUniform3f("u_ViewPos", camera2.Position.x, camera2.Position.y, camera2.Position.z);


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

				//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
				renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
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
			renderer.Draw(*m_LampVAO, *m_LampIB, *m_LampShader);

			m_LampShader->UnBind();
			m_LampIB->UnBind();
			m_LampVB->UnBind();
			m_LampVAO->UnBind();
		}

	}

	void TestLumiere::OnImGuiRender(){

		ImGui::ColorEdit3("Couleur de la lumiere", m_CouleurLumiere);

		//Creation du contenu de la fenêtre imGui

		//ImGui::SliderFloat("Rotation camera2", &m_angle, 0.0f, 2 * glm::pi<float>());

		ImGui::SliderFloat3("Position lumiere", &m_TranslationB.x, -1500.0f, 1500.0f);

	}




	// Execute des fonctions selon la touche presse :
	// Demande à GLFW si une touche qui nous interesse est pressé et agit en consequence
	void processInput2(GLFWwindow *window)
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
			glfwSetCursorPosCallback(window, mouse_callback2);
		}


		//ZQSD mode
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera2.ProcessKeyboard(FORWARD, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera2.ProcessKeyboard(BACKWARD, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera2.ProcessKeyboard(LEFT, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera2.ProcessKeyboard(RIGHT, deltaTime2);

		//Mode fleches
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera2.ProcessKeyboard(FORWARD, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera2.ProcessKeyboard(BACKWARD, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera2.ProcessKeyboard(LEFT, deltaTime2);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera2.ProcessKeyboard(RIGHT, deltaTime2);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback2(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void mouse_callback2(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse2)
		{
			lastX2 = xpos;
			lastY2 = ypos;
			firstMouse2 = false;
		}

		float xoffset = xpos - lastX2;
		float yoffset = lastY2 - ypos; // reversed since y-coordinates go from bottom to top

		lastX2 = xpos;
		lastY2 = ypos;

		camera2.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback2(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera2.ProcessMouseScroll(yoffset);
	}

}