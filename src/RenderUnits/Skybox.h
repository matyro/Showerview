#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include <sstream>
#include <string>

#include <GL/glew.h>



#include "RenderUnit.h"

namespace render
{

	class Skybox : public RenderUnit
	{
	private:

		void activateContext();
		void deactivateContext();

	public:

		~Skybox();

		void init();

		void draw(glm::mat4);


	};

}
#endif
