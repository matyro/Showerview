/*
 * shader.cpp
 *
 *  Created on: 01.09.2015
 *      Author: dominik
 */

#include "Texture.h"

#include <iostream>
#include <cstring>

#include <FreeImage.h>



const BYTE* Texture::loadImage(const char* path)
{
	texInfo tmpInfo;

	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(path, 0);

	if (imageFormat == FIF_UNKNOWN)									// Try filetype from filename
	{
		imageFormat = FreeImage_GetFIFFromFilename(path);

		if (imageFormat == FIF_UNKNOWN)								//still unknown?
			return nullptr;
	}


	if (!FreeImage_FIFSupportsReading(imageFormat))
		return nullptr;


	FIBITMAP* imageBitmap = FreeImage_Load(imageFormat, path);


	if (!imageBitmap)
		return nullptr;

	BYTE* data = FreeImage_GetBits(imageBitmap); // Retrieve the image data

	tmpInfo.iWidth = FreeImage_GetWidth(imageBitmap); // Get the image width and height
	tmpInfo.iHeight = FreeImage_GetHeight(imageBitmap);
	tmpInfo.iBPP = FreeImage_GetBPP(imageBitmap);
	tmpInfo.std_sPath = path;

	if (data == NULL || tmpInfo.iWidth == 0 || tmpInfo.iHeight == 0)
		return nullptr;


	std::unique_ptr<BYTE[]> imageData(new BYTE[tmpInfo.iHeight * tmpInfo.iWidth * tmpInfo.iBPP]);

	tmpInfo.ucImageData = std::move(imageData);


	memcpy(tmpInfo.ucImageData.get(), data, tmpInfo.iHeight * tmpInfo.iWidth *  tmpInfo.iBPP);



	FreeImage_Unload(imageBitmap); 

	m_std_vTextureData.push_back( std::move(tmpInfo) );

	return tmpInfo.ucImageData.get();
}


void Texture::generateTexture(GLuint count, GLuint target)
{
	// Generate an OpenGL texture ID for this texture
	glGenTextures(count, &m_uiTexture);
	glBindTexture(target, m_uiTexture);

	m_uiTextureType = target;
}


Texture::Texture()
{
	m_o_sSampler = nullptr;
	m_uitextureCount = 0;
	m_uiTextureType = 0;
}

Texture::~Texture()
{
	
}



void Texture::bindTexture(GLuint texUnit)
{
	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(m_uiTextureType, m_uiTexture);

	if (m_o_sSampler != nullptr)
		glBindSampler(texUnit, m_o_sSampler->getSampler());

}

void Texture::releaseTexture()
{
	glDeleteTextures(1, &m_uiTexture);

}
