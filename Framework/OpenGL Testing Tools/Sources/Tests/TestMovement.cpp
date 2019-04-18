#include "TestMovement.h"
#include "GLFW/glfw3.h"


#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "Camera.h"
#include "Force.h"
#include "PhysicObject.h"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <glm/gtx/string_cast.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace testMovement {

	// settings
	const unsigned int SCR_WIDTH = 960;
	const unsigned int SCR_HEIGHT = 540;

	// camera
	Camera camera(glm::vec3(0.0f, 100.0f, 300.0f));
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = glfwGetTime();
	float totalTime = 0.0f;

	Force jumpForce(glm::vec3(0.0f, 0.2f, 0.0f));
	Force movementForce(glm::vec3(0.1f, 0.0f, 0.0f));
	float playerAngle = 0.0f;

	glm::vec3* cubePositions[] = {
		new glm::vec3(0.0f,  250.0f,  0.0f),
		new glm::vec3(0.0f,  0.0f, 0.0f)
	};

	glm::vec3 cubeSpeeds[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f)
	};

	glm::mat4 playerRotation = glm::rotate(playerAngle, glm::vec3(0.0f, 1.0f, 0.0f));

	PhysicObject objects[] = {
		PhysicObject(cubePositions[0], 100.0, 100.0, 100.0, 10.0, false),
		PhysicObject(cubePositions[1], 100.0, 100.0, 100.0, 10.0, true)
	};

	void processInput(GLFWwindow *window);

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	// glfw: whenever the mouse moves, this callback is called
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


	TestMovement::TestMovement() : m_angle(1.0f), m_TranslationB(0, 0, 0),
		m_Proj(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 2000.0f)),
		m_Vue(glm::mat4(1.0f)), m_gravity(glm::vec3(0.0f, -0.0005f, 0.0f)) {


		GLFWwindow* fen = glfwGetCurrentContext();

		//On initialise nos fonctions liées aux captures d'évènements (resized windows, mouse, etc.)
		glfwSetFramebufferSizeCallback(fen, framebuffer_size_callback);
		glfwSetCursorPosCallback(fen, mouse_callback);
		glfwSetScrollCallback(fen, scroll_callback);

		// Demande à GLFW de capturer la souris (pour la caméra)
		glfwSetInputMode(fen, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		//Tableau contenant les positions des points (8 pour un cube) et leur UV coords (2) 
		//Ici, on partage tous les points au max (avec chaque face)
		float positions[] = {

			-50.0f, -50.0f, 50.0f, 0.0f, 0.0f,	//0
			50.0f,	-50.0f, 50.0f, 1.0f, 0.0f,	//1
			50.0f,	 50.0f, 50.0f,	1.0f, 1.0f,	//2
			-50.0f,  50.0f, 50.0f,	0.0f, 1.0f,	//3

			-50.0f, -50.0f,-50.0f, 0.0f, 0.0f,	//4
			50.0f,	-50.0f, -50.0f, 1.0f, 0.0f,	//5
			50.0f,	 50.0f, -50.0f,	1.0f, 1.0f,	//6
			-50.0f,  50.0f, -50.0f,	0.0f, 1.0f	//7

		};

		//Pour éviter la redondance (créer 2x ou plus le même point) on crée un IndiceBuffer (tab d'indice)
		//qui va stocker les indices des points à tracer
		unsigned int indices[] = {

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

		//Ici aucun partage des points 36 points (6 faces * 2 triangles * 3 points chaque).
		float positions2[] = {
			-50.0f, -50.0f, -50.0f,  0.0f, 0.0f,
			50.0f, -50.0f, -50.0f,  1.0f, 0.0f,
			50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 0.0f,

			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
			50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
			-50.0f,  50.0f,  50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,

			-50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

			50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			50.0f, -50.0f, -50.0f,  1.0f, 1.0f,
			50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,

			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
			50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f,  50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f
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

		m_VAO = std::make_unique<VertexArray>();

		//On stocke nos données (les positions)
		//						nb de pts * nombre de floats/pt * size(float)
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions2, 36 * 5 * sizeof(float));
		//Définition de notre layout pour notre VertexArray
		VertexBufferLayout layout;

		//On utilse 3 floats par points (positions) => layout regroupe ce qui va ensemble
		//layout location 0 (cf vertex shader)
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

		//Création du shader
		m_Shader = std::make_unique<Shader>("Ressources/Shaders/Basic.shader");

		m_Shader->Bind();

		//Uniform --> envoi de données du CPU vers les shaders pour être utilisé comme une variable dans le shader
		//Utilisation uniform après bind avec le shader qui utilise l'uniform
		//Uniform appelé à chaque dessin (draw call)
		m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

		m_Texture = std::make_unique<Texture>("Ressources/Textures/texture-caisse.png");

		//On spécifie au shader (via un uniform) quel slot de texture on utilise
		m_Shader->SetUniform1i("u_Texture", 0);
		
	}


	TestMovement::~TestMovement() {

		//Rien à faire ici => smarts pointeurs sont détruits lorsqu'ils sont hors de portée

	}

	void TestMovement::OnUpdate(float delta) {

		//Maj du temps pour la camera
		float currentFrame = glfwGetTime();
		deltaTime = 60 * (currentFrame - lastFrame);
		totalTime += deltaTime;
		lastFrame = currentFrame;
	}

	void TestMovement::OnRender() {

		GLCALL(glClearColor(235.0f / 255.0f, 200.0f / 255.0f, 60.0f / 255.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		GLFWwindow* fen = glfwGetCurrentContext();

		// input
		// -----
		processInput(fen);


		//Ici notre renderer ne stocke rien => on peut se permettre de le créer à chaque frame
		Renderer* renderer = Renderer::getInstance();

		m_Texture->Bind();

		//Dessin 1
		{
			//Definition d'une matrice du modèle => Considère toutes les transformations 
			//aka translation, rotation, scaling
			//à appliquer à notre modèle...
			//Ici on le déplace vers en haut à droite
			glm::mat4 modele = glm::translate(glm::mat4(1.0f), m_TranslationB);

			/*
			m_Vue = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Vue = glm::rotate(m_Vue, (float)m_angle + (float)m_angle, glm::vec3(0.0f, 1.0f, 0.0f));
			*/

			glm::mat4 vue = camera.GetViewMatrix();

			//Seul moyen de ne pas gérer les uniform à la main => materials
			//materials = shader + des donnees utiles au dessin
			m_Shader->Bind();

			Sides collisionSide;

			if (objects[0].CollidesWith(&objects[1], &collisionSide)) {
				objects[0].OnCollision(&objects[1], &collisionSide);
			}

			objects[0].ApplyForce(&m_gravity);

			for (int i = 0; i < 2; i++) {

				*(cubePositions[i]) = objects[i].GetPos();

				objects[i].Update(deltaTime);

				modele = glm::translate(glm::mat4(1.0f), glm::vec3(*(cubePositions[i])));

				if (i == 0) {
					//modele = glm::translate(modele, cubePositions[i] * -1.0f);
					modele = glm::rotate(modele, playerAngle, glm::vec3(0.0f, 1.0f, 0.0f));
					//modele = glm::translate(modele, cubePositions[i]);
				}
				
				//float angle = 20.0f * i;
				//modele = glm::rotate(modele, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

				//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
				// /!\ ORDRE IMPORTANT ICI /!\ 
				glm::mat4 mvp = m_Proj * vue * modele;

				//On envoit la matrice de projection au shader
				m_Shader->SetUniformMat4f("u_MVP", mvp);

				//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
				renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
			}




		}


	}

	void TestMovement::OnImGuiRender() {

		//Creation du contenu de la fenêtre imGui

		ImGui::SliderFloat("Rotation Camera", &m_angle, 0.0f, 2 * glm::pi<float>());

		ImGui::SliderFloat3("Translation Camera", &m_TranslationB.x, -1500.0f, 1500.0f);

		ImGui::SliderFloat4("Gravity :", &(m_gravity.GetPointerToDirection()->x), -0.1f, 0.1f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 500.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

	// Execute des fonctions selon la touche presse :
	// Demande à GLFW si une touche qui nous interesse est pressé et agit en consequence
	void processInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		//Saut
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			//glfwSetCursorPosCallback(window, NULL);
			objects[0].ApplyForce(&jumpForce);
		}

		//on quitte le mode camera
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			glfwSetCursorPosCallback(window, NULL);
		}

		//on rentre en mode camera
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPosCallback(window, mouse_callback);
		}

		// Get controller's state
		int nbJoystickAxes, nbJoystickButtons; // 6 et 14 avec manette xbox 360
		const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes);
		// 0 : left joystick -> axe X (cosinus)
		// 1 : left joystick -> axe Y (sinus)
		// 2 : right joystick -> axe X (cosinus)
		// 3 : right joystick -> axe Y (sinus)
		// 4 : LT button
		// 5 : RT button
		const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);
		// 0 : A button
		// 1 : B button
		// 2 : X button
		// 3 : Y button
		// 4 : LB button
		// 5 : RB button
		// 6 : ??
		// 7 : ??
		// 8 : left joystick click
		// 9 : right joystick click
		// 10 : top cross
		// 11 : right cross
		// 12 : bottom cross
		// 13 : left cross7

		//ZQSD mode
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || axes[1] > 0.5f)
			objects[0].ApplyForce(&movementForce.MultiplyByMatrix(playerRotation));
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || axes[1] < -0.5f)
			objects[0].ApplyForce(&movementForce.MultiplyByMatrix(playerRotation).MultiplyByScalar(-1.0f));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || axes[0] > 0.5f) {
			playerAngle += glm::radians(5.0f);
			playerRotation = glm::rotate(playerAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || axes[0] < -0.5f) {
			playerAngle -= glm::radians(5.0f);
			playerRotation = glm::rotate(playerAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		}

		//Mode fleches
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(yoffset);
	}

}