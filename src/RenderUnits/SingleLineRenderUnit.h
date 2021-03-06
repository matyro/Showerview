/*
 * LineRender.h
 *
 *  Created on: 03.09.2015
 *      Author: dominik
 */

#ifndef LINERENDER_H_
#define LINERENDER_H_

#include "../OGLWrapper/Shader.h"

#include "RenderUnit.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <GL/glew.h>

namespace render
{
	class LineRender : public RenderUnit
	{
	private:

		Shader* shader;

		GLuint vertexBufferObject;
		GLuint vertexArrayObject;

		glm::mat4 ModelMatrix;

		static inline double GET_ABS(double x)
		{
			return x > 0 ? x : -x;
		}

		void activateContext() const ;
		void deactivateContext() const ;
	public:

		LineRender();

		~LineRender();

		void init();

		void line(double x1, double y1, double z1, double x2, double y2, double z2, //coordinates of the line
				float w,                            //width/thickness of the line in pixel
				float Cr, float Cg, float Cb,    //RGB color components
				float alpha    					// alpha channel
				);

		void draw(glm::mat4 camMatrix) const;

	};
}

#endif /* LINERENDER_H_ */
