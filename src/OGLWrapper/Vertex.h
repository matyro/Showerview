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

struct VertexData2
{
  GLfloat position[3];
  GLubyte color[4];
};

struct VertexData3
{
  GLfloat position[3];
  GLfloat normal[3];
  GLubyte color[4];
};

template<std::size_t tNumberOfAttributes>
class Vertex
{
private:
	GLuint m_uiVBO;
	GLuint m_uiVAO;
	GLuint m_uiIBO;	//Index Buffer Object

	AttributeSetting m_std_AttribList[tNumberOfAttributes];

	std::shared_ptr<void> m_vData;
	std::shared_ptr<GLuint> m_uiIndexList;

	unsigned int m_uiDataSize;	//Size in byte
	unsigned int m_uiBlockSize;

	unsigned int m_uiIndexSize;	//Size in Vertices

public:

	Vertex() :
			m_uiVAO(0), m_uiVBO(0), m_uiDataSize(0), m_uiBlockSize(0)
	{
		glGenVertexArrays(1, &m_uiVAO);
		glGenBuffers(1, &m_uiVBO);
	}

	template<unsigned int id>
	void setSettings(const AttributeSetting& attribute)
	{
		static_assert(id >= 0 || id < tNumberOfAttributes, "ID out of bounds!");
		m_std_AttribList[id].set( attribute );
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
	
	void applySettings(const unsigned int stride)
	{
		glBindVertexArray(m_uiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);

		int counter = 0;
		int offset = 0;
		for (auto itr : m_std_AttribList)
		{
			itr.applySetting(stride, offset);
			counter++;

			offset = itr.getSize();
		}

		m_uiBlockSize = offset;

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void transmitData(GLenum usage = GL_STATIC_DRAW) const
	{
		glBindVertexArray(this->m_uiVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, this->m_uiDataSize, this->m_vData.get(), usage);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIndexSize * sizeof(GLuint), this->m_uiIndexList.get(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	//end, start = start and stop index to draw
	void draw(const GLenum drawMode = GL_POINTS, const unsigned int start = 0) const
	{
		this->draw(drawMode, this->m_uiDataSize / this->m_uiBlockSize, start);
	}

	void draw(const GLenum drawMode, const unsigned int end, const unsigned int start = 0) const
	{
		glBindVertexArray(this->m_uiVAO);

		// Index buffer
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);

		// Draw the triangles !
		glDrawElements(drawMode,      // mode
				end,    // count
				GL_UNSIGNED_INT,   // type (optimization potential)
				(void*) start           // element array buffer offset
				);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//pointer to head of data, length in byte
	inline void copyData(const void* const data, const unsigned int length)
	{
		this->m_vData = std::static_pointer_cast<void>(std::shared_ptr<char>(new char[length], std::default_delete<char[]>()));
		memcpy(this->m_vData.get(), data, length);
	}

	//
	inline void setData(std::shared_ptr<void> data)
	{
		this->m_vData = data;
	}

	//length in Vertices
	inline void setIndices(std::shared_ptr<GLuint> indices, const int length)
	{
		this->m_uiIndexList = indices;
		m_uiIndexSize = length;

		if (m_uiIBO != 0)
		{
			glDeleteBuffers(1, &m_uiIBO);
		}
		// Generate a buffer for the indices
		glGenBuffers(1, &m_uiIBO);
	}

};

#endif /* SRC_OGLWRAPPER_VERTEX_H_ */
