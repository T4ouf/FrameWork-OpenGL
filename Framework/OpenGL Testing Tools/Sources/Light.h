#pragma once

#include "Positionable.h"
#include "glm/glm.hpp"

struct Light : public Positionable {
	float m_intensity; 
	glm::vec4 m_color; 

	Light(glm::vec3 position = glm::vec3(0.0f), float intensity = 1.0f, glm::vec4 color = glm::vec4(1.0f));
};