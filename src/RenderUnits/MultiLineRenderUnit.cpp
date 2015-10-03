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
#include <glm/gtx/string_cast.hpp>

namespace render
{

	void MultiLineRenderUnit::activateContext() const
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
	}

	void MultiLineRenderUnit::deactivateContext() const
	{ 
		glDisable(GL_BLEND); 
		glDisable(GL_DEPTH_TEST);
	}

	void MultiLineRenderUnit::init()
	{
	}

	MultiLineRenderUnit::MultiLineRenderUnit()
	{
		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/MultiLine.vs", "Shader/MultiLine.frag", "Shader/MultiLine.geo"));//
		m_o_Shader->addUniform("projectionView");
		m_o_Shader->addUniform("camPos");

		m_o_Shader->addAttribute("position");
		m_o_Shader->addAttribute("width");
		m_o_Shader->addAttribute("color");
		

		AttributeSetting vertSetting(AttributeSetting::vec3, AttributeSetting::Float, m_o_Shader->attribute("position"), offsetof(struct LineVertex, pos), sizeof(LineVertex));
		AttributeSetting widthSetting(AttributeSetting::vec1, AttributeSetting::Float, m_o_Shader->attribute("width"), offsetof(struct LineVertex, width), sizeof(LineVertex));
		AttributeSetting colSetting(AttributeSetting::vec4, AttributeSetting::Float, m_o_Shader->attribute("color"), offsetof(struct LineVertex, color), sizeof(LineVertex));

		m_o_Vertex.setSettings<0>(vertSetting);
		m_o_Vertex.setSettings<1>(widthSetting);
		m_o_Vertex.setSettings<2>(colSetting);
	}

	void MultiLineRenderUnit::draw(glm::mat4 projection, glm::mat4 view) const
	{
		glm::mat4 projectionView = projection * view;

		this->activateContext();

		m_o_Shader->Use();

		glm::vec3 camPos = glm::vec3(glm::inverse(view)[3]);
		glUniform3fv(m_o_Shader->uniform("camPos"), 1, glm::value_ptr(camPos));			
		glUniformMatrix4fv(m_o_Shader->uniform("projectionView"), 1, GL_FALSE, glm::value_ptr(projectionView));
		
		m_o_Vertex.draw(GL_LINES, m_std_lines.size());		

		this->deactivateContext();
	}

	void MultiLineRenderUnit::updateLines()
	{
		m_o_Vertex.copyData(this->m_std_lines);
		m_o_Vertex.transmitData(sizeof(LineVertex));		
	}

	void MultiLineRenderUnit::addLine(const LineVertex start, const LineVertex end)
	{
		this->m_std_lines.push_back(start);
		this->m_std_lines.push_back(end);
	}

	void MultiLineRenderUnit::setBufferSize(const unsigned int numberOfLines)
	{
		this->m_std_lines.resize(2*numberOfLines);
	}
	

} /* namespace render */
