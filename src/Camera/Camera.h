/*
 * Camera.h
 *
 *  Created on: 15.09.2015
 *      Author: dominik
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace camera
{
	class Camera
	{
	private:


	protected:
		glm::vec3 m_vec3CamPos;
		glm::vec3 m_vec3ViewDirection;

		bool m_bCanMove = false;

	public:
		virtual ~Camera();

		virtual const glm::mat4 getMatrix() const = 0;

		virtual void rotateCam(const float, const float, const float) = 0;	//pitch, yaw, roll
		virtual void moveCam(const float, const float, const float) = 0;	// x, y, z movement in Worldspace

		inline void setMovable(const bool bCanMove){ this->m_bCanMove = bCanMove; };
		inline const bool isMovable() const { return this->m_bCanMove; }

	};
}
#endif /* CAMERA_H_ */
