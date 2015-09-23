/*
 * Camera.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_CONTROLL_H_
#define CAMERA_CONTROLL_H_

#include <memory>

#include "../Camera/Camera.h"

namespace helper
{
	class CameraControll
	{
	private:
		std::shared_ptr<camera::Camera> m_camOpenGLCam;			// Controlled camera

		CameraControll();
		CameraControll(CameraControll const &);					// No copy-Constructor
		CameraControll& operator=(CameraControll const &){};	// No assignment opertor


		enum direction
		{
			forward,
			backward,
			left,
			right,
			up,
			down,
			stop
		};

		double m_dDeltaTime;

		float m_fDeltaXAcc, m_fDeltaYAcc;
		const float m_fRotationSpeed = 0.005; // Degree/Pixel

	protected:
		

	public:
		~CameraControll();	


		static inline CameraControll& getInstance()
		{			
			static CameraControll singelton;
			return singelton; 
		}

		inline void setCamera(std::shared_ptr<camera::Camera> cam)
		{
			this->m_camOpenGLCam = cam;

			this->m_camOpenGLCam->rotateCam(0,0,0);
		}
		
		inline void activateMovment() const { if(m_camOpenGLCam != nullptr) m_camOpenGLCam->setMovable(true); }
		inline void deactivateMovment() const { if (m_camOpenGLCam != nullptr) m_camOpenGLCam->setMovable(false); }

		void rotate(const float deltaX, const float deltaY);
		void move(direction dir);
		void setTime(const double time);
	};
}

helper::CameraControll& SCamControll();
#endif /* CAMERA_CONTROLL_H_ */
