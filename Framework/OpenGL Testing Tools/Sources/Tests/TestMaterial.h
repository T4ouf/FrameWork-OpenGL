#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"


#include <memory>

namespace test {


	//Hérite de test
	class TestMaterial : public Test {

	public:
		TestMaterial();
		~TestMaterial();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private : 

		Material* Gold;
		Material* Ruby;
		Material* Chrome;
		Material* Pearl;
		Material* Copper;
		Material* Obsidian;
		Material* BlackRubber;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;


		std::unique_ptr<VertexArray> m_LampVAO;
		std::unique_ptr<VertexBuffer> m_LampVB;

		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<IndexBuffer> m_LampIB;
		Shader* m_Shader;
		std::unique_ptr<Shader> m_LampShader;
		Texture* m_Texture;

		Material* m_Material;

		glm::mat4 m_Proj, m_Vue;
		glm::vec3 m_TranslationB;

		float m_CouleurLumiere[4];
		float m_ClearCouleur[3];
		float m_angle;


	};

}