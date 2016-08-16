#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <tuple>


#include "OCL/OpenCL_Include.h"
#include "OCL/OpenCL_Error.h"


class OCL_Kernel
{
private:

	cl::Kernel m_kernel;

	std::string m_functionName;


	std::vector< cl::Memory > m_memory;
	std::vector<std::tuple<cl::Buffer, void*, size_t>> m_buffer;


public:

	OCL_Kernel(std::string functionName);


	void createKernel(cl::Program& prog);	


	void addMemoryGL(cl::Context& context, const int parameterNumber, const cl_GLint textureFlag, const cl_GLuint textureID);

	
	template<class T>
	inline void addBuffer(cl::Context& context, cl_mem_flags memoryFlag, cl_uint parameterNumber, std::vector<T>& data)
	{
		cl_int error;

		size_t size = sizeof(T) * data.size();

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (vector) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (vector) Set", error);

		m_buffer.push_back(std::make_tuple(buffer, data.data(), size ));
	}

	// Size in bytes
	inline void addBuffer(cl::Context& context, cl_mem_flags memoryFlag, const cl_uint parameterNumber, size_t size)
	{
		cl_int error;

		cl::Buffer buffer = cl::Buffer(context, memoryFlag, size, 0, &error);
		errorCheck("Add Buffer (size) Create", error);

		error = m_kernel.setArg(parameterNumber, buffer);
		errorCheck("Add Buffer (size) Set", error);		

		m_buffer.push_back(std::make_tuple(buffer, nullptr, 0));
	}

	
	
	void setupQueue(cl::CommandQueue& queue);

	template<class T>
	inline void setArg(const cl_uint parameterNumber, const T& data)
	{
		cl_int error = m_kernel.setArg(parameterNumber, data);

		errorCheck("Kernel Argument", error);
	}


	void init(cl::CommandQueue& queue);

	void release(cl::CommandQueue& queue);



	void info();


	inline cl::Kernel& getKernel() { return m_kernel; }

	inline cl::Buffer& getBuffer(unsigned int i)
	{
		if(i < m_buffer.size() )
			return std::get<0>(m_buffer[i]);
	
		std::cout << "Buffer dont exist!" << std::endl;
		std::cin.get();
		exit(-1);
	}

};

