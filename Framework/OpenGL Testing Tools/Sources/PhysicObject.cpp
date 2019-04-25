#include "PhysicObject.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>


void PhysicObject::CenterToCorner(glm::vec3& position)
{
	OffsetPosition(-1.0f, position);
}

void PhysicObject::CornerToCenter(glm::vec3& position)
{
	OffsetPosition(1.0f, position);
}

/*
	Sign must be 1 or -1 exclusively. 1 gets the position from the corner to the center, -1 does the opposite
*/
void PhysicObject::OffsetPosition(float sign, glm::vec3& position)
{
	position += sign * glm::vec3(m_width / 2, m_height / 2, m_length / 2);
}

PhysicObject::PhysicObject(glm::vec3* position, double width, double height, double length, double mass, bool isAnchor): m_width(width), m_height(height), m_length(length), m_speed(glm::vec3(0.0)), m_mass(mass), m_isAnchor(isAnchor)
{
	m_forces = std::vector<Force*>();
}

PhysicObject::~PhysicObject()
{
}

void PhysicObject::ApplyForce(Force* f)
{
	m_forces.push_back(f);
}

glm::vec3 PhysicObject::Update(float delta, glm::vec3& position){
	if (this->m_isAnchor) {
		this->m_speed = glm::vec3(0.0f, 0.0f, 0.0f);
		return this->m_speed;
	}
	Force total = Force(glm::vec3(0));
	for (std::vector<int>::size_type i = 0; i < m_forces.size(); i++) {
		total.AddSelfForce(m_forces.at(i));
	}
	//Remove all applied forces so applied forces can change over time
	m_forces.clear();
	m_speed = m_speed + total.MultiplyByScalar(delta).GetDirection();
	position = position + m_speed*delta;
	return m_speed * delta;
}

std::vector<glm::vec3> PhysicObject::GetCornersPos(glm::vec3& position)
{
	std::vector<glm::vec3> corners = std::vector<glm::vec3>();
	glm::vec3 corner(position.x, position.y, position.z);
	corners.push_back(corner);
	std::cout << "Error : operation not yet supported" << std::endl;
	return corners;
}

bool PhysicObject::CollidesWith(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition){

	CenterToCorner(thisPosition);

	/* Ver Thomas
	float thisLeft = thisPosition.x - this->m_width / 2, thatRight = thatPosition.x + that->m_width/2, thisRight = thisPosition.x + this->m_width/2, thatLeft = thatPosition.x - that->m_width / 2;
	float thisTop = thisPosition.y + this->m_height / 2, thatBot = thatPosition.y - that->m_height/2, thisBot = thisPosition.y - this->m_height/2, thatTop = thatPosition.y + that->m_height / 2;
	float thisFront = thisPosition.z + this->m_length / 2, thatBehind = thatPosition.z - that->m_length/2, thisBehind = thisPosition.z - this->m_length/2, thatFront = thatPosition.z + that->m_length / 2;
	*/

	//These are the coordinates for each sides
	float thisLeft = thisPosition.x, thatRight = thatPosition.x + that->m_width, thisRight = thisPosition.x + this->m_width, thatLeft = thatPosition.x;
	float thisTop = thisPosition.y, thatBot = thatPosition.y + that->m_height, thisBot = thisPosition.y + this->m_height, thatTop = thatPosition.y;
	float thisFront = thisPosition.z, thatBehind = thatPosition.z + that->m_length, thisBehind = thisPosition.z + this->m_length, thatFront = thatPosition.z;
	
	if(thisLeft < thatRight && thisRight > thatLeft){//horizontal check
		if(thisTop < thatBot && thisBot > thatTop) {//vertical check
			if(thisFront < thatBehind && thisBehind > thatFront) {//depth check

				//Now calculates distance to each sides, to determine on which the collision has occured
				//All distances are from the indicated sides on this object
				float distances[] = {/*leftDistance = */ abs(thisLeft - thatRight), /*rightDistance = */abs(thisRight - thatLeft), /*topDistance = */abs(thisTop - thatBot), /*botDistance = */abs(thisBot - thatTop), /*frontDistance = */abs(thisFront - thatBehind), /*behindDistance =*/ abs(thisBehind - thatFront) };
				int indexOfSmallestDistance = 0;

				for (int i = 1; i < 6; i++) {
					if (distances[i] < distances[indexOfSmallestDistance]) indexOfSmallestDistance = i;
				}

				//Same order of sides in distances and Sides enum
				*collisionSide = (Sides)indexOfSmallestDistance;
				CornerToCenter(thisPosition);
				return true;
			}
		}
	}
	CornerToCenter(thisPosition);
	return false;
}

void PhysicObject::OnCollision(PhysicObject* that, Sides* collisionSide, glm::vec3& thisPosition, glm::vec3& thatPosition){
	Sides side = *collisionSide;
	glm::vec3 normalToFace;
	glm::vec3 thisSpeed = this->m_speed, thatSpeed = that->m_speed; 

	if (side == LEFTSIDE) normalToFace = glm::vec3(-1.0f, 0.0f, 0.0f);
	else if(side == RIGHTSIDE) normalToFace = glm::vec3(1.0f, 0.0f, 0.0f);
	else if (side == TOP) normalToFace = glm::vec3(0.0f, -1.0f, 0.0f);
	else if (side == BOT) normalToFace = glm::vec3(0.0f, 1.0f, 0.0f);
	else if (side == FRONT) normalToFace = glm::vec3(0.0f, 0.0f, -1.0f);
	else if (side == BEHIND) normalToFace = glm::vec3(0.0f, 0.0f, 1.0f);

	float thisNormalComponent = glm::dot(normalToFace, thisSpeed);

	//If the other object is immobile, simply reverse the normal component of this speed
	if (thatSpeed == glm::vec3(0.0f)) {
		thisSpeed = -2 * thisNormalComponent * normalToFace + thisSpeed;
		this->m_speed = thisSpeed;
		this->Update(2.0f, thisPosition);
		return;
	}

	float thatNormalComponent = glm::dot(normalToFace, thatSpeed);

	thisSpeed = (-2.0f * thisNormalComponent * normalToFace + thisSpeed) * (float) (that->m_mass/(this->m_mass+that->m_mass));
	thatSpeed = (-2.0f * thatNormalComponent * normalToFace + thatSpeed) * (float) (this->m_mass / (this->m_mass + that->m_mass));

	glm::vec3 distanceCenterToCenter = thisPosition - thatPosition;


	this->m_speed = glm::vec3(thisSpeed);
	that->m_speed = glm::vec3(thatSpeed);
	this->Update(2.0f, thisPosition);
}

glm::vec3 PhysicObject::GetSpeed()
{
	return m_speed;
}

void PhysicObject::SetSpeed(glm::vec3 speed)
{
	m_speed = speed;
}


Force Force::MultiplyByMatrix(glm::mat4 mat)
{
	glm::vec3 newDirection = mat * glm::vec4(m_direction, 1.0f);
	Force result(newDirection);
	return result;
}

void PhysicObject::Scale(float xFactor, float yFactor, float zFactor)
{
	m_width *= xFactor;
	m_height *= yFactor;
	m_length *= zFactor;
}
