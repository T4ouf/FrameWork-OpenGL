#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Application::Application(){
	//Creating the maps
	m_mapControllerButton = std::unordered_map<ControllerButton, ButtonFunction>();
	m_mapControllerAnalogButton = std::unordered_map<ControllerAnalogButton, AnalogButtonFunction>();
}

Application::~Application(){

}

void Application::mapButtonInput(ControllerButton b, ButtonFunction f){

	//Add the button to the map (replace if already mapped)
	m_mapControllerButton[b] = f;

}

void Application::mapAnalogButtonInput(ControllerAnalogButton AnalogB, AnalogButtonFunction f){

	//Add the Analog button to the map (replace if already mapped)
	m_mapControllerAnalogButton[AnalogB] = f;

}

void Application::processInput(){

	// Get controller's state
	int nbJoystickAxes, nbJoystickButtons; // 6 and 14 for xbox 360


	// 0 : left joystick  -> axe X (cosinus) (left -1 -> 1 right)
	// 1 : left joystick  -> axe Y (sinus) (down -1 -> 1 up)
	// 2 : right joystick -> axe X (cosinus) (left -1 -> 1 right)
	// 3 : right joystick -> axe Y (sinus) (down -1 -> 1 up)
	// 4 : LT button (free -1 -> triggered 1)
	// 5 : RT button (free -1 -> triggered 1)
	const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &nbJoystickAxes);


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
	// 13 : left cross
	const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &nbJoystickButtons);

	if (buttons == NULL || axes == NULL) {
		//no Joystick plugged

		return;
	}

	try {
		// Process actions

		//TODO : RELEASE BUTTON ACTION
		if (buttons[0] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_A);
			f();
		}
		if (buttons[1] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_B);
			f();
		}
		if (buttons[2] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_X);
			f();
		}
		if (buttons[3] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_Y);
			f();
		}
		if (buttons[4] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_LB);
			f();
		}
		if (buttons[5] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_RB);
			f();
		}
		if (buttons[6] == GLFW_PRESS) {
			//Unknown button
		}
		if (buttons[7] == GLFW_PRESS) {
			//Unknown button
		}
		if (buttons[8] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_LSTICK_PRESS);
			f();
		}
		if (buttons[9] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_RSTICK_PRESS);
			f();
		}
		if (buttons[10] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_UPCROSS);
			f();
		}
		if (buttons[11] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_RIGHTCROSS);
			f();
		}
		if (buttons[13] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_DOWNCROSS);
			f();
		}
		if (buttons[14] == GLFW_PRESS) {
			auto f = m_mapControllerButton.at(BUTTON_LEFTCROSS);
			f();
		}
	}
	catch (std::out_of_range& ex) {
		//ex is launched if the button si not mapped (maybe mapping it with void func is better ?)
	}

	//Analog stick (stick are always processed)
	try {
		//stick starts at 0 (-1 - 1)
		auto f = m_mapControllerAnalogButton.at(ANALOG_LEFTSTICK_X);
		f(axes[0]);

		f = m_mapControllerAnalogButton.at(ANALOG_LEFTSTICK_Y);
		f(axes[1]);

		f = m_mapControllerAnalogButton.at(ANALOG_RIGHTSTICK_X);
		f(axes[2]);

		f = m_mapControllerAnalogButton.at(ANALOG_RIGHTSTICK_Y);
		f(axes[3]);


		//Triggers starts at 0 (0 - 2)
		f = m_mapControllerAnalogButton.at(ANALOG_LEFTTRIGGER);
		f(axes[4]+1);

		f = m_mapControllerAnalogButton.at(ANALOG_RIGHTTRIGGER);
		f(axes[5]+1);

	}catch (std::out_of_range& ex) {
		//ex is launched if the button si not mapped (maybe mapping it with void func is better ?)
	}
	

	
}
