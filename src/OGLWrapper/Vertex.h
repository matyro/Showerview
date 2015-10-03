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

template<std::size_t tNumberOfAttributes, typename TDataType = float>
class Vertex
{
private:
	GLuint m_uiVBO;
	GLuint m_uiVAO;
	GLuint m_uiIBO;	//Index Buffer Object

	AttributeSetting m_std_AttribList[tNumberOfAttributes];

	std::vector<TDataType> m_std_Data;
	std::vector<GLuint> m_std_uiIndexList;

public:

	Vertex() :
			m_uiVBO(0), m_uiVAO(0), m_uiIBO(0)
	{
		glGenVertexArrays(1, &m_uiVAO);
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
		if (m_uiIBO != 0)
		{
			glDeleteBuffers(1, &m_uiIBO);
		}

	}
	
	void transmitData(const unsigned int stride, GLenum usage = GL_STATIC_DRAW)
	{		
		glBindVertexArray(m_uiVAO);

		if (this->m_std_uiIndexList.size() > 0)
		{			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->m_std_uiIndexList.size() * sizeof(this->m_std_uiIndexList[0]), this->m_std_uiIndexList.data(), GL_STATIC_DRAW);
		}
		
		glBindBuffer(GL_ARRAY_BUFFER, this->m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, this->m_std_Data.size() * sizeof(TDataType), &(this->m_std_Data[0]), usage);

		for (auto itr : m_std_AttribList)
		{
			itr.applySetting(stride);
		}	
		
		glBindVertexArray(0);
	}


	//End = number of incices
	void draw(const GLenum drawMode, const GLsizei end, const unsigned int start = 0) const
	{
		glBindVertexArray(this->m_uiVAO);

		// Index buffer
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_uiIBO);

		if(m_std_uiIndexList.size() == 0)
		{
			glDrawArrays(drawMode, start, end);
		}
		else
		{
			glDrawElements(drawMode,			// mode
							end,    		// count
							GL_UNSIGNED_INT,   	// type (optimization potential)
							(void*)start			// element array buffer offset
							);
		}


		glBindVertexArray(0);
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

	//length in Vertices
	inline void moveIndices(std::vector<unsigned int>&& indices)
	{
		this->m_std_uiIndexList = std::move(indices);
	}

};

#endif /* SRC_OGLWRAPPER_VERTEX_H_ */
