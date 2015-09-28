#ifndef ERROR_LOG_H
#define ERROR_LOG_H

#include <GL/glew.h>

#include <iostream>
#include <ostream>
#include <type_traits>

#define VERBOSE_LEVEL 1				//Everything smaller or equal gets printed


template<unsigned int TdebugLevel>
class ErrorLog
{
private:

	static std::ostream m_std_output;

	static constexpr bool verboseCheck()
	{
		return (TdebugLevel - VERBOSE_LEVEL) <= 0;
	}
	
	ErrorLog()
	{};

public:

	
	static void setOutput(const std::ostream output = std::cerr)
	{
		m_std_output = output;
	}
	   
	static inline void OpenGLError(const char* const message) 
	{
		if (verboseCheck() ||true)
		{
			GLenum glError = GL_NO_ERROR;
			while ((glError = glGetError()) != GL_NO_ERROR)
			{
				std::cerr << "OpenGL Error error: " << glError << " | " << gluErrorString(glError) << "\n";
				std::cerr << "\t" << message << std::endl;
			}
		}
		else
		{

		}		
	}

	static inline void ErrorMessage(const char* const message) 
	{
		if (verboseCheck())
		{
		}
	
		else
		{

		}
	}
};

//template<unsigned int TdebugLevel>
//std::ostream ErrorLog<TdebugLevel>::m_std_output = std::cerr;

#endif
