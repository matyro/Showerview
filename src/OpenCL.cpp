
#include <iostream>
#include <vector>
#include <tuple>




#include "OpenCL.h"



OpenCL::OpenCL(std::string kernel)
{
	m_hardware = std::make_unique<OCL_Hardware>();

	m_program = std::make_unique<OCL_Program>( kernel.c_str() );
	m_program->load(m_hardware->getDevice(), m_hardware->getContext());

	m_queue = std::make_unique<OCL_Queue>(m_hardware->getContext(), m_hardware->getDevice());
}


void OpenCL::loadKernelFunction(std::string name)
{
	auto kernel = OCL_Kernel(name);
	kernel.createKernel( m_program->getProgram() );
	kernel.info();

	m_kernel.push_back(kernel);
	return;
}

void OpenCL::writeData(unsigned int kernelID)
{
	m_kernel[kernelID].writeData(m_queue->getQueue());
}

void OpenCL::readData(unsigned int kernelID, unsigned int parameterID)
{
	m_kernel[kernelID].readData(m_queue->getQueue(), parameterID);
}

void OpenCL::addOpenGL(unsigned int kernelID, cl_GLenum textureFlag, unsigned int textureID)
{
	m_kernel[kernelID].addMemoryGL(m_hardware->getContext(), 0, textureFlag, textureID);
}

void OpenCL::deploy(unsigned int kernelID, cl::NDRange offset, cl::NDRange size, cl::NDRange box)
{
	cl_int error = m_queue->getQueue().enqueueNDRangeKernel(m_kernel[kernelID].getKernel(), offset, size, box);
	errorCheck("Exe Kernel", error);
}


OCL_Kernel& OpenCL::getKernel(unsigned int kernelID)
{
	return m_kernel[kernelID];
}









