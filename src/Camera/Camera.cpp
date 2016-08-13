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


void Camera::update()
{
	glm::vec3 right = glm::normalize(glm::cross(m_vec3ViewDirection, m_glm_up));
	glm::vec3 up = glm::normalize(glm::cross(right, m_vec3ViewDirection));

	right = right * std::sin(m_fFov / 2.0f) * glm::length(m_vec3ViewDirection);
	right = right * 2.0f;


	up = up * glm::length(right) * m_fiRatio;

	this->m_vec3TopLeft = m_vec3CamPos + m_vec3ViewDirection - right / 2.0 + up / 2.0;
	
	this->m_vec3Right = 1.0 * right;
	this->m_vec3Down = -1.0 * up;
}


Camera::Camera(const float dist, const float fov, const float ratio)
	:m_fDist(dist), m_fFov(static_cast<float>(M_PI * fov) / 180.0f), m_fiRatio(1.0f / ratio), m_bCanMove(false)
{
	
	m_vec3CamPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_vec3ViewDirection = glm::vec3(0.0f, 1.0f, 0.0f) * dist;

	m_glm_up = glm::vec3(0, 0, 1);	


	update();
}




void Camera::rotateCam(const float pitch, const float yaw, const float roll)
{
	if (m_bCanMove)
	{
		//m_vec3ViewDirection = glm::vec3(0.0f, 0.0f, -1.0f);

		glm::vec3 cameraRight = glm::normalize(glm::cross(m_vec3ViewDirection, m_glm_up));
		glm::vec3 cameraUp = glm::cross(m_vec3ViewDirection, cameraRight);


		const float zAngle = glm::angle(m_vec3ViewDirection, m_glm_up);
		const float zAxisCut = 0.25f;

		m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, yaw, cameraUp);

		if ((zAngle > (glm::pi<float>() - zAxisCut) && (pitch>0))
			|| ((pitch<0) && zAngle < zAxisCut))
		{			
			return;
		}
		else
		{
			update();
			m_vec3ViewDirection = glm::rotate(m_vec3ViewDirection, pitch, cameraRight);			
		}


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
		
		update();
	}

}


const glm::vec3 Camera::getPosition()
{
	return this->m_vec3CamPos;
}

const cl_float16 Camera::plane() const
{
	std::cout << "Plane: " << std::endl;
	std::cout << "vec3 CamPos: " << glm::to_string(m_vec3CamPos) << std::endl;

	cl_float16 ret;
	std::memset(ret.s, 0, sizeof(cl_float16) );
	for (int i = 0; i < 3; i++)
	{
		ret.s[0 + i] = glm::value_ptr(m_vec3CamPos)[i];
		ret.s[4 + i] = glm::value_ptr(m_vec3TopLeft)[i];
		ret.s[8 + i] = glm::value_ptr(m_vec3Right)[i];
		ret.s[12 + i] = glm::value_ptr(m_vec3Down)[i];
	}


	return ret;
}






