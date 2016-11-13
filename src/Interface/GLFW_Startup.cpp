
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>



#include "Interface/GLFW_Startup.h"
#include "Interface/Callback.h"

GLFW_control::GLFW_control(int width, int height)
{
	
	std::cout << "Init GLFW" << std::endl;
	if (!glfwInit())
	{
		std::cerr << "GLFW Init error" << std::endl;
		std::cerr << "Press any key to continue";
		std::cin.get();
		exit(-1);
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

	m_window = glfwCreateWindow(width, height, "Showerview", NULL, NULL);
	if (!m_window)
	{
		std::cerr << "Window creation failed" << std::endl;
	}

	glfwMakeContextCurrent(m_window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Glew Init error" << std::endl;
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	}
	
	int realWidth, realHeight;
	glfwGetFramebufferSize(m_window, &realWidth, &realHeight);

	if (width != realWidth || height != realHeight)
	{
		std::cerr << "Selected height and accepted height are differen!";
		std::cin.get();
		exit(-2);
	}


	glViewport(0, 0, width, height);

	glfwSwapInterval(1);
	
	////////////////////////////////////////////////
	/// Init things before the main loop
	//Callback

	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, window_size_callback);
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwSetWindowFocusCallback(m_window, window_focus_callback);

		
}

void GLFW_control::loop(double elapsedTime)
{
	static long long counter = 0;

	// Check and call events
	glfwPollEvents();

	//FPS Calculation
	if (elapsedTime > m_last_loop_call + 1)
	{
		std::stringstream sstr;
		sstr << ((counter / (elapsedTime - m_last_loop_call))) << " fps";
		sstr << "  |  CL:";
		sstr << ((elapsedTime - m_last_loop_call) / counter)*1000.0f << "ms";
		
		glfwSetWindowTitle(m_window, sstr.str().c_str());			

		m_last_loop_call = elapsedTime;
	}

	counter++;
	

	//Buffer switch	
	glfwSwapBuffers(m_window);

}


void GLFW_control::close()
{
	//Close window
	glfwDestroyWindow(m_window);
	glfwTerminate();
}