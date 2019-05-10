#pragma once
#include "glm/glm.hpp"
#include "Force.h"
#include <vector>

enum Sides { LEFTSIDE, RIGHTSIDE, TOP, BOT, FRONT, BEHIND };

class PhysicObject
{
private:
	//glm::vec3* m_position;
	glm::vec3 m_speed;//position is left corner, for collisions
	double m_height, m_length, m_mass, m_bounceCoeff;//x, y and z axis dimension of the axis aligned bounding box
	double m_width;
	bool m_isAnchor;
	std::vector<Force*> m_forces;
	int m_counter;

	void CenterToCorner(glm::vec3& position, PhysicObject* object);
	void CornerToCenter(glm::vec3& position, PhysicObject* object);
	void OffsetPosition(float sign, glm::vec3& position, PhysicObject* object);
public:
	PhysicObject(glm::vec3* position, double width, double height, double length, double mass, bool isAnchor, double bounceCoeff);
	PhysicObject(glm::vec3* position, double width, double height, double length, double mass, bool isAnchor);
	PhysicObject(glm::vec3* position, double size, double mass, bool isAnchor);
	~PhysicObject();

	void ApplyForce(Force* f);
	glm::vec3 Update(float delta, glm::vec3& position);

	std::vector<glm::vec3> GetCornersPos(glm::vec3& position);

	bool CollidesWith(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition);
	void OnCollision(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition);

	glm::vec3 GetSpeed();
	void SetSpeed(glm::vec3 speed);
	void SetBounceCoeff(double coeff);
	bool IsAnchor() { return m_isAnchor; }
	void Scale(float xFactor, float yFactor, float zFactor);
};
