#pragma once

#include "glm/glm.hpp"

#include "Mesh.h"
#include "PhysicObject.h"
#include "Positionable.h"

class Object : public Positionable{
private: 
	Mesh* m_graphicObject;
	PhysicObject* m_physicObject;

	glm::mat4 m_modelMatrix;
	
	void updatePos(glm::mat4 ModelMatrix);

	std::vector<Force*> m_tmpForces;

	friend class Scene;

	
public: 

	Object(Mesh* graphics, PhysicObject* physic, glm::vec3 position);
	~Object();
	
	bool isAnchor();

	void addForce(Force* f);

	//Object transformation
	//void translate(glm::vec3 translation) override;
	void scale(float factor);
	void scale(float xfactor, float yfactor, float zfactor);
	//void rotate(glm::vec3 axis, float DegreeAngle)

	void Render(Renderer& renderer);
	void UpdatePhysic(const std::vector<Force*>& OutsideForces, double deltaTime);

};
