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
#include <vector>

#include "VertexAttribute.h"

template<std::size_t tNumberOfAttributes>
class Vertex
{
private:
	GLuint m_uiVBO;
	GLuint m_uiVAO;
	GLuint m_uiIBO;	//Index Buffer Object

	AttributeSetting m_std_AttribList[tNumberOfAttributes];

	std::vector<float> m_std_fData;
	std::vector<GLuint> m_std_uiIndexList;

public:

	Vertex() :
			m_uiVBO(0), m_uiVAO(0), m_uiIBO(0)
	{
		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);

		glGenBuffers(1, &m_uiVBO);
		glGenBuffers(1, &m_uiIBO);

		glBindVertexArray(0);
	}

	template<unsigned int id>
	void setSettings(const AttributeSetting& attribute)
	{
		static_assert(id >= 0 || id < tNumberOfAttributes, "ID out of bounds!");
		m_std_AttribList[id].set(attribute);
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
	
	void transmitData(const unsigned int stride, GLenum usage = GL_STATIC_DRAW)
	{
		glBindVertexArray(m_uiVAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, this->m_std_fData.size() * sizeof(this->m_std_fData[0]), this->m_std_fData.data(), usage);

		for (auto itr : m_std_AttribList)
		{
			itr.applySetting(stride);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_std_uiIndexList.size() * sizeof(this->m_std_uiIndexList[0]), this->m_std_uiIndexList.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	//end, start = start and stop index to draw
	/*void draw(const GLenum drawMode = GL_POINTS, const unsigned int start = 0) const
	{
		this->draw(drawMode, this->m_uiDataSize / this->m_uiBlockSize, start);
	}*/

	void draw(const GLenum drawMode, const GLsizei end, const unsigned int start = 0) const
	{
		glBindVertexArray(this->m_uiVAO);

		// Index buffer
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);

		if(m_std_uiIndexList.size() == 0)
		{
			glDrawArrays(GL_TRIANGLES, 0, end);
		}
		else
		{
			glDrawElements(drawMode,     // mode
							end-start,    		// count
							GL_UNSIGNED_INT,   	// type (optimization potential)
							(void*) 0       // element array buffer offset
							);
		}


		glBindVertexArray(0);
	}

	//pointer to head of data, length in byte
	/*inline void copyData(const float* const data, const unsigned int length)
	{
		this->m_std_fData.resize(length);
		memcpy(this->m_std_fData.data(), data, length);
	}*/

	//
	inline void moveData(std::vector<float>&& data)
	{
		this->m_std_fData = std::move(data);
	}

	//length in Vertices
	inline void moveIndices(std::vector<unsigned int>&& indices)
	{
		this->m_std_uiIndexList = std::move(indices);
	}

};

#endif /* SRC_OGLWRAPPER_VERTEX_H_ */
