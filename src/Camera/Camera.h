/*
 * Camera.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace camera
{
	class Camera
	{
	private:


	public:


		virtual glm::mat4 getMatrix() = 0;
	};
}
#endif /* CAMERA_H_ */
