#pragma once

#include <vector>

#include "Camera.h"
#include "Light.h"
#include "Object.h"

//TO KEEP HERE ??? (For camera stuff below)
#include "GLFW/glfw3.h"

class Scene {
private:
	Camera m_camera;
	Light m_light;

	std::vector<Object*> m_sceneObjects;
	std::vector<Force*> m_sceneForces;

	glm::mat4 m_projectionMatrix;	//DANS CAMERA OU ICI ?

public:

	Scene(const Camera& camera = Camera(), const Light& light = Light());
	~Scene();

	void addObject(Object* o);
	void addGlobalForce(Force* f);

	//clear the scene objects
	void clearScene();

	Light& retrieveLight();
	Camera& retrieveCamera();

	void setLightColor(glm::vec3 color);
	void setLightColor(float r, float g, float b,float a=1.0f);
	void setLightPos(glm::vec3 pos);
	void setLightPos(float x, float y, float z);
	void setLight(Light light);

	void Render(Renderer* renderer);
	void UpdatePhysic(double deltaTime); //Apply physics to all the scene objects


					/*Camera Stuff => to keep here ?*/
	/*TODO change camera de FPS (actuellement) à normale => translate, rotate, etc.*/
	/*
	// settings
	const unsigned int SCR_WIDTH = 960;
	const unsigned int SCR_HEIGHT = 540;

	// camera
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;

	// timing
	float deltaTime = 0.0f;	// time between current frame and last frame
	float lastFrame = 0.0f;

	void processInput(GLFWwindow *window);

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	// glfw: whenever the mouse moves, this callback is called
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	*/
};