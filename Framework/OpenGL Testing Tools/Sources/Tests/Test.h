#pragma once

#include <functional>
#include <vector>

namespace test {

	/**
	* Classe qui va permettre de la généricité dans les tests
	* On va factoriser tout le code possible pour que l'on puisse créer des mini applications tests
	*/
	class Test {

	public :

		Test() {};
		virtual ~Test() {};


		//Fonction a redef par nos tests qui héritent de Test
		virtual void OnUpdate(float delta) {};
		virtual void OnRender() {};
		virtual void OnImGuiRender() {};


	};


	//Classe qui va gérer notre menu
	class TestMenu : public Test {

	public : 

		TestMenu(Test*& PointeurTestEnCours);

		void OnImGuiRender() override;

		template<typename T>
		void AjouteTest(const std::string NomTest) {

			std::cout << "Ajout du test : " << NomTest << '\n';

			//On ajoute le test à la liste des tests
			//										lambda expression (pour le constructeur ici)
			m_Tests.push_back(std::make_pair(NomTest, []() { return new T(); }));
		}


	private:

		//Référence sur le pointeur de test actuel (dans le main) et le modifie en temps réel
		Test*& m_TestActuel;

		//Tableau de paires (composée du nom du test et une fonction qui renvoit un pointeur sur ce test)
		//en général un constructeur de test
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;

	};



}