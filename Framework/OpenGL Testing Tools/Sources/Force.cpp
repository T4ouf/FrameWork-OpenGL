#include "Force.h"
#include "glm/gtc/matrix_transform.hpp"



Force::Force(glm::vec3 direction): m_direction(direction)
{
}

Force::~Force()
{
}

Force* Force::AddForce(Force* other)
{
	glm::vec3 sum = other->GetDirection() + m_direction;
	//two added forces should have the same application point, might change when moments are added
	return new Force(sum);
}


Force Force::MultiplyByScalar(float k)
{
	glm::vec3 newDirection = m_direction * k;
	Force result(newDirection);
	return result;
}

