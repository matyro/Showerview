/*
 * Ball.cpp
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#include "Square.h"

#include "OGLWrapper/VertexAttribute.h"

#include <glm/gtc/type_ptr.hpp>

namespace render
{

	void Square::activateContext() const
	{

	}

	void Square::deactivateContext() const
	{

	}

	Square::Square()
	{
		memset(m_fCenter, 0, 3 * sizeof(float));
	}

	Square::~Square()
	{

	}

	void Square::init()
	{
		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/line.vs", "Shader/line.frag"));
		m_o_Shader->addUniform("projection");
		m_o_Shader->addAttribute("color");
		m_o_Shader->addAttribute("position");

		const float vertex[3 * 8] =	{
				-2.0f, -2.0f, 2.0f,
				2.0f, -2.0f, 2.0f,
				2.0f, 2.0f, 2.0f,
				-2.0f, 2.0f, 2.0f,
				-2.0f, -2.0f, -2.0f,
				2.0f, -2.0f, -2.0f,
				2.0f, 2.0f, -2.0f,
				-2.0f, 2.0f,-2.0f };

		const float color[4 * 8] =	{
		1.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 0.0f, 0.5f,
		1.0f, 1.0f, 1.0f, 0.5f,
		0.0f, 1.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 1.0f, 0.5f,
		0.0f, 0.0f, 0.0f, 0.5f,
		1.0f, 0.0f, 1.0f, 0.5f,
		0.5f, 0.5f, 0.5f, 0.5f };

		// Element Indicies for the Cube
		std::vector<unsigned int> indices;
		indices.resize(6 * 6);

		const GLuint indicesTmp[] =
		{ 0, 1, 2, 2, 3, 0, 3, 2, 6, 6, 7, 3, 7, 6, 5, 5, 4, 7, 4, 0, 3, 3, 7, 4, 0, 1, 5, 5, 4, 0, 1, 5, 6, 6, 2, 1 };

		for (int i = 0; i < 36; i++)
		{
			indices[i] = indicesTmp[i];
		}

		std::vector<float> data;
		data.resize(7 * 8);

		for (int i = 0; i < 8; i++)
		{
			data[i * 7 + 0] = vertex[i * 3 + 0] + this->m_fCenter[0];
			data[i * 7 + 1] = vertex[i * 3 + 1] + this->m_fCenter[1];
			data[i * 7 + 2] = vertex[i * 3 + 2] + this->m_fCenter[2];

			data[i * 7 + 3] = color[i * 4 + 0];
			data[i * 7 + 4] = color[i * 4 + 1];
			data[i * 7 + 5] = color[i * 4 + 2];
			data[i * 7 + 6] = color[i * 4 + 3];
		}

		m_o_Vertices.moveData(std::move(data));
		m_o_Vertices.moveIndices(std::move(indices));

		AttributeSetting vertSetting(AttributeSetting::vec3, AttributeSetting::Float, m_o_Shader->attribute("position"),0, 7*sizeof(float));
		AttributeSetting colSetting(AttributeSetting::vec4, AttributeSetting::Float, m_o_Shader->attribute("color"),3*sizeof(float), 7*sizeof(float));

		m_o_Vertices.setSettings<0>(vertSetting);
		m_o_Vertices.setSettings<1>(colSetting);

		m_o_Vertices.transmitData(7 * sizeof(float));
	}

	GLuint vbo_triangle, vbo_triangle_colors;
	GLint attribute_coord2d = 0, attribute_v_color = 1;

	void Square::draw(glm::mat4 viewProjection) const
	{

		this->activateContext();

		m_o_Shader->Use();
		glUniformMatrix4fv(m_o_Shader->uniform("projection"), 1, GL_FALSE, glm::value_ptr(viewProjection));

		m_o_Vertices.draw(GL_TRIANGLES, (unsigned int) 8, (unsigned int) 0);

		this->deactivateContext();
	}

} /* namespace render */
