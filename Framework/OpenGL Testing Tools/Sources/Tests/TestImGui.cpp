#include "TestImGui.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "ObjectFactory.h"
#include "Object.h"
#include "Camera.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- STATIC AND GLOBAL --------------------------------------------- //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* test::TestImGui::m_objectsAvailable[] = { "Start", "Finish", "Cube", "Boxe", "Sphere" };
const char* test::TestImGui::m_materialsAvailable[] = { "Gold", "Ruby", "Chrome", "Pearl", "Copper", "Obsidian", "BlackRubber", "LampMaterial" };

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// settings
const unsigned int SCR_WIDTH = 960;
const unsigned int SCR_HEIGHT = 540;

//camera
Camera* camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void processInput(GLFWwindow *window);
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- CONSTRUCTORS AND DESTROYER --------------------------------------------- //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

test::TestImGui::TestImGui() : m_angle(1.0f), m_TranslationB(0, 0, 0),
m_Proj(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 2000.0f)),
m_Vue(glm::mat4(1.0f)) {
	
	scene = new Scene();
	camera = &scene->retrieveCamera();
	camera->translate(0.0f, 0, 400);

	GLFWwindow* fen = glfwGetCurrentContext();

	//On initialise nos fonctions liées aux captures d'évènements (resized windows, mouse, etc.)
	glfwSetFramebufferSizeCallback(fen, framebuffer_size_callback);
	glfwSetCursorPosCallback(fen, NULL);
	glfwSetScrollCallback(fen, scroll_callback);

	// Demande à GLFW de capturer la souris (pour la caméra)
	glfwSetInputMode(fen, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	
	// Background Color
	m_colorBackground[0] = 0;
	m_colorBackground[1] = 0;
	m_colorBackground[2] = 0;
	
	// Objects and combo list associated
	m_currentObjectInCombo = -1;
	// m_currentSelectedObject = nullptr;
	
	m_startPlaced = false;
	m_startX = 0;
	m_startY = 0;
	m_startZ = 0;
	m_startCube = nullptr;
	
	m_finishPlaced = false;
	m_finishX = 0;
	m_finishY = 0;
	m_finishZ = 0;
	m_finishWidth = 1;
	m_finishHeight = 1;
	m_finishLength = 1;
	m_finishCube = nullptr;

	m_cubeX = 0;
	m_cubeY = 0;
	m_cubeZ = 0;
	m_cubeSize = 10;
	m_cubeMass = 0;
	m_cubeIsAnchor = false;

	m_boxeX = 0;
	m_boxeY = 0;
	m_boxeZ = 0;
	m_boxeSizeX = 10;
	m_boxeSizeY = 10;
	m_boxeSizeZ = 10;
	m_boxeMass = 0;
	m_boxeIsAnchor = false;

	m_sphereX = 0;
	m_sphereY = 0;
	m_sphereZ = 0;
	m_sphereRadius = 5;
	m_sphereMass = 0;
	m_sphereIsAnchor = false;

	m_displayCreateBtn = false;

	// Windows
	m_displayLevelNameWindow = false;
	m_displayFileGenerationSuccessWindow = false;

	// Joystick setup if present
	m_joystickPresent = glfwJoystickPresent(GLFW_JOYSTICK_1);

	// OpenGL setup
	//On autorise et paramètre la transparence
	GLCALL(glEnable(GL_BLEND));
	GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	//On active le test de profondeur (z-buffer test)
	glEnable(GL_DEPTH_TEST);
	//On accepte que les pixels qui sont devant (les autres ne seront pas dessinés par dessus)
	glDepthFunc(GL_LESS);

	// Material creation
	m_Texture = new Texture("Ressources/Textures/texture-caisse.png");
	m_Shader = new Shader("Ressources/Shaders/Material.shader");

	Gold = new Material(m_Shader, m_Texture,
		glm::vec3(0.329412f, 0.223529f, 0.027451f),
		glm::vec3(0.780392f, 0.568627f, 0.113725f),
		glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f);

	Ruby = new Material(m_Shader, m_Texture,
		glm::vec3(0.1745f, 0.01175f, 0.01175f),
		glm::vec3(0.61424f, 0.04136f, 0.04136f),
		glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f * 128);

	Chrome = new Material(m_Shader, nullptr,
		glm::vec3(0.25f, 0.25f, 0.25f),
		glm::vec3(0.4f, 0.4f, 0.4f),
		glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f * 128);

	Pearl = new Material(m_Shader, nullptr,
		glm::vec3(0.25f, 0.20725f, 0.20725f),
		glm::vec3(1.0f, 0.829f, 0.829f),
		glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f * 128);

	Copper = new Material(m_Shader, nullptr,
		glm::vec3(0.19125f, 0.0735f, 0.0225f),
		glm::vec3(0.7038f, 0.27048f, 0.0828f),
		glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f * 128);

	Obsidian = new Material(m_Shader, nullptr,
		glm::vec3(0.05375f, 0.05f, 0.06625f),
		glm::vec3(0.18275f, 0.17f, 0.22525f),
		glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f * 128);

	BlackRubber = new Material(m_Shader, nullptr,
		glm::vec3(0.02f, 0.02f, 0.02f),
		glm::vec3(0.01f, 0.01f, 0.01f),
		glm::vec3(0.4f, 0.4f, 0.4f), 10.0f);

	LampMaterial = new Material(LightSourceShader, "Ressources/Textures/texture-caisse.png",
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
	
	m_currentMaterialSelected = Gold;
	m_currentMaterialInCombo = 0;

	// Light and camera setting
	Light& l = scene->retrieveLight();
	l.setPosition(0, 1000, 0);
	
}

test::TestImGui::~TestImGui() {
	delete Gold;
	delete Ruby;
	delete Chrome;
	delete Pearl;
	delete Copper;
	delete Obsidian;
	delete LampMaterial;

	delete m_Texture;
	delete m_Shader;

	scene->clearScene();
	delete scene;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------- FUNCTIONS --------------------------------------------- //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// OpenGL Window Render ------------------------------------------------------------------------------------------

void test::TestImGui::OnRender() {
	
	GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
	
	GLFWwindow* fen = glfwGetCurrentContext();
	processInput(fen);

	scene->setBackgroundColor(m_colorBackground[0], m_colorBackground[1], m_colorBackground[2]);
	scene->Render(Renderer::getInstance());

	//processMouseAction();
	//if(m_joystickPresent == GLFW_TRUE)
		//processJoystickInput();
}

void test::TestImGui::OnUpdate(float delta) {
	float currentFrame = glfwGetTime();
	deltaTime = 60 * (currentFrame - lastFrame);
	lastFrame = currentFrame;
}

// ImGui Render --------------------------------------------------------------------------------------------------

void test::TestImGui::OnImGuiRender() {
	createObjectWindow();
	listObjectsWindow();
	mousePositionWindow();
	backgroundColorWindow();
	musicFileWindow();
	if (ImGui::Button("Terminer et convertir en XML")) {
		m_displayLevelNameWindow = true;
	}
	if (m_displayLevelNameWindow) {
		levelNameWindow();
	}
	if (m_displayFileGenerationSuccessWindow) {
		fileGenerationSuccessWindow();
	}
}

// ImGui Windows Render ------------------------------------------------------------------------------------------

// ImGui window with options to add a new graphical object
void test::TestImGui::createObjectWindow() {
	
	ImGui::Begin("Graphical Object Creation", nullptr);

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
	case 3: // Boxe
		boxeItemsImGuiRender();
		break;
	case 4: // Sphere
		sphereItemsImGuiRender();
		break;
	}

	if (m_currentObjectInCombo >= 0 && m_displayCreateBtn && ImGui::Button("Create")) {
		// fonction cree structure, ajoutee au render des objets 3D à afficher
		// m_currentSelectedStructure = l'objet 3D
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
		case 3: // Boxe
			boxeCreation();
			break;
		case 4: // Sphere
			sphereCreation();
			break;
		}
	}

	ImGui::End();
}

// ImGui window with options to inspect objects created
void test::TestImGui::listObjectsWindow() {
	ImGui::Begin("Element list");
	int pos = 0;
	int posToDelete = -1;
	for (CreatedObject& eot : m_existingObjects) {
		ImGui::Selectable(eot.type.c_str(), &(eot.isSelected)); ImGui::SameLine();
		ImGui::Button("Modify"); ImGui::SameLine();
		if (ImGui::Button("Delete")) {
			posToDelete = pos;
		}
		ImGui::SameLine();
		pos++;
	}
	if (posToDelete != -1) {
		m_existingObjectsXML.erase(m_existingObjects[posToDelete].obj);
		m_existingObjects.erase(m_existingObjects.begin() + posToDelete);
		// DELETE DANS LA SCENE L'OBJET !
	}
	ImGui::End();
}

// Used for test to show the mouse position
void test::TestImGui::mousePositionWindow() {
	ImGui::Begin("Mouse Position");
	ImGuiIO io = ImGui::GetIO();
	ImGui::InputFloat("X", &(io.MousePos).x);
	ImGui::InputFloat("Y", &(io.MousePos).y);
	ImGui::End();
}

// Window to set background color
void test::TestImGui::backgroundColorWindow() {
	ImGui::Begin("Background Color");
	ImGui::ColorPicker3("Color", m_colorBackground);
	ImGui::End();
}

// Window to set the file xml name
void test::TestImGui::levelNameWindow() {
	ImGui::Begin("XML File Name");
	ImGui::Text("Enter a name for your level");
	ImGui::InputText("Name", m_levelName, m_levelNameSize);
	ImGui::InputText("Path", m_levelFilePath, m_levelFilePathSize);
	if (ImGui::Button("Validate")) {
		generateXMLFile();
		m_displayLevelNameWindow = false;
	}
	if (ImGui::Button("Cancel")) {
		m_displayLevelNameWindow = false;
	}
	ImGui::End();
}

// Show to user that the xml file creation is over
void test::TestImGui::fileGenerationSuccessWindow() {
	ImGui::Begin("File generation finished");
	ImGui::Text("Your level file has been generate !");
	if (ImGui::Button("Okay !")) {
		m_displayFileGenerationSuccessWindow = false;
	}
	ImGui::End();
}

// Window to choose music file for this level
void test::TestImGui::musicFileWindow() {
	ImGui::Begin("Music");
	ImGui::InputText("File Path", m_musicFilePath, m_musicFilePathSize);
	ImGui::End();
}

// ImGui Render Structures -------------------------------------------------------------------------------------

// Appearance of creation window when start is selected
void test::TestImGui::startItemsImGuiRender() {
	ImGui::InputInt("X position", &m_startX);
	ImGui::InputInt("Y position", &m_startY);
	ImGui::InputInt("Z position", &m_startZ);
	m_displayCreateBtn = !m_startPlaced;
	if (m_startPlaced && ImGui::Button("Modify")) {
		m_startCube->setPosition(glm::vec3(m_startX, m_startY, m_startZ));
	}
}

// Appearance of creation window when finish is selected
void test::TestImGui::finishItemsImGuiRender() {
	ImGui::InputInt("X position", &m_finishX);
	ImGui::InputInt("Y position", &m_finishY);
	ImGui::InputInt("Z position", &m_finishZ);
	ImGui::InputInt("Width", &m_finishWidth);
	ImGui::InputInt("Height", &m_finishHeight);
	ImGui::InputInt("Length", &m_finishLength);
	m_displayCreateBtn = !m_finishPlaced;
	if (m_finishPlaced && ImGui::Button("Modify")) {
		m_finishCube->setPosition(glm::vec3(m_finishX, m_finishY, m_finishZ));
	}
}

// Appearance of creation window when cube is selected
void test::TestImGui::cubeItemsImGuiRender() {
	ImGui::InputInt("X position", &m_cubeX);
	ImGui::InputInt("Y position", &m_cubeY);
	ImGui::InputInt("Z position", &m_cubeZ);
	ImGui::InputFloat("Size", &m_cubeSize);
	ImGui::InputFloat("Mass", &m_cubeMass);
	ImGui::Combo("Available Materials", &m_currentMaterialInCombo, m_materialsAvailable, IM_ARRAYSIZE(m_materialsAvailable));
	switch(m_currentMaterialInCombo) {
	case 0: // Gold
		m_currentMaterialSelected = Gold;
		break;
	case 1: // Ruby
		m_currentMaterialSelected = Ruby;
		break;
	case 2: // Chrome
		m_currentMaterialSelected = Chrome;
		break;
	case 3: // Pearl
		m_currentMaterialSelected = Pearl;
		break;
	case 4: // Copper
		m_currentMaterialSelected = Copper;
		break;
	case 5: // Obsidian
		m_currentMaterialSelected = Obsidian;
		break;
	case 6: // BlackRubber
		m_currentMaterialSelected = BlackRubber;
		break;
	case 7: // LampMaterial
		m_currentMaterialSelected = LampMaterial;
		break;
	}
	ImGui::Checkbox("Anchor", &m_cubeIsAnchor);
	m_displayCreateBtn = true;
}

// Appearance of creation window when boxe is selected
void test::TestImGui::boxeItemsImGuiRender() {
	ImGui::InputInt("X position", &m_boxeX);
	ImGui::InputInt("Y position", &m_boxeY);
	ImGui::InputInt("Z position", &m_boxeZ);
	ImGui::InputFloat("Size X", &m_boxeSizeX);
	ImGui::InputFloat("Size Y", &m_boxeSizeY);
	ImGui::InputFloat("Size Z", &m_boxeSizeZ);
	ImGui::InputFloat("Mass", &m_boxeMass);
	ImGui::Combo("Available Materials", &m_currentMaterialInCombo, m_materialsAvailable, IM_ARRAYSIZE(m_materialsAvailable));
	switch (m_currentMaterialInCombo) {
	case 0: // Gold
		m_currentMaterialSelected = Gold;
		break;
	case 1: // Ruby
		m_currentMaterialSelected = Ruby;
		break;
	case 2: // Chrome
		m_currentMaterialSelected = Chrome;
		break;
	case 3: // Pearl
		m_currentMaterialSelected = Pearl;
		break;
	case 4: // Copper
		m_currentMaterialSelected = Copper;
		break;
	case 5: // Obsidian
		m_currentMaterialSelected = Obsidian;
		break;
	case 6: // BlackRubber
		m_currentMaterialSelected = BlackRubber;
		break;
	case 7: // LampMaterial
		m_currentMaterialSelected = LampMaterial;
		break;
	}
	ImGui::Checkbox("Anchor", &m_boxeIsAnchor);
	m_displayCreateBtn = true;
}

// Appearance of creation window when sphere is selected
void test::TestImGui::sphereItemsImGuiRender() {
	ImGui::InputInt("X position", &m_sphereX);
	ImGui::InputInt("Y position", &m_sphereY);
	ImGui::InputFloat("Radius", &m_sphereRadius);
	ImGui::InputFloat("Mass", &m_sphereMass);
	ImGui::Checkbox("Anchor", &m_sphereIsAnchor);
	m_displayCreateBtn = true;
}

// Structure Objects creation --------------------------------------------------------------------------------------------

void test::TestImGui::startCreation() {
	m_startPlaced = true;
	m_startCube = ObjectFactory::CreateCube(glm::vec3(m_startX, m_startY, m_startZ), 1, 0, true, Ruby);
	scene->addObject(m_startCube);
}

void test::TestImGui::finishCreation() {
	m_finishPlaced = true;
	m_finishCube = ObjectFactory::CreateCube(glm::vec3(m_finishX, m_finishY, m_finishZ), 1, 0, true, Ruby);
	scene->addObject(m_finishCube);
}

void test::TestImGui::cubeCreation() {
	// Create Cube and add it to the Scene
	glm::vec3 pos = glm::vec3(m_cubeX, m_cubeY, m_cubeZ);
	Object* cube = ObjectFactory::CreateCube(pos, m_cubeSize, m_cubeMass, m_cubeIsAnchor, m_currentMaterialSelected);
	scene->addObject(cube);
	
	// Created the XML line
	std::stringstream ss;
	ss << "<Obj posX=\"" << m_cubeX << "\" posY=\"" << m_cubeY << "\" posZ=\"" << m_cubeZ
		<< "\" sizeX=\"" << m_cubeSize << "\" sizeY=\"" << m_cubeSize << "\" sizeZ=\"" << m_cubeSize
		<< "\" mass=\"" << m_cubeMass << "\" isAnchor=\"" << m_cubeIsAnchor
		<< "\" material=\"" << m_materialsAvailable[m_currentMaterialInCombo] << "\" />\n";
	std::string xml = ss.str();

	// Keep the object saved to be modified with xml line
	m_existingObjectsXML[cube] = xml;
	CreatedObject tmp;
	tmp.obj = cube;
	tmp.isSelected = false;
	tmp.type = "Cube";
	m_existingObjects.push_back(tmp);

	// Reset values
	m_cubeX = 0;
	m_cubeY = 0;
	m_cubeZ = 0;
	m_cubeSize = 10;
	m_cubeMass = 0;
	m_cubeIsAnchor = false;
}

void test::TestImGui::boxeCreation() {
	// Create Cube and add it to the Scene
	glm::vec3 pos = glm::vec3(m_boxeX, m_boxeY, m_boxeZ);
	Object* boxe = ObjectFactory::CreateBoxe(pos, m_boxeSizeX, m_boxeSizeY, m_boxeSizeZ, m_boxeMass, m_boxeIsAnchor, m_currentMaterialSelected);
	scene->addObject(boxe);

	// Created the XML line
	std::stringstream ss;
	ss << "<Obj posX=\"" << m_boxeX << "\" posY=\"" << m_boxeY << "\" posZ=\"" << m_boxeZ
		<< "\" sizeX=\"" << m_boxeSizeX << "\" sizeY=\"" << m_boxeSizeY << "\" sizeZ=\"" << m_boxeSizeZ
		<< "\" mass=\"" << m_boxeMass << "\" isAnchor=\"" << m_boxeIsAnchor
		<< "\" material=\"" << m_materialsAvailable[m_currentMaterialInCombo] << "\" />\n";
	std::string xml = ss.str();

	// Keep the object saved to be modified with xml line
	m_existingObjectsXML[boxe] = xml;
	CreatedObject tmp;
	tmp.obj = boxe;
	tmp.isSelected = false;
	tmp.type = "Boxe";
	m_existingObjects.push_back(tmp);

	// Reset values
	m_boxeX = 0;
	m_boxeY = 0;
	m_boxeZ = 0;
	m_boxeSizeX = 10;
	m_boxeSizeY = 10;
	m_boxeSizeZ = 10;
	m_boxeMass = 0;
	m_boxeIsAnchor = false;
}

void test::TestImGui::sphereCreation() {
	// Create a sphere and add it to the Scene
	// glm::vec3 pos = glm::vec3(m_sphereX, m_sphereY, (m_foregroundMode ? 10 : m_sphereZ));
	// Object* cube = ObjectFactory.createCube(pos, m_cubeSize, m_cubeMass, m_cubeIsAnchor, m_currentMaterialSelected);
	
	// Reset values
	m_sphereX = 0;
	m_sphereY = 0;
	m_sphereRadius = 5;
	m_sphereMass = 0;
	m_sphereIsAnchor = false;
}

// XML File Generation --------------------------------------------------------------------------------------------

void test::TestImGui::generateXMLFile() {
	std::ofstream outfile(std::string(m_levelFilePath) + std::string(m_levelName) + ".xml");

	outfile << "<Level name=\"" << m_levelName << "\">\n";
	outfile << "<Background>\n";
	outfile << "<Color red=\"" << m_colorBackground[0] << "\" green=\"" << m_colorBackground[1] << "\" blue=\"" << m_colorBackground[2] << "\"/>\n";
	outfile << "<Music name=\"" << m_musicFilePath << "\"/>\n";
	outfile << "<Light posX=\"" << (scene->retrieveLight().getPosition())[0] 
		<< "\" posY=\"" << (scene->retrieveLight().getPosition())[1]
		<< "\" posZ=\"" << (scene->retrieveLight().getPosition())[2] << "\"/>\n";
	outfile << "</Background>\n";
	outfile << "<Structure>\n";
	outfile << "<Start posX=\"" << m_startX << "\" posY=\"" << m_startY << "\" posZ=\"" << m_startZ << "\"/>\n";
	outfile << "<End posX=\"" << m_finishX << "\" posY=\"" << m_finishY << "\" posZ=\"" << m_finishZ
		<< "\" length=\"" << m_finishLength << "\" width=\"" << m_finishWidth << "\" height=\"" << m_finishHeight << "\"/>\n";
	outfile << "<Objects>\n";
	for (std::unordered_map<Object*, std::string>::iterator it = m_existingObjectsXML.begin(); it != m_existingObjectsXML.end(); it++) {
		outfile << it->second;
	}
	outfile << "</Objects>\n";
	outfile << "</Structure>\n";
	outfile << "</Level>";

	outfile.close();

	// Display message to user about file creation end
	m_displayFileGenerationSuccessWindow = true;
}

// Interaction with mouse ----------------------------------------------------------------------------------------

void test::TestImGui::processMouseAction() {
	if (!ImGui::IsMouseHoveringAnyWindow() && !ImGui::IsAnyItemActive()) {
		if (ImGui::IsMouseDoubleClicked(0) || ImGui::IsMouseClicked(0)) {
			//glClearColor(1, 0, 0, 1.0f);
		}
		else if (ImGui::IsMouseDoubleClicked(1) || ImGui::IsMouseClicked(1)) {
			//glClearColor(1, 0, 0, 1.0f);
		}
		else if (ImGui::IsMouseDragging(0) || ImGui::IsMouseDragging(1)) {
			//glClearColor(0, 0, 1, 1.0f);
		}
		else if (ImGui::IsMouseReleased(0) || ImGui::IsMouseReleased(1)) {
			//glClearColor(1, 0, 1, 1.0f);
		}
		else {
			//glClearColor(0, 1, 0, 1.0f);
		}
	}
	else {
		//glClearColor(0, 1, 0, 1.0f);
	}
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Interactions with controller / joystick ------------------------------------------------------------------------

void test::TestImGui::processJoystickInput() {
	// Get controller's state
	int nbJoystickAxes, nbJoystickButtons; // taille 6 et 14 avec manette xbox 360
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes); 
	// 0 : left joystick -> axe X (cosinus)
	// 1 : left joystick -> axe Y (sinus)
	// 2 : right joystick -> axe X (cosinus)
	// 3 : right joystick -> axe Y (sinus)
	// 4 : LT button
	// 5 : RT button
	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);
	// 0 : A button
	// 1 : B button
	// 2 : X button
	// 3 : Y button
	// 4 : LB button
	// 5 : RB button
	// 6 : Back button
	// 7 : Start button
	// 8 : left joystick click
	// 9 : right joystick click
	// 10 : top cross
	// 11 : right cross
	// 12 : bottom cross
	// 13 : left cross

	// Process actions
	if (buttons[7] == GLFW_PRESS) {
		//glClearColor(1, 0, 0, 1.0f);
	}
	else {
		//glClearColor(0, 1, 0, 1.0f);
	}
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Camera management ----------------------------------------------------------------------------------------------

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//on quitte le mode camera
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(window, NULL);
	}


	//on rentre en mode camera
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(window, mouse_callback);
	}


	//ZQSD mode
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
		printf("Z");
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
		printf("S");
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera->ProcessKeyboard(LEFT, deltaTime);
		printf("Q");
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
		printf("D");
	}

	//Mode fleches
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}