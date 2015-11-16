#include<iostream>
#include<memory>
#include<sstream>

//#include <functional>
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

#include "Network/Client.h"
#include "Network/Server.h"
//#include "Network/Basic.h"


#include <unistd.h>

int main(int argc, const char* argv[])
{

	network::Server s(12345);
	auto recv = [](unsigned int id, std::vector<char>&& data)->void{
				std::cout << "Message from ID-" << id << " :";
				for(auto itr : data)
					std::cout << itr;
				std::cout << std::endl;
				};

	auto newCon = [](unsigned int id, Socket* sock)->bool{
		std::cout << "New Connection with ID-" << id << " from IP " << sock->getIP() << std::endl;
		return true;
		};

	s.setRecvFunc(recv);
	s.setNewConFunc(newCon);

	sleep(2);
	network::Client c1("127.0.0.1", 12345);
	network::Client c2("127.0.0.1", 12345);
	network::Client c3("127.0.0.1", 12345);

	std::cout << "Client Send:" << std::endl;
	c1.send("HalloWelt\0",5);





	std::cin.get();
	exit(0);

	FreeImage_Initialise(true);
	FreeImage_SetOutputMessage(FreeImageErrorHandler);

	if (!glfwInit())
	{
		std::cerr << "GLFW Init error" << std::endl;
		std::cerr << "Press any key to continue";
		std::cin.get();
		return -1;
	}

	std::cout << "GLFW Version: " << glfwGetVersionString() << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	glfwSetWindowSizeCallback(window, window_size_callback);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetWindowFocusCallback(window, window_focus_callback);

	// Inits

	std::shared_ptr<camera::Camera> cam(new camera::CameraPerspective);
	SCamControll().setCamera(cam);
	ErrorLog<1>::OpenGLError("in Camera");

	render::Skybox sky;
	sky.init();
	ErrorLog<1>::OpenGLError("in Skybox");

	render::Square square;	
	

	render::MultiLineRenderUnit lines;	
	lines.addLine( render::LineVertex({ {3,1,5},0.5f,{1.0f,1.0f,0.0f,0.5f} }),
		render::LineVertex({ { -5,-1,-1 },0.2f,{ 1.0f,0.0f,0.0f,1.0f } }));	

	lines.addLine(render::LineVertex({ { 3,1,5 },0.5f,{ 0.0f,1.0f,1.0f,0.5f } }),
		render::LineVertex({ { 5,1,1 },0.7f,{ 0.0f,0.0f,1.0f,1.0f } }));

	for (float i = 0.0f; i < 2000.0f; i = i + 1.0f)
	{
		lines.addLine(render::LineVertex({ { 7.0f*(float)sin(i/3.1f),9 - (18.0f*i / 2000.0f),0 },0.1f,{ 1.0f,0.0f,0.0f,1.0f } }),
			render::LineVertex({ { 9.0f*(float)sin(i/3.0f), 9 - (18.0f*i / 2000.0f), -9 },0.1f,{ (float)sin(i/2.76f),(float)cos(i / 4.9f),(float)sin(i / 2.1f)*(float)cos(i / 1.4f),0.5f } }));
	}

	lines.updateLines();

	std::cout << "Init timer" << std::endl;
	double time, timeCache = glfwGetTime();
	unsigned long long counter = 0;

	/////////////////////////////////////
	//OpenGL Settings
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	////////////////////////////////////////////////
	//Mainloop
	std::cout << "Enter mainloop" << std::endl;
	while (!glfwWindowShouldClose(window))
	{
		time = glfwGetTime(); //time in seconds since init

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

		// Animations:
		square.setPosition(6 * sin(time), 10 * cos(time), 0);
		square.setScale(1 + 0.8*sin(time / 0.25f), 1.2 + 0.8*cos(time / 0.4), 1 + 0.5*(sin(time) * cos(time)) );
		square.setAngel(time*0.7, time*0.5, 0.0f);
		square.update();

		// Clear the colorbuffer		
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sky.draw(cam->getProjectionMatrix(), cam->getViewMatrix());		

		square.draw(cam->getProjectionMatrix(),cam->getViewMatrix());

		lines.draw(cam->getProjectionMatrix(), cam->getViewMatrix());



		ErrorLog<0>::OpenGLError("Draw error");
		
		

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
