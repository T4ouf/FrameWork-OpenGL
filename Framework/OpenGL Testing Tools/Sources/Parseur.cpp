#include "Parseur.h"


Parseur::Parseur(std::string pathFile) {
	path = pathFile;
	// Initiate the structure
	lvl.startPosX = 0.0f;
	lvl.startPosY = 0.0f;
	lvl.startPosZ = 0.0f;
	lvl.levelPath = "";
	lvl.musicPath = "";
	lvl.endPosX = 0.0f;
	lvl.endPosY = 0.0f;
	lvl.endPosZ = 0.0f;
	lvl.length = 0.0f;
	lvl.width = 0.0f;
	lvl.height = 0.0f;
}

Parseur::~Parseur() {

}


void Parseur::setPath(std::string pathFile) {
	path = pathFile;
}

std::string Parseur::getPath() {
	return path;
}

Level Parseur::parse(Scene& scene) {
	// Clear the structure
	lvl.startPosX = 0.0f;
	lvl.startPosY = 0.0f;
	lvl.startPosZ = 0.0f;
	lvl.levelPath = "";
	lvl.musicPath = "";
	lvl.endPosX = 0.0f;
	lvl.endPosY = 0.0f;
	lvl.endPosZ = 0.0f;
	lvl.length = 0.0f;
	lvl.width = 0.0f;
	lvl.height = 0.0f;

	// ************** Open File ***************//
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> * root_node;
	std::ifstream theFile(this->path);

	// if the file exist and is open
	if (theFile) {

		// declaration of buffer to read file
		std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		doc.parse<0>(&buffer[0]);
		// first node level
		root_node = doc.first_node("Level");
		std::string nameLevel = root_node->first_attribute("name")->value();
		lvl.levelPath = nameLevel;
		// first node Background
		rapidxml::xml_node<> * background_node = root_node->first_node("Background");

		//*********************** COLOR / MUSIC / LIGHT ******************//
		// first node name Color if it exists
		rapidxml::xml_node<>* color_node = background_node->first_node("Color");
		if (color_node != nullptr) {
			std::string::size_type sz;
			float r, g, b;
			r = std::stof(color_node->first_attribute("red")->value(), &sz);
			g = std::stof(color_node->first_attribute("green")->value(), &sz);
			b = std::stof(color_node->first_attribute("blue")->value(), &sz);
			scene.setBackgroundColor(r/255.0f,g/255.0f,b/255.0f);
		}
		rapidxml::xml_node<>* music_node = background_node->first_node("Music");
		if (music_node != nullptr) {
			std::string musicName = music_node->first_attribute("name")->value();
			lvl.musicPath = musicName;
		}
		rapidxml::xml_node<>* light_node = background_node->first_node("Light");
		if (light_node != nullptr) {
			float posX, posY, posZ;
			posX = std::stof(light_node->first_attribute("posX")->value());
			posY = std::stof(light_node->first_attribute("posY")->value());
			posZ = std::stof(light_node->first_attribute("posZ")->value());
			scene.setLightPos(posX/255.0f, posY/255.0f, posZ/255.0f);
		}

		//*********************** START, END AND OBJECT ******************//
		// node structure of Level
		rapidxml::xml_node<> * structure_node = root_node->first_node("Structure");
		// node start of structure
		rapidxml::xml_node<> * start_node = structure_node->first_node("Start");
		if (start_node != nullptr) {
			float posX = std::stof(start_node->first_attribute("posX")->value());
			float posY = std::stof(start_node->first_attribute("posY")->value());
			float posZ = std::stof(start_node->first_attribute("posZ")->value());
			// Store in structure
			lvl.startPosX = posX;
			lvl.startPosY = posY;
			lvl.startPosZ = posZ;
		}
		// node end of structure
		rapidxml::xml_node<> * end_node = structure_node->first_node("End");
		if (end_node != nullptr) {
			float posX = std::stof(end_node->first_attribute("posX")->value());
			float posY = std::stof(end_node->first_attribute("posY")->value());
			float posZ = std::stof(end_node->first_attribute("posZ")->value());
			float length = std::stof(end_node->first_attribute("length")->value());
			float width = std::stof(end_node->first_attribute("width")->value());
			float height = std::stof(end_node->first_attribute("height")->value());
			// Store in structure
			lvl.endPosX = posX;
			lvl.endPosY = posY;
			lvl.endPosZ = posZ;
			lvl.length = length;
			lvl.width = width;
			lvl.height = height;
		}

		rapidxml::xml_node<>* objects_node = structure_node->first_node("Objects");
		if (objects_node != nullptr) {

			for (rapidxml::xml_node<> * obj_node = objects_node->first_node("Obj");; obj_node = obj_node->next_sibling()) {

				if (obj_node != nullptr) {
					if (strcmp(obj_node->name(), "Obj") != 0) {
						break;
					}
					// We get attribut object
					float posX = std::stof(obj_node->first_attribute("posX")->value());
					float posY = std::stof(obj_node->first_attribute("posY")->value());
					float posZ = std::stof(obj_node->first_attribute("posZ")->value());
					float sizeX = std::stof(obj_node->first_attribute("sizeX")->value());
					float sizeY = std::stof(obj_node->first_attribute("sizeY")->value());
					float sizeZ = std::stof(obj_node->first_attribute("sizeZ")->value());
					float mass = std::stof(obj_node->first_attribute("mass")->value());
					bool isAnchor = atoi(obj_node->first_attribute("isAnchor")->value());
					std::string nameMaterial = obj_node->first_attribute("material")->value();

					Texture * texture = new Texture("Ressources/Textures/texture-caisse.png");
					Shader * shader = new Shader("Ressources/Shaders/Material.shader");
					Material * material;
					// we create material according to the name of material in the file
					if (nameMaterial == "Gold") {
						material = new Material(shader, texture,
							glm::vec3(0.329412f, 0.223529f, 0.027451f),
							glm::vec3(0.780392f, 0.568627f, 0.113725f),
							glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f);
					}
					else if (nameMaterial == "Ruby") {
						material = new Material(shader, texture,
							glm::vec3(0.1745f, 0.01175f, 0.01175f),
							glm::vec3(0.61424f, 0.04136f, 0.04136f),
							glm::vec3(0.727811f, 0.626959f, 0.626959f), 0.6f * 128);
					}
					else if (nameMaterial == "Chrome") {
						material = new Material(shader, nullptr,
							glm::vec3(0.25f, 0.25f, 0.25f),
							glm::vec3(0.4f, 0.4f, 0.4f),
							glm::vec3(0.774597f, 0.774597f, 0.774597f), 0.6f * 128);
					}
					else if (nameMaterial == "Pearl") {
						material = new Material(shader, nullptr,
							glm::vec3(0.25f, 0.20725f, 0.20725f),
							glm::vec3(1.0f, 0.829f, 0.829f),
							glm::vec3(0.296648f, 0.296648f, 0.296648f), 0.088f * 128);
					}
					else if (nameMaterial == "Copper") {
						material = new Material(shader, nullptr,
							glm::vec3(0.19125f, 0.0735f, 0.0225f),
							glm::vec3(0.7038f, 0.27048f, 0.0828f),
							glm::vec3(0.256777f, 0.137622f, 0.086014f), 0.1f * 128);
					}
					else if (nameMaterial == "Obsidian") {
						material = new Material(shader, nullptr,
							glm::vec3(0.05375f, 0.05f, 0.06625f),
							glm::vec3(0.18275f, 0.17f, 0.22525f),
							glm::vec3(0.332741f, 0.328634f, 0.346435f), 0.3f * 128);
					}
					else if (nameMaterial == "BlackRubber") {
						material = new Material(shader, nullptr,
							glm::vec3(0.02f, 0.02f, 0.02f),
							glm::vec3(0.01f, 0.01f, 0.01f),
							glm::vec3(0.4f, 0.4f, 0.4f), 10.0f);
					}
					else if (nameMaterial == "LampMaterial") {
						material = new Material(LightSourceShader, "Ressources/Textures/texture-caisse.png",
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
					}
					else {
						nameMaterial = "No";
						material = new Material(shader, nullptr,
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3(1.0f, 1.0f, 1.0f),
							glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
					}
					
					//add object at the scene
					Object *b = ObjectFactory::CreateBoxe(glm::vec3(posX, posY, posZ), sizeX, sizeY, sizeZ, mass, isAnchor, material);
					scene.addObject(b);

					if (obj_node->next_sibling() == nullptr) {
						break;
					}
				}
			}
		}

		doc.clear();
		
	}
	else {
		// There is a error with file
		std::cerr << "Impossible to open file !" << std::endl;
	}
	return lvl;
}
