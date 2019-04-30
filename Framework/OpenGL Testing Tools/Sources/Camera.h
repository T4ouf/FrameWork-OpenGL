#ifndef CAMERA_H
#define CAMERA_H


//CAMERA CLASS INSPIRED FROM learnOpenGL.com and Modified:
//Accessible here :
//https ://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Object.h"

#include <vector>

//TODO FINIR LA CAMERA AU PROPRE : https://github.com/TheCherno/Sparky/tree/master/Sparky-core/src/sp/graphics/camera

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera{

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

public:

	/*TODO : FAIRE HERITER LA CAM DE POSITIONABLE*/

	// Camera Attributes
	glm::vec3 m_position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()	{ return glm::lookAt(m_position, m_position + Front, Up);}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset);

	// Makes the Camera looking at a 3D Point
	void lookAt(float x, float y, float z);
	
	// Makes the Camera looking at a 2D Point (z axis = 0)
	void lookAt(float x, float y);

	// Looking at an object
	void lookAt(const Object& o, float offsetX = 0, float offsetY = 0, float offsetZ = 0);

	//translate the camera (MUST MOVE TO POSITIONABLE)
	void translate(float x, float y, float z);
	void translate(glm::vec3 translation);

};


#endif