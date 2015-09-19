#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>

class Texture
{
private:

	int iWidth, iHeight, iBPP; // Texture width, height, and bytes per pixel
	GLuint uiTexture; // Texture name
	GLuint uiSampler; // Sampler name
	bool bMipMapsGenerated;

	int tfMinification, tfMagnification;

	std::string sPath;

public:

	Texture();
  
	bool loadTexture2D(std::string a_sPath, bool bGenerateMipMaps = false);
	void bindTexture(int iTextureUnit = 0);

	void setFiltering(int a_tfMagnification, int a_tfMinification);

	int getMinificationFilter();
	int getMagnificationFilter();

	void releaseTexture();
};

#endif
