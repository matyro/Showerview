/*
 * Vertex.h
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#pragma once

#include <GL/glew.h>

#include <memory>
#include <vector>

template<typename TDataType = float>
class Vertex
{
private:
	GLuint m_uiVBO;
	GLuint m_uiVAO;	
	
	std::vector<TDataType> m_std_Data;

public:

	Vertex() :
			m_uiVBO(0), m_uiVAO(0)
	{
		glGenVertexArrays(1, &m_uiVAO);
		glGenBuffers(1, &m_uiVBO);		

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Vertex(const Vertex<TDataType>& rhs) = delete;


	Vertex(Vertex<TDataType>&& rhs)
		:m_uiVBO(rhs.m_uiVBO), m_uiVAO(rhs.m_uiVAO), m_std_Data(rhs.m_std_Data)
	{
		rhs.m_uiVAO = 0;
		rhs.m_uiVBO = 0;
	}
		

	~Vertex()
	{
		if (m_uiVAO != 0)
		{
			glDeleteVertexArrays(1, &m_uiVAO);
		}
		if (m_uiVBO != 0)
		{
			glDeleteBuffers(1, &m_uiVBO);
		}		

	}
	
	void transmitData(GLuint binding, const unsigned int elements, const unsigned int stride = 0, GLenum usage = GL_STATIC_DRAW)
	{		
		glBindVertexArray(m_uiVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, this->m_std_Data.size() * sizeof(TDataType), &(this->m_std_Data[0]), usage);

		
		glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVBO);
		glVertexAttribPointer(binding, elements, GL_FLOAT, false, stride, 0);
		
		glEnableVertexAttribArray(binding);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);	
		glFinish();
	}


	//End = number of incices
	void draw(const GLenum drawMode, const GLsizei end, const unsigned int start = 0) const
	{
		glBindVertexArray(this->m_uiVAO);		
		
		glDrawArrays(drawMode, start, end);

		glBindVertexArray(0);
	}

	void bind()
	{
		glBindVertexArray(this->m_uiVAO);
	}

	//pointer to head of data, length in elements
	inline void copyData(const std::vector<TDataType> data)
	{
		this->m_std_Data.resize(data.size());
		this->m_std_Data.assign(data.begin(), data.end());		
	}

	//
	inline void moveData(std::vector<TDataType>&& data)
	{
		this->m_std_Data = std::move(data);
	}

};






