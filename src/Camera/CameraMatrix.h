#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <iostream>
#include <math.h>

#include <CL/cl.hpp>

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



	inline const glm::vec3 getPosition() { return m_vec3CamPos; };


	inline void setMovable(const bool bCanMove) { this->m_bCanMove = bCanMove; };
	inline const bool isMovable() const { return this->m_bCanMove; }


};



cl_float16 toFloat16(glm::mat4 mat);

