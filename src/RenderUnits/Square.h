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
	
	class Square: public RenderUnit
	{
	private:

		Vertex<2> m_o_Vertices;
		std::unique_ptr<Shader> m_o_Shader;

		float m_fCenter[3];

		void activateContext() const;
		void deactivateContext() const;

	public:

		Square();
		~Square();

		void init();

		void draw(glm::mat4) const;

		inline void setPosition(float x, float y, float z)
		{
			m_fCenter[0] = x;
			m_fCenter[1] = y;
			m_fCenter[2] = z;
		}
	};

} /* namespace render */

#endif /* SRC_RENDERUNITS_BALL_H_ */
