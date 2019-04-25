#pragma once

#include "glm/glm.hpp"

class Positionable {

private:


protected:

	glm::vec3 m_position;

public:

	Positionable(float x = 0, float y = 0, float z = 0);
	Positionable(glm::vec3 pos);

	const glm::vec3& getPosition() const { return m_position; }

	virtual void translate(float x, float y, float z);
	virtual void translate(glm::vec3 translation);
	
	virtual void setPosition(float x, float y, float z) ;
	virtual void setPosition(glm::vec3 pos);

};