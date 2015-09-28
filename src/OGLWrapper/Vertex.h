/*
 * Vertex.h
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#ifndef SRC_OGLWRAPPER_VERTEX_H_
#define SRC_OGLWRAPPER_VERTEX_H_

#include <GL/glew.h>

#include <array>
#include <memory>

#include "VertexAttribute.h"

template<std::size_t tNumberOfAttributes>
class Vertex
{
private:
	GLuint m_uiVBO;
	GLuint m_uiVAO;

	std::array<AttributeSetting, tNumberOfAttributes> m_std_AttribList;

	std::shared_ptr<char[]> m_cData;
	unsigned int m_uiDataSize;	//Size in byte

	unsigned int m_uiBlockSize;


public:

	Vertex() :
			m_uiVAO(0), m_uiVBO(0), m_uiDataSize(0), m_uiBlockSize(0)
	{
	}

	void setSettings(std::array<AttributeSetting, tNumberOfAttributes>&& attribute)
	{
		if(m_uiVAO != 0)
		{
			glDeleteVertexArrays(1, &m_uiVAO);
		}
		if(m_uiVBO != 0)
		{
			glDeleteBuffers(1, &m_uiVBO);
		}

		this->m_std_AttribList = std::move(attribute);

		glGenVertexArrays(1, &m_uiVAO);
		glGenBuffers(1, &m_uiVBO);
	}
	
	~Vertex()
	{
		glDeleteVertexArrays(1, &m_uiVAO);
		glDeleteBuffers(1, &m_uiVBO);
	}

	void applySettings(const std::array<GLuint, tNumberOfAttributes>&& index) const
	{
		glBindVertexArray(m_uiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

		int counter = 0;
		int offset = 0;
		int stride = 0;
		for (auto itr : m_std_AttribList)
		{
			itr.applySetting(index[counter], stride, offset);
			counter++;

			offset = itr.getSize();
		}

		m_uiBlockSize = offset;

		glBindVertexArray(0);

	}

	void transmitData(GLenum usage = GL_STATIC_DRAW) const
	{
		glBindVertexArray(m_uiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

		glBufferData(GL_ARRAY_BUFFER, m_uiDataSize, m_cData.get(), usage);
	}

	//end, start = start and stop index to draw
	void draw(const GLenum drawMode = GL_POINTS, const unsigned int start = 0)
	{
		this->draw(drawMode, this->m_uiDataSize/this->m_uiBlockSize, start);
	}

	void draw(const GLenum drawMode, const unsigned int end, const unsigned int start = 0)
	{
		glBindVertexArray(m_uiVAO);
		glDrawArrays(drawMode, start, end);

		glBindVertexArray(0);
	}

};

#endif /* SRC_OGLWRAPPER_VERTEX_H_ */
