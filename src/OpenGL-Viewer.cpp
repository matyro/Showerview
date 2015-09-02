#include<iostream>
#include<thread>

#include <unistd.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "TextRender.h"

int main(int argc, const char* argv[])
{
	if(!glfwInit())
        {
                std::cerr << "GLFW Init error" << std::endl;
        }


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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


	// Set OpenGL options
    	glEnable(GL_CULL_FACE);
    	glEnable(GL_BLEND);
    	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	 // Compile and setup the shader
    	Shader shader("/home/dominik/workspace/OpenGL-Viewer/Shader/text.vs", "/home/dominik/workspace/OpenGL-Viewer/Shader/text.frag");
    	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), 0.0f, static_cast<GLfloat>(height));
    	shader.Use();
    	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    	loadFont();

	//Mainloop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		double time = glfwGetTime(); //time since init


		 // Clear the colorbuffer
		 glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		 glClear(GL_COLOR_BUFFER_BIT);

		 RenderText(shader, "This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		 RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
