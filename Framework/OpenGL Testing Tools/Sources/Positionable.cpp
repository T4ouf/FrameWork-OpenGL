#include "Positionable.h"

Positionable::Positionable(float x, float y, float z) : m_position(glm::vec3(x,y,z)){

}

Positionable::Positionable(glm::vec3 pos) : m_position(pos){

}


void Positionable::translate(float x, float y, float z) {

	m_position.x += x;
	m_position.y += y;
	m_position.z += z;


}

void Positionable::translate(glm::vec3 translation) {

	translate(translation.x, translation.y, translation.z);

}

void Positionable::setPosition(float x, float y, float z){

	m_position = glm::vec3(x, y, z);

}

void Positionable::setPosition(glm::vec3 pos) {

	m_position = pos;

}
