#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){

	m_position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();

}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM){

	m_position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {

	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		m_position += Front * velocity;
	if (direction == BACKWARD)
		m_position -= Front * velocity;
	if (direction == LEFT)
		m_position -= Right * velocity;
	if (direction == RIGHT)
		m_position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {

	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void Camera::lookAt(float x, float y, float z){

	m_position = glm::vec3(x, y, z - 50);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	Yaw = YAW;
	Pitch = PITCH;
	updateCameraVectors();

}

void Camera::lookAt(float x, float y) {

	lookAt(x, y, 0);

}

void Camera::lookAt(const Object & o, float offsetX, float offsetY, float offsetZ){
	const glm::vec3& pos = o.getPosition();
	lookAt(pos.x+offsetX, pos.y + offsetY, pos.z + offsetZ);
}


void Camera::translate(float x, float y, float z){

	m_position.x += x;
	m_position.y += y;
	m_position.z += z;


}

void Camera::translate(glm::vec3 translation){

	translate(translation.x, translation.y, translation.z);

}


void Camera::updateCameraVectors(){

	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

