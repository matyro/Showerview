#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <iostream>



#include <GL/glew.h>


#include "Helper/ErrorLog.h"



class Texture
{
private:




	unsigned int m_uitextureCount;
	
	GLuint m_uiTexture;		// Texture ID
	GLuint m_uiTextureType;

	bool m_bMipMapsGenerated;


	const int m_iWidth;
	const int m_iHeight;

	

	//const bool loadImage(const char* path);

	void generateTexture(GLuint target);

public:

	Texture(const int width, const int height);
	~Texture();
  
	
	void bindTexture() const;
	
	
	inline const GLuint getTextureID() const { return m_uiTexture; }
	


};

#endif
