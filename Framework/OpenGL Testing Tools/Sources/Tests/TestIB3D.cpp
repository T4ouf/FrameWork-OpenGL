#include "TestIB3D.h"
#include "GLFW/glfw3.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {


	TestIB3D::TestIB3D() : m_angle(1.0f), m_TranslationB(0, 0, -300),
		m_Proj(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 400.0f)),
		m_Vue(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))) {

		//Tableau contenant les positions des points (8 pour un cube) et leur UV coords (2) 
		//Ici, on partage tous les points au max (avec chaque face)
		float positions[] = {
						
			-50.0f, -50.0f, 50.0f, 0.0f, 0.0f,	//0
			50.0f,	-50.0f, 50.0f, 1.0f, 0.0f,	//1
			50.0f,	 50.0f, 50.0f,	1.0f, 1.0f,	//2
			-50.0f,  50.0f, 50.0f,	0.0f, 1.0f,	//3

			- 50.0f, -50.0f,-50.0f, 0.0f, 0.0f,	//4
			50.0f,	-50.0f, -50.0f, 1.0f, 0.0f,	//5
			50.0f,	 50.0f, -50.0f,	1.0f, 1.0f,	//6
			-50.0f,  50.0f, -50.0f,	0.0f, 1.0f	//7

		};

		//Pour éviter la redondance (créer 2x ou plus le même point) on crée un IndiceBuffer (tab d'indice)
		//qui va stocker les indices des points à tracer
		unsigned int indices[] = {
		
			0,1,2,	//Devant
			2,3,0,

			0,4,3,	//Gauche
			3,7,4,

			4,5,6,	//Derrière
			6,7,4,

			4,5,0,	//Dessous
			0,1,5,

			5,1,6,	//Droite
			6,2,5,

			6,2,3,	//Dessus
			3,7,6

		};
		
		//Ici aucun partage des points 36 points (6 faces * 2 triangles * 3 points chaque).
		float positions2[] = {
			-50.0f, -50.0f, -50.0f,  0.0f, 0.0f,
			 50.0f, -50.0f, -50.0f,  1.0f, 0.0f,
			 50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			 50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 0.0f,

			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			 50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			 50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
			 50.0f,  50.0f,  50.0f,  1.0f, 1.0f,
			-50.0f,  50.0f,  50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,

			-50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

			 50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			 50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			 50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			 50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			 50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			 50.0f,  50.0f,  50.0f,  1.0f, 0.0f,

			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,
			 50.0f, -50.0f, -50.0f,  1.0f, 1.0f,
			 50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			 50.0f, -50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f, -50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f, -50.0f, -50.0f,  0.0f, 1.0f,

			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f,
			 50.0f,  50.0f, -50.0f,  1.0f, 1.0f,
			 50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			 50.0f,  50.0f,  50.0f,  1.0f, 0.0f,
			-50.0f,  50.0f,  50.0f,  0.0f, 0.0f,
			-50.0f,  50.0f, -50.0f,  0.0f, 1.0f
		};

		unsigned int indices2[] = {

			0,1,2,	//Devant
			3,4,5,

			6,7,8,	//Gauche
			9,10,11,

			12,13,14,	//Derrière
			15,16,17,

			18,19,20,	//Dessous
			21,22,23,

			24,25,26,	//Droite
			27,28,29,

			30,31,32,	//Dessus
			33,34,35

		};


		//On autorise et paramètre la transparence
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//On active le test de profondeur (z-buffer test)
		glEnable(GL_DEPTH_TEST);
		//On accepte que les pixels qui sont devant (les autres ne seront pas dessinés par dessus)
		glDepthFunc(GL_LESS);
		
		m_VAO = std::make_unique<VertexArray>();

		//On stocke nos données (les positions)
		//						nb de pts * nombre de floats/pt * size(float)
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 8 * 5 * sizeof(float));
		//Définition de notre layout pour notre VertexArray
		VertexBufferLayout layout;

		//On utilse 3 floats par points (positions) => layout regroupe ce qui va ensemble
		//layout location 0 (cf vertex shader)
		layout.Push<float>(3);

		// On utilise 2 points par UV coords
		//layout location 1 (cf vertex shader)
		layout.Push<float>(2);

		//layout.Push<float>(3); si on utilse 3 floats par points (positions en 3D)
		//layout.Push<float>(3); si on utilse 3 floats par normale (Normales en 3D)
		//layout.Push<float>(2); si on utilse 2 floats par UV coord (UV coord pour texture)
		//etc..

		//On regroupe tout dans notre vertex Array (on donne du sens à nos données)
		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//On stocke et lie à l'index buffer les indices
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		//Création du shader
		m_Shader = std::make_unique<Shader>("Ressources/Shaders/Basic.shader");

		m_Shader->Bind();

		//Uniform --> envoi de données du CPU vers les shaders pour être utilisé comme une variable dans le shader
		//Utilisation uniform après bind avec le shader qui utilise l'uniform
		//Uniform appelé à chaque dessin (draw call)
		m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 1.0f, 1.0f);

		m_Texture = std::make_unique<Texture>("Ressources/Textures/texture-caisse.png");

		//On spécifie au shader (via un uniform) quel slot de texture on utilise
		m_Shader->SetUniform1i("u_Texture", 0);

		m_Vue = glm::lookAt(glm::vec3(0.0f, 0.0f, -200.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));

	}


	TestIB3D::~TestIB3D() {

		//Rien à faire ici => smarts pointeurs sont détruits lorsqu'ils sont hors de portée

	}

	void TestIB3D::OnUpdate(float delta) {


	}

	void TestIB3D::OnRender() {

		GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
		//Ici notre renderer ne stocke rien => on peut se permettre de le créer à chaque frame
		Renderer* renderer = Renderer::getInstance();

		m_Texture->Bind();

		//Dessin 1
		{
			//Definition d'une matrice du modèle => Considère toutes les transformations 
			//aka translation, rotation, scaling
			//à appliquer à notre modèle...
			//Ici on le déplace vers en haut à droite
			glm::mat4 modele = glm::translate(glm::mat4(1.0f), m_TranslationB);
			modele = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
			
			m_Vue = glm::translate(glm::mat4(1.0f), m_TranslationB);
			m_Vue = glm::rotate(m_Vue, (float)glm::cos(glfwGetTime())*3.14f + (float)glm::sin(glfwGetTime())*3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
			
			//Matrice finale (que l'on va multiplier par chaque point pour qu'il s'affiche correctement)
			// /!\ ORDRE IMPORTANT ICI /!\ 
			glm::mat4 mvp = m_Proj * m_Vue * modele;

			//Seul moyen de ne pas gérer les uniform à la main => materials
			//materials = shader + des donnees utiles au dessin

			m_Shader->Bind();

			//On envoit la matrice de projection au shader
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			//en tant normal, un renderer prend un vertexArray + IndexBuffer + Materials (pas shader)
			renderer->Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

			
		}
		

	}

	void TestIB3D::OnImGuiRender() {

		//Creation du contenu de la fenêtre imGui

		ImGui::SliderFloat("Rotation", &m_angle, 0.0f, 360.0f);

		ImGui::SliderFloat3("TranslationB", &m_TranslationB.x, -150.0f, 150.0f);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}

}