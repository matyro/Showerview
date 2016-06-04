#pragma once


#include <iostream>


#include "OCL/OpenCL_Include.h"

#include "OCL/OpenCL_Error.h"


class OCL_Queue
{
private:

	cl::CommandQueue m_queue;


public:

	OCL_Queue(cl::Context con, cl::Device dev);

	inline cl::CommandQueue& getQueue() { return m_queue; }
	

	

};

