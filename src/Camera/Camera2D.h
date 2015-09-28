/*
 * CameraOrthographic.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_CAMERAORTHOGRAPHIC_H_
#define CAMERA_CAMERAORTHOGRAPHIC_H_

#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace camera {

	class Camera2D : public Camera
	{
	private:



	public:
		glm::mat4 getProjectionViewMatrix(){ return glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600)); }

		void rotateCam(const float, const float, const float){}
		void moveCam(const float, const float, const float){}
	};

} /* namespace camera */

#endif /* CAMERA_CAMERAORTHOGRAPHIC_H_ */
