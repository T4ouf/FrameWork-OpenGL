#include "Positionable.h"

Positionable::Positionable(float x, float y, float z) : m_position(glm::vec3(x,y,z)){

}

Positionable::Positionable(glm::vec3 pos) : m_position(pos){

}

void Positionable::setPosition(float x, float y, float z){

	m_position = glm::vec3(x, y, z);

}

void Positionable::setPosition(glm::vec3 pos) {

	m_position = pos;

}
