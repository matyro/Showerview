/*
 * Callback.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <GLFW/glfw3.h>

#include "Camera/Camera.h"

void error_callback(int error, const char* description)
{
    std::cerr << description << std::endl;
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "Mousemove " << xpos << " - " << ypos << std::endl;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	static float xpos;
	static float ypos;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
    	std::cout << "Right press" << std::endl;
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
    	std::cout << "Right release" << std::endl;
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    }
    else
    {

    }
}

#endif /* CALLBACK_H_ */
