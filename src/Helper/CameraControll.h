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
		std::shared_ptr<camera::Camera> m_o_OpenGLCam;			// Controlled camera

		CameraControll();
		CameraControll(CameraControll const &);					// No copy-Constructor
		CameraControll& operator=(CameraControll const &){return *this;}	// No assignment opertor

		double m_dDeltaTime;

		float m_fDeltaXAcc, m_fDeltaYAcc;

		const float m_fRotationSpeed = 0.005; 	// Degree/Pixel
		const float m_fMovementSpeed = 0.5;		// OpenGL Units / sec

		bool m_uiMovmentDir[6] = {0,0,0,0,0,0};

	protected:
		

	public:

		enum direction
				{
					noMovment=-1,
					forward=0,
					backward=1,
					right=2,
					left=3,
					up=4,
					down=5
				};


		~CameraControll();	


		static inline CameraControll& getInstance()
		{			
			static CameraControll singelton;
			return singelton; 
		}

		inline void setCamera(std::shared_ptr<camera::Camera> cam)
		{
			this->m_o_OpenGLCam = cam;

			this->m_o_OpenGLCam->rotateCam(0,0,0);
		}
		
		inline void activateMovment() const { if(m_o_OpenGLCam != nullptr) m_o_OpenGLCam->setMovable(true); }
		inline void deactivateMovment() const { if (m_o_OpenGLCam != nullptr) m_o_OpenGLCam->setMovable(false); }

		void rotate(const float deltaX, const float deltaY);
		void move(direction dir, bool stop = false);
		void setTime(const double time);

		void update();
	};
}

helper::CameraControll& SCamControll();
#endif /* CAMERA_CONTROLL_H_ */
