#pragma once


#include <array>

#include "Camera/Camera.h"

#include "Helper/Line.h"


#include "OCL/OpenCL_Include.h"
#include "OCL/OpenCL_Error.h"
#include "OCL/OpenCL_Hardware.h"
#include "OCL/OpenCL_Kernel.h"
#include "OCL/OpenCL_Program.h"
#include "OCL/OpenCL_Queue.h"

#include<string>
#include<memory>


class OpenCL
{
private:

	std::unique_ptr<OCL_Hardware> m_hardware;
	std::unique_ptr<OCL_Program> m_program;
	std::unique_ptr<OCL_Queue> m_queue;

	std::vector<OCL_Kernel> m_kernel;


public:

	OpenCL(std::string kernel);

	void loadKernelFunction(std::string name);

	void writeData(unsigned int kernelID);
	void readData(unsigned int kernelID, unsigned int parameterID);

	void addOpenGL(unsigned int kernelID, cl_GLenum textureFlag, unsigned int textureID);

	OCL_Kernel& getKernel(unsigned int kernelID);
	

	void deploy(unsigned int kernelID, cl::NDRange offset, cl::NDRange size, cl::NDRange box);

	template<class T>
	void addBuffer(unsigned int kernelID, std::vector<T>& data, unsigned int parameterNumber, cl_mem_flags memoryFlag = CL_MEM_READ_WRITE);

	template<class T>
	void replaceBuffer(unsigned int kernelID, unsigned int bufferID, std::vector<T>& data, unsigned int parameterNumber, cl_mem_flags memoryFlag = CL_MEM_READ_WRITE);
	
	template<class T>
	void setArg(unsigned int kernelID, unsigned int parameterNumber, T data);

	inline void acquireOGL(unsigned int kernelID) { m_kernel[kernelID].acquireOGL(m_queue->getQueue()); }
	inline void releaseOGL(unsigned int kernelID) { m_kernel[kernelID].releaseOGL(m_queue->getQueue()); }
	inline void finish() { m_queue->getQueue().finish(); }
};


template<class T>
void OpenCL::addBuffer(unsigned int kernelID, std::vector<T>& data, unsigned int parameterNumber, cl_mem_flags memoryFlag)
{
	m_kernel[kernelID].addBuffer(m_hardware->getContext(), memoryFlag, parameterNumber, data);
}

template<class T>
void OpenCL::replaceBuffer(unsigned int kernelID, unsigned int bufferID, std::vector<T>& data, unsigned int parameterNumber, cl_mem_flags memoryFlag)
{
	m_kernel[kernelID].replaceBuffer(bufferID, m_hardware->getContext(), memoryFlag, parameterNumber, data);

}

template<class T>
void OpenCL::setArg(unsigned int kernelID, unsigned int parameterNumber, T data)
{
	m_kernel[kernelID].setArg(parameterNumber, data);
}

