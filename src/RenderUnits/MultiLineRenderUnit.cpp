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
		glDepthFunc(GL_EQUAL);// Accept fragment if it closer to the camera than the former one

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void MultiLineRenderUnit::deactivateContext() const
	{ 
		glDisable(GL_BLEND); //restore blending options
		glDisable(GL_DEPTH_TEST);
	}

	void MultiLineRenderUnit::init()
	{
		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/MultiLine.vert", "Shader/MultiLine.frak", "Shader/MultiLine.geo"));
		m_o_Shader->addUniform("projection");
		m_o_Shader->addUniform("cameraPos");

		GLint inPosLocation = m_o_Shader->addAttribute("inPos");
		GLint inDirLocation = m_o_Shader->addAttribute("dir");
		GLint inColLocation = m_o_Shader->addAttribute("color");
		GLint inWidthLocation = m_o_Shader->addAttribute("width");

		glGenBuffers(1, &m_uiVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
		glBufferData(GL_ARRAY_BUFFER, 0*sizeof(float), m_vertexData.get(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_uiVAO);
		glBindVertexArray(m_uiVAO);

		glEnableVertexAttribArray(inPosLocation);	// Set up the vertex attribute pointer for the Vertex attribute
		glVertexAttribPointer(inPosLocation,  		// Attribute location
						3,                    	// Number of elements per vertex, here (x,y,z)
						GL_FLOAT,               // Data type of each element
						GL_FALSE,               // Normalised?
						11 * sizeof(float),     // Stride
						0                       // Offset
						);
		glEnableVertexAttribArray(inDirLocation);
		glVertexAttribPointer(inDirLocation,
						3,
						GL_FLOAT,
						GL_FALSE,
						11 * sizeof(float),
						(void*) (3 * sizeof(float))
						);
		glEnableVertexAttribArray(inColLocation);
		glVertexAttribPointer(inColLocation,
						4,
						GL_FLOAT,
						GL_FALSE,
						11 * sizeof(float),
						(void*) (6 * sizeof(float))
						);
		glEnableVertexAttribArray(inWidthLocation);
		glVertexAttribPointer(inWidthLocation,
						1,
						GL_FLOAT,
						GL_FALSE,
						11 * sizeof(float),
						(void*) (10 * sizeof(float))
						);

		glBindVertexArray(0);

	}

	void MultiLineRenderUnit::draw(glm::mat4 camMatrix) const
	{
		this->activateContext();

		m_o_Shader->Use();

		glm::vec3 camPos = glm::vec3(camMatrix[3]);
		glUniformMatrix4fv(m_o_Shader->uniform("camPos"), 1, GL_FALSE, glm::value_ptr(camPos));
		glUniformMatrix4fv(m_o_Shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(camMatrix));


		glBindVertexArray(this->m_uiVAO);
		glDrawArrays(GL_POINTS, 0, m_std_lines.size());

		this->deactivateContext();
	}

	void MultiLineRenderUnit::updateLines()
	{
		// 0,1,2: 		x,y,z
		// 3,4,5:		direction
		// 6,7,8,9:	Color
		// 10:			width
		m_vertexData = std::unique_ptr<float[]>( new float[m_std_lines.size() * 11] );

		int counter = 0;
		for(const Line itr : m_std_lines)
		{
			std::memcpy(&m_vertexData[counter*11 + 0], itr.start.pos, 3*sizeof(float) );
			m_vertexData[counter*11 + 3] = itr.end.pos[0] - itr.start.pos[0];
			m_vertexData[counter*11 + 4] = itr.end.pos[1] - itr.start.pos[1];
			m_vertexData[counter*11 + 5] = itr.end.pos[2] - itr.start.pos[1];
			std::memcpy(&m_vertexData[counter*11 + 6], itr.start.color, 4*sizeof(float) );
			m_vertexData[counter*11 + 10] = itr.start.width;

			/*counter++;
			std::memcpy(&data[counter*11 + 0], itr.end.pos, 3*sizeof(float) );
			data[counter*11 + 3] = itr.end.pos[0] - itr.start.pos[0];
			data[counter*11 + 4] = itr.end.pos[1] - itr.start.pos[1];
			data[counter*11 + 5] = itr.end.pos[2] - itr.start.pos[2];
			std::memcpy(&data[counter*11 + 6], itr.end.color, 4*sizeof(float) );
			data[counter*11 + 10] = itr.end.width;*/
		}


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
