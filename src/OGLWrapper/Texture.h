#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <GL/glew.h>

class Texture
{
private:

	int m_iWidth;	//Texture width
	int m_iHeight;	//Texture height
	int m_iBPP;		// bytes per pixel

	GLuint m_uiTexture;		// Texture ID
	GLuint m_uiSampler;		// Sampler ID

	bool m_bMipMapsGenerated;

	std::string m_sPath;

public:

	Texture();
  
	bool loadTexture2D(std::string path, GLint format, bool mipMaps = false);
	void bindTexture(GLuint texUnit = 0);
	
	void releaseTexture();
};

#endif
