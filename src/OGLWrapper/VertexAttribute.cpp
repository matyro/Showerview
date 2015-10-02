/*
 * VertexAttribute.cpp
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#include "VertexAttribute.h"



void AttributeSetting::applySetting(const GLsizei stride) const
{
	glVertexAttribPointer(m_uiIndex,  		// Attribute location
			this->m_eAttributeSize, // Number of elements per vertex, here (x,y,z)
			m_eAttributeType,       // Data type of each element
			GL_FALSE,               // Normalised?
			stride,     			// Stride
			reinterpret_cast<void*>(m_uiOffset)   // Offset
			);

	glEnableVertexAttribArray(m_uiIndex);	// Set up the vertex attribute pointer for the Vertex attribute

}
