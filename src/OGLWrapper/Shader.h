#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>


#include "Helper/ErrorLog.h"

class Shader
{
private:

	// Map of attributes and their binding locations
	std::map<std::string,int> m_std_attributeLocationList;

	// Map of uniforms and their binding locations
	std::map<std::string,int> m_std_uniformLocationList;

	GLuint m_uiProgram;


	const std::string readShader(const char* const path);
	const GLuint createShader(GLenum type, const char* source);


public:
    
	Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath);
	Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath, const GLchar* const geometryPath);

    // Uses the current shader
    inline void Use() const
    {
        glUseProgram(this->m_uiProgram);
    }

	// Returns the bound location of a named attribute
	GLint attribute(const std::string &attribute);


	// Method to returns the bound location of a named uniform
	GLint uniform(const std::string &uniform);


	// Method to add an attrbute to the shader and return the bound location
	GLint addAttribute(const std::string &attributeName);


	// Method to add a uniform to the shader and return the bound location
	GLint addUniform(const std::string &uniformName);

};

#endif
