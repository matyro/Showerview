#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>

#include <GL/glew.h>


#include "Helper\ErrorLog.h"

class Shader
{
private:

	// Map of attributes and their binding locations
	std::map<std::string,int> m_std_attributeLocationList;

	// Map of uniforms and their binding locations
	std::map<std::string,int> m_std_uniformLocationList;

	GLuint Program;

public:
    
    // Constructor generates the shader on the fly
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
    {
    	std::cout << "Compile Shader: " << vertexPath << " | " << fragmentPath << std::endl;
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::badbit);
        try
        {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar * fShaderCode = fragmentCode.c_str();
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Shader Program
        this->Program = glCreateProgram();
        glAttachShader(this->Program, vertex);
        glAttachShader(this->Program, fragment);

        /////////////////////////////////////////Bind here

        glLinkProgram(this->Program);
        // Print linking errors if any
        glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }
    // Uses the current shader
    void Use()
    {
        glUseProgram(this->Program);
    }

	// Returns the bound location of a named attribute
	GLint attribute(const std::string &attribute)
	{
		// You could do this function with the single line:
		//
		//		return attributeLocList[attribute];
		//
		// BUT, if you did, and you asked it for a named attribute
		// which didn't exist, like, attributeLocList["ThisAttribDoesn'tExist!"]
		// then the method would return an invalid value which will likely cause
		// the program to segfault. So we're making sure the attribute asked
		// for exists, and if it doesn't we can alert the user and stop rather than bombing out later.

		// Create an iterator to look through our attribute map and try to find the named attribute
		std::map<std::string, int>::iterator it = m_std_attributeLocationList.find(attribute);

		// Found it? Great -return the bound location! Didn't find it? Alert user and halt.
		if ( it != m_std_attributeLocationList.end() )
		{
			return m_std_attributeLocationList[attribute];
		}
		else
		{
			std::cout << "Could not find attribute in shader program: " << attribute << std::endl;
			exit(-1);
		}
	}


	// Method to returns the bound location of a named uniform
	GLint uniform(const std::string &uniform)
	{
		// Note: You could do this method with the single line:
		//
		// 		return uniformLocList[uniform];
		//
		// But we're not doing that. Explanation in the attribute() method above.

		// Create an iterator to look through our uniform map and try to find the named uniform
		std::map<std::string, int>::iterator it = m_std_uniformLocationList.find(uniform);

		// Found it? Great - pass it back! Didn't find it? Alert user and halt.
		if ( it != m_std_uniformLocationList.end() )
		{
			return m_std_uniformLocationList[uniform];
		}
		else
		{
			std::cout << "Could not find uniform in shader program: " << uniform << std::endl;
			exit(-1);
		}
	}


	// Method to add an attrbute to the shader and return the bound location
	GLint addAttribute(const std::string &attributeName)
	{
		m_std_attributeLocationList[attributeName] = glGetAttribLocation( this->Program, attributeName.c_str() );

		// Check to ensure that the shader contains an attribute with this name
		if (m_std_attributeLocationList[attributeName] == -1)
		{
			std::cout << "Could not add attribute: " << attributeName << " - location returned -1!" << std::endl;
			exit(-1);
		}
		else
		{
			std::cout << "Attribute " << attributeName << " bound to location: " << m_std_attributeLocationList[attributeName] << std::endl;
		}

		return m_std_attributeLocationList[attributeName];
	}


	// Method to add a uniform to the shader and return the bound location
	GLint addUniform(const std::string &uniformName)
	{
		m_std_uniformLocationList[uniformName] = glGetUniformLocation( this->Program, uniformName.c_str() );

		// Check to ensure that the shader contains a uniform with this name
		if (m_std_uniformLocationList[uniformName] == -1)
		{
			std::cout << "Could not add uniform: " << uniformName << " - location returned -1!" << std::endl;
			exit(-1);
		}
		else
		{
			std::cout << "Uniform " << uniformName << " bound to location: " << m_std_uniformLocationList[uniformName] << std::endl;
		}

		return m_std_uniformLocationList[uniformName];
	}
};

#endif
