#include "TestScene.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Object.h"
#include "ObjectFactory.h"
#include "Shader.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	TestScene::TestScene() : m_CouleurLumiere{ 1.0f,1.0f,1.0f,1.0f }, m_translation(glm::vec3(0.0f)), deltaTime(0.0f) {

		lastFrame = 0.0;
		frameNumber = 0;

		m_gravite = new Force(glm::vec3(0.0f,-0.0981,0.0f));

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

		Object* Cube = ObjectFactory::CreateCube(glm::vec3(0.0f, 200.0f, -150.0f), 50, 150, false, m_Material);
		Object* Boxe = ObjectFactory::CreateBoxe(glm::vec3(0.0f, -100.0f, -150.0f), 150, 50, 150, 150, true, Ruby);
		//Boxe->scale(1.0f, 0.33f, 1.0f);	//SERA FAIT DANS LE CREATE BOX

		Light& l = m_scene->retrieveLight();
		//l.setPosition(150.0f, 150.0f, -175.0f);
		l.setPosition(0, 300, 300);

		Object* Lamp = ObjectFactory::CreateCube(glm::vec3(0, 350, -150.0f),25,1,false,LampMaterial);
		//Lamp->translate(glm::vec3(150.0f, 150.0f, -175.0f));

		Camera& camera = m_scene->retrieveCamera();
		//camera.lookAt(*Cube);
		camera.translate(0.0f, 150.0f, 400.0f);
		m_scene->addObject(Cube);
		m_scene->addObject(Boxe);
		m_scene->addObject(Lamp);


	}


	TestScene::~TestScene() {
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

	void TestScene::OnUpdate(float delta) {

		//Maj du temps pour la camera
		double currentFrame = glfwGetTime();
		
		deltaTime = 60 * (currentFrame - lastFrame);

		lastFrame = currentFrame;
		
	}

	void TestScene::OnRender() {

		GLCALL(glClearColor(m_ClearCouleur[0], m_ClearCouleur[1], m_ClearCouleur[2], 0.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		//ATTENTIN ICI C'EST UNE REFERENCE ! LE & EST IMPORTANT
		Camera& c = m_scene->retrieveCamera();
		c.translate(m_translation);
		m_scene->setLightColor(glm::vec3(m_CouleurLumiere[0], m_CouleurLumiere[1], m_CouleurLumiere[2]));

		//if (frameNumber % 4 == 0) { //ralentissement fort de l'animation
		m_scene->UpdatePhysic(deltaTime);		//BUG PHYSICS COLLISION
		//}

		m_scene->Render(Renderer::getInstance());

		frameNumber++;

	}

	void TestScene::OnImGuiRender() {
		ImGui::ColorEdit3("Couleur du Vide", m_ClearCouleur);

		ImGui::ColorEdit3("Couleur de la lumiere", m_CouleurLumiere);

		ImGui::SliderFloat3("Translation Camera", &m_translation.x, -10.0f, 10.0f);

		if (ImGui::Button("Reset Camera Translation")) {
			m_translation = glm::vec3(0.0f);
		}
		else if (ImGui::Button("Find light")) {

			auto lPos = m_scene->retrieveLight().getPosition();

			m_scene->retrieveCamera().lookAt(lPos.x,lPos.y,lPos.z+400);
		}
		else if (ImGui::Button("Gravite++")) {
			m_scene->addGlobalForce(m_gravite); //A VERIFIER SI CE N'EST PAS AJOUTE PLUSIEURS FOIS..
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}