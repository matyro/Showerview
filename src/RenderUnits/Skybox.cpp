/*
 * shader.cpp
 *
 *  Created on: 01.09.2015
 *      Author: dominik
 */

#include "Skybox.h"


#include <FreeImage.h>
#include <glm/gtc/type_ptr.hpp>




namespace render
{

	void Skybox::activateContext()
	{		
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);// Accept fragment if it closer to the camera than the former one

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthMask(GL_FALSE);
	}

	void Skybox::deactivateContext()
	{
		glDisable(GL_DEPTH_TEST);

		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDepthMask(GL_TRUE);
	}


	Skybox::Skybox()
	{
		m_uiSkyboxVAO = 0;
		m_uiSkyboxVBO = 0;
		m_o_Shader = nullptr;
	}

	Skybox::~Skybox()
	{

	}

	void Skybox::init()
	{
		std::cout << "Skybox init .."<< std::endl;


		m_o_Shader = std::unique_ptr<Shader>(new Shader("Shader/SkyBox.vs", "Shader/SkyBox.frag") );
		m_o_Shader->addUniform("camera");
		m_o_Shader->addUniform("skybox");



		m_o_Sampler = std::shared_ptr<Sampler>( new Sampler() );
		m_o_Sampler->setSetting(GL_TEXTURE_MAG_FILTER,	GL_LINEAR);
		m_o_Sampler->setSetting(GL_TEXTURE_MIN_FILTER,	 GL_LINEAR);
		m_o_Sampler->setSetting(GL_TEXTURE_WRAP_S,		GL_CLAMP_TO_EDGE);
		m_o_Sampler->setSetting(GL_TEXTURE_WRAP_T,		GL_CLAMP_TO_EDGE);
		m_o_Sampler->setSetting(GL_TEXTURE_WRAP_R,		GL_CLAMP_TO_EDGE);
		
		m_o_Texture.setSampler(m_o_Sampler);

		// Loads a cubemap texture from 6 individual texture faces
		// Order should be:
		// +X (right)
		// -X (left)
		// +Y (top)
		// -Y (bottom)
		// +Z (front)
		// -Z (back)
		m_o_Texture.loadTexture<6>({ "Material/Skybox1/positivX.jpg", "Material/Skybox1/negativX.jpg",
			"Material/Skybox1/positivY.jpg", "Material/Skybox1/negativY.jpg", "Material/Skybox1/positivZ.jpg",
			"Material/Skybox1/negativZ.jpg" },
			GL_TEXTURE_CUBE_MAP,
			{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
			{ GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB, GL_RGB },
			true);




		GLfloat skyboxVertices[] = {			        
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};	

		for (unsigned int i = 0; i < sizeof(skyboxVertices)/sizeof(float); i++)
		{
			skyboxVertices[i] = skyboxVertices[i] * 20;
		}
		
		glGenVertexArrays(1, &m_uiSkyboxVAO);
		glGenBuffers(1, &m_uiSkyboxVBO);
		glBindVertexArray(m_uiSkyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiSkyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		std::cout << "Skybox finished"<< std::endl;
	}

	void Skybox::draw(glm::mat4 camMatrix)
	{

		this->activateContext();

		glm::mat4 view = glm::mat4( glm::mat3( camMatrix ) );	//Remove translation
		
		
		
		glUniformMatrix4fv(m_o_Shader->uniform("camera"), 1, GL_FALSE, glm::value_ptr(view));
		// skybox cube
		glBindVertexArray(m_uiSkyboxVAO);
		glUniform1i(m_o_Shader->uniform("skybox"), 0);

		m_o_Texture.bindTexture(0);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		this->deactivateContext();
	}
}
