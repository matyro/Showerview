
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




std::tuple<OCL_Queue, OCL_Kernel, OCL_Kernel> initOCL(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines)
{
	

	auto hardware = OCL_Hardware();

	auto program = OCL_Program("../Shader/kernel3.cl");
	program.load(hardware.getDevice(), hardware.getContext());

	auto queue = OCL_Queue(hardware.getContext(), hardware.getDevice());
	
	
	std::cout << "Init Kernel 1" << std::endl;
	auto kernel1 = OCL_Kernel("tansform");
	kernel1.createKernel(program.getProgram());
	kernel1.info();


	kernel1.addBuffer(hardware.getContext(), CL_MEM_READ_WRITE, 2, lines);	
	kernel1.setArg(3, static_cast<unsigned int>(lines.size()) );

	kernel1.setupQueue(queue.getQueue());


	std::cout << "Init Kernel 2" << std::endl;
	auto kernel2 = OCL_Kernel("sortToBins");
	kernel2.createKernel(program.getProgram());
	kernel2.info();	

	kernel2.setArg(0, kernel1.getBuffer(0));		
	kernel2.setArg(1, static_cast<unsigned int>(lines.size()) );
	kernel2.addBuffer(hardware.getContext(), CL_MEM_READ_WRITE, 2, 8 * 8 * sizeof(unsigned int));
	kernel2.addBuffer(hardware.getContext(), CL_MEM_READ_WRITE, 3, 8 * 8 * sizeof(Line) * lines.size());

	kernel2.setupQueue(queue.getQueue());



	std::cout << "Init Kernel 3" << std::endl;
	auto kernel3 = OCL_Kernel("sortToTile");
	kernel3.createKernel(program.getProgram());
	kernel3.info();

	kernel3.setupQueue(queue.getQueue());

	std::cout << "Init Kernel 4" << std::endl;
	auto kernel4 = OCL_Kernel("writeTexture");
	kernel4.createKernel(program.getProgram());
	kernel4.info();

	kernel4.addMemoryGL(hardware.getContext(), 0, textureFlag, textureID);

	kernel4.setupQueue(queue.getQueue());



	return std::make_tuple(queue, kernel1, kernel2);
}


void calcOCL(std::tuple<OCL_Queue, OCL_Kernel, OCL_Kernel>& data, const float frameCount, const cl_float16& view, const cl_float16& proj, const int lineCount, const int width, const int height)
{
	std::cout << "Start OCL: " << frameCount << std::endl;

	cl_int error = 0;

	auto& queue = std::get<0>(data);
	auto& kernel1= std::get<1>(data);
	auto& kernel2 = std::get<2>(data);
	
	

	kernel1.setArg(0, view);
	kernel1.setArg(1, proj);
	

	const int groupSize = 128;
	int exeSize1 = std::ceil(lineCount / static_cast<float>(groupSize)) * groupSize;


	std::cout << "Kernel1 Start" << std::endl;
	error = queue.getQueue().enqueueNDRangeKernel(kernel1.getKernel(), cl::NDRange(0), cl::NDRange(exeSize1), cl::NDRange(groupSize));
	errorCheck("Exe Kernel1", error);	
	std::cout << "Kernel1 End" << std::endl;

	
	//kernel2.init(queue.getQueue());

	std::cout << "Kernel2" << std::endl;
	error = queue.getQueue().enqueueNDRangeKernel(kernel2.getKernel(), cl::NDRange(0, 0, 0), cl::NDRange(exeSize1, 8 , 8) , cl::NDRange(groupSize, 1, 1));
	errorCheck("Exe Kernel2", error);
	
	//kernel2.release(queue.getQueue());
	

	std::cout << "Wait OCL:" << std::endl;
	queue.getQueue().finish();	

	std::cout << "End OCL:" << std::endl;
}
