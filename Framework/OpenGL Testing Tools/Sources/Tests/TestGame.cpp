#include "TestScene.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "Shader.h"
#include "Parseur.h"
#include "Application.h"

#include "imgui/imgui.h"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include "TestGame.h"
#include <glm/gtx/string_cast.hpp>

namespace testGame {
	Object* player;

	Application app;


	Force* forwardForce = new Force(glm::vec3(0.0f, 0.0f, -0.5f));
	Force* rightForce = new Force(glm::vec3(0.4f, 0.0f, 0.0f));
	Force* upForce = new Force(glm::vec3(0.0f, 1.5f, 0.0f));

	Object* platform;

	glm::vec3 oldCameraPosition(0.0f);

	double JUMP_COOLDOWN = 1.0, timeOfLastJump = 0.0;

	bool isSprinting = false, canJump = true;

	void jump() {
		if (canJump) {
			player->addForce(new Force(*upForce));
			timeOfLastJump = glfwGetTime();
			canJump = false;
		}
	}
	ButtonFunction jumpCallback = jump;

	void movement(float value) {
		if (isSprinting)
			value = value * 2.0f;
		player->addForce(new Force(rightForce->MultiplyByScalar(value)));
	}
	AnalogButtonFunction sideMovement = movement;

	void teleportation() {
		if (player->getPosition().y + 150.0f <= 850.0f)
			player->translate(glm::vec3(0.0f, 150.0f, 0.0f));
	}
	ButtonFunction teleportationCallback = teleportation;

	void sprint() {
		isSprinting = !isSprinting;
	}
	ButtonFunction sprintCallback = sprint;

	TestGame::~TestGame() {
		delete Gold;
		delete Ruby;
		delete Chrome;
		delete Pearl;
		delete Copper;
		delete Obsidian;
		delete LampMaterial;

		delete m_Texture;
		delete m_Shader;

		delete m_gravite;

		m_scene->clearScene();
		delete m_scene;
	}

	void TestGame::OnUpdate(float delta) {

		//Maj du temps pour la camera
		double currentFrame = glfwGetTime();

		deltaTime = 60 * (currentFrame - lastFrame);

		lastFrame = currentFrame;

		app.processInput();

		//Slow down
		m_player->addForce(new Force(glm::vec3(-0.1f * m_player->GetSpeed().x, 0.0f, -0.1f * m_player->GetSpeed().z)));

		m_scene->UpdatePhysic(deltaTime);
		//Only move camera when big movement is achieved
		glm::vec3 newCameraPosition(m_player->getPosition() + glm::vec3(0.0f, 20.0f, 250.0f));
		if ((oldCameraPosition - newCameraPosition).length() > 0.8f) {
			oldCameraPosition = newCameraPosition;
			m_scene->retrieveCamera().m_position = newCameraPosition;
		}

		if (player->getPosition().y < -200.0f)
			glfwSetWindowShouldClose(glfwGetCurrentContext(), true);

		if (currentFrame - timeOfLastJump > JUMP_COOLDOWN) {
			canJump = true;
		}
	}

