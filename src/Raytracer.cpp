
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>




#include "Callback.h"

#include "Helper/Line.h"
#include "Camera/CameraMatrix.h"

#include "OCL.h"
#include "OGL.h"


int main()
{
	std::cout << "Init GLFW" << std::endl;
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
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//Early error Callback
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1024, 768, "Showerview", NULL, NULL);
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

	// dist, fov, ratio
	auto cam = std::make_shared<CameraMatrix>(1.0f, 90.0f, static_cast<float>(width) / static_cast<float>(height) );
	
	SCamControll().setCamera(cam);	

	//glfwMakeContextCurrent(window);

	int textureWidth = static_cast<int>(width * 1.00f);
	int textureHeight = static_cast<int>(height * 1.00f);

	//initOCL();
	auto oglData = initOGL(textureWidth, textureHeight);
	std::get<0>(oglData)->bindTexture();


	LineSet lines;
	/*lines.addLine({  1.0f,  1.0f, 1.0f, 0.0f }, {  1.0f,  1.0f,-1.0f, 0.0f }, { 0.1f,0.8f,0.1f, 1.0f });
	lines.addLine({ -1.0f,  1.0f, 1.0f, 0.0f }, { -1.0f,  1.0f,-1.0f, 0.0f }, { 0.1f,0.1f,0.8f, 1.0f });
	lines.addLine({  1.0f, -1.0f, 1.0f, 0.0f }, {  1.0f, -1.0f,-1.0f, 0.0f }, { 0.8f,0.1f,0.1f, 1.0f });
	lines.addLine({ -1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f,-1.0f, 0.0f }, { 0.1f,0.8f,0.8f, 1.0f });

	lines.addLine({ 1.0f,  1.0f, 1.0f, 0.0f }, { -1.0f, -1.0f,  1.0f, 0.0f }, { 0.8f,0.8f,0.1f, 1.0f });
	lines.addLine({ 1.0f, -1.0f, 1.0f, 0.0f }, { -1.0f, 1.0f,  1.0f, 0.0f }, { 0.8f,0.1f,0.8f, 1.0f });*/

	std::default_random_engine g(1937);
	std::uniform_real_distribution<float> dis(-100, 100);
	std::uniform_real_distribution<float> dis2(0, 1);


	lines.addLine({ 10, 0.0f, 0.0f, 1.0f }, { -10, 0.0f, 0.0f, 1.0f }, { dis2(g), dis2(g), dis2(g), 1.0f });
	lines.addLine({ 0, 10.0f, 0.0f, 1.0f }, { 0, -10.0f, 0.0f, 1.0f }, { dis2(g), dis2(g), dis2(g), 1.0f });

	const int tmpSize = 0;
	for (int i = 0; i < tmpSize; i++)
	{
		float x = -tmpSize + 2.0f * i;

		for (int j = 0; j < tmpSize; j++)
		{
			float y = -tmpSize + 2.0f * j;

			lines.addLine({ x, y, 10, 1.0f }, { x, y, -10, 1.0f }, { dis2(g), dis2(g), dis2(g), 1.0f });
		}		
	}
	
	std::cout << "Init OCL: " << lines.size() << std::endl;

	auto oclData = initOCL(GL_TEXTURE_2D, std::get<0>(oglData)->getTextureID(), lines.vec());

	std::cout << "Init ready, start rendering..." << std::endl;
	
	
	////////////////////////////////////////////////
	//Mainloop	
	double time, timeCache = glfwGetTime();
	double timeCache2 = 0;
	double time2, timeCache3 = 0, timeCache4 = 0;
	unsigned long long counter = 0;
	unsigned long long absCounter = 0;

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
			sstr << ((counter / (time - timeCache))) << " fps";
			sstr << "  |  CL:";
			sstr << (timeCache2 / counter)*1000.0f << "ms";
			sstr << "  |  CL+GL";
			sstr << (timeCache3 / counter)*1000.0f << "ms";	
			sstr << "  |  all:";
			sstr << (timeCache4 / counter)*1000.0f << "ms";
			glfwSetWindowTitle(window, sstr.str().c_str());

			counter = 0;
			timeCache = time;
			timeCache2 = 0;
			timeCache3 = 0;
			timeCache4 = 0;
		}
		
		time2 = glfwGetTime();
		calcOCL( oclData, static_cast<float>(absCounter), toFloat16(cam->getProjectionViewMatrix()), textureWidth, textureHeight);
		timeCache2 += glfwGetTime() - time2;

		drawOGL( oglData );

		timeCache3 += glfwGetTime() - time2;
		timeCache4 += glfwGetTime() - time;

		//Buffer switch
		glfwSwapBuffers(window);
		counter++;
		absCounter++;

		//std::cin.get();
	}

	//Terminate everything

	//Close window
	glfwDestroyWindow(window);
	glfwTerminate();

	

	std::cout << "Press enter to terminate" << std::endl;
	std::cin.get();
	std::cin.get();

	return 0;
}

