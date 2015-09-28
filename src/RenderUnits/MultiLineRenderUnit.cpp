/*
 * MultiLineRenderUnit.cpp
 *
 *  Created on: 17.09.2015
 *      Author: dominik
 */

#include "MultiLineRenderUnit.h"

#include <cstring>

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

namespace render
{

	void MultiLineRenderUnit::activateContext() const
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void MultiLineRenderUnit::deactivateContext() const
	{ 
		glDisable(GL_BLEND); 
		glDisable(GL_DEPTH_TEST);
	}

	void MultiLineRenderUnit::init()
	{
		m_vertexData = std::unique_ptr<float[]>(new float[21]);
		
		float arr[] = { 
			-1.0f, -1.0f, -3.0f,
			1.0f, 0.0f, 0.0f, 0.5f,

			1.0f, -1.0f, -3.0f,
			0.0f, 1.0f, 0.0f, 0.5f,

			0.0f, 1.0f, -3.0f,
			0.0f, 0.0f, 1.0f, 0.5f 
		};
	
		memcpy(m_vertexData.get(), arr, sizeof(float) * 21);

		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/MultiLine.vs", "Shader/MultiLine.frag"));//
		m_o_Shader->Use();
		
		GLint inPosLocation = m_o_Shader->addAttribute("position");
		GLint inColLocation = m_o_Shader->addAttribute("color");
		
		//m_o_Shader->addUniform("projection");
		
		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);


		glGenBuffers(1, &m_uiVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 21 * sizeof(float), m_vertexData.get(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);	// Set up the vertex attribute pointer for the Vertex attribute
		glVertexAttribPointer(0,  		// Attribute location
						3,                    	// Number of elements per vertex, here (x,y,z)
						GL_FLOAT,               // Data type of each element
						GL_FALSE,               // Normalised?
						0,     // Stride
						0                       // Offset
						);		

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1,
						4,
						GL_FLOAT,
						GL_FALSE,
						0,
						(void*) (3 * sizeof(float))
						);		

		glBindVertexArray(0);

		//glDeleteBuffers(1, &m_uiVBO);
	}

	void MultiLineRenderUnit::draw(glm::mat4 camMatrix) const
	{
		this->activateContext();

		m_o_Shader->Use();

		glm::vec3 camPos = glm::vec3(camMatrix[3]);
		//glUniformMatrix4fv(m_o_Shader->uniform("camPos"), 1, GL_FALSE, glm::value_ptr(camPos));
		//glUniformMatrix4fv(m_o_Shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(camMatrix));

		
		glBindVertexArray(this->m_uiVAO);
		glDrawArrays(GL_TRIANGLES, 0, m_std_lines.size()*2);

		glDisableVertexAttribArray(0); 
		glBindBuffer(GL_ARRAY_BUFFER, 0); 

		this->deactivateContext();
	}

	void MultiLineRenderUnit::updateLines()
	{
		// 0,1,2: 		x,y,z
		// 3,4,5:		direction
		// 6,7,8,9:	Color
		// 10:			width
		m_vertexData = std::unique_ptr<float[]>( new float[m_std_lines.size() * 7 * 3] );

		int counter = 0;
		for(const Line itr : m_std_lines)
		{
			std::memcpy(&m_vertexData[counter*7 + 0], itr.start.pos, 3*sizeof(float) );
			std::memcpy(&m_vertexData[counter*7 + 3], itr.start.color, 4*sizeof(float) );
			counter++;

			std::memcpy(&m_vertexData[counter * 7 + 0], itr.end.pos, 3 * sizeof(float));
			std::memcpy(&m_vertexData[counter * 7 + 3], itr.end.color, 4 * sizeof(float));
			counter++;

			std::memcpy(&m_vertexData[counter * 7 + 0], itr.end.pos, 3 * sizeof(float));
			std::memcpy(&m_vertexData[counter * 7 + 3], itr.end.color, 4 * sizeof(float));

			m_vertexData[counter * 7 + 1] += 2.0;
			counter++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, m_std_lines.size() * 7 * 2 * sizeof(float), m_vertexData.get(), GL_STATIC_DRAW);
	}

	void MultiLineRenderUnit::addLine(const LineVertex start, const LineVertex end)
	{
		this->m_std_lines.push_back(Line(start,end));
	}

	void MultiLineRenderUnit::setBufferSize(const unsigned int numberOfLines)
	{
		this->m_std_lines.resize(numberOfLines);
	}


	const Line MultiLineRenderUnit::operator[](unsigned int i) const
	{
		return m_std_lines[i];
	}

	Line& MultiLineRenderUnit::operator[](unsigned int i)
	{
		return m_std_lines[i];
	}

} /* namespace render */
