#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {


	TestTexture2D::TestTexture2D() : m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0),
		m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		m_Vue(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))){

		//Tableau contenant les positions des points (4 pour un rectangle) et leur UV coords (2-2)
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,	//0
			50.0f,	-50.0f, 1.0f, 0.0f,	//1
			50.0f,	 50.0f,	1.0f, 1.0f,	//2
			-50.0f,  50.0f,	0.0f, 1.0f	//3
		};

		//Pour �viter la redondance (cr�er 2x ou plus le m�me point) on cr�e un IndiceBuffer (tab d'indice)
		//qui va stocker les indices des points � tracer
		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};

		//On autorise et param�tre la transparence
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		
		m_VAO = std::make_unique<VertexArray>();

			
		//On stocke nos donn�es (les positions)
		//						nb de pts * nombre de floats/pt * size(float)
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		//D�finition de notre layout pour notre VertexArray
		VertexBufferLayout layout;

		//On utilse 2 floats par points (positions) => layout regroupe ce qui va ensemble
		//layout location 0 (cf vertex shader)
		layout.Push<float>(2);

		// On utilise 2 points par UV coords
		//layout location 1 (cf vertex shader)
		layout.Push<float>(2);

		//layout.Push<float>(3); si on utilse 3 floats par points (positions en 3D)
		//layout.Push<float>(3); si on utilse 3 floats par normale (Normales en 3D)
		//layout.Push<float>(2); si on utilse 2 floats par UV coord (UV coord pour texture)
		//etc..

		//On regroupe tout dans notre vertex Array (on donne du sens � nos donn�es)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//On stocke et lie � l'index buffer les indices
		
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//Cr�ation du shader
		m_Shader = std::make_unique<Shader>("Ressources/Shaders/Basic.shader");

		m_Shader->Bind();

		//Uniform --> envoi de donn�es du CPU vers les shaders pour �tre utilis� comme une variable dans le shader
		//Utilisation uniform apr�s bind avec le shader qui utilise l'uniform
		//Uniform appel� � chaque dessin (draw call)
		m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

		m_Texture = std::make_unique<Texture>("Ressources/Textures/img_test.png");
		
		//On sp�cifie au shader (via un uniform) quel slot de texture on utilise
		m_Shader->SetUniform1i("u_Texture", 0);

	}
	

	TestTexture2D::~TestTexture2D(){

		//Rien � faire ici => smarts pointeurs sont d�truits lorsqu'ils sont hors de port�e

	}

	void TestTexture2D::OnUpdate(float delta){


	}

	void TestTexture2D::OnRender(){
		
		GLCALL(glClearColor(0.0f,0.0f,0.0f,1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT));


		//Ici notre renderer ne stocke rien => on peut se permettre de le cr�er � chaque frame
		Renderer* renderer = Renderer::getInstance();

		m_Texture->Bind();

		//Dessin 1
		{
			//Definition d'une matrice du mod�le => Consid�re toutes les transformations 
			//aka translation, rotation, scaling
			//� appliquer � notre mod�le...
			//Ici on le d�place vers en haut � droite
			glm::mat4 modele = glm::translate(glm::mat4(1.0f), m_TranslationA);


			//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
			// /!\ ORDRE IMPORTANT ICI /!\ 
			glm::mat4 mvp = m_Proj * m_Vue * modele;


			//Seul moyen de ne pas g�rer les uniform � la main => materials
			//materials = shader + des donnees utiles au dessin

			m_Shader->Bind();

			//On envoit la matrice de projection au shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
			renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		//Dessin 2
		{
			//Definition d'une matrice du mod�le => Consid�re toutes les transformations 
			//aka translation, rotation, scaling
			//� appliquer � notre mod�le...
			//Ici on le d�place vers en haut � droite
			glm::mat4 modele = glm::translate(glm::mat4(1.0f), m_TranslationB);


			//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
			// /!\ ORDRE IMPORTANT ICI /!\ 
			glm::mat4 mvp = m_Proj * m_Vue * modele;


			//Seul moyen de ne pas g�rer les uniform � la main => materials
			//materials = shader + des donnees utiles au dessin

			m_Shader->Bind();

			//On envoit la matrice de projection au shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
			renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}


	}

	void TestTexture2D::OnImGuiRender(){

		//Creation du contenu de la fen�tre imGui
		
		ImGui::SliderFloat3("TranslationA", &m_TranslationA.x, 0.0f, 960.0f);

		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, 0.0f, 960.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}