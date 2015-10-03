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

#include <glm/matrix.hpp>

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
		float m_fScale[3];
		float m_fAngle[3];	// in Radiant

		glm::mat4 m_o_ModelMatrix;
		bool m_bModifiedMatrix;

		void activateContext() const;
		void deactivateContext() const;

		void init();

	public:

		Square();
		~Square();

		void update();

		void draw(glm::mat4 projection, glm::mat4 view) const;

		inline void setPosition(float x, float y, float z)
		{
			m_fCenter[0] = x;
			m_fCenter[1] = y;
			m_fCenter[2] = z;

			m_bModifiedMatrix = true;
		}

		inline void setScale(float x, float y, float z)
		{
			m_fScale[0] = x;
			m_fScale[1] = y;
			m_fScale[2] = z;

			m_bModifiedMatrix = true;
		}

		inline void setAngel(float x, float y, float z)	//Rotate arround axis
		{
			m_fAngle[0] = x;
			m_fAngle[1] = y;
			m_fAngle[2] = z;

			m_bModifiedMatrix = true;
		}
	};

} /* namespace render */

#endif /* SRC_RENDERUNITS_BALL_H_ */
