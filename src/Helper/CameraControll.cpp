/*
 * Camera.cpp
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#include "CameraControll.h"



namespace helper
{

	CameraControll::CameraControll()
	{
		m_fPitch = 0;
		m_fYaw = 0;
		m_dDeltaTime = 0;
	}


	CameraControll::~CameraControll()
	{		
	}

	void CameraControll::rotate(const float deltaX, const float deltaY)
	{
		if (m_camOpenGLCam != nullptr)
		{
			m_fYaw += deltaX*m_fRotationSpeed;
			m_fPitch += deltaY*m_fRotationSpeed;

			m_camOpenGLCam->rotateCam(m_fPitch,m_fYaw,0);
		}
	}

	void CameraControll::setTime(const double time)
	{
		static double oldTime = time;
		m_dDeltaTime = oldTime - time;
		oldTime = time;
	}
}

helper::CameraControll& SCamControll()
{
	return helper::CameraControll::getInstance(); 
}


