
#include "Texture.h"

#include <iostream>
#include <cstring>



void Texture::generateTexture(GLuint target = GL_TEXTURE_2D)
{
	glActiveTexture(GL_TEXTURE0);
	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_uiTexture);

	glBindTexture(target, m_uiTexture);

	

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(target, 0, GL_RGBA8, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	
	
	glBindTexture(target, 0);
	glFinish();

	//glBindImageTexture(0, m_uiTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);

	m_uiTextureType = target;
}


Texture::Texture(const int width, const int height)
	:m_iHeight(height), m_iWidth(width)
{
	m_uitextureCount = 0;
	m_uiTextureType = 0;

	generateTexture(GL_TEXTURE_2D);
}

Texture::~Texture()
{	
	glDeleteTextures(1, &m_uiTexture);
}



void Texture::bindTexture() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);
	//glBindImageTexture(0, m_uiTexture, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);
	
}

