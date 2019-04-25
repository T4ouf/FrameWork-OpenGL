#include "Object.h"
//printing mat and vec
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Object::Object(Mesh * graphics, PhysicObject * physic, glm::vec3 position):Positionable(position){

	m_tmpForces = std::vector<Force*>();

	m_graphicObject = graphics;
	m_physicObject = physic;

	m_modelMatrix = glm::identity<glm::mat4>();
	m_modelMatrix = glm::translate(glm::mat4(1.0f), position);

	//std::cout << glm::to_string(m_modelMatrix) << "\n\n";

}

Object::~Object(){
	//We have the ownership of those pointers => must be delete correctly
	delete m_graphicObject;
	delete m_physicObject;
}

void Object::updatePos(glm::mat4 ModelMatrix){

	m_position = ModelMatrix * glm::vec4(m_position,1.0f);
}

bool Object::isAnchor()
{
	return m_physicObject->IsAnchor();
}

void Object::addForce(Force * f){
	m_tmpForces.push_back(f);
	m_physicObject->ApplyForce(f);
}

void Object::translate(glm::vec3 translation){

	m_modelMatrix = glm::translate(m_modelMatrix,translation);
	//Positionable::translate(translation);
	
	//std::cout << glm::to_string(m_modelMatrix * glm::vec4(m_position,1.0f)) << "\n\n";
	updatePos(m_modelMatrix);
}

void Object::scale(float factor){

	m_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(factor, factor, factor));
	m_physicObject->Scale(factor, factor, factor);

	//std::cout << glm::to_string(glm::vec4(m_position,1.0f)) << "\n\n";
	// POSITION IS CHANGING ????? UPDATE OR NOT ?
	//updatePos(m_modelMatrix);

}

void Object::scale(float xfactor, float yfactor, float zfactor){

	m_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(xfactor, yfactor, zfactor));
	m_physicObject->Scale(xfactor, yfactor, zfactor);

	// POSITION IS CHANGING ????? UPDATE OR NOT ?
	//updatePos(m_modelMatrix);
}


void Object::Render(Renderer& renderer){

	Material* material = m_graphicObject->GetMaterial();
	material->Bind();
	material->getShader()->SetUniformMat4f("u_ModelMat", m_modelMatrix);

	if (m_graphicObject->GetMaterial()->getShaderName() == MaterialShader) {

		material->getShader()->SetUniformMat4f("u_InverseModelMat", glm::inverse(m_modelMatrix));
	
	}
	else if (m_graphicObject->GetMaterial()->getShaderName() == LightSourceShader) {
		//Nothing now...
	}

	m_graphicObject->Render(renderer);

	//reset modelMatrix
	m_modelMatrix = glm::translate(glm::mat4(1.0f), m_position);
}

void Object::UpdatePhysic(const std::vector<Force*>& OutsideForces, double deltaTime){

	for (size_t i = 0; i < OutsideForces.size(); i++) {
		//Apply Outside forces
		m_physicObject->ApplyForce(OutsideForces.at(i));
	}

	//Update the object's physic
	glm::vec3 deltaPos = m_physicObject->Update(deltaTime, m_position);

	//clear the temp forces
	for (auto f:m_tmpForces) {
		delete f;
	}
	m_tmpForces.clear();

	m_modelMatrix = glm::translate(m_modelMatrix, deltaPos);

}

void Object::setPosition(float x, float y, float z){

	m_modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x,y,z));
	Positionable::setPosition(x, y, z);
	//Adapt the Model Matrix

}

void Object::setPosition(glm::vec3 pos){

	m_modelMatrix = glm::translate(glm::mat4(1.0f), pos);
	Positionable::setPosition(pos);
	//Adapt the Model Matrix

}
