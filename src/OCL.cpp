
#include <iostream>
#include <vector>
#include <tuple>
#include <string>


#ifdef _WIN32
#include <Windows.h>
#include <Wingdi.h>
#else
#include <GL/glx.h>
#endif

#include <GL/glew.h>

#include "OCL.h"




std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel> initOCL(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines)
{
	

	auto hardware = OCL_Hardware();

	auto program = OCL_Program("../Shader/kernel.cl");
	program.load(hardware.getDevice(), hardware.getContext());

	auto queue = OCL_Queue(hardware.getContext(), hardware.getDevice());
	
	
	
	
	std::cout << "Init Kernel" << std::endl;
	auto kernel = OCL_Kernel("writeTexture");
	kernel.createKernel(program.getProgram());
	kernel.info();

	kernel.addMemoryGL(hardware.getContext(), 0, textureFlag, textureID);

	kernel.addBuffer(hardware.getContext(), CL_MEM_READ_ONLY, 1, lines);
	kernel.setArg(3, static_cast<unsigned int>(lines.size()));



	kernel.setupQueue(queue.getQueue());



	return std::make_tuple(hardware, program, queue, kernel);
}


void calcOCL(std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel>& data, const float frameCount, const Camera& cam, const int lineCount, const int width, const int height)
{
	//std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;

	auto& queue = std::get<2>(data);
	auto& kernel= std::get<3>(data);

	
	kernel.setArg(2, lineCount);
	kernel.setArg(4, cam.plane());	

	//std::cout << "Init" << std::endl;
	kernel.init(queue.getQueue());

	//std::cout << "Kernel Start" << std::endl;
	error = queue.getQueue().enqueueNDRangeKernel(kernel.getKernel(), cl::NDRange(0,0), cl::NDRange(width, height), cl::NDRange(8,8));
	errorCheck("Exe Kernel", error);	
	//std::cout << "Kernel End" << std::endl;

	kernel.release(queue.getQueue());
	

	//std::cout << "Wait OCL:" << std::endl;
	queue.getQueue().finish();	

	//std::cout << "End OCL:" << std::endl;
}
