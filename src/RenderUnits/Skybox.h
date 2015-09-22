#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <sstream>
#include <string>

#include <GL/glew.h>



#include "RenderUnit.h"
#include "OGLWrapper/Texture.h"
#include "OGLWrapper/Shader.h"

namespace render
{

	class Skybox : public RenderUnit
	{
	private:

		Texture m_o_Texture;
		std::shared_ptr<Sampler> m_o_Sampler;

		GLuint m_uiSkyboxVAO;
		GLuint m_uiSkyboxVBO;

		std::unique_ptr<Shader> m_o_Shader;

		void activateContext();
		void deactivateContext();

		Skybox(const Skybox& cpy){}
		Skybox(Skybox&& mv){}

	public:

		Skybox();
		~Skybox();

		void init();

		void draw(glm::mat4);


	};

}
#endif
