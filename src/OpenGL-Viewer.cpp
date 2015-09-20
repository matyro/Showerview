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

#include <FreeImage.h>


#include "OGLWrapper/Shader.h"

#include "RenderUnits/SingleLineRenderUnit.h"
#include "RenderUnits/Skybox.h"

#include "Camera/Camera.h"
#include "Camera/CameraPerspective.h"

#include "Callback.h"

#include "Helper/CameraControll.h"



int main(int argc, const char* argv[])
{
	FreeImage_Initialise(true);

	if(!glfwInit())
    {
                std::cerr << "GLFW Init error" << std::endl;
    }


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS,24);

	//Early error Callback
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window)
	{
		std::cerr << "Window creation failed" << std::endl;
	}

	glfwMakeContextCurrent(window);

	glewExperimental=GL_TRUE;
    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
                std::cerr << "Glew Init error" << std::endl;
                std::cerr<<"Error: " << glewGetErrorString(err) << std::endl;

	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSwapInterval(1);

	////////////////////////////////////////////////
	/// Init things before the main loop
	//Callback

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);

	glfwSetKeyCallback(window, key_callback);


	// Inits

	std::shared_ptr<camera::Camera> cam ( new camera::CameraPerspective );

	SCamControll().setCamera(cam);



    double timeCache = glfwGetTime();
    unsigned long long counter = 0;

    render::LineRender line[10];
    for (int i=0; i<10; i++)
    {
    	line[i].init();

		line[i].line(-5.0f + ((5.0f*i) / 3000.0f), -3.0f, 0.0f, -5.0f + ((5.0f*i) / 3000.0f), 3, 0, 0.1f, i / 3000.0f, i / 3000.0f, i / 3000.0f, 1.0f);
    }

	double time = glfwGetTime();

	//OpenGL Settings

	

    ////////////////////////////////////////////////
	//Mainloop
	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime(); //time since init

		SCamControll().setTime(time);

		// Check and call events
		glfwPollEvents();
		

		//FPS Calculation
		if(time > timeCache+1)
		{
			std::stringstream sstr;
			sstr << ((counter/(time - timeCache)));
			glfwSetWindowTitle(window,sstr.str().c_str());

			counter = 0;
			timeCache = time;
		}
		counter++;



		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Rendertext
		/*
		rndmText.RenderText( "Corsika Viewer", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		rndmText.RenderText( "-!*", 200.0f, 70.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
		*/

		// Render lines

		for(int i=0; i<10; i++)
		{
			line[i].draw( cam->getMatrix() );
		}

		//Buffer switch
		glfwSwapBuffers(window);
	}

	//Terminate everything


	//Close window
	glfwDestroyWindow(window);
	glfwTerminate();

	FreeImage_DeInitialise();
	return 0;
}
