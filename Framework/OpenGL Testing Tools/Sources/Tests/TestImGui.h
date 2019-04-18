#pragma once

#include "Test.h"
#include "imgui/imgui.h"
#include <vector>
#include <string>

namespace test {
	class TestImGui : public Test {
	public:
		//  ----------------------------------- CONSTRUCTORS / DESTRUCTOR  -----------------------------------
		
		TestImGui();
		~TestImGui();

		//  ----------------------------------- OVERRIDE  -----------------------------------
		
		void OnImGuiRender() override;
		void OnRender() override;		

	private:
		// ----------------------------------- ATTRIBUTS -----------------------------------

		// Graphical Objects -----------------------------------
		static const char* m_objectsAvailable[]; // Objects types
		int m_currentObjectInCombo; // Object selected in combo to be created
		// Object* m_currentSelectedObject; // Point to a Object selected in the interface, new or selected by clicking on it in the level
		
		// Start and finish
		bool m_startPlaced, m_finishPlaced;
		int m_startX, m_startY, m_finishX, m_finishY;
		int m_finishWidth, m_finishHeight;

		// Cube
		int m_cubeX, m_cubeY; 
		float m_cubeSize, m_cubeMass;
		bool m_cubeIsAnchor;

		// Sphere
		int m_sphereX, m_sphereY;
		float m_sphereRadius, m_sphereMass;
		bool m_sphereIsAnchor;

		// Option color
		bool m_isWindowColorOpened;
		float m_colorRGBObj[3];

		// Button Create
		bool m_displayCreateBtn;

		// Object Container
		std::vector<std::string> m_objectCreatedList;

		// Inputs ----------------------------------------
		int m_joystickPresent;

		//  ----------------------------------- METHODS  -----------------------------------
		
		// Interface ImGui Windows
		void createWindowImGuiRender();
		void inspectWindowImGuiRender();
		void colorWindowImGuiRender();
		void mousePositionWindowImGuiRender();

		// Interface ImGui Graphical Objects
		void startItemsImGuiRender();
		void finishItemsImGuiRender();
		void cubeItemsImGuiRender();
		void sphereItemsImGuiRender();
		
		// Structure creation
		void startCreation();
		void finishCreation();
		void cubeCreation();
		void sphereCreation();

		// Interactions with mouse
		void processMouseAction();

		// Interactions with controller / joystick
		void processJoystickInput();

	};
}