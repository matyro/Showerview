
#include <iostream>
#include <vector>
#include <tuple>
#include <string>


#include <GL/glew.h>

#ifdef _WIN32
#include <Windows.h>
#include <Wingdi.h>
#else
#include <GL/glx.h>
#endif


#include "OCL.h"




std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel, OCL_Kernel> initOCL(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines)
{
	

	auto hardware = OCL_Hardware();

	auto program = OCL_Program("../Shader/kernel.cl");
	program.load(hardware.getDevice(), hardware.getContext());

	auto queue = OCL_Queue(hardware.getContext(), hardware.getDevice());
	
	
	
	
	std::cout << "Init Kernel" << std::endl;

	auto kernel1 = OCL_Kernel("transform");
	kernel1.createKernel(program.getProgram());
	kernel1.info();

	kernel1.addBuffer(hardware.getContext(), CL_MEM_READ_WRITE, 0, lines);
	kernel1.setArg(1, static_cast<unsigned int>(lines.size()) );

	kernel1.setupQueue(queue.getQueue());


	auto kernel2 = OCL_Kernel("writeTexture");
	kernel2.createKernel(program.getProgram());
	kernel2.info();

	kernel2.addMemoryGL(hardware.getContext(), 0, textureFlag, textureID);

	kernel2.setArg(1, kernel1.getBuffer(0) );
	
	kernel2.setArg(2, static_cast<unsigned int>(lines.size()));



	//kernel2.setupQueue(queue.getQueue());



	return std::make_tuple(hardware, program, queue, kernel1, kernel2);
}


void calcOCL(std::tuple<OCL_Hardware, OCL_Program, OCL_Queue, OCL_Kernel, OCL_Kernel>& data, const float frameCount, const Camera& cam, const int lineCount, const int width, const int height, cl_float16 view, cl_float16 projection)
{
	//std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;

	auto& queue = std::get<2>(data);
	auto& kernel1= std::get<3>(data);
	auto& kernel2 = std::get<4>(data);
	

	int lineCount_rounded = static_cast<int>( (static_cast<float>(lineCount) / 256.0) + 1.0) * 256;

	kernel1.setArg(2, view);
	kernel1.setArg(3, projection);

	kernel1.setupQueue(queue.getQueue());

	error = queue.getQueue().enqueueNDRangeKernel(kernel1.getKernel(), cl::NDRange(0), cl::NDRange(lineCount_rounded), cl::NDRange(256));
	errorCheck("Exe Kernel1", error);

	
	//kernel2.setArg(3, cam.plane());	

	//std::cout << "Init" << std::endl;
	kernel2.init(queue.getQueue());

	//std::cout << "Kernel Start" << std::endl;
	error = queue.getQueue().enqueueNDRangeKernel(kernel2.getKernel(), cl::NDRange(0,0), cl::NDRange(width, height), cl::NDRange(16,16));
	errorCheck("Exe Kernel2", error);	
	//std::cout << "Kernel End" << std::endl;

	kernel2.release(queue.getQueue());
	

	//std::cout << "Wait OCL:" << std::endl;
	queue.getQueue().finish();	

	//std::cout << "End OCL:" << std::endl;
}
