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


class Camera
{
private:
	glm::vec3 vec3CamPos;
	glm::vec3 vec3ViewDirection;

	bool bCanMov;

	glm::mat4 CameraMatrix;
	glm::mat4 projectionMatrix;


public:
	Camera(float, float);

	inline void setMovment(bool bCanMove)
	{
		this->bCanMov = bCanMove;
	}

	glm::mat4 getMatrix();

	void rotateCam(float pitch, float yaw, float roll);
};

#endif /* CAMERA_H_ */
