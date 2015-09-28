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
		uCompressed = GL_UNSIGNED_INT_2_10_10_10_REV
	};
	enum eSize
	{
		vec1 = 1, vec2 = 2, vec3 = 3, vec4 = 4
	};

private:

	const eSize m_eAttributeSize;
	const eType m_eAttributeType;

public:

	//Returns the size in Byte for the block
	constexpr unsigned int getSize() const;

	constexpr AttributeSetting(const eSize size, const eType type) :
			m_eAttributeSize(size), m_eAttributeType(type)
	{
	}

	void applySetting(const GLuint index, const unsigned int stride, const unsigned int offset) const;

};

#endif /* SRC_OGLWRAPPER_VERTEXATTRIBUTE_H_ */
