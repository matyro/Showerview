#pragma once


#include <CL/cl.hpp>

#include <array>

#include "Camera/Camera.h"

#include "Helper/Line.h"

std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory> initOCL(unsigned int textureFlag, unsigned int textureID, std::vector<Line>& lines);

void calcOCL(std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory>& , const float , const cl_float4& , const cl_float16& , const cl_float16& , const int, const int);
