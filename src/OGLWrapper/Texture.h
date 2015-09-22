#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <iostream>



#include <GL/glew.h>


#include "Helper\ErrorLog.h"


class Sampler
{
private:

	GLuint m_uiSampler;		// Sampler ID

	Sampler(const Sampler& cpy) {}
	Sampler(Sampler&& mv) {}

public:
	Sampler()
	{
		glGenSamplers(1, &m_uiSampler);		// State and settings "memory"
	}

	~Sampler()
	{
		glDeleteSamplers(1, &m_uiSampler);
	}

	void setSetting(GLenum pname, GLint param)
	{
		glSamplerParameteri(m_uiSampler, pname, param);
	};

	const GLuint getSampler() const { return m_uiSampler; }
};

class Texture
{
private:

	struct texInfo
	{
		int iWidth;	//Texture width
		int iHeight;	//Texture height
		int iBPP;		// bytes per pixel

		std::string std_sPath;	// Path to texture

		std::unique_ptr<unsigned char[]> ucImageData;

		texInfo()
		{
			iWidth = 0;
			iHeight = 0;
			iBPP = 0;
		}

		texInfo(texInfo&& rhs)
		{
			this->iWidth = rhs.iWidth;
			this->iHeight = rhs.iHeight;
			this->iBPP = rhs.iBPP;

			this->std_sPath = std::move(rhs.std_sPath);

			this->ucImageData = std::move(rhs.ucImageData);
		}
	};


	unsigned int m_uitextureCount;
	std::vector<texInfo> m_std_vTextureData;


	std::shared_ptr<Sampler> m_o_sSampler;


	GLuint m_uiTexture;		// Texture ID
	GLuint m_uiTextureType;

	bool m_bMipMapsGenerated;

	

	

	const bool loadImage(const char* path);

	void generateTexture(GLuint count, GLuint target);

public:

	Texture();
	~Texture();
  
	
	void bindTexture(GLuint texUnit = 0);
	
	void releaseTexture();

	void setSampler(std::shared_ptr<Sampler> samp){ this->m_o_sSampler = samp; };



	template<unsigned int texCount>	//										GL_TEXTURE_2D
	bool loadTexture(const std::array<const std::string, texCount> path, const GLuint bindTarget, const std::array<const GLuint, texCount> loadTarget, const std::array<const GLint, texCount> format, const bool mipMaps = false)
	{
		std::cout << "Load " << texCount << " textures: " << std::endl;

		m_uitextureCount = texCount;

		generateTexture(1, bindTarget);	//GL_TEXTURE_CUBE_MAP

		for (int i = 0; i < texCount; i++)
		{
			bool data = this->loadImage(path[i].c_str());
			if (!data)
			{
				std::cerr << "Texture " << path[i] << " could not be loaded" << std::endl;
				return false;
			}

			std::cout << "Image: " << this->m_std_vTextureData[i].std_sPath << " (" << this->m_std_vTextureData[i].iWidth << "," << this->m_std_vTextureData[i].iHeight << std::endl;


			glTexImage2D(loadTarget[i], 0, GL_RGB, m_std_vTextureData[i].iWidth, m_std_vTextureData[i].iHeight, 0, format[i], GL_UNSIGNED_BYTE, m_std_vTextureData[i].ucImageData.get() );

			m_bMipMapsGenerated = mipMaps;

			if (mipMaps && !(bindTarget == GL_TEXTURE_CUBE_MAP))
			{
				glGenerateMipmap(bindTarget);
			}
		}

		if (mipMaps && (bindTarget== GL_TEXTURE_CUBE_MAP))
		{
			glGenerateMipmap(bindTarget);
		}
		

		m_std_vTextureData.resize(m_std_vTextureData.size() - 1);
		glBindTexture(bindTarget, 0);

		ErrorLog<5>::OpenGLError(" textureLoad");

		return true; // Success
	}


};

#endif
