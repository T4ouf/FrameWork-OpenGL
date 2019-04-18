#include "TestImGui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include "imgui/imgui.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- STATIC --------------------------------------------- //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* test::TestImGui::m_objectsAvailable[] = { "Start", "Finish", "Cube", "Sphere" };

// https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#arrays, used for vector in ListBox of the "inspect" window
static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
	auto& vector = *static_cast<std::vector<std::string>*>(vec);
	if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
	*out_text = vector.at(idx).c_str();
	return true;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- CONSTRUCTORS AND DESTROYER --------------------------------------------- //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

test::TestImGui::TestImGui() {
	m_currentObjectInCombo = -1;
	// m_currentSelectedObject = nullptr;
	m_isWindowColorOpened = false;
	
	m_startPlaced = false;
	m_startX = 0;
	m_startY = 0;
	
	m_finishPlaced = false;
	m_finishX = 0;
	m_finishY = 0;
	m_finishWidth = 1;
	m_finishHeight = 1;

	m_cubeX = 0;
	m_cubeY = 0;
	m_cubeSize = 0;
	m_cubeMass = 0;
	m_cubeIsAnchor = false;

	m_sphereX = 0;
	m_sphereY = 0;
	m_sphereRadius = 0;
	m_sphereMass = 0;
	m_sphereIsAnchor = false;

	m_displayCreateBtn = false;

	m_joystickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);
}

