#include<iostream>
#include<thread>
#include<memory>
#include<sstream>

#include <functional>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <FreeImage.h>

#include "OGLWrapper/Shader.h"

#include "RenderUnits/SingleLineRenderUnit.h"
#include "RenderUnits/Skybox.h"
#include "RenderUnits/MultiLineRenderUnit.h"
#include "RenderUnits/Square.h"

#include "Camera/Camera.h"
#include "Camera/CameraPerspective.h"

#include "Callback.h"

#include "Helper/CameraControll.h"

int main(int argc, const char* argv[])
{

	FreeImage_Initialise(true);
	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	if (!glfwInit())
	{
		std::cerr << "GLFW Init error" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	//Early error Callback
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Showerview", NULL, NULL);
	if (!window)
	{
		std::cerr << "Window creation failed" << std::endl;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Glew Init error" << std::endl;
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(1);

	ErrorLog<5>::OpenGLError("in Main");

	////////////////////////////////////////////////
	/// Init things before the main loop
	//Callback

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwSetKeyCallback(window, key_callback);

	// Inits

	std::shared_ptr<camera::Camera> cam(new camera::CameraPerspective);
	SCamControll().setCamera(cam);
	ErrorLog<1>::OpenGLError("in Camera");

	render::Skybox sky;
	sky.init();
	ErrorLog<1>::OpenGLError("in Skybox");

	render::Square square;
	square.init();

	std::cout << "Init timer" << std::endl;
	double time, timeCache = glfwGetTime();
	unsigned long long counter = 0;
	//OpenGL Settings
	


	////////////////////////////////////////////////
	//Mainloop
	std::cout << "Enter mainloop" << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime(); //time since init

		SCamControll().setTime(time);
		SCamControll().update();

		// Check and call events
		glfwPollEvents();
		
		//FPS Calculation
		if (time > timeCache + 1)
		{
			std::stringstream sstr;
			sstr << ((counter / (time - timeCache)));
			glfwSetWindowTitle(window, sstr.str().c_str());

			counter = 0;
			timeCache = time;
		}
		counter++;

		// Clear the colorbuffer
		std::cout << "ClearColor" << std::endl;
		glClearColor(0.2f, 0.3f, 0.3f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		std::cout << "SquareDraw" << std::endl;
		square.draw(cam->getProjectionViewMatrix());
		ErrorLog<0>::OpenGLError("Square draw");
		
		std::cout << "SkyDraw" << std::endl;
		sky.draw(cam->getProjectionViewMatrix());
		ErrorLog<0>::OpenGLError("Skybox draw");

		//Buffer switch
		glfwSwapBuffers(window);
	}

	//Terminate everything

	//Close window
	glfwDestroyWindow(window);
	glfwTerminate();

	FreeImage_DeInitialise();

	std::cout << "Press enter to terminate" << std::endl;
	std::cin.get();
	std::cin.get();
	
	return 0;
}
