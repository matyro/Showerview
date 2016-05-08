/*
 * shader.cpp
 *
 *  Created on: 01.09.2015
 *      Author: dominik
 */

#include "Shader.h"

#include <vector>

const std::string Shader::readShader(const char* const path)
{
	std::ifstream shaderFile;

	// setException
	shaderFile.exceptions(std::ifstream::badbit);

	try
	{
		shaderFile.open(path);

		std::stringstream sstr;

		//
		sstr << shaderFile.rdbuf();

		shaderFile.close();

		return std::move(sstr.str());
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "Shader Error: " << path << " could not be read!" << std::endl;
		std::cout << e.what() << std::endl;
	}
	return std::move(std::string(""));
}

const GLuint Shader::createShader(GLenum type, const char* source)
{

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Print compile errors if any
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint messageLength = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<GLchar> errorLog(messageLength);

		glGetShaderInfoLog(shader, messageLength, NULL, &errorLog[0]);

		glDeleteShader(shader);

		if(type==GL_VERTEX_SHADER)
			std::cout << "Shader Error (Vertex): " << &errorLog[0] << std::endl;
		else if(type==GL_FRAGMENT_SHADER)
			std::cout << "Shader Error (Fragment): " << &errorLog[0] << std::endl;
		else if(type==GL_GEOMETRY_SHADER)
			std::cout << "Shader Error (Geometry): " << &errorLog[0] << std::endl;
		return -1;
	}
	return shader;
}

Shader::Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath)
{
	std::cout << "Compile Shader: " << vertexPath << " | " << fragmentPath << std::endl;

	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode = readShader(vertexPath);
	std::string fragmentCode = readShader(fragmentPath);

	// 2. Compile shaders
	const GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
	const GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

	// Shader Program
	this->m_uiProgram = glCreateProgram();
	glAttachShader(this->m_uiProgram, vertexShader);
	glAttachShader(this->m_uiProgram, fragmentShader);

	//Linking
	glLinkProgram(this->m_uiProgram);

	// Print linking errors if any
	GLint success;
	glGetProgramiv(this->m_uiProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint messageLength = 0;

		glGetProgramiv(this->m_uiProgram, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<GLchar> errorLog(messageLength);

		glGetProgramInfoLog(this->m_uiProgram, messageLength, NULL, &errorLog[0]);

		glDeleteProgram(this->m_uiProgram);
		std::cout << "Shader Error (Linking failed): " << &errorLog[0] << std::endl;
		exit(-2);
	}
	else
	{
		glDetachShader(this->m_uiProgram, vertexShader);
		glDetachShader(this->m_uiProgram, fragmentShader);
	}
	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


}

Shader::Shader(const GLchar* const vertexPath, const GLchar* const fragmentPath, const GLchar* const geometryPath)
{
	std::cout << "Compile Shader: " << vertexPath << " | " << fragmentPath << " | " << geometryPath << std::endl;

	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode = readShader(vertexPath);
	std::string fragmentCode = readShader(fragmentPath);
	std::string geometryCode = readShader(geometryPath);

	// 2. Compile shaders
	const GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
	const GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
	const GLuint geometryShader = createShader(GL_GEOMETRY_SHADER, geometryCode.c_str());

	// Shader Program
	this->m_uiProgram = glCreateProgram();
	glAttachShader(this->m_uiProgram, vertexShader);
	glAttachShader(this->m_uiProgram, fragmentShader);
	glAttachShader(this->m_uiProgram, geometryShader);

	//Linking
	glLinkProgram(this->m_uiProgram);

	// Print linking errors if any
	GLint success = -1;
	glGetProgramiv(this->m_uiProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLint messageLength = 0;

		glGetProgramiv(this->m_uiProgram, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<GLchar> errorLog(messageLength);

		glGetProgramInfoLog(this->m_uiProgram, messageLength, NULL, errorLog.data());

		glDeleteProgram(this->m_uiProgram);
		std::cout << "Shader Error (Linking failed): " << errorLog.data() << std::endl;
		exit(-3);
	}
	else
	{
		glDetachShader(this->m_uiProgram, vertexShader);
		glDetachShader(this->m_uiProgram, fragmentShader);
		glDetachShader(this->m_uiProgram, geometryShader);
	}
	// Delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);	
}


Shader::Shader(Shader&& rhs)
	:m_uiProgram(rhs.m_uiProgram), m_std_attributeLocationList(rhs.m_std_attributeLocationList), m_std_uniformLocationList(rhs.m_std_uniformLocationList)
{
	rhs.m_uiProgram = 0;
}

Shader::~Shader()
{
	if (this->m_uiProgram != 0)
	{
		glDeleteProgram(this->m_uiProgram);
	}
}


GLint Shader::attribute(const std::string &attribute)
{
	// Create an iterator to look through our attribute map and try to find the named attribute
	std::map<std::string, int>::iterator it = m_std_attributeLocationList.find(attribute);

	// Found it? Great -return the bound location! Didn't find it? Alert user and halt.
	if (it != m_std_attributeLocationList.end())
	{
		return m_std_attributeLocationList[attribute];
	}
	else
	{
		std::cout << "Could not find attribute in shader program: " << attribute << std::endl;
		exit(-14);
	}
}

GLint Shader::uniform(const std::string &uniform)
{
	// Create an iterator to look through our uniform map and try to find the named uniform
	std::map<std::string, int>::iterator it = m_std_uniformLocationList.find(uniform);

	// Found it? Great - pass it back! Didn't find it? Alert user and halt.
	if (it != m_std_uniformLocationList.end())
	{
		return m_std_uniformLocationList[uniform];
	}
	else
	{
		std::cout << "Could not find uniform in shader program: " << uniform << std::endl;
		exit(-12);
	}
}

GLint Shader::addAttribute(const std::string& attributeName)
{
	m_std_attributeLocationList[attributeName] = glGetAttribLocation(this->m_uiProgram, attributeName.c_str());
	
	// Check to ensure that the shader contains an attribute with this name
	if (m_std_attributeLocationList[attributeName] == -1)
	{
		std::cout << "Could not add attribute: " << attributeName << " - location returned -1!" << std::endl;
		std::cout << "Press any key to continue:";
		std::cin.get();
		exit(-13);
	}
	else
	{
		std::cout << "Attribute " << attributeName << " bound to location: " << m_std_attributeLocationList[attributeName] << std::endl;
	}

	return m_std_attributeLocationList[attributeName];
}

GLint Shader::addUniform(const std::string &uniformName)
{
	m_std_uniformLocationList[uniformName] = glGetUniformLocation(this->m_uiProgram, uniformName.c_str());

	// Check to ensure that the shader contains a uniform with this name
	if (m_std_uniformLocationList[uniformName] == -1)
	{
		std::cout << "Could not add uniform: " << uniformName << " - location returned -1!" << std::endl;
		exit(-11);
	}
	else
	{
		std::cout << "Uniform " << uniformName << " bound to location: " << m_std_uniformLocationList[uniformName] << std::endl;
	}

	return m_std_uniformLocationList[uniformName];
}
