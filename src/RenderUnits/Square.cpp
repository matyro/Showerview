/*
 * Ball.cpp
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#include "Square.h"

#include "OGLWrapper/VertexAttribute.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

namespace render
{

	void Square::activateContext() const
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

	}

	void Square::deactivateContext() const
	{
		glDisable(GL_BLEND); //restore blending options		
		glDisable(GL_DEPTH_TEST);		
	}

	void Square::init()
	{
		const float vertex[3 * 8] = {
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f,-1.0f };

		const float color[4 * 8] = {
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f };

		const GLuint indicesTmp[] =
		{
			0, 1, 2,
			2, 3, 0,
			3, 2, 6,
			6, 7, 3,
			7, 6, 5,
			5, 4, 7,
			4, 0, 3,
			3, 7, 4,
			0, 5, 1,
			4, 5, 0,
			1, 5, 6,
			6, 2, 1
		};


		// Element Indicies for the Cube
		std::vector<unsigned int> indices;
		indices.resize(6 * 6);
		for (int i = 0; i < 36; i++)
		{
			indices[i] = indicesTmp[i];
		}

		//Element Data for the Cube
		std::vector<float> data;
		data.resize(7 * 8);

		for (int i = 0; i < 8; i++)
		{
			data[i * 7 + 0] = vertex[i * 3 + 0];
			data[i * 7 + 1] = vertex[i * 3 + 1];
			data[i * 7 + 2] = vertex[i * 3 + 2];

			data[i * 7 + 3] = color[i * 4 + 0];
			data[i * 7 + 4] = color[i * 4 + 1];
			data[i * 7 + 5] = color[i * 4 + 2];
			data[i * 7 + 6] = color[i * 4 + 3];
		}

		m_o_Vertices.moveData(std::move(data));
		m_o_Vertices.moveIndices(std::move(indices));

		m_o_Vertices.transmitData(7 * sizeof(float));
	}

	Square::Square()
	{
		m_bModifiedMatrix = true;
		m_o_ModelMatrix = glm::mat4(1);

		for (int i = 0; i < 3; i++)
		{
			m_fCenter[i] = 0;
			m_fAngle[i] = 0;
			m_fScale[i] = 1;
		}

		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/generic.vs", "Shader/generic.frag"));
		m_o_Shader->addUniform("projectionView");
		m_o_Shader->addUniform("model");
		m_o_Shader->addAttribute("color");
		m_o_Shader->addAttribute("position");


		AttributeSetting vertSetting(AttributeSetting::vec3, AttributeSetting::Float, m_o_Shader->attribute("position"), 0, 7 * sizeof(float));
		AttributeSetting colSetting(AttributeSetting::vec4, AttributeSetting::Float, m_o_Shader->attribute("color"), 3 * sizeof(float), 7 * sizeof(float));

		m_o_Vertices.setSettings<0>(vertSetting);
		m_o_Vertices.setSettings<1>(colSetting);

		init();
	}

	Square::~Square()
	{

	}	

	void Square::update()
	{
		if (m_bModifiedMatrix)
		{
			glm::mat4 rotateX = glm::rotate(m_fAngle[0], glm::tvec3<float>(1.0f, 0.0f, 0.0f));		// Second parameter = axis
			glm::mat4 rotateY = glm::rotate(m_fAngle[1], glm::tvec3<float>(0.0f, 1.0f, 0.0f));
			glm::mat4 rotateZ = glm::rotate(m_fAngle[2], glm::tvec3<float>(0.0f, 0.0f, 1.0f));

			glm::mat4 translate = glm::translate(glm::tvec3<float>(this->m_fCenter[0], this->m_fCenter[1], this->m_fCenter[2]));
			glm::mat4 scale = glm::scale(glm::tvec3<float>(this->m_fScale[0], this->m_fScale[1], this->m_fScale[2]));

			//First scale, then rotate, then translate
			m_o_ModelMatrix = translate * (rotateZ*rotateY*rotateX) * scale;

			m_bModifiedMatrix = false;
		}
	}

	void Square::draw(glm::mat4 projection, glm::mat4 view) const
	{
		glm::mat4 projectionView = projection * view;

		this->activateContext();
		m_o_Shader->Use();		
		
		glUniformMatrix4fv(m_o_Shader->uniform("model"), 1, GL_FALSE, glm::value_ptr(m_o_ModelMatrix));
		glUniformMatrix4fv(m_o_Shader->uniform("projectionView"), 1, GL_FALSE, glm::value_ptr(projectionView));

		m_o_Vertices.draw(GL_TRIANGLES, 36, 0);

		this->deactivateContext();
	}

} /* namespace render */
