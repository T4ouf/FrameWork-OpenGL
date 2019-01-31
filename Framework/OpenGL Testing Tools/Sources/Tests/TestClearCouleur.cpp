#include "TestClearCouleur.h"
#include "Renderer.h"
#include "imgui/imgui.h"



namespace test {

	
	TestClearCouleur::TestClearCouleur()
		: m_ClearCouleur{ 0.2f,0.3f,0.8f,1.0f } {
	
	}
	

	TestClearCouleur::~TestClearCouleur(){


	}

	void TestClearCouleur::OnUpdate(float delta){


	}

	void TestClearCouleur::OnRender(){
		
		GLCALL(glClearColor(m_ClearCouleur[0], m_ClearCouleur[1], m_ClearCouleur[2], m_ClearCouleur[3]));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearCouleur::OnImGuiRender(){

		ImGui::ColorEdit4("Couleur du Vide", m_ClearCouleur);

	}

}