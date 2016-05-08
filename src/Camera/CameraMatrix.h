/*
 * CameraPerspective.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_CAMERAPERSPECTIVE_H_
#define CAMERA_CAMERAPERSPECTIVE_H_

#include "Camera.h"

#include <CL/cl.hpp>

namespace camera
{

	class CameraMatrix
	{

	private:
		glm::vec3 m_vec3CamPos;
		glm::vec3 m_vec3ViewDirection;

	
		glm::mat4 m_glm_viewMatrix;
		glm::mat4 m_glm_projectionMatrix;

		glm::vec3 m_glm_up;

		void calcMatrix();

		bool m_bCanMove;

		// Basic infos
		const float m_fFov;		// in Radiant
		const float m_fDist;
		const float m_fiRatio;	// heigt / witdh (invertet ratio)

	public:

		CameraMatrix(const float dist, const float fov, const float ratio);

		const glm::mat4 getProjectionViewMatrix() const;
		const glm::mat4 getProjectionMatrix() const;
		const glm::mat4 getViewMatrix() const;


		void rotateCam(const float pitch, const float yaw, const float roll);
		void moveCam(const float forward_backward, const float right_left, const float up_down);



		inline const glm::vec3 getPosition() { return m_vec3CamPos };


		inline void setMovable(const bool bCanMove) { this->m_bCanMove = bCanMove; };
		inline const bool isMovable() const { return this->m_bCanMove; }

		
	};



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

} /* namespace camera */

#endif /* CAMERA_CAMERAPERSPECTIVE_H_ */
