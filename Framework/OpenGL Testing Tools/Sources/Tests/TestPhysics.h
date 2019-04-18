#pragma once

#include "Test.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "Force.h"

#include <memory>

namespace testPhysics {


	//Hérite de test
	class TestPhysics : public test::Test {

	public:
		TestPhysics();
		~TestPhysics();

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

		Force m_gravity;


		float m_angle;



	};

}