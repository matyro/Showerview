

#pragma once

#include <memory>


#include <GLFW/glfw3.h>


class GLFW_control
{
private:
	GLFWwindow* m_window;


	double m_last_loop_call; // time in seconds since program start

public:
	GLFW_control(int width = 1024, int height = 976);

	void loop(double elapsedTime);

	void close();

	inline bool closed() {
		return !glfwWindowShouldClose(m_window);
	};


};
