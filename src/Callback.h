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
	//std::cout << "Mousemove " << dx << " - " << dy << std::endl;

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
	if (mods != 0 && action != GLFW_RELEASE)
	{

		return;
	}
	else
	{
		helper::CameraControll::direction mov = helper::CameraControll::noMovment;

		switch (key)
		{
		case GLFW_KEY_A:
			mov = helper::CameraControll::left;
			break;
		case GLFW_KEY_W:
			mov = helper::CameraControll::forward;
			break;
		case GLFW_KEY_S:
			mov = helper::CameraControll::backward;
			break;
		case GLFW_KEY_D:
			mov = helper::CameraControll::right;
			break;
		case GLFW_KEY_SPACE:
			mov = helper::CameraControll::up;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			mov = helper::CameraControll::down;
			break;
		default:
			break;
		}

		if (action == GLFW_PRESS)
		{
			SCamControll().move(mov, false);
		}
		else if (action == GLFW_RELEASE)
		{
			SCamControll().move(mov, true);
		}
		else
		{

		}


	}

}

void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	std::cout << "\n*** ";
	if (fif != FIF_UNKNOWN)
	{
		printf("%s Format\n", FreeImage_GetFormatFromFIF(fif));
	}
	std::cout << message << "\n***\n";

}


void window_size_callback(GLFWwindow* window, int width, int height)
{

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void window_focus_callback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		// The window gained input focus
	}
	else
	{
		// The window lost input focus
	}
}



#endif /* CALLBACK_H_ */
