/*
 * CameraPerspective.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_CAMERAPERSPECTIVE_H_
#define CAMERA_CAMERAPERSPECTIVE_H_

#include "Camera.h"

namespace camera
{

	class CameraPerspective : public Camera
	{

	private:
		glm::vec3 vec3CamPos;
		glm::vec3 vec3ViewDirection;

		bool bCanMov;

		glm::mat4 CameraMatrix;
		glm::mat4 projectionMatrix;

	public:

		CameraPerspective();

		inline void setMovment(bool bCanMove)
		{
			this->bCanMov = bCanMove;
		}

		glm::mat4 getMatrix();

		void rotateCam(float pitch, float yaw, float roll);
	};

} /* namespace camera */

#endif /* CAMERA_CAMERAPERSPECTIVE_H_ */
