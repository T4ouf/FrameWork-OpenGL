#pragma once
#include "glm/glm.hpp"

class Force
{
private:
	glm::vec3 m_direction;
public:
	Force(glm::vec3 direction);
	~Force();

	Force* AddForce(Force* other);
	void AddSelfForce(Force* other);

	glm::vec3 GetDirection() {
		return m_direction;
	}

	glm::vec3* GetPointerToDirection() {
		return &m_direction;
	}

	Force MultiplyByScalar(float k);
	Force MultiplyByScalar(int k);
	void MultiplySelfByScalar(float k);
	Force MultiplyByMatrix(glm::mat4 matrix);
	Force Reverse();
};

