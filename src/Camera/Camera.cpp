/*
 * Camera.cpp
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#include "Camera.h"


#include <iostream>
#include <math.h>

# define M_PI           3.14159265358979323846  /* pi */



struct Plane
{
	cl_float4 camPos;
	//cl_float4 viewDir;
	cl_float4 planeTL;
	cl_float4 planeR;
	cl_float4 planeD;
};



Camera::Camera(const float dist, const float fov, const float ratio)
	:m_fDist(dist), m_fFov(static_cast<float>(M_PI * fov) / 180.0f), m_fiRatio(1.0f / ratio), m_bCanMove(false), m_fPitch(0.0f), m_fYaw(0.0f),
	m_glm_up(0.0, 1.0, 0.0), m_glm_right(1.0, 0.0, 0.0)
{
	
	m_vec3CamPos = glm::vec3(0.0f, 0.0f, 30.0f);
	m_vec3ViewDirection = glm::vec3(0.0f, 0.0f, -1.0f) * dist;

}




void Camera::rotateCam(const float pitch, const float yaw, const float roll)
{
	if (m_bCanMove)
	{		
		

		

		const float zAxisCut = (glm::pi<float>() / 2.0f) - 0.25f;

		m_fYaw += yaw;

		if ((m_fPitch > zAxisCut && (pitch>0))
			|| ((pitch<0) && m_fPitch < -zAxisCut))
		{			
			return;
		}
		else
		{
			const glm::vec3 cameraRight = glm::normalize(glm::cross(m_vec3ViewDirection, m_glm_up));

			m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, -pitch, cameraRight);
			m_fPitch += pitch;
		}

		m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, -yaw, m_glm_up);
		

	}


}

void Camera::moveCam(const float forward_backward, const float right_left, const float up_down)
{
	//if (m_bCanMove)
	{
		const glm::vec3 cameraForward = glm::normalize(m_vec3ViewDirection);
		const glm::vec3 cameraRight = glm::normalize(glm::cross(m_vec3ViewDirection, m_glm_up));
		const glm::vec3 cameraUp = glm::normalize(glm::cross(m_vec3ViewDirection, cameraRight));

		this->m_vec3CamPos += cameraForward*forward_backward + cameraRight*right_left + cameraUp*up_down;		
	}

}


void Camera::setViewDir(float pitch, float yaw)
{
	//std::cout << "(vorher) Pitch:" << m_fPitch << " Yaw:" << m_fYaw << std::endl;
	//std::cout << "(input) Pitch:" << pitch << " Yaw:" << yaw << std::endl;

	bool old = this->m_bCanMove;
	this->m_bCanMove = true;
	this->rotateCam(pitch - m_fPitch, yaw - m_fYaw, 0.0);
	this->m_bCanMove = old;

	//std::cout << "(nachher) Pitch:" << m_fPitch << " Yaw:" << m_fYaw << std::endl;
}

void Camera::setPosition(float x, float y, float z)
{
	m_vec3CamPos.x = x;
	m_vec3CamPos.y = y;
	m_vec3CamPos.z = z;
}

const glm::vec3 Camera::getPosition()
{
	return this->m_vec3CamPos;
}




const cl_float16 Camera::getViewMatrix() const
{
	glm::mat4 rot1 = glm::rotate(m_fYaw, this->m_glm_up);

	glm::mat4 rot2 = glm::rotate(m_fPitch, glm::vec3(1.0, 0.0, 0.0) );

	glm::mat4 scale = glm::mat4(1);

	glm::mat4 view = rot2 * rot1 * scale;

	view[3] = glm::vec4(-m_vec3CamPos.x, -m_vec3CamPos.y, -m_vec3CamPos.z, 0.0);

	//std::cout << "Matrix: " << glm::to_string(rot1)<<std::endl;

	return toFloat16( view );
}

const cl_float16 Camera::getProjectionMatrix() const
{
	return toFloat16(glm::mat4(1));
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