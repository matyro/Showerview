/*
 * Camera.cpp
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#include "CameraControll.h"

#include <iostream>


namespace helper
{

	CameraControll::CameraControll()
	{
		m_fDeltaXAcc = 0;
		m_fDeltaYAcc = 0;
		m_dDeltaTime = 0;

	}


	CameraControll::~CameraControll()
	{		
	}

	void CameraControll::rotate(const float deltaX, const float deltaY)
	{
		if (m_camOpenGLCam != nullptr)
		{
			m_camOpenGLCam->rotateCam(deltaY*m_fRotationSpeed, -deltaX*m_fRotationSpeed, 0);
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


