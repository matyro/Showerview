/*
 * CameraPerspective.h
 *
 *  Created on: 16.09.2015
 *      Author: dominik
 */

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <iostream>
#include <math.h>

#include <CL/cl.hpp>



struct Plane
{
	cl_float4 camPos;
	cl_float4 viewDir;
	cl_float4 planeTL;
	cl_float4 planeR;
	cl_float4 planeD;
};

struct Matrix
{
	cl_float4 a;	// Zeile 1
	cl_float4 b;   // Zeile 2
	cl_float4 c;	// Zeile 3
	cl_float4 d;	// Zeile 4
};

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


	inline const Plane plane()
	{
		return  {m_vec3CamPos.x, m_vec3CamPos.y, m_vec3CamPos.z, 0.0f,
				 m_vec3ViewDirection.x,  m_vec3ViewDirection.y,  m_vec3ViewDirection.z, 0.0f,
				 m_vec3TopLeft.x, m_vec3TopLeft.y, m_vec3TopLeft.z, 0.0f,
				 m_vec3Right.x, m_vec3Right.y, m_vec3Right.z, 0.0f,
				 m_vec3Down.x, m_vec3Down.y, m_vec3Down.z, 0.0f } ;
	}



};

