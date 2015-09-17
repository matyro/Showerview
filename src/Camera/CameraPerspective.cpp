/*
 * CameraPerspective.cpp
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#include "CameraPerspective.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext.hpp>

namespace camera
{
	CameraPerspective::CameraPerspective()
	{
		CameraMatrix = glm::lookAt(glm::vec3(1, 0, 10), // the position of your camera, in world space
		glm::vec3(0, 0, 0), // where you want to look at, in world space
		glm::vec3(0, 1, 0) // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
				);

		projectionMatrix = glm::perspective(60.0f * (3.1415926f / 180.0f), // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
				1000.0f // Far clipping plane. Keep as little as possible.
				);

		vec3CamPos = glm::vec3(0, 0, 0);
		vec3ViewDirection = glm::vec3(1, 0, 0);
		bCanMov = false;
	}

	glm::mat4 CameraPerspective::getMatrix()
	{
		return projectionMatrix * CameraMatrix;
	}

	void CameraPerspective::rotateCam(float pitch, float yaw, float roll)
	{
		if (bCanMov)
		{
			vec3ViewDirection = glm::vec3(1, 0, 0);
			vec3ViewDirection = glm::rotateY(vec3ViewDirection, pitch);
			vec3ViewDirection = glm::rotateZ(vec3ViewDirection, yaw);
		}

	}

} /* namespace camera */