	void TestGame::OnRender() {

		GLCALL(glClearColor(m_ClearCouleur[0], m_ClearCouleur[1], m_ClearCouleur[2], 0.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//ATTENTIN ICI C'EST UNE REFERENCE ! LE & EST IMPORTANT
		Camera& c = m_scene->retrieveCamera();
		c.translate(m_translation);
		m_scene->setLightColor(glm::vec3(m_CouleurLumiere[0], m_CouleurLumiere[1], m_CouleurLumiere[2]));

		m_scene->Render(Renderer::getInstance());
		/*
		if (frameNumber % 15 == 0) {
			std::cout << "Player rightSide : " << m_player->getPosition().x + (m_player->m_physicObject->m_width / 2.0) << std::endl;
			std::cout << "Player leftSide : " << m_player->getPosition().x - (m_player->m_physicObject->m_width / 2.0) << std::endl;
			std::cout << "Platform rightSide : " << platform->getPosition().x + (platform->m_physicObject->m_width / 2.0) << std::endl;
			std::cout << "Platform leftSide : " << platform->getPosition().x - (platform->m_physicObject->m_width / 2.0) << std::endl;
			std::cout << "Platform position : " << glm::to_string(platform->getPosition()) << std::endl;
			std::cout << "Player position : " << glm::to_string(m_player->getPosition()) << std::endl << std::endl;
		}*/
		frameNumber++;

		if (isSprinting) {
			player->setMaterial(LampMaterial);
		}
		else {
			player->setMaterial(Ruby);
		}
	}

	void TestGame::OnImGuiRender() {
		ImGui::ColorEdit3("Couleur du Vide", m_ClearCouleur);

		ImGui::ColorEdit3("Couleur de la lumiere", m_CouleurLumiere);

		ImGui::SliderFloat3("Translation Camera", &m_translation.x, -10.0f, 10.0f);

		if (ImGui::Button("Reset Camera Translation")) {
			m_translation = glm::vec3(0.0f);
		}
		else if (ImGui::Button("Find light")) {

			auto lPos = m_scene->retrieveLight().getPosition();

			m_scene->retrieveCamera().lookAt(lPos.x, lPos.y, lPos.z + 400);
		}
		else if (ImGui::Button("Gravite++")) {
			m_scene->addGlobalForce(m_gravite); //A VERIFIER SI CE N'EST PAS AJOUTE PLUSIEURS FOIS..
		}
		else if (ImGui::Button("Gravite--")) {
			m_scene->addGlobalForce(m_reverseGravity);
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

	TestGame::TestGame() : m_CouleurLumiere{ 1.0f,1.0f,1.0f,1.0f }, m_translation(glm::vec3(0.0f)), deltaTime(0.0f) {

		lastFrame = 0.0;
		frameNumber = 0;

		glm::vec3 gravityDirection(0.0f, -0.0981f, 0.0f);
		m_gravite = new Force(gravityDirection);
		m_reverseGravity = new Force(gravityDirection*-1.0f);

		//On autorise et paramètre la transparence
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//On active le test de profondeur (z-buffer test)
		glEnable(GL_DEPTH_TEST);
		//On accepte que les pixels qui sont devant (les autres ne seront pas dessinés par dessus)
		glDepthFunc(GL_LESS);

		m_Texture = new Texture("Ressources/Textures/texture-caisse.png");

		//Création du shader
		m_Shader = new Shader("Ressources/Shaders/Material.shader");


		//Material : Gold PRESQUE OK ! => manque texture
		//BUG TEXTURE NE PEUT PAS ÊTRE NULLPTR....
		Gold = new Material(m_Shader, m_Texture,
			glm::vec3(0.329412f, 0.223529f, 0.027451f),
			glm::vec3(0.780392f, 0.568627f, 0.113725f),
			glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f);

		Ruby = new Material(m_Shader, m_Texture,
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

		LampMaterial = new Material(LightSourceShader, "Ressources/Textures/texture-caisse.png",
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);

		m_Material = Gold;

		m_scene = new Scene();

		Parseur p = Parseur("Ressources/Xml/level1.xml");

		Scene& sceneRef = *m_scene;
		Level lvl = p.parse(sceneRef);

		//Object* Platform = ObjectFactory::CreateCube(glm::vec3(0.0f, 0.0f, 0.0f), 850, 150, true, m_Material);
		Object* Boxe = ObjectFactory::CreateCube(glm::vec3(0.0f, 0.0f, 0.0f), 20, 15, false, Ruby);
		m_player = Boxe;
		player = Boxe;
		m_player->SetBounceCoeff(0.0);
		m_player->translate(lvl.startPosX, lvl.startPosY, lvl.startPosZ);
		//platform = Platform;

		Light& l = m_scene->retrieveLight();
		l.m_intensity = 5000;
		l.setPosition(0, 175, 0);

		Object* Lamp = ObjectFactory::CreateCube(l.getPosition(), 25, 1, true, LampMaterial);
		//Lamp->translate(glm::vec3(150.0f, 150.0f, -175.0f));

		Camera& camera = m_scene->retrieveCamera();
		camera.translate(glm::vec3(0.0f, 200.0f, 650.0f));
		//Platform->translate(glm::vec3(0.0f, -450.0f, 0.0f));
		//m_scene->addObject(Platform);
		m_scene->addObject(Boxe);
		m_scene->addObject(Lamp);

		app = Application();
		app.mapButtonInput(BUTTON_A, jumpCallback);
		app.mapAnalogButtonInput(ANALOG_LEFTSTICK_X, sideMovement);
		app.mapButtonInput(BUTTON_X, teleportationCallback);
		app.mapButtonInput(BUTTON_B, sprintCallback);
	}


	/*

	// Execute des fonctions selon la touche presse :
	// Demande à GLFW si une touche qui nous interesse est pressé et agit en consequence
	void TestGame::ProcessInput(GLFWwindow *window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		// Get controller's state
		int nbJoystickAxes, nbJoystickButtons; // 6 et 14 avec manette xbox 360
		bool isJoystickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
		const float *axes = nullptr;
		const unsigned char *buttons = nullptr;
		if (isJoystickPresent) {
			axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes);
			// 0 : left joystick -> axe X (cosinus)
			// 1 : left joystick -> axe Y (sinus)
			// 2 : right joystick -> axe X (cosinus)
			// 3 : right joystick -> axe Y (sinus)
			// 4 : LT button
			// 5 : RT button
			buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);
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
		}
		//ZQSD mode
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || isJoystickPresent && axes[1] > 0.2f) {
			m_player->addForce(new Force(*forwardForce));
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || isJoystickPresent && axes[1] < -0.2f) {
			m_player->addForce(new Force(forwardForce->MultiplyByScalar(-1.0f)));
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || isJoystickPresent && axes[0] < -0.2f) {
			m_player->addForce(new Force(*leftForce));
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || isJoystickPresent && axes[0] > 0.2f) {
			m_player->addForce(new Force(leftForce->MultiplyByScalar(-1.0f)));
		}
		//Jump
		glfwSetKeyCallback(window, jump_callback);
		
	}
	*/
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
}