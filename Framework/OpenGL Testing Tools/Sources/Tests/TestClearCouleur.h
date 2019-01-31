#pragma once


#include "Test.h"

namespace test {


	//Hérite de test
	class TestClearCouleur : public Test {

	public:
		TestClearCouleur();
		~TestClearCouleur();

		void OnUpdate(float delta) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private : 

		float m_ClearCouleur[4];


	};

}