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

#include <memory>

namespace test {


	//Hérite de test
	class TestMesh : public Test {

	public:
		TestMesh();
		~TestMesh();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private : 

		Material* texturedMaterial;
		Material* Gold;
		Material* Ruby;
		Material* Chrome;
		Material* Pearl;
		Material* Copper;
		Material* Obsidian;
		Material* BlackRubber;


		std::unique_ptr<VertexArray> m_LampVAO;
		std::unique_ptr<VertexBuffer> m_LampVB;
		std::unique_ptr<IndexBuffer> m_LampIB;
		std::unique_ptr<Shader> m_LampShader;

		Mesh* m_Cube;
		Mesh* m_Sphere;

		Shader* m_Shader;

		Texture* m_Texture;

		Material* m_Material;

		glm::mat4 m_Proj, m_Vue;
		glm::vec3 m_TranslationB;

		float m_CouleurLumiere[4];
		float m_ClearCouleur[3];
		float m_angle;


	};

}