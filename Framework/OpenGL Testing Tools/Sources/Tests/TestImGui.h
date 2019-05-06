#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"
#include "Test.h"
#include "Scene.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"

namespace test {

	struct CreatedObject
	{
		Object* obj;
		bool isSelected;
		std::string type;
	};

	class TestImGui : public Test {
	public:
		//  ----------------------------------- CONSTRUCTORS / DESTRUCTOR  -----------------------------------
		
		TestImGui();
		~TestImGui();

		//  ----------------------------------- OVERRIDE  -----------------------------------
		
		void OnImGuiRender() override;
		void OnRender() override;
		void OnUpdate(float delta) override;

	private:
		// ----------------------------------- ATTRIBUTS -----------------------------------

		// Shader
		Shader* m_Shader;

		// Texture
		Texture* m_Texture;

		// Materials
		Material* Gold;
		Material* Ruby;
		Material* Chrome;
		Material* Pearl;
		Material* Copper;
		Material* Obsidian;
		Material* BlackRubber;
		Material* LampMaterial;
		static const char* m_materialsAvailable[];
		int m_currentMaterialInCombo;
		Material* m_currentMaterialSelected;

		Scene* scene;

		// Camera move management
		glm::mat4 m_Proj, m_Vue;
		glm::vec3 m_TranslationB;
		float m_angle;

		// Background color
		float m_colorBackground[3];

		// Music file path
		char m_musicFilePath[100];
		int m_musicFilePathSize = 100;

		// Graphical Objects -----------------------------------
		static const char* m_objectsAvailable[]; // Objects types
		int m_currentObjectInCombo; // Object selected in combo to be created
		// Object* m_currentSelectedObject; // Point to a Object selected in the interface, new or selected by clicking on it in the level

		// Created objects
		std::unordered_map<Object*, std::string> m_existingObjectsXML;
		std::vector<CreatedObject> m_existingObjects;
		
		// Start and finish
		bool m_startPlaced, m_finishPlaced;
		int m_startX, m_startY, m_startZ, m_finishX, m_finishY, m_finishZ;
		int m_finishWidth, m_finishHeight, m_finishLength;
		Object* m_startCube, *m_finishCube;

		// Cube
		int m_cubeX, m_cubeY, m_cubeZ; 
		float m_cubeSize, m_cubeMass;
		bool m_cubeIsAnchor;

		// Boxe
		int m_boxeX, m_boxeY, m_boxeZ;
		float m_boxeSizeX, m_boxeSizeY, m_boxeSizeZ, m_boxeMass;
		bool m_boxeIsAnchor;

		// Sphere
		int m_sphereX, m_sphereY, m_sphereZ;
		float m_sphereRadius, m_sphereMass;
		bool m_sphereIsAnchor;

		// Button Create
		bool m_displayCreateBtn;

		// Level Name and XML file generation
		char m_levelName[30];
		int m_levelNameSize = 30;
		bool m_displayLevelNameWindow;
		bool m_displayFileGenerationSuccessWindow;
		char m_levelFilePath[100];
		int m_levelFilePathSize = 100;

		// Inputs
		int m_joystickPresent;

		//  ----------------------------------- METHODS  -----------------------------------
		
		// Interface ImGui Windows
		void createObjectWindow();
		void listObjectsWindow();
		void mousePositionWindow();
		void backgroundColorWindow();
		void levelNameWindow();
		void fileGenerationSuccessWindow();
		void musicFileWindow();

		// Interface ImGui Graphical Objects
		void startItemsImGuiRender();
		void finishItemsImGuiRender();
		void cubeItemsImGuiRender();
		void sphereItemsImGuiRender();
		void boxeItemsImGuiRender();
		
		// Structure creation
		void startCreation();
		void finishCreation();
		void cubeCreation();
		void sphereCreation();
		void boxeCreation();

		// File creation and finish
		void generateXMLFile();

		// Interactions with mouse
		void processMouseAction();

		// Interactions with controller / joystick
		void processJoystickInput();
	};
}