#pragma once

//Pour unique_ptr
#include <memory>

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Mesh.h"


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
		
		//Création du shader
		Shader* shader;

		//Material : Gold PRESQUE OK ! => manque texture
		Material* Gold;

		Mesh* m_Quad;
		Mesh* m_Cube;
		//matrices de vue et de projection
		glm::mat4 m_Proj, m_Vue;

	};

}