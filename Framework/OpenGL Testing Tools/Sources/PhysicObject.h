#pragma once
#include "glm/glm.hpp"
#include "Force.h"
#include <vector>

enum Sides {LEFTSIDE, RIGHTSIDE, TOP, BOT, FRONT, BEHIND};

class PhysicObject
{
private:
	//glm::vec3* m_position;
	glm::vec3 m_speed;//position is left corner, for collisions
	double m_width, m_height, m_length, m_mass;//x, y and z axis dimension of the axis aligned bounding box
	bool m_isAnchor;
	std::vector<Force*> m_forces;
	int m_counter;

	void CenterToCorner(glm::vec3& position);
	void CornerToCenter(glm::vec3& position);
	void OffsetPosition(float sign, glm::vec3& position);
public:
	PhysicObject(glm::vec3* position, double width, double height, double length, double mass, bool isAnchor);
	~PhysicObject();

	void ApplyForce(Force* f);
	glm::vec3 Update(float delta, glm::vec3& position);

	std::vector<glm::vec3> GetCornersPos(glm::vec3& position);

    bool CollidesWith(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition);
    void OnCollision(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition);

	glm::vec3 GetSpeed();
	void SetSpeed(glm::vec3 speed);
	bool IsAnchor(){return m_isAnchor;}
	void Scale(float xFactor, float yFactor, float zFactor);
};

