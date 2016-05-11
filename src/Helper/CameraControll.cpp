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
		if (m_o_OpenGLCam != nullptr)
		{
			m_o_OpenGLCam->rotateCam(-deltaY*m_fRotationSpeed, -deltaX*m_fRotationSpeed, 0);
		}
	}

	void CameraControll::move(direction dir, bool stop)
	{
		if(dir != noMovment)
			this->m_uiMovmentDir[dir] = stop ? 0 : 1;
	}

	void CameraControll::setTime(const double time)
	{
		static double oldTime = time;
		m_dDeltaTime = oldTime - time;
		oldTime = time;
	}

	void CameraControll::update()
	{
		float mov[3];

		float movmentSpeed = m_fMovementSpeed;
		if (this->m_uiMovmentDir[boost])
		{
			movmentSpeed *= 10;
		}
		
		mov[0] = m_dDeltaTime * movmentSpeed * (this->m_uiMovmentDir[forward] - this->m_uiMovmentDir[backward]);
		mov[1] = m_dDeltaTime * movmentSpeed * (this->m_uiMovmentDir[right] - this->m_uiMovmentDir[left]);
		mov[2] = m_dDeltaTime * movmentSpeed * (this->m_uiMovmentDir[up] - this->m_uiMovmentDir[down]);

		if( mov[0] || mov[1] || mov[2])
			this->m_o_OpenGLCam->moveCam(mov[0], mov[1], -mov[2]);
	}
}

helper::CameraControll& SCamControll()
{
	return helper::CameraControll::getInstance(); 
}


