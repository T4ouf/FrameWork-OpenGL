#pragma once

//Pour unique_ptr
#include <memory>

#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"


namespace test {

	//Hérite de test
	class MonTest : public Test {

	public:
		MonTest();
		~MonTest();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		
		//pointeur unique sur notre VertexArray
		std::unique_ptr<VertexArray> m_VAO;

		//pointeur unique sur notre VertexBuffer
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		
		//pointeur unique sur notre IndexBuffer
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		//pointeur unique sur notre Shader
		std::unique_ptr<Shader> m_Shader;

		//matrices de vue et de projection
		glm::mat4 m_Proj, m_Vue;

	};

}