/*
 * shader.cpp
 *
 *  Created on: 01.09.2015
 *      Author: dominik
 */

#include "Texture.h"

#include <iostream>

#include <FreeImage.h>

Texture::Texture()
{

}


bool Texture::loadTexture2D(std::string path,GLint format, bool mipMaps)
{	
	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(path.c_str(), 0);

	if (imageFormat == FIF_UNKNOWN)									// Try filetype from filename
	{
		imageFormat = FreeImage_GetFIFFromFilename(path.c_str());

		if (imageFormat == FIF_UNKNOWN)								//still unknown?
			return false;
	}
		
	
	if (!FreeImage_FIFSupportsReading(imageFormat))
		return false;


	FIBITMAP* imageBitmap = FreeImage_Load(imageFormat, path.c_str());


	if (!imageBitmap)
		return false;

	BYTE* data = FreeImage_GetBits(imageBitmap); // Retrieve the image data

	m_iWidth = FreeImage_GetWidth(imageBitmap); // Get the image width and height
	m_iHeight = FreeImage_GetHeight(imageBitmap);
	m_iBPP = FreeImage_GetBPP(imageBitmap);

	
	if (data == NULL || m_iWidth == 0 || m_iHeight == 0)
		return false;

	// Generate an OpenGL texture ID for this texture
	glGenTextures(1, &m_uiTexture);
	glBindTexture(GL_TEXTURE_2D, m_uiTexture);	

	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, data);

	

	if (mipMaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	FreeImage_Unload(imageBitmap);

	glGenSamplers(1, &m_uiSampler);

	GLenum glError = glGetError();
	if (glError)
	{
		std::cerr << "Texture load error" << std::endl;
		return false;
	}

	m_sPath = path;
	m_bMipMapsGenerated = mipMaps;

	return true; // Success
}


void Texture::bindTexture(GLuint texUnit)
{

}

void Texture::releaseTexture()
{
	glDeleteSamplers(1, &m_uiSampler);
	glDeleteTextures(1, &m_uiTexture);
}