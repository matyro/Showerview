/*
 * CameraMatrix.cpp
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#include "CameraMatrix.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

# define M_PI           3.14159265358979323846  /* pi */

#include <iostream>

namespace camera
{
	void CameraMatrix::calcMatrix()
	{
		m_glm_viewMatrix = glm::lookAt(m_vec3CamPos, // the position of your camera, in world space
			m_vec3CamPos + m_vec3ViewDirection, // where you want to look at, in world space
			m_glm_up // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great too
			);

		
		m_glm_projectionMatrix = glm::perspective(m_fFov, // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
			m_fiRatio, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
			0.3f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
			1000.0f // Far clipping plane. Keep as little as possible.
			);

		//std::cout << "\nOriginal Position: " << glm::to_string(m_vec3CamPos) << std::endl;
		//std::cout << "Cam Position: " << glm::to_string(glm::inverse(m_glm_viewMatrix)[3]) << std::endl;
		//std::cout << "Position: " << glm::to_string(glm::inverse(getProjectionViewMatrix())[3]) << std::endl;
	}

	CameraMatrix::CameraMatrix(const float dist, const float fov, const float ratio)
		:m_fDist(dist), m_fFov((M_PI * fov) / 180.0f), m_fiRatio(ratio), m_bCanMove(false)
	{
		m_vec3CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_vec3ViewDirection = glm::vec3(0.0f, 1.0f, 0.0f);

		m_glm_up = glm::vec3(0.0f, 0.0f, 1.0f);		

		calcMatrix();
	}



	const glm::mat4 CameraMatrix::getProjectionViewMatrix() const
	{
		return m_glm_projectionMatrix * m_glm_viewMatrix;
	}

	const glm::mat4 CameraMatrix::getProjectionMatrix() const
	{
		return m_glm_projectionMatrix;
	}

	const glm::mat4 CameraMatrix::getViewMatrix() const
	{
		return m_glm_viewMatrix;
	}

	void CameraMatrix::rotateCam(const float pitch, const float yaw, const float roll)
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

	void CameraMatrix::moveCam(const float forward_backward, const float right_left, const float up_down)
	{
		//if (m_bCanMove)
		{
			const glm::vec3 cameraForward = -glm::normalize(m_vec3ViewDirection);
			const glm::vec3 cameraRight = glm::normalize(glm::cross(m_glm_up, m_vec3ViewDirection));
			const glm::vec3 cameraUp =  -glm::normalize(glm::cross(m_vec3ViewDirection, cameraRight));

			this->m_vec3CamPos += cameraForward*forward_backward + cameraRight*right_left + cameraUp*up_down;

			this->calcMatrix();
		}

	}

} /* namespace camera */
