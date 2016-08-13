#pragma once


#include <array>

#include "Camera/Camera.h"

#include "Helper/Line.h"



#include "OCL/OpenCL_Error.h"
#include "OCL/OpenCL_Hardware.h"
#include "OCL/OpenCL_Kernel.h"
#include "OCL/OpenCL_Program.h"
#include "OCL/OpenCL_Queue.h"


std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel> initOCL(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines);

void calcOCL(std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel>&, const float frameCount, const Camera& cam, const int lineCount, const int width, const int height);