test::TestImGui::~TestImGui() {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- FUNCTIONS --------------------------------------------- //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OpenGL Window Render ------------------------------------------------------------------------------------------

void test::TestImGui::OnRender() {
	processMouseAction();
	if(m_joystickPresent == GLFW_TRUE)
		processJoystickInput();
}

// ImGui Render --------------------------------------------------------------------------------------------------

void test::TestImGui::OnImGuiRender() {
	createWindowImGuiRender();
	inspectWindowImGuiRender();
	mousePositionWindowImGuiRender();
	ImGui::Button("Terminer et convertir en XML");
}

// ImGui Windows Render ------------------------------------------------------------------------------------------

// ImGui window with options to add a new graphical object
void test::TestImGui::createWindowImGuiRender() {
	
	ImGui::Begin("Graphical Object Creation");

	ImGui::Combo("Available Objects", &m_currentObjectInCombo, m_objectsAvailable, IM_ARRAYSIZE(m_objectsAvailable));
	switch (m_currentObjectInCombo) {
	case 0: // Start
		startItemsImGuiRender();
		break;
	case 1: // Finish
		finishItemsImGuiRender();
		break;
	case 2: // Cube
		cubeItemsImGuiRender();
		break;
	case 3: // Sphere
		sphereItemsImGuiRender();
		break;
	}

	if (m_currentObjectInCombo >= 0 && m_displayCreateBtn && ImGui::Button("Create")) {
		// fonction cree structure, ajoutee au render des objets 3D à afficher
		// m_currentSelectedStructure = l'objet 3D
		ImGui::LabelText("", "CLIQUE !!!!");
		switch (m_currentObjectInCombo) {
		case 0: // Start
			startCreation();
			break;
		case 1: // Finish
			finishCreation();
			break;
		case 2: // Cube
			cubeCreation();
			break;
		case 3: // Sphere
			sphereCreation();
			break;
		}
	}

	ImGui::End();
}

// ImGui window to choose color of an object
void test::TestImGui::colorWindowImGuiRender() {
	ImGui::Begin("Color Picker", &m_isWindowColorOpened);
	ImGui::ColorPicker3("Color", m_colorRGBObj);
	ImGui::End();
}

// ImGui window with options to inspect objects created
void test::TestImGui::inspectWindowImGuiRender() {
	ImGui::Begin("Element list");
	//ImGui::ListBox("Object created", -1, vector_getter, static_cast<void*>(&m_objectCreatedList), m_objectCreatedList.size());
	ImGui::End();
}

// Used for test to show the mouse position
void test::TestImGui::mousePositionWindowImGuiRender() {
	ImGui::Begin("Mouse Position");
	ImGuiIO io = ImGui::GetIO();
	ImGui::InputFloat("X", &(io.MousePos).x);
	ImGui::InputFloat("Y", &(io.MousePos).y);
	ImGui::End();
}

// ImGui Render Structures -------------------------------------------------------------------------------------

void test::TestImGui::startItemsImGuiRender() {
	ImGui::InputInt("X position", &m_startX);
	ImGui::InputInt("Y position", &m_startY);
	m_displayCreateBtn = !m_startPlaced;
	if (m_startPlaced) {
		ImGui::Button("Modify");
	}
}

void test::TestImGui::finishItemsImGuiRender() {
	ImGui::InputInt("X position", &m_finishX);
	ImGui::InputInt("Y position", &m_finishY);
	ImGui::InputInt("Length", &m_finishWidth);
	ImGui::InputInt("Height", &m_finishHeight);
	m_displayCreateBtn = !m_finishPlaced;
	if (m_finishPlaced) {
		ImGui::Button("Modify");
	}
}

void test::TestImGui::cubeItemsImGuiRender() {
	ImGui::InputInt("X position", &m_cubeX);
	ImGui::InputInt("Y position", &m_cubeY);
	ImGui::InputFloat("Size", &m_cubeSize);
	ImGui::InputFloat("Mass", &m_cubeMass);
	ImGui::Checkbox("Anchor", &m_cubeIsAnchor);
	if (ImGui::Button("Color")) {
		m_isWindowColorOpened = true;
		// Voir pour afficher une case qui indique la couleur sélectionnée, qui se colore de cette couleur
	}
	if (m_isWindowColorOpened) {
		colorWindowImGuiRender();
	}
	m_displayCreateBtn = true;
}

void test::TestImGui::sphereItemsImGuiRender() {
	ImGui::InputInt("X position", &m_sphereX);
	ImGui::InputInt("Y position", &m_sphereY);
	ImGui::InputFloat("Radius", &m_sphereRadius);
	ImGui::InputFloat("Mass", &m_sphereMass);
	ImGui::Checkbox("Anchor", &m_sphereIsAnchor);
	if (ImGui::Button("Color")) {
		m_isWindowColorOpened = true;
		// Voir pour afficher une case qui indique la couleur sélectionnée, qui se colore de cette couleur
	}
	if (m_isWindowColorOpened) {
		colorWindowImGuiRender();
	}
	m_displayCreateBtn = true;
}

// Structure creation --------------------------------------------------------------------------------------------

void test::TestImGui::startCreation() {
	m_startPlaced = true;
}

void test::TestImGui::finishCreation() {
	m_finishPlaced = true;
}

void test::TestImGui::cubeCreation() {
	// Create Cube and add it to the Scene
	// glm::vec4 pos = glm::vec4(m_cubeX, m_cubeY, (m_inBackground ? 100 : 0), 1);
	// Object* cube = ObjectFactory.createCube(pos, m_cubeSize, m_cubeMass, m_cubeIsAnchor, MATERIAL);
	
	// Reset values
	m_cubeX = 0;
	m_cubeY = 0;
	m_cubeSize = 0;
	m_cubeMass = 0;
	m_cubeIsAnchor = false;
}

void test::TestImGui::sphereCreation() {
	// Create a sphere and add it to the Scene
	// glm::vec4 pos = glm::vec4(m_sphereX, m_sphereY, (m_inBackground ? 100 : 0), 1);
	// Object* cube = ObjectFactory.createCube(pos, m_cubeSize, m_cubeMass, m_cubeIsAnchor, MATERIAL);
	
	// Reset values
	m_sphereX = 0;
	m_sphereY = 0;
	m_sphereRadius = 0;
	m_sphereMass = 0;
	m_sphereIsAnchor = false;
}

// Interaction with mouse ----------------------------------------------------------------------------------------

void test::TestImGui::processMouseAction() {
	if (!ImGui::IsMouseHoveringAnyWindow() && !ImGui::IsAnyItemActive()) {
		if (ImGui::IsMouseDoubleClicked(0) || ImGui::IsMouseClicked(0)) {
			std::cout << "Clic Gauche\n";
			glClearColor(1, 0, 0, 1.0f);
		}
		else if (ImGui::IsMouseDoubleClicked(1) || ImGui::IsMouseClicked(1)) {
			glClearColor(1, 0, 0, 1.0f);
			std::cout << "Clic Droit\n";
		}
		else if (ImGui::IsMouseDragging(0) || ImGui::IsMouseDragging(1)) {
			glClearColor(0, 0, 1, 1.0f);
			std::cout << "Drag\n";
		}
		else if (ImGui::IsMouseReleased(0) || ImGui::IsMouseReleased(1)) {
			glClearColor(1, 0, 1, 1.0f);
			std::cout << "Clic relache\n";
		}
		else {
			glClearColor(0, 1, 0, 1.0f);
		}
	}
	else {
		glClearColor(0, 1, 0, 1.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Interactions with controller / joystick ------------------------------------------------------------------------

void test::TestImGui::processJoystickInput() {
	// Get controller's state
	int nbJoystickAxes, nbJoystickButtons; // taille 6 et 14 avec manette xbox 360
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes); 
	// 0 : left joystick -> axe X (cosinus) (gauche -1 -> 1 droite)
	// 1 : left joystick -> axe Y (sinus) (bas -1 -> 1 haut)
	// 2 : right joystick -> axe X (cosinus) (gauche -1 -> 1 droite)
	// 3 : right joystick -> axe Y (sinus) (bas -1 -> 1 haut)
	// 4 : LT button (relaché -1 -> appuyé 1)
	// 5 : RT button (relaché -1 -> appuyé 1)
	//ATTENTION: ARRONDI DEGUEULASSE DES FOIS < 1
	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);
	// 0 : A button
	// 1 : B button
	// 2 : X button
	// 3 : Y button
	// 4 : LB button
	// 5 : RB button
	// 6 : ??
	// 7 : ??
	// 8 : left joystick click
	// 9 : right joystick click
	// 10 : top cross
	// 11 : right cross
	// 12 : bottom cross
	// 13 : left cross7

	// Process actions
	if (buttons[12] == GLFW_PRESS) {
		glClearColor(1, 0, 0, 1.0f);
	}
	else if (axes[0] > 0){
		glClearColor(1, 0, 1, 1.0f);
	}
	else {
		glClearColor(0, 1, 0, 1.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}