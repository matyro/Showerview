#pragma once


#include <CL/cl.hpp>

#include <array>

#include <Camera/Camera.h>

#include "Helper/Line.h"

std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory> initOCL(unsigned int textureFlag, unsigned int textureID, std::vector<Line>& lines);

void calcOCL(std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory>&, const float, const Plane&, const int, const int );
