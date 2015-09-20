#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <array>
#include <vector>
#include <map>
#include <memory>
#include <iostream>



#include <GL/glew.h>


class Sampler
{
private:

	GLuint m_uiSampler;		// Sampler ID


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


	Sampler* m_o_sSampler;


	GLuint m_uiTexture;		// Texture ID
	GLuint m_uiTextureType;

	bool m_bMipMapsGenerated;

	

	

	const unsigned char* loadImage(const char* path);

	void generateTexture(GLuint count, GLuint target);

public:

	Texture();
	~Texture();
  
	//template<unsigned int texCount>	
	//bool loadTexture(const std::string path[texCount], const GLuint bindTarget, const GLuint loadTarget[texCount], const GLint format[texCount], const bool mipMaps = false);
	
	


	void bindTexture(GLuint texUnit = 0);
	
	void releaseTexture();

	void setSampler(Sampler* const samp){ this->m_o_sSampler = samp; };



	template<unsigned int texCount>	//										GL_TEXTURE_2D
	bool loadTexture(const std::array<const std::string, texCount> path, const GLuint bindTarget, const std::array<const GLuint, texCount> loadTarget, const std::array<const GLint, texCount> format, const bool mipMaps = false)
	{
		m_uitextureCount = texCount;

		generateTexture(texCount, bindTarget);	//GL_TEXTURE_CUBE_MAP

		for (int i = 0; i < texCount; i++)
		{
			const unsigned char* data = this->loadImage(path[i].c_str());
			if (data == nullptr)
			{
				return false;
			}


			glTexImage2D(loadTarget[i], 0, GL_RGB, m_std_vTextureData[i].iWidth, m_std_vTextureData[i].iHeight, 0, format[i], GL_UNSIGNED_BYTE, data);


			if (mipMaps)
			{
				//glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		Move to sampler
				//glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{

				//glSamplerParameteri(m_uiSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				//glSamplerParameteri(m_uiSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}



			GLenum glError = glGetError();
			if (glError)
			{
				std::cerr << "Texture load error" << std::endl;
				return false;
			}



			m_bMipMapsGenerated = mipMaps;

		}

		m_std_vTextureData.resize(m_std_vTextureData.size() - 1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return true; // Success
	}


};

#endif
