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


# define M_PI           3.14159265358979323846f  /* pi */

#include <iostream>


void CameraMatrix::calcMatrix()
{	
	
	const float a = m_vec3CamPos.x;
	const float b = m_vec3CamPos.y;
	const float c = m_vec3CamPos.z;

	
	m_vec3ViewDir = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(m_glm_up, m_vec3ViewDir));
	glm::vec3 cameraUp = glm::cross(m_vec3ViewDir, cameraRight);
	m_vec3ViewDir = glm::rotate(m_vec3ViewDir, m_fYaw, cameraUp);
	m_vec3ViewDir = glm::rotate(m_vec3ViewDir, m_fPitch, cameraRight);

	
	std::cout << "ViewDir: " << glm::to_string(m_vec3ViewDir) << std::endl;

	glm::mat4 yRot = customRotate(m_fYaw, cameraUp);

	glm::mat4 xRot = customRotate(m_fPitch, cameraRight);
	
	

	glm::mat4 tran(	1.0f,	0.0f,	0.0f,	a,
					0.0f,	1.0f,	0.0f,	b,
					0.0f,	0.0f,	1.0f,	c,
					0.0f,	0.0f,	0.0f,	1.0f);

	m_glm_viewMatrix = tran * yRot * xRot;

	//m_glm_viewMatrix = glm::inverse(m_glm_viewMatrix);

	std::cout << "YRot: " << glm::to_string(yRot) << std::endl;
	std::cout << "XRot: " << glm::to_string(xRot) << std::endl;
	std::cout << "Tran: " << glm::to_string(tran) << std::endl;


	std::cout << "Tran yRot: " << glm::to_string(tran * yRot) << std::endl; 
	std::cout << "yRot xRot: " << glm::to_string(yRot * xRot) << std::endl;

	std::cout << "All: " << glm::to_string(m_glm_viewMatrix) << std::endl << std::endl;


	float n = 1.0f;			//near
	float f = 32768.0f;		//far

	float l = sin(m_fFov / 2.0f) * n;	// left

	float t = m_fiRatio * l;			// top

	


	m_glm_projectionMatrix = glm::mat4( n/l,	0.0f,	0.0f,	0.0f,
										0.0f,   n/t,	0.0f,	0.0f,
										0.0f,	0.0f,	1.0f,	0.0f,
										0.0f,	0.0f,	0.0f,	1.0f);

	

	//std::cout << "\nOriginal Position: " << glm::to_string(m_vec3CamPos) << std::endl;
	//std::cout << "Cam Position: " << glm::to_string(glm::inverse(m_glm_viewMatrix)[3]) << std::endl;
	//std::cout << "Position: " << glm::to_string(glm::inverse(getProjectionViewMatrix())[3]) << std::endl;

	//std::cout << "\nOriginal Position: " << glm::to_string(this->getProjectionViewMatrix()) << std::endl;
}

CameraMatrix::CameraMatrix(const float dist, const float fov, const float ratio)
	:m_fDist(dist), m_fFov((M_PI * fov) / 180.0f), m_fiRatio(1.0f/ratio), m_bCanMove(false), m_fPitch(0.0f), m_fYaw(0.0f)
{
	m_vec3CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
	

	m_glm_up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vec3ViewDir = glm::vec3(0.0f, 0.0f, 1.0f);

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

		const float zAxisCut = 0.2f;
		

		if ( (m_fPitch < (-M_PI + zAxisCut) && (pitch < 0))
			|| (m_fPitch > (M_PI - zAxisCut) && (pitch > 0)) )
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
	//if (m_bCanMove)
	{
		const glm::vec3 cameraForward = glm::normalize(m_vec3ViewDir);
		const glm::vec3 cameraRight = glm::normalize(glm::cross(m_glm_up, m_vec3ViewDir));
		const glm::vec3 cameraUp = glm::normalize(glm::cross(m_vec3ViewDir, cameraRight));

		this->m_vec3CamPos += cameraForward*forward_backward + cameraRight*right_left + cameraUp*up_down;

		this->calcMatrix();
	}

}



cl_float16 toFloat16(glm::mat4 mat)
{
	cl_float16 tmp;

	for (int i = 0; i < 4; i++)
	{
		tmp.s[0 + i * 4] = mat[i][0];
		tmp.s[1 + i * 4] = mat[i][1];
		tmp.s[2 + i * 4] = mat[i][2];
		tmp.s[3 + i * 4] = mat[i][3];
	}
	return tmp;
}


glm::mat4 customRotate(const float t, glm::vec3 u)
{
	const float ct = cos(t);
	const float st = sin(t);


	return glm::mat4(
		ct + u.x*u.x*(1-ct),	u.x*u.y*(1-ct)-u.z*st,	u.x*u.z*(1-ct) + u.y*st, 0.0f,
		u.y*u.x*(1-ct)+ u.z*st,	ct+u.y*u.y*(1-ct),		u.y*u.z*(1-ct) - u.x*st, 0.0f,
		u.z*u.x*(1-ct)-u.y*st,	u.z*u.y*(1-ct)+u.x*st,	ct+u.z*u.z*(1-ct),		 0.0f,
		0.0f,					0.0f,					0.0f,					 1.0f	);
}