/*
 * Ball.h
 *
 *  Created on: 28.09.2015
 *      Author: dominik
 */

#ifndef SRC_RENDERUNITS_BALL_H_
#define SRC_RENDERUNITS_BALL_H_

#include "RenderUnit.h"


#include <memory>
#include <functional>

#include <GL/glew.h>

#include "OGLWrapper/Vertex.h"
#include "OGLWrapper/Shader.h"



namespace render
{
	
	class Ball: public RenderUnit
	{
	private:

		Vertex<2> m_o_Vertices;
		Shader m_o_Shader;

		void activateContext() const;
		void deactivateContext() const;

	public:

		~Ball();

		void init();

		void draw(glm::mat4) const ;

		void setPosition(float x, float y, float z);
		void setRadius(std::function<float(*)(float, float, float)> func);
	};

} /* namespace render */

#endif /* SRC_RENDERUNITS_BALL_H_ */
