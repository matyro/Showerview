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

	
		glm::mat4 m_glm_viewMatrix;
		glm::mat4 m_glm_projectionMatrix;

		glm::vec3 m_glm_up;

		void calcMatrix();

	public:

		CameraPerspective();				

		const glm::mat4 getMatrix() const;

		void rotateCam(const float pitch, const float yaw, const float roll);
		void moveCam(const float forward_backward, const float right_left, const float up_down);
	};

} /* namespace camera */

#endif /* CAMERA_CAMERAPERSPECTIVE_H_ */
