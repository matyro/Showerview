
#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <fstream>
#include <string>


#ifdef _WIN32
#include <Windows.h>
#include <Wingdi.h>
#else

#endif

#include <CL/cl.hpp>

#include "Helper/OpenCL_Error.h"
#include "Camera/Camera.h"


#include "OCL.h"

std::tuple<cl::Platform, cl::Device, cl::Context> createContext()
{
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) {
		std::cout << " No OpenCL platforms found. Check OpenCL installation!\n";
		std::cin.get();
		exit(1);
	}

	for (auto itr : all_platforms)
	{
		static int count = 1;
		std::cout << "Platform [" << count++ << "]: " << itr.getInfo<CL_PLATFORM_NAME>() << std::endl;
	}
	cl::Platform platform;

	while (true)
	{
		std::cout << "Select platform: ";
		std::string str;
		std::getline(std::cin, str);
		std::stringstream sstr(str);
		int id = -1;
		sstr >> id;

		if (id > 0 && id <= all_platforms.size())
		{
			platform = all_platforms[id - 1];
			break;
		}
		else
			std::cout << "Selection wrong: " << id << "\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	}

	std::cout << "\n >> Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n\n\n";



	//Get Devices
	std::vector<cl::Device> all_devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		std::cout << " No OpenCL devices found. Check OpenCL installation!\n";
		std::cin.get();
		exit(1);
	}


	std::cout << "Needed Extension: cl_khr_gl_sharing" << std::endl;
	for (auto itr : all_devices)
	{
		static int count = 1;
		std::cout << "Devices [" << count++ << "]: " << itr.getInfo<CL_DEVICE_NAME>() << std::endl;		
		std::cout << "Extensions cl_khr_gl_sharing: " << std::boolalpha << (itr.getInfo<CL_DEVICE_EXTENSIONS>().find("cl_khr_gl_sharing") != std::string::npos) << std::endl;
	}


	cl::Device device;
	while (true)
	{
		std::cout << "Select device: ";
		std::string str;
		std::getline(std::cin, str);
		std::stringstream sstr(str);
		int id = -1;
		sstr >> id;

		if (id > 0 && id <= all_devices.size())
		{
			device = all_devices[id - 1];
			break;
		}
		else
			std::cout << "Selection wrong: " << id << "\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	}

		
	if (device.getInfo<CL_DEVICE_EXTENSIONS>().find("cl_khr_gl_sharing") == std::string::npos)
	{
		std::cout << "\n\n Extension cl_khr_gl_sharing needed but not available!" << std::endl;
		std::cout << "Currently no fallback modus implemented ..." << std::endl;
		std::cin.get();
		exit(-1);
	}
	else
	{
		std::cout << "\nUsing device: " << device.getInfo<CL_DEVICE_NAME>() << "\n\n\n\n";
	}
	

    #ifdef _WIN32
		auto hDC = wglGetCurrentDC();
		auto hRC = wglGetCurrentContext();
	#else
		auto hDC = glXGetCurrentContext();
		auto hRC = glfwGetWGLContext();
	#endif

		cl_context_properties props[] =
		{
			CL_GL_CONTEXT_KHR, (cl_context_properties)hRC,
			CL_WGL_HDC_KHR, (cl_context_properties)hDC,
			CL_CONTEXT_PLATFORM, (cl_context_properties)(platform)(),
			0
		};

	
	cl_int error;
	auto context = cl::Context(device, props,0,0, &error);
	errorCheck("context creation", error);

	return std::make_tuple(platform, device, context );
}



cl::Program loadProgram(cl::Device& device, cl::Context& context, std::string path)
{
	std::string kernel_code;

	std::ifstream file;

	// setException
	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(path);

		std::stringstream sstr;

		//
		sstr << file.rdbuf();

		file.close();

		kernel_code = sstr.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "OpenCL Error: " << path << " could not be read!" << std::endl;
		std::cout << e.what() << std::endl;

		std::cin.get();
		exit(-1);
	}
		
		
	cl::Program::Sources sources;

	sources.push_back({ kernel_code.c_str(),kernel_code.length() });

	cl::Program program(context, sources);
	if (program.build({ device }, "-cl-single-precision-constant -cl-mad-enable -cl-fast-relaxed-math") != CL_SUCCESS) {
		std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << "\n";
		std::cin.get();
		exit(1);
	}

	return program;
}



std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory> initOCL(cl_GLenum textureFlag, unsigned int textureID, std::vector<Line>& lines)
{
	//std::tuple<cl::Platform, cl::Device, cl::Context> 
	auto hardware = createContext();

	auto prog = loadProgram(std::get<1>(hardware), std::get<2>(hardware), "../Shader/kernel.cl");
	
	
	//create queue to which we will push commands for the device.
	cl_int error;
	cl::CommandQueue queue(std::get<2>(hardware), std::get<1>(hardware),0, &error);		
	errorCheck("CommandQueue creation", error);

	cl::Kernel texture_writer(prog, "writeTexture", &error);
	errorCheck("Kernel creation", error);

	{
		cl_uint tmp;
		texture_writer.getInfo(CL_KERNEL_NUM_ARGS, &tmp);
		std::cout << "Number of Arguments: " << tmp << std::endl;
	}
	
	
	
	cl::Memory texture = cl::ImageGL(std::get<2>(hardware), CL_MEM_WRITE_ONLY, textureFlag, 0, textureID, &error);
	errorCheck("shared texture creation", error);	
	texture_writer.setArg(0, sizeof(cl::Memory), &texture);

	
	static const cl::Buffer lineBuffer = cl::Buffer(std::get<2>(hardware), CL_MEM_READ_ONLY , sizeof(Line) * lines.size(), 0, &error);
	errorCheck("LineBuffer Create", error);
	error = texture_writer.setArg(3, lineBuffer);
	errorCheck("LineBuffer Set", error);
	error = queue.enqueueWriteBuffer(lineBuffer, CL_TRUE, 0, sizeof(Line) * lines.size(), lines.data());
	errorCheck("LineBuffer Transmit", error);

	int size = lines.size();
	texture_writer.setArg(4, size);


	return std::make_tuple(queue, texture_writer, texture);
}


void calcOCL(std::tuple<cl::CommandQueue, cl::Kernel, cl::Memory>& data, const float count, const Plane& plane, const int width, const int height)
{
	auto tmp_ptr = std::vector<cl::Memory>({ std::get<2>(data) });
	std::get<0>(data).enqueueAcquireGLObjects(&tmp_ptr);

	std::get<1>(data).setArg(1, count);		
	std::get<1>(data).setArg(2, plane);

	
	std::get<0>(data).enqueueNDRangeKernel(std::get<1>(data), cl::NDRange(0, 0), cl::NDRange(width, height) , cl::NDRange(8, 48));


	std::get<0>(data).enqueueReleaseGLObjects(&tmp_ptr);

	std::get<0>(data).finish();
	
}
