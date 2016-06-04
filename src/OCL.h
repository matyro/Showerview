#pragma once


#include <array>

#include "Camera/Camera.h"

#include "Helper/Line.h"



#include "OCL/OpenCL_Error.h"
#include "OCL/OpenCL_Hardware.h"
#include "OCL/OpenCL_Kernel.h"
#include "OCL/OpenCL_Program.h"
#include "OCL/OpenCL_Queue.h"


std::tuple<OCL_Queue, OCL_Kernel, OCL_Kernel> initOCL(unsigned int textureFlag, unsigned int textureID, std::vector<Line>& lines);

void calcOCL(std::tuple<OCL_Queue, OCL_Kernel, OCL_Kernel>& , const float , const cl_float16&, const cl_float16&, const int,  const int, const int);
