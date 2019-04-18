#pragma once

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include <memory>

namespace test {


	//H�rite de test
	class TestTexture2D : public Test {

	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private : 
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;



		glm::mat4 m_Proj, m_Vue;
		glm::vec3 m_TranslationA, m_TranslationB;

	};

}