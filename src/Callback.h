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
#include "Helper/CameraControll.h"

void error_callback(int error, const char* description)
{
    std::cerr << description << std::endl;
}


static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	static double oldX = xpos;
	static double oldY = ypos;

	double dx = xpos - oldX;
	double dy = ypos - oldY;

	SCamControll().rotate(dx, dy);
	std::cout << "Mousemove " << dx << " - " << dy << std::endl;

	oldX = xpos;
	oldY = ypos;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
    	std::cout << "Right press" << std::endl;
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		SCamControll().activateMovment();
		
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
    	std::cout << "Right release" << std::endl;
    	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		SCamControll().deactivateMovment();

    }
    else
    {

    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_D:
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_A:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_S:
			break;
		case GLFW_KEY_D:
			break;
		default:
			break;
		}
	}
	else
	{

	}
}


#endif /* CALLBACK_H_ */
