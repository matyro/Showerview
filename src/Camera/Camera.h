#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <iostream>
#include <math.h>

#include "OCL/OpenCL_Include.h"


struct Plane;


struct Matrix;


class Camera
{

private:

	glm::vec3 m_vec3CamPos;
	glm::vec3 m_vec3ViewDirection;

	glm::vec3 m_glm_up;


	//View raster
	glm::vec3 m_vec3TopLeft;
	glm::vec3 m_vec3Right;
	glm::vec3 m_vec3Down;


	// Basic infos
	const float m_fFov;		// in Radiant
	const float m_fDist;
	const float m_fiRatio;	// heigt / witdh (invertet ratio)
	

	bool m_bCanMove;

	void update();

public:

	/*
	dist: Distance baseplane to eye
	fov: Field of view in degree
	ratio: aspect ratio width/height
	*/
	Camera(const float dist, const float fov, const float ratio);


	void rotateCam(const float pitch, const float yaw, const float roll);
	void moveCam(const float forward_backward, const float right_left, const float up_down);

	

	const glm::vec3 getPosition();


	inline void setMovable(const bool bCanMove) { this->m_bCanMove = bCanMove; };
	inline const bool isMovable() const { return this->m_bCanMove; }


	const cl_float16 Camera::plane() const;



};

