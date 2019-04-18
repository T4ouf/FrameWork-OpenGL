#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void Application::mapButtonInput(ControllerButton b, ButtonFunction f){

	m_mapControllerButton.insert({b,f});

}

void Application::mapAnalogButtonInput(ControllerAnalogButton AnalogB, AnalogButtonFunction f){

	m_mapControllerAnalogButton.insert({ AnalogB, f});

}

void Application::processInput(){

	// Get controller's state
	int nbJoystickAxes, nbJoystickButtons; // taille 6 et 14 avec manette xbox 360
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes);
	// 0 : left joystick -> axe X (cosinus) (gauche -1 -> 1 droite)
	// 1 : left joystick -> axe Y (sinus) (bas -1 -> 1 haut)
	// 2 : right joystick -> axe X (cosinus) (gauche -1 -> 1 droite)
	// 3 : right joystick -> axe Y (sinus) (bas -1 -> 1 haut)
	// 4 : LT button (relaché -1 -> appuyé 1)
	// 5 : RT button (relaché -1 -> appuyé 1)
	//ATTENTION: ARRONDI DEGUEULASSE DES FOIS < 1
	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);
	// 0 : A button
	// 1 : B button
	// 2 : X button
	// 3 : Y button
	// 4 : LB button
	// 5 : RB button
	// 6 : ??
	// 7 : ??
	// 8 : left joystick click
	// 9 : right joystick click
	// 10 : top cross
	// 11 : right cross
	// 12 : bottom cross
	// 13 : left cross7

	// Process actions
	if (buttons[0] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_A);
		f();
	}
	else if (buttons[1] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_B);
		f();
	}
	else if (buttons[2] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_X);
		f();
	}
	else if (buttons[3] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_Y);
		f();
	}
	else if (buttons[4] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_LB);
		f();
	}
	else if (buttons[5] == GLFW_PRESS) {
		auto f = m_mapControllerButton.at(BUTTON_RB);
		f();
	}



	if (axes[0] > 0) {
		glClearColor(1, 0, 1, 1.0f);
	}
	else {
		glClearColor(0, 1, 0, 1.0f);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
