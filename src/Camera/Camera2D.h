/*
 * CameraOrthographic.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_CAMERAORTHOGRAPHIC_H_
#define CAMERA_CAMERAORTHOGRAPHIC_H_

#include "Camera.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>


namespace camera {

	class Camera2D : public Camera
	{
	private:



	public:
		const glm::mat4 getProjectionViewMatrix(){ return glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600)) * glm::translate(-m_vec3CamPos); }
		const glm::mat4 getProjectionMatrix() { return glm::ortho(0.0f, static_cast<float>(800), 0.0f, static_cast<float>(600)); }
		const glm::mat4 getViewMatrix() { return glm::translate(-m_vec3CamPos); }

		void rotateCam(const float, const float, const float){}
		void moveCam(const float, const float, const float){}
	};

} /* namespace camera */

#endif /* CAMERA_CAMERAORTHOGRAPHIC_H_ */
