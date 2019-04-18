#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshFactory.h"
#include "Scene.h"

#include <memory>

namespace test {


	//Hérite de test
	class TestScene : public Test {

	public:
		TestScene();
		~TestScene();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:

		Material* texturedMaterial;
		Material* Gold;
		Material* Ruby;
		Material* Chrome;
		Material* Pearl;
		Material* Copper;
		Material* Obsidian;
		Material* BlackRubber;
		Material* LampMaterial;


		Scene* m_scene;

		Material* m_Material;

		Texture* m_Texture;

		//Création du shader
		Shader* m_Shader;

		Force* m_gravite;

		float m_CouleurLumiere[4];
		float m_ClearCouleur[3];
		float m_angle;

		glm::vec3 m_translation;

		float deltaTime;
		double lastFrame;

		int frameNumber;
	};

}