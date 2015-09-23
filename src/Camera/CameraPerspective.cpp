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
#include <glm/gtc/constants.hpp>

#include <iostream>

namespace camera
{
	void CameraPerspective::calcMatrix()
	{
		m_glm_viewMatrix = glm::lookAt(m_vec3CamPos, // the position of your camera, in world space
			m_vec3CamPos + m_vec3ViewDirection, // where you want to look at, in world space
			m_glm_up // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
			);

		m_glm_projectionMatrix = glm::perspective(60.0f * (3.1415926f / 180.0f), // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			1000.0f // Far clipping plane. Keep as little as possible.
			);
	}

	CameraPerspective::CameraPerspective()		
	{
		m_vec3CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_vec3ViewDirection = glm::vec3(-1.0f, 0.0f, 0.0f);

		m_glm_up = glm::vec3(0, 1, 0);
		m_bCanMove = false;

		calcMatrix();
	}



	const glm::mat4 CameraPerspective::getMatrix() const
	{
		return m_glm_projectionMatrix * m_glm_viewMatrix;
	}

	void CameraPerspective::rotateCam(const float pitch, const float yaw, const float roll)
	{
		if (m_bCanMove)
		{
			//m_vec3ViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

			glm::vec3 cameraRight = glm::normalize(glm::cross(m_glm_up, m_vec3ViewDirection));
			glm::vec3 cameraUp = glm::cross(m_vec3ViewDirection, cameraRight);


			const float zAngle = glm::angle(m_vec3ViewDirection, m_glm_up);
			const float zAxisCut = 0.2;

			m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, yaw, cameraUp);

			if( (zAngle > (glm::pi<float>() - zAxisCut) && (pitch>0))
					||	((pitch<0) &&  zAngle < 0.2)  )
			{
				this->calcMatrix();
				return;
			}
			else
			{
				m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, pitch, cameraRight);
				this->calcMatrix();
			}



		}


	}

	void CameraPerspective::moveCam(const float x, const float y, const float z)
	{
		if (m_bCanMove)
		{
			m_vec3CamPos += glm::vec3(x, y, z);
		}
		this->calcMatrix();
	}

} /* namespace camera */
