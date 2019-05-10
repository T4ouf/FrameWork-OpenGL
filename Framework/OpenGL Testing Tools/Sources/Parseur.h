#pragma once
#include "Scene.h"
#include "ObjectFactory.h"
#include "xml/rapidxml.hpp"
#include "xml/rapidxml_utils.hpp"
#include <iostream>
#include <string>


struct Level {
	std::string levelPath;
	std::string musicPath;
	float startPosX, startPosY, startPosZ;
	float endPosX, endPosY, endPosZ, length, width, height;
};

class Parseur {

private:
	std::string path;
	Level lvl;

public:

	Parseur(std::string path);
	~Parseur();
	void setPath(std::string path);
	std::string getPath();
	Level parse(Scene& s);

};