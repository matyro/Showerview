/*
 * MultiLineRenderUnit.cpp
 *
 *  Created on: 17.09.2015
 *      Author: dominik
 */

#include "MultiLineRenderUnit.h"

namespace render
{

	void MultiLineRenderUnit::activateContext() const
	{

	}

	void MultiLineRenderUnit::deactivateContext() const
	{ 

	}

	void MultiLineRenderUnit::init()
	{

	}

	void MultiLineRenderUnit::draw(glm::mat4) const
	{

	}

	void MultiLineRenderUnit::addLine(LineVertex start, LineVertex end)
	{

	}

	void MultiLineRenderUnit::setBufferSize(const unsigned int numberOfLines)
	{

	}


	const Line MultiLineRenderUnit::operator[](unsigned int i) const
	{
		if (i >= this->m_uiLineCount)
		{
			return Line(LineVertex(), LineVertex());
		}
	}

	/*ine& MultiLineRenderUnit::operator[](unsigned int i)
	{
		if (i >= this->m_uiLineCount)
		{

		}
		else
		{
			return
		}
	}*/

} /* namespace render */
