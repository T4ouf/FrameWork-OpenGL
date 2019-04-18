#include "Scene.h"
#include "Force.h"

Scene::Scene(const Camera& camera , const Light& light) : 
	m_camera(camera), m_light(light), 
	m_projectionMatrix(glm::perspective(glm::radians(45.0f), 16.0f / 9.0f, 1.0f, 5000.0f)){

}

Scene::~Scene() {

}

void Scene::addObject(Object* o) {
	m_sceneObjects.push_back(o);
}

void Scene::addGlobalForce(Force * f){

	m_sceneForces.push_back(f);

}

void Scene::clearScene() {

	//We delete every object of the scene
	for (int i = 0; i < m_sceneObjects.size(); i++){
		delete m_sceneObjects.at(i); //The scene is owner of the objects' pointer
	}

	m_sceneObjects.clear();
	m_sceneForces.clear();		//We clear forces BUT don't delete forces of the scene
								//The scene is not the owner of the global forces' pointer

}

Light & Scene::retrieveLight(){
	return m_light;
}

Camera & Scene::retrieveCamera()
{
	return m_camera;
}

void Scene::setLightColor(glm::vec3 color){
	m_light.m_color = glm::vec4(color,1.0f);
}

void Scene::setLightColor(float r, float g, float b, float a){
	m_light.m_color = glm::vec4(r,g,b,a);
}

void Scene::setLightPos(glm::vec3 pos){
	m_light.setPosition(pos);
}

void Scene::setLightPos(float x, float y, float z){
	m_light.setPosition(x, y, z);
}

void Scene::setLight(Light light){
	m_light = light;
}

void Scene::Render(Renderer* renderer){

	//1) Setup rendering (Light + Camera)
	//2) Render every Object
	for (auto o : m_sceneObjects) {
		o->m_graphicObject->GetMaterial()->Bind();
		Shader* s = o->m_graphicObject->GetMaterial()->getShader();

		s->Bind(); //binding the shader
		//Setting up the camera
		s->SetUniformMat4f("u_ProjectionMat", m_projectionMatrix);
		s->SetUniformMat4f("u_ViewMat", m_camera.GetViewMatrix());

		//Material shader
		if (o->m_graphicObject->GetMaterial()->getShaderName() == MaterialShader) {

			//Seting up the light
			s->SetUniform3f("u_LightPos", m_light.getPosition().x, m_light.getPosition().y, m_light.getPosition().z);
			s->SetUniform3f("u_LightColor", m_light.m_color.r, m_light.m_color.g, m_light.m_color.b);

			//Camera Position use to compute light effects
			s->SetUniform3f("u_ViewPos", m_camera.m_position.x, m_camera.m_position.y, m_camera.m_position.z);
		}
		else if (o->m_graphicObject->GetMaterial()->getShaderName() == LightSourceShader) {

			s->SetUniform4f("u_LightColor", m_light.m_color.r, m_light.m_color.g, m_light.m_color.b, m_light.m_color.a);

		}

		//render the object
		o->Render(*(Renderer::getInstance()));


		

	}


}

void Scene::UpdatePhysic(double deltaTime){

	//We compute pair of object's collision
	for (size_t i = 0; i < m_sceneObjects.size(); i++) {

		Object* o1 = m_sceneObjects.at(i);
		bool ObjectAnchor = m_sceneObjects.at(i)->isAnchor();

		for (size_t j = i; j < m_sceneObjects.size(); j++) {

			Object* o2 = m_sceneObjects.at(j);

			//if the first object is anchor...
			if (ObjectAnchor) {

				//but not the 2nd one...
				if (!m_sceneObjects.at(j)->isAnchor()) {

					Sides collisionSide;

					//if collision :
					if (o2->m_physicObject->CollidesWith(o1->m_physicObject, &collisionSide)) {
						//then we compute the collision (reverse)
						o2->m_physicObject->OnCollision(o1->m_physicObject, &collisionSide);
					}
				}
			}
			//Otherwhise (moving object)
			else {
				//=> We compute the collision anyway :
				Sides collisionSide;

				//if collision :
				if (o1->m_physicObject->CollidesWith(o2->m_physicObject, &collisionSide)) {
					//then we compute the collision
					o1->m_physicObject->OnCollision(o2->m_physicObject, &collisionSide);
				}	
			}
		}
		//We update the object's physic
		o1->UpdatePhysic(m_sceneForces, deltaTime);
	}
}
