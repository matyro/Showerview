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


void CameraMatrix::calcMatrix()
{
	const float p = -m_fPitch;
	const float y = -m_fYaw;

	const float a = -m_vec3CamPos.x;
	const float b = -m_vec3CamPos.y;
	const float c = -m_vec3CamPos.z;

	
	glm::mat4 xRot(	1.0f,	0.0f,	0.0f,	0.0f,
					0.0f,	cos(p),	-sin(p),0.0f,
					0.0f,	sin(p),	cos(p),	0.0f,
					0.0f,	0.0f,	0.0f,	1.0f);

	glm::mat4 zRot(	cos(y), -sin(y),0.0f,	0.0f,
					sin(y),	cos(y),	0.0f,	0.0f,
					0.0f,	0.0f,	1.0f,	0.0f,
					0.0f,	0.0f,	0.0f,	1.0f);

	glm::mat4 tran(	1.0f,	0.0f,	0.0f,	a,
					0.0f,	1.0f,	0.0f,	b,
					0.0f,	0.0f,	1.0f,	c,
					0.0f,	0.0f,	0.0f,	1.0f);

	m_glm_viewMatrix = (xRot * zRot) + tran;

	m_glm_projectionMatrix = glm::perspective(m_fFov, // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
		m_fiRatio, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
		0.3f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
		1000.0f // Far clipping plane. Keep as little as possible.
	);

	//std::cout << "\nOriginal Position: " << glm::to_string(m_vec3CamPos) << std::endl;
	//std::cout << "Cam Position: " << glm::to_string(glm::inverse(m_glm_viewMatrix)[3]) << std::endl;
	//std::cout << "Position: " << glm::to_string(glm::inverse(getProjectionViewMatrix())[3]) << std::endl;

	//std::cout << "\nOriginal Position: " << glm::to_string(this->getProjectionViewMatrix()) << std::endl;
}

CameraMatrix::CameraMatrix(const float dist, const float fov, const float ratio)
	:m_fDist(dist), m_fFov((M_PI * fov) / 180.0f), m_fiRatio(ratio), m_bCanMove(false), m_fPitch(0), m_fYaw(0)
{
	m_vec3CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
	

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
		m_fYaw += yaw;

		const float zAxisCut = 0.2;
		

		if ((m_fPitch > (glm::pi<float>() - zAxisCut) && (pitch > 0))
			|| ((pitch < 0) && m_fPitch < 0.2))
		{
			this->calcMatrix();
			return;
		}
		else
		{
			m_fPitch += pitch;
			this->calcMatrix();
		}


	}


}

void CameraMatrix::moveCam(const float forward_backward, const float right_left, const float up_down)
{
	const float p = m_fPitch;
	const float y = m_fYaw;

	glm::mat3 xRot(1.0f, 0.0f, 0.0f,
		0.0f, cos(p), -sin(p), 
		0.0f, sin(p), cos(p));

	glm::mat3 zRot(cos(y), -sin(y), 0.0f, 
		sin(y), cos(y), 0.0f,
		0.0f, 0.0f, 1.0f);


	glm::vec3 viewDir(0.0f,	1.0f,	0.0f);
	viewDir = (xRot * zRot) * viewDir;

	//if (m_bCanMove)
	{
		const glm::vec3 cameraForward = glm::normalize(viewDir);
		const glm::vec3 cameraRight = glm::normalize(glm::cross(m_glm_up, viewDir));
		const glm::vec3 cameraUp = glm::normalize(glm::cross(viewDir, cameraRight));

		this->m_vec3CamPos += cameraForward*forward_backward + cameraRight*right_left + cameraUp*up_down;

		this->calcMatrix();
	}

}



cl_float16 toFloat16(glm::mat4 mat)
{
	cl_float16 tmp;

	for (int i = 0; i < 4; i++)
	{
		tmp.s[0 + i * 4] = mat[0][i];
		tmp.s[1 + i * 4] = mat[1][i];
		tmp.s[2 + i * 4] = mat[2][i];
		tmp.s[3 + i * 4] = mat[3][i];
	}
	return tmp;
}