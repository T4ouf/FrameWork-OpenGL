#pragma once

#include "Test.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

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
	class Test3D : public Test {

	public:
		Test3D();
		~Test3D();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_Vue;
		glm::vec3 m_TranslationB;

		
		float m_angle;


	
	};

}