/*
 * RenderUnit.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#ifndef RENDERUNITS_RENDERUNIT_H_
#define RENDERUNITS_RENDERUNIT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace render
{

	class RenderUnit
	{
	private:

		virtual void activateContext() const = 0;
		virtual void deactivateContext() const = 0;

	public:

		virtual ~RenderUnit();

		virtual void init();

		virtual void draw(glm::mat4) const = 0;


	};

} /* namespace render */

#endif /* RENDERUNITS_RENDERUNIT_H_ */
