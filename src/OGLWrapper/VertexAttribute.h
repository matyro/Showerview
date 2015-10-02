/*
 * VertexAttribute.h
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#ifndef SRC_OGLWRAPPER_VERTEXATTRIBUTE_H_
#define SRC_OGLWRAPPER_VERTEXATTRIBUTE_H_

#include<GL/glew.h>

class AttributeSetting
{
public:
	enum eType
	{
		Byte = GL_BYTE,
		uByte = GL_UNSIGNED_BYTE,
		Short = GL_SHORT,
		uShort = GL_UNSIGNED_SHORT,
		Int = GL_INT,
		uInt = GL_UNSIGNED_INT,
		hFloat = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		Double = GL_DOUBLE,
		Compressed = GL_INT_2_10_10_10_REV,
		uCompressed = GL_UNSIGNED_INT_2_10_10_10_REV,
		_typeUnknown = -1
	};
	enum eSize
	{
		vec1 = 1, vec2 = 2, vec3 = 3, vec4 = 4, _vecUnknown = -1
	};

private:

	eSize m_eAttributeSize;
	eType m_eAttributeType;

	GLuint m_uiOffset;
	GLuint m_uiStride;

	GLuint m_uiIndex;
public:

	constexpr AttributeSetting() :
			m_eAttributeSize(_vecUnknown), m_eAttributeType(_typeUnknown), m_uiOffset(0), m_uiStride(0), m_uiIndex(0)
	{
	}

	constexpr AttributeSetting(const eSize size, const eType type, const GLuint index, const GLuint offset = 0, const GLuint stride = 0) :
			m_eAttributeSize(size), m_eAttributeType(type), m_uiOffset(offset), m_uiStride(stride), m_uiIndex(index)
	{
	}

	constexpr AttributeSetting(const AttributeSetting& cpy) :
			m_eAttributeSize(cpy.m_eAttributeSize), m_eAttributeType(cpy.m_eAttributeType), m_uiOffset(cpy.m_uiOffset), m_uiStride(cpy.m_uiStride), m_uiIndex(
					cpy.m_uiIndex)
	{
	}

	void set(const AttributeSetting& rhs)
	{
		this->m_eAttributeSize = rhs.m_eAttributeSize;
		this->m_eAttributeType = rhs.m_eAttributeType;
		this->m_uiOffset = rhs.m_uiOffset;
		this->m_uiStride = rhs.m_uiStride;
		this->m_uiIndex = rhs.m_uiIndex;
	}


	void applySetting(const GLsizei stride) const;

	//Returns the size in Byte for the block
	constexpr unsigned int getSize() const
	{
		return m_eAttributeSize
				* ((m_eAttributeType == eType::Byte || m_eAttributeType == eType::uByte || m_eAttributeType == eType::Int || m_eAttributeType == eType::uInt
						|| m_eAttributeType == eType::Float || m_eAttributeType == eType::Compressed || m_eAttributeType == eType::uCompressed) ?
						4 :
						((m_eAttributeType == eType::Short || m_eAttributeType == eType::uShort || m_eAttributeType == eType::hFloat) ? 2 :
							(m_eAttributeType == eType::Double) ? 4 : 0));
	}

};

#endif /* SRC_OGLWRAPPER_VERTEXATTRIBUTE_H_ */
