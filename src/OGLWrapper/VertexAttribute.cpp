/*
 * VertexAttribute.cpp
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#include "VertexAttribute.h"

constexpr unsigned int AttributeSetting::getSize() const
{
	return m_eAttributeSize
			* ((m_eAttributeType == eType::Byte || m_eAttributeType == eType::uByte || m_eAttributeType == eType::Int || m_eAttributeType == eType::uInt
					|| m_eAttributeType == eType::Float || m_eAttributeType == eType::Compressed || m_eAttributeType == eType::uCompressed) ?
					4 :
					((m_eAttributeType == eType::Short || m_eAttributeType == eType::uShort || m_eAttributeType == eType::hFloat) ? 2 :
						(m_eAttributeType == eType::Double) ? 4 : 0));
}

void AttributeSetting::applySetting(const GLuint index, const unsigned int stride, const unsigned int offset) const
	{
		glVertexAttribPointer(index,  		// Attribute location
				this->m_eAttributeSize, // Number of elements per vertex, here (x,y,z)
				m_eAttributeType,       // Data type of each element
				GL_FALSE,               // Normalised?
				stride,     			// Stride
				&offset                  // Offset
				);
		glEnableVertexAttribArray(index);	// Set up the vertex attribute pointer for the Vertex attribute
	}
